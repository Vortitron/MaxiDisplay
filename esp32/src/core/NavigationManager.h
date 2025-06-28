#pragma once
#include <Arduino.h>

// Forward declarations
constexpr int MAX_COLUMNS = 4;
constexpr int TOTAL_ROWS = 2;

enum NavigationState {
	NAV_SHOWING_CONTENT,     // Normal content display
	NAV_SHOWING_COLUMN_TITLE // Showing column title after select press
};

struct RowState {
	int currentColumn;       // 0-3, which column is currently selected
	unsigned long titleStartTime;  // When column title started showing
	NavigationState navState;       // Current navigation state
	bool needsUpdate;        // Flag to indicate display needs updating
};

class NavigationManager {
public:
	static void initialize();
	static void update();
	
	// Row selection handling
	static void handleRowSelect(int rowIndex);
	static void cycleColumn(int rowIndex);
	
	// State queries
	static int getCurrentColumn(int rowIndex);
	static NavigationState getNavigationState(int rowIndex);
	static bool needsDisplayUpdate(int rowIndex);
	static void clearUpdateFlag(int rowIndex);
	
	// Column information
	static String getColumnTitle(int columnIndex);
	static void setColumnTitle(int columnIndex, const String& title);
	
	// Display helpers
	static bool shouldShowColumnTitle(int rowIndex);
	static bool shouldShowContent(int rowIndex);
	static int getDisplayIndexForRowColumn(int rowIndex, int columnIndex);
	
	// Configuration
	static void setColumnDisplayTime(unsigned long timeMs);
	static void resetToColumn(int rowIndex, int columnIndex);
	static void forceDisplayUpdate(int rowIndex);
	
private:
	static RowState rowStates[TOTAL_ROWS];
	static String columnTitles[MAX_COLUMNS];
	static unsigned long columnDisplayTimeMs;
	
	static void updateRowState(int rowIndex);
	static void transitionToContent(int rowIndex);
};

// Default column titles
extern const char* DEFAULT_COLUMN_TITLES[MAX_COLUMNS]; 