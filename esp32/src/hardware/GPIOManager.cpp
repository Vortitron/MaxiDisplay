#include "GPIOManager.h"
#include "../config/GPIOConfig.h"
#include <Arduino.h>

// Static member definitions
bool GPIOManager::taskButtonStates[TOTAL_TASK_BUTTONS] = {false};
bool GPIOManager::taskButtonPreviousStates[TOTAL_TASK_BUTTONS] = {false};
bool GPIOManager::rowSelectStates[TOTAL_ROW_SELECT_BUTTONS] = {false};
bool GPIOManager::rowSelectPreviousStates[TOTAL_ROW_SELECT_BUTTONS] = {false};
unsigned long GPIOManager::lastDebounceTime[TOTAL_TASK_BUTTONS + TOTAL_ROW_SELECT_BUTTONS] = {0};

// Pin arrays for easier iteration
const int taskButtonPins[TOTAL_TASK_BUTTONS] = TASK_BUTTON_PINS;
const int taskLEDPins[TOTAL_TASK_LEDS] = TASK_LED_PINS;
const int rowSelectPins[TOTAL_ROW_SELECT_BUTTONS] = ROW_SELECT_PINS;
const int tftCSPins[TOTAL_TFT_DISPLAYS] = TFT_CS_PINS;

void GPIOManager::initialize() {
	Serial.println("GPIOManager: Initializing GPIO pins...");
	setupPins();
	
	// Initialize button states
	updateTaskButtonStates();
	updateRowSelectStates();
	
	// Turn ON all LEDs initially (tasks are incomplete at startup)
	setAllTasksIncomplete();
	
	// Set display backlight on
	setDisplayBacklight(true);
	
	Serial.println("GPIOManager: Initialization complete");
	Serial.printf("Hardware: 8 task buttons, 8 LEDs, 2 row select buttons, 8 displays\n");
	Serial.println("LED Logic: ON = Task Incomplete, OFF = Task Complete");
}

void GPIOManager::setupPins() {
	// Setup task button pins (non-momentary, with pullup) - 8 buttons
	for (int i = 0; i < TOTAL_TASK_BUTTONS; i++) {
		pinMode(taskButtonPins[i], INPUT_PULLUP);
		Serial.printf("Task Button %d (Display %d) on GPIO %d\n", i, i, taskButtonPins[i]);
	}
	
	// Setup task LED pins (output) - 8 LEDs
	for (int i = 0; i < TOTAL_TASK_LEDS; i++) {
		pinMode(taskLEDPins[i], OUTPUT);
		digitalWrite(taskLEDPins[i], LED_STATE_TASK_INCOMPLETE);  // Start with tasks incomplete
		Serial.printf("Task LED %d (Display %d) on GPIO %d\n", i, i, taskLEDPins[i]);
	}
	
	// Setup row select button pins (momentary, with pullup)
	for (int i = 0; i < TOTAL_ROW_SELECT_BUTTONS; i++) {
		pinMode(rowSelectPins[i], INPUT_PULLUP);
		Serial.printf("Row Select %d on GPIO %d\n", i, rowSelectPins[i]);
	}
	
	// Setup TFT CS pins (output, start high)
	for (int i = 0; i < TOTAL_TFT_DISPLAYS; i++) {
		pinMode(tftCSPins[i], OUTPUT);
		digitalWrite(tftCSPins[i], HIGH);  // CS is active low, start deselected
		Serial.printf("TFT CS %d on GPIO %d\n", i, tftCSPins[i]);
	}
	
	// Setup shared TFT pins
	pinMode(TFT_SCK, OUTPUT);
	pinMode(TFT_MOSI, OUTPUT);
	pinMode(TFT_DC, OUTPUT);
	pinMode(TFT_BL, OUTPUT);
	
	Serial.printf("TFT Shared: SCK=%d, MOSI=%d, DC=%d, BL=%d\n", 
	              TFT_SCK, TFT_MOSI, TFT_DC, TFT_BL);
}

bool GPIOManager::hasTaskButtonChanged(int buttonIndex) {
	if (buttonIndex < 0 || buttonIndex >= TOTAL_TASK_BUTTONS) {
		return false;
	}
	
	bool currentState = (digitalRead(taskButtonPins[buttonIndex]) == TASK_BUTTON_ACTIVE_STATE);
	bool changed = (currentState != taskButtonPreviousStates[buttonIndex]);
	
	// Simple debouncing
	unsigned long now = millis();
	if (changed && (now - lastDebounceTime[buttonIndex] > BUTTON_DEBOUNCE_MS)) {
		lastDebounceTime[buttonIndex] = now;
		taskButtonPreviousStates[buttonIndex] = currentState;
		taskButtonStates[buttonIndex] = currentState;
		
		Serial.printf("Task Button %d (Display %d) changed to %s\n", 
		              buttonIndex, buttonIndex, currentState ? "ACTIVE" : "INACTIVE");
		
		// Automatically handle task completion when button changes
		if (currentState) {
			setTaskComplete(buttonIndex);  // Turn LED OFF
			Serial.printf("Task %d (Display %d) COMPLETED - LED OFF\n", buttonIndex, buttonIndex);
		} else {
			setTaskIncomplete(buttonIndex);  // Turn LED ON
			Serial.printf("Task %d (Display %d) RESET - LED ON\n", buttonIndex, buttonIndex);
		}
		
		return true;
	}
	
	return false;
}

bool GPIOManager::getTaskButtonState(int buttonIndex) {
	if (buttonIndex < 0 || buttonIndex >= TOTAL_TASK_BUTTONS) {
		return false;
	}
	return taskButtonStates[buttonIndex];
}

void GPIOManager::updateTaskButtonStates() {
	for (int i = 0; i < TOTAL_TASK_BUTTONS; i++) {
		hasTaskButtonChanged(i);  // This updates the state if changed
	}
}

void GPIOManager::setTaskLED(int ledIndex, bool taskComplete) {
	if (ledIndex < 0 || ledIndex >= TOTAL_TASK_LEDS) {
		return;
	}
	
	// INVERTED LOGIC: taskComplete=true means LED OFF, taskComplete=false means LED ON
	digitalWrite(taskLEDPins[ledIndex], taskComplete ? LED_STATE_TASK_COMPLETE : LED_STATE_TASK_INCOMPLETE);
	Serial.printf("Task LED %d set to %s (task %s)\n", 
	              ledIndex, 
	              taskComplete ? "OFF" : "ON",
	              taskComplete ? "COMPLETE" : "INCOMPLETE");
}

void GPIOManager::setTaskIncomplete(int ledIndex) {
	if (ledIndex < 0 || ledIndex >= TOTAL_TASK_LEDS) {
		return;
	}
	digitalWrite(taskLEDPins[ledIndex], LED_STATE_TASK_INCOMPLETE);  // LED ON
	Serial.printf("Task %d marked INCOMPLETE - LED ON\n", ledIndex);
}

void GPIOManager::setTaskComplete(int ledIndex) {
	if (ledIndex < 0 || ledIndex >= TOTAL_TASK_LEDS) {
		return;
	}
	digitalWrite(taskLEDPins[ledIndex], LED_STATE_TASK_COMPLETE);  // LED OFF
	Serial.printf("Task %d marked COMPLETE - LED OFF\n", ledIndex);
}

void GPIOManager::setAllTasksIncomplete() {
	for (int i = 0; i < TOTAL_TASK_LEDS; i++) {
		setTaskIncomplete(i);
	}
	Serial.println("All tasks marked INCOMPLETE - All LEDs ON");
}

void GPIOManager::setAllTasksComplete() {
	for (int i = 0; i < TOTAL_TASK_LEDS; i++) {
		setTaskComplete(i);
	}
	Serial.println("All tasks marked COMPLETE - All LEDs OFF");
}

bool GPIOManager::isRowSelectPressed(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROW_SELECT_BUTTONS) {
		return false;
	}
	
	bool currentState = (digitalRead(rowSelectPins[rowIndex]) == ROW_SELECT_ACTIVE_STATE);
	bool pressed = false;
	
	// Detect button press (transition from released to pressed)
	if (currentState && !rowSelectPreviousStates[rowIndex]) {
		unsigned long now = millis();
		if (now - lastDebounceTime[TOTAL_TASK_BUTTONS + rowIndex] > ROW_SELECT_DEBOUNCE_MS) {
			lastDebounceTime[TOTAL_TASK_BUTTONS + rowIndex] = now;
			pressed = true;
			Serial.printf("Row Select %d pressed\n", rowIndex);
		}
	}
	
	rowSelectPreviousStates[rowIndex] = currentState;
	return pressed;
}

void GPIOManager::updateRowSelectStates() {
	for (int i = 0; i < TOTAL_ROW_SELECT_BUTTONS; i++) {
		isRowSelectPressed(i);  // This handles the state update
	}
}

void GPIOManager::setDisplayBacklight(bool state) {
	digitalWrite(TFT_BL, state ? HIGH : LOW);
	Serial.printf("Display backlight set to %s\n", state ? "ON" : "OFF");
}

int GPIOManager::getDisplayForPosition(int row, int col) {
	if (row < 0 || row >= TOTAL_ROWS || col < 0 || col >= DISPLAYS_PER_ROW) {
		return -1;  // Invalid position
	}
	return GET_DISPLAY_IDX(row, col);
}

int GPIOManager::getRowFromDisplay(int displayIndex) {
	if (displayIndex < 0 || displayIndex >= TOTAL_TFT_DISPLAYS) {
		return -1;
	}
	return GET_ROW_FROM_DISPLAY(displayIndex);
}

int GPIOManager::getColFromDisplay(int displayIndex) {
	if (displayIndex < 0 || displayIndex >= TOTAL_TFT_DISPLAYS) {
		return -1;
	}
	return GET_COL_FROM_DISPLAY(displayIndex);
}

int GPIOManager::getDisplayForButton(int buttonIndex) {
	// Direct 1:1 mapping - button 0 controls display 0, etc.
	if (buttonIndex < 0 || buttonIndex >= TOTAL_TASK_BUTTONS) {
		return -1;
	}
	return buttonIndex;
}

int GPIOManager::getButtonForDisplay(int displayIndex) {
	// Direct 1:1 mapping - display 0 controlled by button 0, etc.
	if (displayIndex < 0 || displayIndex >= TOTAL_TFT_DISPLAYS) {
		return -1;
	}
	return displayIndex;
} 