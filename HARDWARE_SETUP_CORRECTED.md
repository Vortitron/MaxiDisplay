# ChoreTracker Hardware Setup - CORRECTED

## Final Hardware Configuration

Based on user requirements:
- **2 rows of 4 screens each** (8 total displays)
- **4 buttons with LEDs** (non-momentary, state change detection)
- **LEDs work independently** from button position
- **One select button per row** (momentary)
- **Select button cycles through columns**, showing child name/"weather" on screen 1 of row for few seconds

## Hardware Components

### ESP32 DevKitC V4
- **MCU**: ESP32-WROOM-32 module
- **GPIO**: 38 pins available
- **Power**: 5V USB input, 3.3V logic

### 8 × ST7789 TFT Displays (170×320 pixels)
**Display Pin Labels**: GND VCC SCL SDA RES DC CS BLK

**Wiring Connections**:
- **GND**: Ground (ESP32 GND)
- **VCC**: Power (ESP32 3.3V)
- **SCL**: SPI Clock (ESP32 GPIO 18) - *Same as SCLK/SCK*
- **SDA**: SPI Data (ESP32 GPIO 23) - *Same as MOSI/DIN*
- **RES**: Reset (ESP32 Reset Pin) - *Connected to ESP32 reset, not GPIO*
- **DC**: Data/Command (ESP32 GPIO 19)
- **CS**: Chip Select (Individual GPIOs: 9,10,1,3,39,36,37,38)
- **BLK**: Backlight (ESP32 GPIO 8) - *All displays shared*

### 8 × Task Buttons with LEDs (Non-Momentary)
- **Button Type**: Latching/toggle switches
- **Detection**: State change monitoring (not press detection)
- **LEDs**: Independent control from button state
- **Logic**: LED ON = Task Incomplete, LED OFF = Task Complete

### 2 × Row Select Buttons (Momentary)
- **Function**: Cycle through columns in each row
- **Row 1 Select**: GPIO 17
- **Row 2 Select**: GPIO 22

## Complete GPIO Assignment

### Task Buttons (Input with Pullup)
```
Button 0 (Display 0): GPIO 13 → LED GPIO 15
Button 1 (Display 1): GPIO 12 → LED GPIO 2
Button 2 (Display 2): GPIO 14 → LED GPIO 0
Button 3 (Display 3): GPIO 27 → LED GPIO 4
Button 4 (Display 4): GPIO 26 → LED GPIO 5
Button 5 (Display 5): GPIO 33 → LED GPIO 25
Button 6 (Display 6): GPIO 35 → LED GPIO 32
Button 7 (Display 7): GPIO 34 → LED GPIO 21
```

### TFT Display Connections
**Individual CS Pins**:
```
Display 0: CS GPIO 9
Display 1: CS GPIO 10  
Display 2: CS GPIO 1
Display 3: CS GPIO 3
Display 4: CS GPIO 39
Display 5: CS GPIO 36
Display 6: CS GPIO 37
Display 7: CS GPIO 38
```

**Shared TFT Pins**: SCL(18), SDA(23), DC(19), RES(Reset), Backlight(8)

### Row Select Buttons (Input with Pullup)
```
Row 1 Select: GPIO 17
Row 2 Select: GPIO 22
```

### Spare GPIO
```
GPIO 11: Audio/Buzzer (reserved)
GPIO 16: Wake button (optional)
GPIO 7:  Expansion
```

## Physical Layout

```
Row 1: [Select Button GPIO 17]
┌─────────┬─────────┬─────────┬─────────┐
│ Disp 0  │ Disp 1  │ Disp 2  │ Disp 3  │
│ CS=9    │ CS=10   │ CS=1    │ CS=3    │
│ Btn=13  │ Btn=12  │ Btn=14  │ Btn=27  │
│ LED=15  │ LED=2   │ LED=0   │ LED=4   │
└─────────┴─────────┴─────────┴─────────┘

Row 2: [Select Button GPIO 22]  
┌─────────┬─────────┬─────────┬─────────┐
│ Disp 4  │ Disp 5  │ Disp 6  │ Disp 7  │
│ CS=39   │ CS=36   │ CS=37   │ CS=38   │
│ Btn=26  │ Btn=33  │ Btn=35  │ Btn=34  │
│ LED=5   │ LED=25  │ LED=32  │ LED=21  │
└─────────┴─────────┴─────────┴─────────┘
```

## Wiring Instructions

### TFT Display Wiring (All 8 Displays)

**Shared Connections** (connect all displays):
```
ESP32 GPIO 18 → All TFT SCL pins (SPI Clock)
ESP32 GPIO 23 → All TFT SDA pins (SPI Data)
ESP32 GPIO 19 → All TFT DC pins (Data/Command)
ESP32 Reset   → All TFT RES pins (Reset)
ESP32 GPIO 8  → All TFT BLK pins (Backlight)
ESP32 3.3V    → All TFT VCC pins (Power)
ESP32 GND     → All TFT GND pins (Ground)
```

**Individual CS Connections**:
```
ESP32 GPIO 9  → Display 0 CS
ESP32 GPIO 10 → Display 1 CS
ESP32 GPIO 1  → Display 2 CS
ESP32 GPIO 3  → Display 3 CS
ESP32 GPIO 39 → Display 4 CS
ESP32 GPIO 36 → Display 5 CS
ESP32 GPIO 37 → Display 6 CS
ESP32 GPIO 38 → Display 7 CS
```

### Task Button & LED Wiring

**Buttons** (with internal pullup enabled):
```
Button 0: GPIO 13 (INPUT_PULLUP)
Button 1: GPIO 12 (INPUT_PULLUP)
Button 2: GPIO 14 (INPUT_PULLUP)
Button 3: GPIO 27 (INPUT_PULLUP)
Button 4: GPIO 26 (INPUT_PULLUP)
Button 5: GPIO 33 (INPUT_PULLUP)
Button 6: GPIO 35 (INPUT_PULLUP) - Note: Input only
Button 7: GPIO 34 (INPUT_PULLUP) - Note: Input only
```

**LEDs** (with current limiting resistors):
```
LED 0: GPIO 15 → 220Ω resistor → LED → GND
LED 1: GPIO 2  → 220Ω resistor → LED → GND
LED 2: GPIO 0  → 220Ω resistor → LED → GND
LED 3: GPIO 4  → 220Ω resistor → LED → GND
LED 4: GPIO 5  → 220Ω resistor → LED → GND
LED 5: GPIO 25 → 220Ω resistor → LED → GND
LED 6: GPIO 32 → 220Ω resistor → LED → GND
LED 7: GPIO 21 → 220Ω resistor → LED → GND
```

### Row Select Buttons

```
Row 1 Select: GPIO 17 (INPUT_PULLUP) → Momentary button → GND
Row 2 Select: GPIO 22 (INPUT_PULLUP) → Momentary button → GND
```

## Important Notes

### TFT Pin Label Translation
Your displays show **I2C-style labels** but are **SPI devices**:
- **SCL** = SPI Clock (same as SCK/SCLK)
- **SDA** = SPI Data (same as MOSI/DIN)

This is normal for many ST7789 displays!

### GPIO Limitations
- **GPIO 35, 34**: Input only (cannot drive LEDs)
- **GPIO 1, 3**: Used for USB/Serial during programming
- **GPIO 39, 36**: Input only but can be used for CS (output)

### Power Considerations
- **Total Current**: ~460mA (8 displays + LEDs)
- **Supply**: 1A USB recommended
- **Backlight Control**: All displays share GPIO 8

### Reset Connection
- **TFT RES pins** connect to **ESP32 Reset pin** (not a GPIO)
- This automatically resets displays when ESP32 resets
- Saves one GPIO pin for other uses

## Testing Procedure

1. **Power Test**: Verify 3.3V on all VCC pins
2. **Display Test**: Check each display individually
3. **Button Test**: Verify state change detection
4. **LED Test**: Confirm independent LED control
5. **Navigation Test**: Test row select functionality

## Code Configuration

The hardware matches this configuration in `esp32/src/config/GPIOConfig.h`:

```cpp
// TFT Shared Pins (note the pin label translation)
#define TFT_SCK 18       // Connected to TFT SCL pins
#define TFT_MOSI 23      // Connected to TFT SDA pins  
#define TFT_DC 19        // Connected to TFT DC pins
#define TFT_RST -1       // Connected to ESP32 reset pin
#define TFT_BL 8         // Connected to TFT BLK pins

// Individual CS pins
#define TFT_CS_PINS {9, 10, 1, 3, 39, 36, 37, 38}

// Task buttons and LEDs
#define TASK_BUTTON_PINS {13, 12, 14, 27, 26, 33, 35, 34}
#define TASK_LED_PINS {15, 2, 0, 4, 5, 25, 32, 21}

// Row select buttons  
#define ROW_SELECT_PINS {17, 22}
```

This hardware configuration provides a robust, scalable chore tracking system with intuitive navigation and independent task completion tracking! 🎯 