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

Update `esp32/src/config/Config.h`:

```cpp
#define WIFI_SSID "YourWiFiName"
#define WIFI_PASSWORD "YourWiFiPassword"
#define HA_SERVER "http://homeassistant.local:8123"
#define HA_TOKEN "your_long_lived_access_token"
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