#include "NavigationManager.h"
#include "../config/GPIOConfig.h"
#include "../hardware/GPIOManager.h"
#include <Arduino.h>

// Static member definitions
RowState NavigationManager::rowStates[TOTAL_ROWS] = {{0, 0, NAV_SHOWING_CONTENT, false}};
String NavigationManager::columnTitles[MAX_COLUMNS] = {"Child 1", "Child 2", "Weather", "Family"};
unsigned long NavigationManager::columnDisplayTimeMs = COLUMN_DISPLAY_TIME_MS;

// Default column titles
const char* DEFAULT_COLUMN_TITLES[MAX_COLUMNS] = {"Child 1", "Child 2", "Weather", "Family"};

void NavigationManager::initialize() {
	Serial.println("NavigationManager: Initializing...");
	
	// Initialize row states
	for (int i = 0; i < TOTAL_ROWS; i++) {
		rowStates[i].currentColumn = 0;
		rowStates[i].titleStartTime = 0;
		rowStates[i].navState = NAV_SHOWING_CONTENT;
		rowStates[i].needsUpdate = true;  // Force initial update
	}
	
	// Set up default column titles
	for (int i = 0; i < MAX_COLUMNS; i++) {
		columnTitles[i] = String(DEFAULT_COLUMN_TITLES[i]);
	}
	
	Serial.println("NavigationManager: Initialization complete");
}

void NavigationManager::update() {
	// Check for row select button presses
	for (int row = 0; row < TOTAL_ROWS; row++) {
		if (GPIOManager::isRowSelectPressed(row)) {
			handleRowSelect(row);
		}
		updateRowState(row);
	}
}

void NavigationManager::handleRowSelect(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS) {
		return;
	}
	
	Serial.printf("NavigationManager: Row %d select button pressed\n", rowIndex);
	
	// Cycle to next column
	cycleColumn(rowIndex);
	
	// Show column title
	rowStates[rowIndex].navState = NAV_SHOWING_COLUMN_TITLE;
	rowStates[rowIndex].titleStartTime = millis();
	rowStates[rowIndex].needsUpdate = true;
	
	Serial.printf("NavigationManager: Row %d now showing column %d (\"%s\")\n", 
	              rowIndex, rowStates[rowIndex].currentColumn, 
	              columnTitles[rowStates[rowIndex].currentColumn].c_str());
}

void NavigationManager::cycleColumn(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS) {
		return;
	}
	
	rowStates[rowIndex].currentColumn = (rowStates[rowIndex].currentColumn + 1) % MAX_COLUMNS;
}

void NavigationManager::updateRowState(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS) {
		return;
	}
	
	RowState& state = rowStates[rowIndex];
	
	// Check if we should transition from title back to content
	if (state.navState == NAV_SHOWING_COLUMN_TITLE) {
		unsigned long now = millis();
		if (now - state.titleStartTime >= columnDisplayTimeMs) {
			transitionToContent(rowIndex);
		}
	}
}

void NavigationManager::transitionToContent(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS) {
		return;
	}
	
	rowStates[rowIndex].navState = NAV_SHOWING_CONTENT;
	rowStates[rowIndex].needsUpdate = true;
	
	Serial.printf("NavigationManager: Row %d transitioning to content for column %d\n", 
	              rowIndex, rowStates[rowIndex].currentColumn);
}

int NavigationManager::getCurrentColumn(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS) {
		return 0;
	}
	return rowStates[rowIndex].currentColumn;
}

NavigationState NavigationManager::getNavigationState(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS) {
		return NAV_SHOWING_CONTENT;
	}
	return rowStates[rowIndex].navState;
}

bool NavigationManager::needsDisplayUpdate(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS) {
		return false;
	}
	return rowStates[rowIndex].needsUpdate;
}

void NavigationManager::clearUpdateFlag(int rowIndex) {
	if (rowIndex >= 0 && rowIndex < TOTAL_ROWS) {
		rowStates[rowIndex].needsUpdate = false;
	}
}

String NavigationManager::getColumnTitle(int columnIndex) {
	if (columnIndex < 0 || columnIndex >= MAX_COLUMNS) {
		return "Unknown";
	}
	return columnTitles[columnIndex];
}

void NavigationManager::setColumnTitle(int columnIndex, const String& title) {
	if (columnIndex >= 0 && columnIndex < MAX_COLUMNS) {
		columnTitles[columnIndex] = title;
		Serial.printf("NavigationManager: Column %d title set to \"%s\"\n", 
		              columnIndex, title.c_str());
	}
}

bool NavigationManager::shouldShowColumnTitle(int rowIndex) {
	return getNavigationState(rowIndex) == NAV_SHOWING_COLUMN_TITLE;
}

bool NavigationManager::shouldShowContent(int rowIndex) {
	return getNavigationState(rowIndex) == NAV_SHOWING_CONTENT;
}

int NavigationManager::getDisplayIndexForRowColumn(int rowIndex, int columnIndex) {
	// For this hardware setup: Row 0 = displays 0-3, Row 1 = displays 4-7
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS || 
	    columnIndex < 0 || columnIndex >= MAX_COLUMNS) {
		return -1;
	}
	
	return (rowIndex * MAX_COLUMNS) + columnIndex;
}

void NavigationManager::setColumnDisplayTime(unsigned long timeMs) {
	columnDisplayTimeMs = timeMs;
	Serial.printf("NavigationManager: Column display time set to %lu ms\n", timeMs);
}

void NavigationManager::resetToColumn(int rowIndex, int columnIndex) {
	if (rowIndex < 0 || rowIndex >= TOTAL_ROWS || 
	    columnIndex < 0 || columnIndex >= MAX_COLUMNS) {
		return;
	}
	
	rowStates[rowIndex].currentColumn = columnIndex;
	rowStates[rowIndex].navState = NAV_SHOWING_CONTENT;
	rowStates[rowIndex].needsUpdate = true;
	
	Serial.printf("NavigationManager: Row %d reset to column %d\n", rowIndex, columnIndex);
}

void NavigationManager::forceDisplayUpdate(int rowIndex) {
	if (rowIndex >= 0 && rowIndex < TOTAL_ROWS) {
		rowStates[rowIndex].needsUpdate = true;
		Serial.printf("NavigationManager: Forced display update for row %d\n", rowIndex);
	}
} 