#pragma once
#include <Arduino.h>

// Forward declarations from GPIOConfig.h
constexpr int TOTAL_TASK_BUTTONS = 8;        // 8 buttons (one per screen)
constexpr int TOTAL_TASK_LEDS = 8;           // 8 LEDs (one per button)
constexpr int TOTAL_ROW_SELECT_BUTTONS = 2;
constexpr int TOTAL_TFT_DISPLAYS = 8;
constexpr int DISPLAYS_PER_ROW = 4;
constexpr int TOTAL_ROWS = 2;

class GPIOManager {
public:
	static void initialize();
	static void setupPins();
	
	// Task button handling (non-momentary) - 8 buttons total
	static bool hasTaskButtonChanged(int buttonIndex);
	static bool getTaskButtonState(int buttonIndex);
	static void updateTaskButtonStates();
	
	// LED control - INVERTED LOGIC: ON = incomplete, OFF = complete
	static void setTaskLED(int ledIndex, bool taskComplete);
	static void setTaskIncomplete(int ledIndex);  // Turn LED ON
	static void setTaskComplete(int ledIndex);    // Turn LED OFF
	static void setAllTasksIncomplete();          // All LEDs ON
	static void setAllTasksComplete();            // All LEDs OFF
	
	// Row select button handling (momentary)
	static bool isRowSelectPressed(int rowIndex);
	static void updateRowSelectStates();
	
	// Display control
	static void setDisplayBacklight(bool state);
	static int getDisplayForPosition(int row, int col);
	
	// Navigation helpers
	static int getRowFromDisplay(int displayIndex);
	static int getColFromDisplay(int displayIndex);
	
	// Task button to display mapping
	static int getDisplayForButton(int buttonIndex);
	static int getButtonForDisplay(int displayIndex);
	
private:
	static bool taskButtonStates[TOTAL_TASK_BUTTONS];
	static bool taskButtonPreviousStates[TOTAL_TASK_BUTTONS];
	static bool rowSelectStates[TOTAL_ROW_SELECT_BUTTONS];
	static bool rowSelectPreviousStates[TOTAL_ROW_SELECT_BUTTONS];
	static unsigned long lastDebounceTime[TOTAL_TASK_BUTTONS + TOTAL_ROW_SELECT_BUTTONS];
}; 