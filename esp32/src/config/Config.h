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
