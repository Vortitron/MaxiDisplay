# ChoreTracker Configuration Guide

## Overview

ChoreTracker uses a flexible, environment-based configuration system that supports:
- ✅ **Secure credential management** with `.env` files
- ✅ **Scalable display support** from 1 to 8 displays
- ✅ **Multiple configuration sources** (environment, SPIFFS, preferences)
- ✅ **Runtime configuration updates**
- ✅ **Compile-time validation**

## 🔐 Environment Configuration (.env)

### Security First

**NEVER commit `.env` files to git!** They contain sensitive credentials.

### Setup Process

1. **Copy the template**:
   ```bash
   cp env.example .env
   ```

2. **Edit `.env` with your actual values**:
   ```bash
   nano .env
   ```

3. **Verify `.env` is in `.gitignore`** (it is by default)

### Complete .env Configuration

```bash
# ======================
# WiFi Configuration
# ======================
WIFI_SSID=YourWiFiNetworkName
WIFI_PASSWORD=YourSecurePassword
WIFI_TIMEOUT_MS=20000

# ======================
# Home Assistant Configuration
# ======================
HA_SERVER=http://homeassistant.local:8123
HA_TOKEN=your_long_lived_access_token_here
HA_DEVICE_ID=choretracker_001
HA_TIMEOUT_MS=5000

# ======================
# Display Configuration - SCALABLE!
# ======================
DISPLAY_COUNT=4  # Supports 1 to 8 displays
DISPLAY_WIDTH=170
DISPLAY_HEIGHT=320
DISPLAY_ROTATION=1
DISPLAY_BRIGHTNESS=200

# ======================
# Hardware GPIO Configuration
# ======================
# Pins must match DISPLAY_COUNT (comma-separated)
TFT_CS_PINS=9,10,1,3
SWITCH_PINS=13,12,14,27
LED_PINS=15,2,0,4

# Shared TFT pins
TFT_SCK=18
TFT_MOSI=23
TFT_DC=19
TFT_RST=-1
TFT_BACKLIGHT=8

# Button pins
WAKE_BUTTON_PIN=16
SCREEN_BUTTON_PINS=17,22,7

# ======================
# Optional Features
# ======================
# Weather API
WEATHER_API_KEY=your_openweather_api_key
WEATHER_CITY=London,GB
WEATHER_ENABLED=true

# Audio feedback
AUDIO_ENABLED=false
AUDIO_PIN=11
AUDIO_VOLUME=128

# MQTT (optional)
MQTT_ENABLED=false
MQTT_SERVER=homeassistant.local
MQTT_PORT=1883
MQTT_USERNAME=your_mqtt_username
MQTT_PASSWORD=your_mqtt_password

# ======================
# Debug Configuration
# ======================
DEBUG_MODE=true
SERIAL_SPEED=115200
LOG_LEVEL=INFO
```

## 🖥️ Scalable Display Support

### Single Display Setup (Minimal)

Perfect for testing or simple installations:

```bash
DISPLAY_COUNT=1
TFT_CS_PINS=9
SWITCH_PINS=13
LED_PINS=15
SCREEN_BUTTON_PINS=  # Empty - only wake button needed
```

**Hardware Requirements**:
- 1x ST7789 TFT display
- 1x latching switch with LED
- 1x wake button
- ESP32 + power supply

### Dual Display Setup

Great for two children:

```bash
DISPLAY_COUNT=2
TFT_CS_PINS=9,10
SWITCH_PINS=13,12
LED_PINS=15,2
SCREEN_BUTTON_PINS=17  # One screen button
```

**Hardware Requirements**:
- 2x ST7789 TFT displays
- 2x latching switches with LEDs
- 2x buttons (wake + screen 1)

### Quad Display Setup (Recommended)

Balanced setup for families:

```bash
DISPLAY_COUNT=4
TFT_CS_PINS=9,10,1,3
SWITCH_PINS=13,12,14,27
LED_PINS=15,2,0,4
SCREEN_BUTTON_PINS=17,22,7  # Three screen buttons
```

**Hardware Requirements**:
- 4x ST7789 TFT displays
- 4x latching switches with LEDs
- 4x buttons (wake + 3 screen selectors)

### Full Display Setup (Maximum)

Complete 8-display installation:

```bash
DISPLAY_COUNT=8
TFT_CS_PINS=9,10,1,3,39,36,37,38
SWITCH_PINS=13,12,14,27,26,33,35,34
LED_PINS=15,2,0,4,5,25,32,21
SCREEN_BUTTON_PINS=17,22,7  # Max 3 screen buttons
```

**Hardware Requirements**:
- 8x ST7789 TFT displays
- 8x latching switches with LEDs
- 4x buttons (wake + 3 screen selectors)
- **Note**: Use wake button to cycle through screens 4-7

## 🔧 Build Configuration

### Custom Display Count

Build firmware for specific display counts:

```bash
cd esp32

# Build for 1 display
pio run --build-flag="-DDISPLAY_COUNT=1"

# Build for 2 displays
pio run --build-flag="-DDISPLAY_COUNT=2"

# Build for 4 displays (recommended)
pio run --build-flag="-DDISPLAY_COUNT=4"

# Build for 8 displays (maximum)
pio run --build-flag="-DDISPLAY_COUNT=8"

# Upload with custom count
pio run -t upload --build-flag="-DDISPLAY_COUNT=4"
```

### Environment Validation

The system validates configuration at compile time:

```cpp
// Automatic validation
static_assert(DISPLAY_COUNT >= 1 && DISPLAY_COUNT <= 8, 
              "DISPLAY_COUNT must be between 1 and 8");

static_assert(DISPLAY_WIDTH > 0 && DISPLAY_HEIGHT > 0, 
              "Display dimensions must be positive");
```

## 🏠 Home Assistant Configuration

### Long-Lived Access Token

1. **Generate token in Home Assistant**:
   - Profile → Long-Lived Access Tokens
   - Create new token
   - Copy token to `.env` file

2. **Update `.env`**:
   ```bash
   HA_TOKEN=your_very_long_token_here
   ```

### Device Discovery

The system supports automatic device discovery:

```bash
# Device identification
HA_DEVICE_ID=choretracker_001
HA_DISCOVERY_PREFIX=homeassistant
HA_ENTITY_PREFIX=choretracker
```

## 📱 Runtime Configuration

### Configuration Sources (Priority Order)

1. **Runtime Settings** (highest priority)
2. **Preferences (NVS)** - persistent across reboots
3. **SPIFFS Config File** - `/config.json`
4. **Environment Variables** - compile-time
5. **Default Values** (lowest priority)

### Configuration Management

```cpp
// Load configuration
ConfigLoader::initialize();
ConfigLoader::loadConfiguration();

// Update at runtime
ConfigLoader::setWiFiSSID("NewNetwork");
ConfigLoader::setDisplayCount(6);
ConfigLoader::saveConfiguration();

// Validate
if (!ConfigLoader::validateConfiguration()) {
    Serial.println("Configuration invalid!");
}
```

## 🛠️ GPIO Pin Allocation

### Pin Usage by Display Count

| Display Count | TFT CS Pins | Switch Pins | LED Pins | Screen Buttons |
|---------------|-------------|-------------|----------|----------------|
| 1 | 9 | 13 | 15 | Wake (16) |
| 2 | 9,10 | 13,12 | 15,2 | Wake (16), Screen1 (17) |
| 4 | 9,10,1,3 | 13,12,14,27 | 15,2,0,4 | Wake (16), Screen1-3 (17,22,7) |
| 8 | 9,10,1,3,39,36,37,38 | 13,12,14,27,26,33,35,34 | 15,2,0,4,5,25,32,21 | Wake (16), Screen1-3 (17,22,7) |

### Shared Pins (All Configurations)

- **TFT SCK**: GPIO 18
- **TFT MOSI**: GPIO 23
- **TFT DC**: GPIO 19
- **TFT RST**: Connected to ESP32 reset
- **TFT Backlight**: GPIO 8 (all displays)
- **Audio**: GPIO 11 (spare/expansion)

## ⚠️ Important Notes

### Security

- **Never commit `.env` files**
- Use strong WiFi passwords
- Rotate HA tokens regularly
- Keep firmware updated

### Hardware Validation

- Verify GPIO pin assignments don't conflict
- Test with minimal setup first (1-2 displays)
- Use multimeter to check connections
- Monitor power consumption

### Performance

- More displays = more memory usage
- Consider power supply capacity
- Test with realistic task loads
- Monitor ESP32 temperature

## 🔍 Troubleshooting

### Configuration Issues

```bash
# Check configuration
grep -v "^#" .env | grep -v "^$"

# Validate GPIO pins
esp32/examples/gpio_configs.md

# Test minimal setup
DISPLAY_COUNT=1 pio run -t upload
```

### Common Problems

1. **WiFi Connection Fails**
   - Check SSID and password in `.env`
   - Verify 2.4GHz network
   - Check signal strength

2. **Display Issues**
   - Verify GPIO pin assignments
   - Check TFT wiring
   - Test with single display first

3. **HA Integration Issues**
   - Validate access token
   - Check HA server URL
   - Verify network connectivity

### Debug Mode

Enable detailed logging:

```bash
DEBUG_MODE=true
LOG_LEVEL=DEBUG
SERIAL_SPEED=115200
```

This configuration system provides maximum flexibility while maintaining security and ease of use! 🎉 