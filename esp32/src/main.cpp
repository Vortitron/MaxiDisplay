#include <Arduino.h>
#include "hardware/GPIOManager.h"
#include "core/NavigationManager.h"
#include "core/DemoManager.h"
#include "ui/DisplayManager.h"
#include "config/Config.h"

// Main application state
bool isDemoMode = false;
unsigned long demoModeActivationStart = 0;
bool bothButtonsPressed = false;

// Demo mode activation constants
#define DEMO_ACTIVATION_TIME_MS 5000  // 5 seconds to activate demo mode
#define BOTH_BUTTONS_HELD_CHECK_MS 100  // Check every 100ms if both buttons still held

void setup() {
	Serial.begin(SERIAL_SPEED);
	Serial.println("ChoreTracker Starting...");
	
	// Initialize hardware GPIO
	GPIOManager::initialize();
	
	// Initialize navigation system
	NavigationManager::initialize();
	
	// Initialize display system
	DisplayManager::initialize();
	
	// Initialize demo system
	DemoManager::initialize();
	
	// Set custom column titles
	NavigationManager::setColumnTitle(0, "Emma");
	NavigationManager::setColumnTitle(1, "Jack");
	NavigationManager::setColumnTitle(2, "Weather");
	NavigationManager::setColumnTitle(3, "Family");
	
	Serial.println("System ready! Press both row select buttons for 5 seconds to enter demo mode.");
	displaySystemInfo();
}

void loop() {
	// Update hardware states
	GPIOManager::updateTaskButtonStates();
	GPIOManager::updateRowSelectStates();
	
	// Check for demo mode activation
	checkDemoModeActivation();
	
	if (isDemoMode) {
		// Run demo mode
		DemoManager::update();
		
		// Check for demo mode exit (any task button press)
		for (int i = 0; i < 8; i++) {
			if (GPIOManager::hasTaskButtonChanged(i)) {
				exitDemoMode();
				break;
			}
		}
	} else {
		// Normal operation mode
		NavigationManager::update();
		handleTaskButtons();
		handleDisplayUpdates();
	}
	
	// Update displays
	DisplayManager::update();
	
	delay(10);
}

void checkDemoModeActivation() {
	// Check if both row select buttons are being held
	bool row0Pressed = digitalRead(17) == LOW;  // GPIO 17 (Row 0 select)
	bool row1Pressed = digitalRead(22) == LOW;  // GPIO 22 (Row 1 select)
	
	if (row0Pressed && row1Pressed) {
		if (!bothButtonsPressed) {
			// Just started holding both buttons
			bothButtonsPressed = true;
			demoModeActivationStart = millis();
			Serial.println("Demo activation: Holding both buttons...");
			
			// Show progress on displays
			DisplayManager::showDemoActivationProgress(0);
		} else {
			// Continue holding both buttons - show progress
			unsigned long elapsed = millis() - demoModeActivationStart;
			int progress = (elapsed * 100) / DEMO_ACTIVATION_TIME_MS;
			progress = min(progress, 100);
			
			DisplayManager::showDemoActivationProgress(progress);
			
			if (elapsed >= DEMO_ACTIVATION_TIME_MS) {
				// Activate demo mode!
				enterDemoMode();
			}
		}
	} else {
		if (bothButtonsPressed) {
			// Released buttons before demo activation
			bothButtonsPressed = false;
			Serial.println("Demo activation cancelled - buttons released");
			DisplayManager::clearDemoActivationProgress();
		}
	}
}

void enterDemoMode() {
	Serial.println("🎮 ENTERING DEMO MODE! 🎮");
	isDemoMode = true;
	bothButtonsPressed = false;
	
	// Start demo mode
	DemoManager::startDemo();
	
	// Show demo mode activation animation
	DisplayManager::showDemoModeActivation();
	
	// Turn off all task LEDs initially
	GPIOManager::setAllTasksComplete();
	
	Serial.println("Demo mode active! Press any task button to exit.");
}

void exitDemoMode() {
	Serial.println("Exiting demo mode - returning to normal operation");
	isDemoMode = false;
	
	// Stop demo
	DemoManager::stopDemo();
	
	// Reset displays to normal content
	DisplayManager::showExitingDemoMode();
	
	// Reset LEDs to normal state
	GPIOManager::setAllTasksIncomplete();
	
	// Force display updates
	for (int row = 0; row < 2; row++) {
		NavigationManager::forceDisplayUpdate(row);
	}
}

void handleTaskButtons() {
	// Handle task button changes in normal mode
	for (int i = 0; i < 8; i++) {
		if (GPIOManager::hasTaskButtonChanged(i)) {
			bool buttonState = GPIOManager::getTaskButtonState(i);
			Serial.printf("Task Button %d changed to: %s\n", i, buttonState ? "ACTIVE" : "INACTIVE");
			
			// Update display content based on task completion
			DisplayManager::updateTaskDisplay(i, buttonState);
		}
	}
}

void handleDisplayUpdates() {
	// Handle display updates based on navigation state
	for (int row = 0; row < 2; row++) {
		if (NavigationManager::needsDisplayUpdate(row)) {
			updateRowDisplay(row);
			NavigationManager::clearUpdateFlag(row);
		}
	}
}

void updateRowDisplay(int rowIndex) {
	int currentColumn = NavigationManager::getCurrentColumn(rowIndex);
	
	if (NavigationManager::shouldShowColumnTitle(rowIndex)) {
		// Show column title on first display of the row
		String title = NavigationManager::getColumnTitle(currentColumn);
		int titleDisplay = (rowIndex == 0) ? 0 : 4;
		
		Serial.printf("ROW %d DISPLAY %d: Showing title \"%s\"\n", 
		              rowIndex + 1, titleDisplay, title.c_str());
		
		DisplayManager::showColumnTitle(titleDisplay, title, currentColumn);
		
	} else if (NavigationManager::shouldShowContent(rowIndex)) {
		// Show content for the current column across all displays in the row
		String columnTitle = NavigationManager::getColumnTitle(currentColumn);
		
		Serial.printf("ROW %d: Showing content for \"%s\" column\n", 
		              rowIndex + 1, columnTitle.c_str());
		
		// Update all 4 displays in this row
		for (int col = 0; col < 4; col++) {
			int displayIndex = NavigationManager::getDisplayIndexForRowColumn(rowIndex, col);
			DisplayManager::showTaskContent(displayIndex, currentColumn, col);
		}
	}
}

void displaySystemInfo() {
	Serial.println("System ready! Hardware layout:");
	Serial.println("┌─────────┬─────────┬─────────┬─────────┐ Row 1 [Select: GPIO 17]");
	Serial.println("│ Disp 0  │ Disp 1  │ Disp 2  │ Disp 3  │");
	Serial.println("│  Emma   │  Jack   │ Weather │ Family  │");
	Serial.println("└─────────┴─────────┴─────────┴─────────┘");
	Serial.println("┌─────────┬─────────┬─────────┬─────────┐ Row 2 [Select: GPIO 22]");
	Serial.println("│ Disp 4  │ Disp 5  │ Disp 6  │ Disp 7  │");
	Serial.println("│ Tasks   │ Tasks   │ Info    │ Tasks   │");
	Serial.println("└─────────┴─────────┴─────────┴─────────┘");
	Serial.println();
	Serial.println("Task Buttons: [0:GPIO13+LED15] [1:GPIO12+LED2] [2:GPIO14+LED0] [3:GPIO27+LED4]");
	Serial.println("              [4:GPIO26+LED5]  [5:GPIO33+LED25] [6:GPIO35+LED32] [7:GPIO34+LED21]");
	Serial.println("Demo Mode: Hold BOTH row select buttons (GPIO17 + GPIO22) for 5 seconds");
	Serial.println();
} 