#!/bin/bash

# ChoreTracker Project Setup Script
# This script initialises the development environment for both ESP32 and Home Assistant components

set -e  # Exit on error

echo "🏠 ChoreTracker Project Setup"
echo "============================="

# Colours for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Colour

# Helper functions
print_status() {
	echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
	echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
	echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
	echo -e "${RED}[ERROR]${NC} $1"
}

check_command() {
	if command -v $1 &> /dev/null; then
		print_success "$1 is installed"
		return 0
	else
		print_error "$1 is not installed"
		return 1
	fi
}

# Check prerequisites
print_status "Checking prerequisites..."

# Check for Python
if ! check_command python3; then
	print_error "Python 3 is required. Please install Python 3.8 or later."
	exit 1
fi

# Check for pip
if ! check_command pip3; then
	print_error "pip3 is required. Please install pip."
	exit 1
fi

# Check for git
if ! check_command git; then
	print_error "Git is required. Please install Git."
	exit 1
fi

# Check for PlatformIO (optional for ESP32 development)
if check_command pio; then
	PLATFORMIO_AVAILABLE=true
else
	print_warning "PlatformIO CLI not found. ESP32 development will require manual setup."
	PLATFORMIO_AVAILABLE=false
fi

print_success "Prerequisites check completed"

# Create directory structure
print_status "Creating directory structure..."

# Core directories
mkdir -p scripts tools docs tests shared

# ESP32 directories
mkdir -p esp32/src/{config,core,hardware,network,storage,ui/themes,utils}
mkdir -p esp32/{lib,test/{native,embedded,mocks},tools/{simulator,config_generator},docs/hardware,examples}

# Home Assistant directories
mkdir -p homeassistant/custom_components/choretracker/translations
mkdir -p homeassistant/config/{automations,scripts,templates,sensors,dashboards}
mkdir -p homeassistant/{tests/fixtures,tools,docs}

# Shared directories
mkdir -p shared/{api/schemas,protocols,utils}

# Project-wide directories
mkdir -p tests/{e2e,performance,fixtures}
mkdir -p tools/{ci,development,monitoring}
mkdir -p docs/{architecture,deployment,api,examples}

# GitHub configuration
mkdir -p .github/{workflows,ISSUE_TEMPLATE}

print_success "Directory structure created"

# ESP32 Setup
print_status "Setting up ESP32 development environment..."

# Create PlatformIO configuration
cat > esp32/platformio.ini << 'EOF'
;PlatformIO Project Configuration File
[env:esp32]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
upload_speed = 921600

; Libraries
lib_deps = 
    adafruit/Adafruit GFX Library@^1.11.5
    adafruit/Adafruit ST7789 Library@^1.9.3
    bblanchon/ArduinoJson@^6.21.3
    arduino-libraries/WiFi@^1.2.7

; Test configuration
test_transport = native
test_framework = unity

; Build flags for debugging
build_flags = 
    -DDEBUG_ESP_PORT=Serial
    -DDEBUG_ESP_CORE
    -DCORE_DEBUG_LEVEL=4

[env:native]
platform = native
test_framework = unity
lib_deps = 
    bblanchon/ArduinoJson@^6.21.3
EOF

# Create basic ESP32 configuration template
cat > esp32/src/config/Config.h << 'EOF'
#pragma once

// WiFi Configuration
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Home Assistant Configuration
#define HA_SERVER "http://homeassistant.local:8123"
#define HA_TOKEN "YOUR_LONG_LIVED_ACCESS_TOKEN"

// OpenWeatherMap Configuration (optional)
#define WEATHER_API_KEY "YOUR_OPENWEATHER_API_KEY"
#define WEATHER_CITY "London,GB"

// Device Configuration
#define DEVICE_NAME "ChoreTracker"
#define UPDATE_INTERVAL_MS 60000  // 1 minute

// Debug Configuration
#define DEBUG_MODE true
#define SERIAL_SPEED 115200

// Display Configuration
#define DISPLAY_COUNT 8
#define DISPLAY_WIDTH 170
#define DISPLAY_HEIGHT 320
#define DISPLAY_ROTATION 1

// Task Configuration
#define MAX_TASKS_PER_PERSON 6
#define MAX_PERSONS 4
#define TASK_TIMEOUT_MS 300000  // 5 minutes

// Audio Configuration (GPIO 11)
#define AUDIO_ENABLED false
#define AUDIO_PIN 11
EOF

# Create GPIO configuration
cat > esp32/src/config/GPIOConfig.h << 'EOF'
#pragma once

// Switch Input Pins (with internal pullup)
#define SWITCH_PINS {13, 12, 14, 27, 26, 33, 35, 34}

// LED Output Pins
#define LED_PINS {15, 2, 0, 4, 5, 25, 32, 21}

// TFT Display CS Pins
#define TFT_CS_PINS {9, 10, 1, 3, 39, 36, 37, 38}

// TFT Shared Pins
#define TFT_SCK 18
#define TFT_MOSI 23
#define TFT_DC 19
#define TFT_RST -1  // Connected to ESP32 reset pin
#define TFT_BL 8    // Backlight control (all displays)

// Button Input Pins
#define WAKE_BUTTON_PIN 16
#define SCREEN_BUTTON_PINS {17, 22, 7}  // Screen 1, 2, 3 selection

// Spare GPIO
#define SPARE_GPIO_1 11  // Available for buzzer/expansion

// Pin counts for validation
#define TOTAL_SWITCH_PINS 8
#define TOTAL_LED_PINS 8
#define TOTAL_TFT_CS_PINS 8
#define TOTAL_SCREEN_BUTTONS 3
EOF

print_success "ESP32 environment configured"

# Home Assistant Setup
print_status "Setting up Home Assistant development environment..."

# Create Home Assistant custom integration manifest
cat > homeassistant/custom_components/choretracker/manifest.json << 'EOF'
{
  "domain": "choretracker",
  "name": "ChoreTracker",
  "version": "1.0.0",
  "documentation": "https://github.com/yourusername/choretracker",
  "issue_tracker": "https://github.com/yourusername/choretracker/issues",
  "dependencies": [],
  "codeowners": ["@yourusername"],
  "requirements": ["aiohttp>=3.8.0"],
  "iot_class": "local_polling",
  "config_flow": true,
  "integration_type": "device"
}
EOF

# Create basic HA integration files
cat > homeassistant/custom_components/choretracker/__init__.py << 'EOF'
"""ChoreTracker integration for Home Assistant."""
from __future__ import annotations

from homeassistant.config_entries import ConfigEntry
from homeassistant.const import Platform
from homeassistant.core import HomeAssistant

from .const import DOMAIN

PLATFORMS: list[Platform] = [
    Platform.SENSOR,
    Platform.SWITCH,
    Platform.BINARY_SENSOR,
]


async def async_setup_entry(hass: HomeAssistant, entry: ConfigEntry) -> bool:
    """Set up ChoreTracker from a config entry."""
    hass.data.setdefault(DOMAIN, {})
    
    # TODO: Implement coordinator and device setup
    
    await hass.config_entries.async_forward_entry_setups(entry, PLATFORMS)
    return True


async def async_unload_entry(hass: HomeAssistant, entry: ConfigEntry) -> bool:
    """Unload a config entry."""
    return await hass.config_entries.async_unload_platforms(entry, PLATFORMS)
EOF

# Create constants file
cat > homeassistant/custom_components/choretracker/const.py << 'EOF'
"""Constants for the ChoreTracker integration."""

DOMAIN = "choretracker"
MANUFACTURER = "ChoreTracker"
MODEL = "ESP32 Task Display"

# Configuration keys
CONF_HOST = "host"
CONF_PORT = "port"
CONF_DEVICE_ID = "device_id"

# Default values
DEFAULT_PORT = 80
DEFAULT_SCAN_INTERVAL = 30

# Entity types
ENTITY_TASK = "task"
ENTITY_SWITCH = "switch"
ENTITY_SENSOR = "sensor"
EOF

print_success "Home Assistant integration template created"

print_success "Setup completed successfully! 🎉"

echo
echo "📋 Next Steps:"
echo "=============="
echo "1. Update WiFi credentials in esp32/src/config/Config.h"
echo "2. Set up Home Assistant access token"
echo "3. Install PlatformIO IDE extension for ESP32 development"
echo "4. Create Python virtual environment for HA development"
echo
echo "🔧 Development Commands:"
echo "======================="
echo "ESP32 Development:"
echo "  cd esp32 && pio run          # Build firmware"
echo "  cd esp32 && pio run -t upload # Upload to device"
echo "  cd esp32 && pio test         # Run tests"
echo
echo "📚 Documentation:"
echo "================"
echo "- Project structure: project_structure.md"
echo "- Development plan: DEVELOPMENT_PLAN.md"
echo "- API documentation: shared/api/"
echo
echo "Happy coding! 🚀" 