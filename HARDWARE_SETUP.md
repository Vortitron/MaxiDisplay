# ChoreTracker Hardware Setup Summary

## User's Actual Hardware Configuration

Based on the user's clarifications, this document outlines the **actual** hardware setup that differs from the initial documentation.

## Physical Layout

```
Hardware Configuration: 2 rows × 4 screens = 8 total displays

┌─────────┬─────────┬─────────┬─────────┐ Row 1 [Select Button: GPIO 17]
│ Disp 0  │ Disp 1  │ Disp 2  │ Disp 3  │
│ Child 1 │ Child 2 │ Weather │ Family  │
└─────────┴─────────┴─────────┴─────────┘
┌─────────┬─────────┬─────────┬─────────┐ Row 2 [Select Button: GPIO 22]
│ Disp 4  │ Disp 5  │ Disp 6  │ Disp 7  │
│ Tasks   │ Tasks   │ Info    │ Tasks   │
└─────────┴─────────┴─────────┴─────────┘

Task Buttons (Non-Momentary + Independent LEDs):
[Button 0: GPIO 13 + LED GPIO 15] [Button 1: GPIO 12 + LED GPIO 2]
[Button 2: GPIO 14 + LED GPIO 0]  [Button 3: GPIO 27 + LED GPIO 4]
```

## Hardware Components

### Button Types

#### 1. Task Buttons (4 total) - NON-MOMENTARY
- **Type**: Latching/toggle switches
- **Behaviour**: Change state when pressed, stay in new position
- **Detection**: Monitor for **state changes**, not button presses
- **LEDs**: Work independently from button position
- **Usage**: Task completion confirmation

**GPIO Pins:**
- Button 0: GPIO 13, LED: GPIO 15
- Button 1: GPIO 12, LED: GPIO 2  
- Button 2: GPIO 14, LED: GPIO 0
- Button 3: GPIO 27, LED: GPIO 4

#### 2. Row Select Buttons (2 total) - MOMENTARY
- **Type**: Traditional momentary push buttons
- **Behaviour**: Generate press events
- **Usage**: Cycle through columns for each row
- **Function**: Show column title → return to content

**GPIO Pins:**
- Row 1 Select: GPIO 17
- Row 2 Select: GPIO 22

### Display Configuration

**8 × ST7789 TFT Displays (170×320 pixels)**
- **CS Pins**: GPIO 9, 10, 1, 3, 39, 36, 37, 38
- **Shared Pins**: SCK(18), MOSI(23), DC(19), RST(-1), BL(8)
- **Layout**: 2 rows × 4 columns

## Navigation System

### Row-Based Operation

Each row operates independently:
- **Row 1** (Displays 0-3): Controlled by select button GPIO 17
- **Row 2** (Displays 4-7): Controlled by select button GPIO 22

### Column Cycling Logic

**When row select button is pressed:**

1. **Cycle to next column** (0→1→2→3→0...)
2. **Show column title** on screen 1 of that row for 2 seconds
3. **Return to content view** showing tasks/info for that column

**Column Definitions:**
- Column 0: "Child 1" / "Emma"
- Column 1: "Child 2" / "Jack"  
- Column 2: "Weather"
- Column 3: "Family"

### Display Update Sequence

**Example: Row 1 select button pressed**

```
Initial: Row 1 showing Column 0 (Child 1) content across displays 0-3

Press Row 1 Select Button:
├── Column advances: 0 → 1 (Child 2)
├── Display 0 shows: "Child 2" title for 2000ms
├── After timeout: All displays 0-3 show Child 2 content
└── Ready for next button press
```

## Implementation Details

### Code Structure

**Files Updated:**
- `esp32/src/config/GPIOConfig.h` - Hardware pin definitions
- `esp32/src/hardware/GPIOManager.h/.cpp` - GPIO control
- `esp32/src/core/NavigationManager.h/.cpp` - Navigation logic
- `README.md` - Documentation updates

### Key Functions

**GPIOManager:**
- `hasTaskButtonChanged(int buttonIndex)` - Detects non-momentary button state changes
- `setTaskLED(int ledIndex, bool state)` - Controls LEDs independently
- `isRowSelectPressed(int rowIndex)` - Detects momentary button presses

**NavigationManager:**
- `handleRowSelect(int rowIndex)` - Processes row selection
- `shouldShowColumnTitle(int rowIndex)` - Determines display mode
- `getDisplayIndexForRowColumn(int row, int col)` - Maps positions

### State Tracking

**Navigation States:**
- `NAV_SHOWING_CONTENT` - Normal task/info display
- `NAV_SHOWING_COLUMN_TITLE` - Temporary title display

**Per-Row State:**
- Current column (0-3)
- Navigation state (content/title)
- Title start time
- Update flags

## Interaction Examples

### Task Completion Flow

```
1. User toggles Task Button 0 (non-momentary switch)
   ├── GPIOManager detects state change
   ├── LED 0 lights up independently
   ├── Task marked complete in system
   └── Home Assistant switch updated

2. User presses Row 2 Select Button
   ├── NavigationManager cycles column: 1 → 2 (Weather)
   ├── Display 4 shows "Weather" title for 2 seconds
   └── Displays 4-7 show weather content
```

### Hardware Differences from Original Design

**Original Plan:**
- 8 momentary switches + 8 LEDs
- 3 screen select buttons + 1 wake button
- Direct button press detection

**Actual Implementation:**
- 4 non-momentary switches + 4 independent LEDs
- 2 row select buttons (momentary)
- State change detection vs. press detection
- Row-based column cycling navigation

## Benefits of This Design

1. **Physical Feedback**: Non-momentary buttons stay in position showing completion state
2. **Independent LEDs**: Can show different information than button position
3. **Intuitive Navigation**: Row-based selection matches physical layout
4. **Fewer Buttons**: 6 total vs. 12 in original design
5. **Clear Mapping**: Each row controlled by its own select button

## Next Steps

1. **Test GPIO Configuration**: Verify all pin assignments work correctly
2. **Display Integration**: Connect NavigationManager to actual TFT displays
3. **Home Assistant Integration**: Link button states to HA switches
4. **Task Management**: Integrate with task parsing and display logic
5. **Hardware Assembly**: Build and test complete system

---

This hardware setup provides an intuitive, physical interface for family chore management with clear visual and tactile feedback. 