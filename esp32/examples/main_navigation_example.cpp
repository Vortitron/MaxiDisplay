#include <Arduino.h>
#include "../src/hardware/GPIOManager.h"
#include "../src/core/NavigationManager.h"
#include "../src/config/Config.h"

// Example implementation showing the new navigation system
// Hardware: 2 rows × 4 screens, 4 non-momentary buttons + LEDs, 2 row select buttons

void setup() {
	Serial.begin(SERIAL_SPEED);
	Serial.println("ChoreTracker Navigation Example Starting...");
	
	// Initialize hardware GPIO
	GPIOManager::initialize();
	
	// Initialize navigation system
	NavigationManager::initialize();
	
	// Set custom column titles if desired
	NavigationManager::setColumnTitle(0, "Emma");
	NavigationManager::setColumnTitle(1, "Jack");
	NavigationManager::setColumnTitle(2, "Weather");
	NavigationManager::setColumnTitle(3, "Family");
	
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
	Serial.println();
}

void loop() {
	// Update hardware states (button debouncing, etc.)
	GPIOManager::updateTaskButtonStates();
	GPIOManager::updateRowSelectStates();
	
	// Update navigation system (handles row select presses and timing)
	NavigationManager::update();
	
	// Check for task button changes (non-momentary switches)
	for (int i = 0; i < 4; i++) {
		if (GPIOManager::hasTaskButtonChanged(i)) {
			bool buttonState = GPIOManager::getTaskButtonState(i);
			Serial.printf("Task Button %d changed to: %s\n", i, buttonState ? "ACTIVE" : "INACTIVE");
			
			// Set LED to match task completion state
			GPIOManager::setTaskLED(i, buttonState);
			
			// Here you would update Home Assistant or mark task complete
			if (buttonState) {
				Serial.printf("Task %d completed!\n", i);
			} else {
				Serial.printf("Task %d reset\n", i);
			}
		}
	}
	
	// Handle display updates based on navigation state
	for (int row = 0; row < 2; row++) {
		if (NavigationManager::needsDisplayUpdate(row)) {
			updateRowDisplay(row);
			NavigationManager::clearUpdateFlag(row);
		}
	}
	
	// Small delay to prevent overwhelming the serial output
	delay(10);
}

void updateRowDisplay(int rowIndex) {
	int currentColumn = NavigationManager::getCurrentColumn(rowIndex);
	
	if (NavigationManager::shouldShowColumnTitle(rowIndex)) {
		// Show column title on first display of the row
		String title = NavigationManager::getColumnTitle(currentColumn);
		int titleDisplay = (rowIndex == 0) ? 0 : 4;  // Display 0 for row 1, display 4 for row 2
		
		Serial.printf("ROW %d DISPLAY %d: Showing title \"%s\"\n", 
		              rowIndex + 1, titleDisplay, title.c_str());
		
		// Here you would update the actual TFT display
		// displayManager.showTitle(titleDisplay, title);
		
	} else if (NavigationManager::shouldShowContent(rowIndex)) {
		// Show content for the current column across all displays in the row
		String columnTitle = NavigationManager::getColumnTitle(currentColumn);
		
		Serial.printf("ROW %d: Showing content for \"%s\" column\n", 
		              rowIndex + 1, columnTitle.c_str());
		
		// Update all 4 displays in this row
		for (int col = 0; col < 4; col++) {
			int displayIndex = NavigationManager::getDisplayIndexForRowColumn(rowIndex, col);
			
			// Here you would update the actual TFT display with content
			// displayManager.showTaskContent(displayIndex, currentColumn, col);
			
			Serial.printf("  Display %d: Content for %s (position %d)\n", 
			              displayIndex, columnTitle.c_str(), col);
		}
	}
}

// Example function to demonstrate LED patterns
void demonstrateLEDs() {
	Serial.println("LED Demo: All on");
	GPIOManager::setAllTaskLEDs(true);
	delay(1000);
	
	Serial.println("LED Demo: Sequential");
	GPIOManager::setAllTaskLEDs(false);
	for (int i = 0; i < 4; i++) {
		GPIOManager::setTaskLED(i, true);
		delay(250);
		GPIOManager::setTaskLED(i, false);
	}
	
	Serial.println("LED Demo: Complete");
}

// Example function to show column cycling
void demonstrateNavigation() {
	Serial.println("Navigation Demo: Cycling Row 1 through all columns");
	
	for (int col = 0; col < 4; col++) {
		NavigationManager::resetToColumn(0, col);
		String title = NavigationManager::getColumnTitle(col);
		Serial.printf("Row 1 Column %d: %s\n", col, title.c_str());
		delay(1000);
	}
	
	// Reset to first column
	NavigationManager::resetToColumn(0, 0);
	NavigationManager::resetToColumn(1, 0);
} 