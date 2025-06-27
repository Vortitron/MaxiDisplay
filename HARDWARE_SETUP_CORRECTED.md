# ChoreTracker Corrected Hardware Setup

## ✅ ACTUAL Hardware Configuration (Corrected)

This document reflects the **user's actual hardware setup** after clarification.

## Physical Layout

```
Hardware: 8 TFT displays + 8 task buttons + 8 LEDs + 2 row select buttons

┌─────────┬─────────┬─────────┬─────────┐ Row 1 [Select Button: GPIO 17]
│ Disp 0  │ Disp 1  │ Disp 2  │ Disp 3  │
│ Child 1 │ Child 2 │ Weather │ Family  │
│[Btn0+LED]│[Btn1+LED]│[Btn2+LED]│[Btn3+LED]│
└─────────┴─────────┴─────────┴─────────┘
┌─────────┬─────────┬─────────┬─────────┐ Row 2 [Select Button: GPIO 22]
│ Disp 4  │ Disp 5  │ Disp 6  │ Disp 7  │
│ Tasks   │ Tasks   │ Info    │ Tasks   │
│[Btn4+LED]│[Btn5+LED]│[Btn6+LED]│[Btn7+LED]│
└─────────┴─────────┴─────────┴─────────┘
```

## Hardware Components

### 1. Task Buttons & LEDs (8 total sets)

**Configuration**: One non-momentary button + one LED per display

| Display | Button GPIO | LED GPIO | Function |
|---------|-------------|----------|----------|
| Display 0 | GPIO 13 | GPIO 15 | Task completion for Display 0 |
| Display 1 | GPIO 12 | GPIO 2  | Task completion for Display 1 |
| Display 2 | GPIO 14 | GPIO 0  | Task completion for Display 2 |
| Display 3 | GPIO 27 | GPIO 4  | Task completion for Display 3 |
| Display 4 | GPIO 26 | GPIO 5  | Task completion for Display 4 |
| Display 5 | GPIO 33 | GPIO 25 | Task completion for Display 5 |
| Display 6 | GPIO 35 | GPIO 32 | Task completion for Display 6 |
| Display 7 | GPIO 34 | GPIO 21 | Task completion for Display 7 |

### 2. LED Logic (INVERTED)

**Critical**: The LED logic is **inverted** from typical expectations:

- **LED ON** = Task is **INCOMPLETE** (needs to be done)
- **LED OFF** = Task is **COMPLETE** (finished)

**Startup State**: All LEDs ON (all tasks incomplete)

### 3. Button Behaviour (Non-Momentary)

- **Type**: Latching/toggle switches
- **Detection**: Monitor for **state changes**, not button presses
- **Behaviour**: Switch stays in new position when pressed
- **Action**: When button state changes → mark task complete → LED turns OFF

### 4. Row Select Buttons (2 total)

| Row | Button GPIO | Function |
|-----|-------------|----------|
| Row 1 | GPIO 17 | Cycle columns for displays 0-3 |
| Row 2 | GPIO 22 | Cycle columns for displays 4-7 |

### 5. TFT Displays (8 total)

| Display | CS GPIO | Position | Function |
|---------|---------|----------|----------|
| Display 0 | GPIO 9  | Row 1, Col 1 | Content for current column |
| Display 1 | GPIO 10 | Row 1, Col 2 | Content for current column |
| Display 2 | GPIO 1  | Row 1, Col 3 | Content for current column |
| Display 3 | GPIO 3  | Row 1, Col 4 | Content for current column |
| Display 4 | GPIO 39 | Row 2, Col 1 | Content for current column |
| Display 5 | GPIO 36 | Row 2, Col 2 | Content for current column |
| Display 6 | GPIO 37 | Row 2, Col 3 | Content for current column |
| Display 7 | GPIO 38 | Row 2, Col 4 | Content for current column |

**Shared TFT Pins**: SCK(18), MOSI(23), DC(19), RST(-1), Backlight(8)

## Operation Logic

### Task Completion Flow

```
Initial State: All LEDs ON (all tasks incomplete)

User presses Task Button 3 (for Display 3):
├── 1. GPIOManager detects state change on GPIO 27
├── 2. System marks task for Display 3 as COMPLETE
├── 3. LED 3 (GPIO 4) turns OFF automatically
├── 4. Display 3 updates to show completion status
└── 5. Home Assistant switch updated

Result: LED OFF indicates task completion
```

### Navigation Flow

```
Row 1 Navigation (Controls Displays 0-3):

User presses Row 1 Select Button (GPIO 17):
├── 1. NavigationManager cycles column: 0→1→2→3→0...
├── 2. Display 0 shows column title "Child 2" for 2 seconds
├── 3. All displays 0-3 show content for "Child 2" column
└── 4. Each display can still be completed independently via its button

Row 2 Navigation (Controls Displays 4-7):
Same logic but controlled by GPIO 22 for displays 4-7
```

### LED Status Examples

```
All tasks incomplete (startup):
[LED0:ON] [LED1:ON] [LED2:ON] [LED3:ON] [LED4:ON] [LED5:ON] [LED6:ON] [LED7:ON]

Child completes task on Display 1:
[LED0:ON] [LED1:OFF] [LED2:ON] [LED3:ON] [LED4:ON] [LED5:ON] [LED6:ON] [LED7:ON]

All tasks complete:
[LED0:OFF] [LED1:OFF] [LED2:OFF] [LED3:OFF] [LED4:OFF] [LED5:OFF] [LED6:OFF] [LED7:OFF]
```

## Code Implementation

### Key Functions

**GPIOManager.cpp** handles the corrected logic:

```cpp
// LED control with inverted logic
void setTaskComplete(int ledIndex) {
    digitalWrite(taskLEDPins[ledIndex], LOW);  // LED OFF = complete
}

void setTaskIncomplete(int ledIndex) {
    digitalWrite(taskLEDPins[ledIndex], HIGH); // LED ON = incomplete
}

// Automatic handling when button changes
if (buttonStateChanged) {
    if (buttonActive) {
        setTaskComplete(buttonIndex);  // Turn LED OFF
    } else {
        setTaskIncomplete(buttonIndex); // Turn LED ON  
    }
}
```

### Button to Display Mapping

**1:1 Direct Mapping**:
- Button 0 → Display 0
- Button 1 → Display 1
- ...
- Button 7 → Display 7

Each button directly controls the completion state of its corresponding display.

## Benefits of This Design

1. **Clear Visual Feedback**: LED state clearly shows task completion
2. **One-to-One Mapping**: Each display has its own completion button
3. **Physical Persistence**: Non-momentary buttons stay in completion position
4. **Intuitive LED Logic**: LED OFF = done (like turning off a light when finished)
5. **Independent Control**: Each task can be completed individually
6. **Row-Based Navigation**: Logical grouping with row select buttons

## Key Differences from Initial Understanding

| Aspect | Initial (Incorrect) | Corrected (Actual) |
|--------|-------------------|-------------------|
| **Button Count** | 4 buttons | 8 buttons (one per display) |
| **LED Logic** | LED ON = complete | LED ON = incomplete |
| **LED State** | LED OFF = incomplete | LED OFF = complete |
| **Mapping** | 4 buttons for 8 displays | 1:1 button to display mapping |
| **Startup State** | All LEDs OFF | All LEDs ON |

## Testing Checklist

- ✅ 8 task buttons configured with pullup resistors
- ✅ 8 LEDs configured as outputs, initially HIGH
- ✅ State change detection (not press detection)
- ✅ LED turns OFF when task completed
- ✅ LED turns ON when task reset
- ✅ 2 row select buttons for navigation
- ✅ 8 TFT displays with correct CS pins
- ✅ 1:1 button-to-display mapping

This corrected configuration provides clear, intuitive task management with proper visual feedback through the inverted LED logic. 