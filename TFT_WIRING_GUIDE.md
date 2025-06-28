# TFT Display Wiring Guide

## Your TFT Display Pin Labels

Your ST7789 displays have these pin labels:
```
GND VCC SCL SDA RES DC CS BLK
```

## Pin Function Translation

Even though your displays use **I2C-style labels**, they are **SPI devices**:

| TFT Pin | Function | ESP32 Connection | Notes |
|---------|----------|------------------|-------|
| **GND** | Ground | ESP32 GND | Common ground |
| **VCC** | Power | ESP32 3.3V | 3.3V power supply |
| **SCL** | SPI Clock | ESP32 GPIO 18 | Same as SCK/SCLK |
| **SDA** | SPI Data | ESP32 GPIO 23 | Same as MOSI/DIN |
| **RES** | Reset | ESP32 Reset Pin | Hardware reset |
| **DC** | Data/Command | ESP32 GPIO 19 | Command/Data select |
| **CS** | Chip Select | Individual GPIOs | Per-display selection |
| **BLK** | Backlight | ESP32 GPIO 8 | Backlight control |

## Complete Wiring Connections

### Shared Connections (All 8 Displays)
Connect these pins from **all displays** to the same ESP32 pins:

```
ESP32 GPIO 18 → All TFT SCL pins (SPI Clock)
ESP32 GPIO 23 → All TFT SDA pins (SPI Data)  
ESP32 GPIO 19 → All TFT DC pins (Data/Command)
ESP32 Reset   → All TFT RES pins (Reset)
ESP32 GPIO 8  → All TFT BLK pins (Backlight)
ESP32 3.3V    → All TFT VCC pins (Power)
ESP32 GND     → All TFT GND pins (Ground)
```

### Individual CS Connections
Each display needs its own CS connection:

```
Display 0: CS → ESP32 GPIO 9
Display 1: CS → ESP32 GPIO 10
Display 2: CS → ESP32 GPIO 1
Display 3: CS → ESP32 GPIO 3
Display 4: CS → ESP32 GPIO 39
Display 5: CS → ESP32 GPIO 36
Display 6: CS → ESP32 GPIO 37
Display 7: CS → ESP32 GPIO 38
```

## Why SCL/SDA Labels on SPI Displays?

This is **completely normal**! Many manufacturers use I2C-style labels even for SPI displays because:

1. **Cost Savings**: Same PCB design for multiple interfaces
2. **Familiarity**: SCL/SDA are well-known pin names
3. **Function Similarity**: Both are clock/data signals

## Wiring Verification

Use this checklist to verify your connections:

### Power & Ground ✓
- [ ] All VCC pins connected to ESP32 3.3V
- [ ] All GND pins connected to ESP32 GND

### Shared SPI Signals ✓
- [ ] All SCL pins connected to ESP32 GPIO 18
- [ ] All SDA pins connected to ESP32 GPIO 23
- [ ] All DC pins connected to ESP32 GPIO 19
- [ ] All RES pins connected to ESP32 Reset pin

### Individual Signals ✓
- [ ] All BLK pins connected to ESP32 GPIO 8
- [ ] Each CS pin connected to its assigned ESP32 GPIO

### Display Assignment ✓
```
Row 1: Display 0(CS=9), Display 1(CS=10), Display 2(CS=1), Display 3(CS=3)
Row 2: Display 4(CS=39), Display 5(CS=36), Display 6(CS=37), Display 7(CS=38)
```

## Code Configuration

Your `esp32/src/config/GPIOConfig.h` file correctly maps these connections:

```cpp
// TFT Shared Pins (all displays)
#define TFT_SCK 18       // SPI Clock (connect to TFT SCL pins)
#define TFT_MOSI 23      // SPI Data (connect to TFT SDA pins)
#define TFT_DC 19        // Data/Command (connect to TFT DC pins)
#define TFT_RST -1       // Connected to ESP32 reset pin (connect to TFT RES pins)
#define TFT_BL 8         // Backlight control (connect to TFT BLK pins)

// Individual CS pins
#define TFT_CS_PINS {9, 10, 1, 3, 39, 36, 37, 38}
```

## Testing Your Connections

1. **Power Test**: Measure 3.3V on all VCC pins
2. **Continuity Test**: Verify all shared connections with multimeter
3. **Individual Test**: Check each CS connection
4. **Display Test**: Upload test firmware to verify each display works

## Common Issues

### Display Not Working
- Check VCC/GND connections first
- Verify SCL/SDA are not swapped
- Ensure CS pins are correctly assigned

### Some Displays Work, Others Don't
- Check individual CS connections
- Verify power supply can handle all displays
- Test each display individually

### Flickering or Corruption
- Check for loose connections
- Verify power supply stability
- Add decoupling capacitors if needed

## Summary

Your displays are **SPI devices** with **I2C-style pin labels**. The key connections are:

- **SCL** → ESP32 GPIO 18 (SPI Clock)
- **SDA** → ESP32 GPIO 23 (SPI Data)
- **Individual CS pins** for display selection

This is a standard configuration and will work perfectly with the ChoreTracker code! 🎯 