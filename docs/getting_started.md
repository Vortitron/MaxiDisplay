# ChoreTracker Getting Started Guide

Welcome to ChoreTracker! This guide will help you set up and start using the ESP32-based chore tracking system with Home Assistant integration.

## Quick Start

### 1. Initial Setup

```bash
# Clone the repository
git clone https://github.com/yourusername/choretracker.git
cd ChoreTracker

# Run setup script
./scripts/setup.sh
```

### 2. ESP32 Development

```bash
# Install PlatformIO
pip install platformio

# Build and upload firmware
cd esp32
pio run -t upload
```

### 3. Home Assistant Integration

```bash
# Set up Python environment
cd homeassistant
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt

# Copy integration to Home Assistant
cp -r custom_components/choretracker /path/to/homeassistant/custom_components/
```

## Hardware Requirements

- ESP32 DevKitC V4
- 8x ST7789 TFT displays (1.9", 170x320)
- 8x latching switches with LEDs
- 4x momentary buttons
- Power supply (1A USB, 5V to 3.3V)

## Configuration

### ESP32 Configuration

The ChoreTracker uses a flexible environment-based configuration system:

#### 1. Environment Configuration (.env)

Copy the template and update with your values:
```bash
cp env.example .env
```

Edit `.env` file (never commit this to git):
```bash
# WiFi Configuration
WIFI_SSID=YourWiFiNetworkName
WIFI_PASSWORD=YourWiFiPassword

# Home Assistant Configuration
HA_SERVER=http://homeassistant.local:8123
HA_TOKEN=your_long_lived_access_token_here

# Display Configuration - SCALABLE!
DISPLAY_COUNT=4  # Supports 1 to 8 displays
TFT_CS_PINS=9,10,1,3
SWITCH_PINS=13,12,14,27
LED_PINS=15,2,0,4

# Optional: Weather API
WEATHER_API_KEY=your_openweather_api_key
WEATHER_ENABLED=true
```

#### 2. Scalable Display Support

The system now supports any number of displays from 1 to 8:

**Single Display (Minimal Setup)**:
```bash
DISPLAY_COUNT=1
TFT_CS_PINS=9
SWITCH_PINS=13
LED_PINS=15
```

**Dual Display Setup**:
```bash
DISPLAY_COUNT=2
TFT_CS_PINS=9,10
SWITCH_PINS=13,12
LED_PINS=15,2
```

**Full 8-Display Setup**:
```bash
DISPLAY_COUNT=8
TFT_CS_PINS=9,10,1,3,39,36,37,38
SWITCH_PINS=13,12,14,27,26,33,35,34
LED_PINS=15,2,0,4,5,25,32,21
```

#### 3. Build with Custom Display Count

```bash
# Build for specific display count
cd esp32
pio run --build-flag="-DDISPLAY_COUNT=4"

# Upload firmware
pio run -t upload --build-flag="-DDISPLAY_COUNT=4"
```

### Home Assistant Configuration

1. Generate a long-lived access token
2. Set up calendar integration
3. Configure task templates
4. Create automation rules

## First Run

1. Power on the ESP32
2. Check serial monitor for connection status
3. Verify displays show startup screen
4. Test switch functionality
5. Check Home Assistant integration

## Troubleshooting

### Common Issues

- **WiFi Connection**: Check SSID and password
- **Display Problems**: Verify SPI wiring
- **Switch Issues**: Check GPIO assignments
- **HA Integration**: Verify API token

### Debug Mode

Enable debug output in `esp32/src/config/Config.h`:
```cpp
#define DEBUG_MODE true
```

## Next Steps

- Configure calendar events for task management
- Set up automation templates
- Customise display themes
- Add additional family members

For detailed documentation, see the project wiki and documentation folders.

Happy tracking! 🏠✨ 