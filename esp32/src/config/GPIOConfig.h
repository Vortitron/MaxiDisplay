#pragma once

// ==============================================
// ChoreTracker GPIO Configuration
// Hardware Setup: 2 rows × 4 screens = 8 displays
// ==============================================

// ==============================================
// Task Button Configuration (NON-MOMENTARY)
// ==============================================
// 8 buttons that toggle state - we watch for changes, not presses
// ONE BUTTON PER SCREEN - each screen has its own completion button
#define TASK_BUTTON_PINS {13, 12, 14, 27, 26, 33, 35, 34}  // 8 task completion buttons
#define TOTAL_TASK_BUTTONS 8

// LED Output Pins (INDEPENDENT from button state)
// LEDs are controlled separately from button states
// LED LOGIC: ON = Task Incomplete, OFF = Task Complete
#define TASK_LED_PINS {15, 2, 0, 4, 5, 25, 32, 21}  // 8 LEDs for task buttons
#define TOTAL_TASK_LEDS 8

// ==============================================
// Row Select Buttons (MOMENTARY)
// ==============================================
// 2 momentary buttons for cycling through columns in each row
#define ROW_SELECT_PINS {17, 22}  // Row 1 select, Row 2 select
#define TOTAL_ROW_SELECT_BUTTONS 2

// ==============================================
// TFT Display Configuration
// ==============================================
// 8 displays arranged as 2 rows × 4 columns
#define TFT_CS_PINS {9, 10, 1, 3, 39, 36, 37, 38}  // CS pins for 8 displays
#define TOTAL_TFT_DISPLAYS 8

// Row mapping for displays
#define ROW_1_DISPLAYS {0, 1, 2, 3}  // Screen indices for row 1
#define ROW_2_DISPLAYS {4, 5, 6, 7}  // Screen indices for row 2

// TFT Shared Pins (all displays)
#define TFT_SCK 18       // SPI Clock
#define TFT_MOSI 23      // SPI MOSI
#define TFT_DC 19        // Data/Command
#define TFT_RST -1       // Connected to ESP32 reset pin
#define TFT_BL 8         // Backlight control (all displays)

// ==============================================
// Navigation Logic Configuration
// ==============================================
// Column titles for navigation display
#define MAX_COLUMNS 4
#define COLUMN_DISPLAY_TIME_MS 2000  // Show column title for 2 seconds

// Column name mappings (can be configured at runtime)
#define DEFAULT_COLUMN_NAMES {"Child 1", "Child 2", "Weather", "Family"}

// ==============================================
// Task Button to Display Mapping
// ==============================================
// Each task button corresponds to one display/screen
// Button 0 → Display 0, Button 1 → Display 1, etc.
#define GET_DISPLAY_FOR_BUTTON(button_idx) (button_idx)
#define GET_BUTTON_FOR_DISPLAY(display_idx) (display_idx)

// ==============================================
// LED Logic Configuration
// ==============================================
// LED states for task completion
#define LED_STATE_TASK_INCOMPLETE HIGH   // LED ON = task not done
#define LED_STATE_TASK_COMPLETE LOW      // LED OFF = task complete

// ==============================================
// Spare GPIO for Future Expansion
// ==============================================
#define SPARE_GPIO_PINS {11, 16, 7}  // Available for buzzer, wake button, etc.
#define AUDIO_PIN 11     // Reserved for audio feedback

// ==============================================
// Hardware Validation Constants
// ==============================================
// Pin counts for compile-time validation
static_assert(TOTAL_TASK_BUTTONS == TOTAL_TASK_LEDS, 
              "Number of task buttons must equal number of task LEDs");

static_assert(TOTAL_TASK_BUTTONS == TOTAL_TFT_DISPLAYS, 
              "Number of task buttons must equal number of displays");

static_assert(TOTAL_TFT_DISPLAYS == 8, 
              "This configuration is designed for exactly 8 displays");

static_assert(TOTAL_ROW_SELECT_BUTTONS == 2, 
              "This configuration requires exactly 2 row select buttons");

// ==============================================
// Display Layout Helper Macros
// ==============================================
#define GET_ROW_FROM_DISPLAY(display_idx) ((display_idx) / 4)
#define GET_COL_FROM_DISPLAY(display_idx) ((display_idx) % 4)
#define GET_DISPLAY_IDX(row, col) ((row) * 4 + (col))

// Row 1: displays 0-3, Row 2: displays 4-7
#define IS_ROW_1(display_idx) ((display_idx) < 4)
#define IS_ROW_2(display_idx) ((display_idx) >= 4)

// ==============================================
// Input Handling Configuration
// ==============================================
// Debounce settings for buttons
#define BUTTON_DEBOUNCE_MS 50
#define ROW_SELECT_DEBOUNCE_MS 200  // Longer debounce for row selection

// Button state tracking
#define TASK_BUTTON_ACTIVE_STATE LOW   // Buttons are active low with pullup
#define ROW_SELECT_ACTIVE_STATE LOW    // Momentary buttons active low

// ==============================================
// Navigation State Configuration
// ==============================================
enum NavigationState {
	NAV_SHOWING_CONTENT,     // Normal content display
	NAV_SHOWING_COLUMN_TITLE // Showing column title after select press
};

struct RowState {
	int currentColumn;       // 0-3, which column is currently selected
	unsigned long titleStartTime;  // When column title started showing
	NavigationState navState;       // Current navigation state
};

// ==============================================
// Hardware Interface Class Declaration
// ==============================================
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
