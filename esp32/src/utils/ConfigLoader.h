#pragma once

#include <Arduino.h>
#include <Preferences.h>
#ifdef ESP32
#include <SPIFFS.h>
#endif
#include "../config/Config.h"

// ==============================================
// Configuration Loader for ChoreTracker
// ==============================================
// Handles loading configuration from multiple sources:
// 1. Environment variables (compile-time)
// 2. SPIFFS configuration file
// 3. Preferences (NVS)
// 4. Default values

class ConfigLoader {
private:
    static Preferences prefs;
    static bool initialized;
    
    // Configuration storage
    struct RuntimeConfig {
        String wifiSSID;
        String wifiPassword;
        String haServer;
        String haToken;
        String deviceName;
        int displayCount;
        bool debugMode;
        bool audioEnabled;
        String weatherAPIKey;
        bool weatherEnabled;
    };
    
    static RuntimeConfig config;

public:
    // ==============================================
    // Initialization and Loading
    // ==============================================
    
    static bool initialize();
    static void loadConfiguration();
    static bool saveConfiguration();
    static void resetToDefaults();
    
    // ==============================================
    // Configuration Getters
    // ==============================================
    
    static String getWiFiSSID() { return config.wifiSSID; }
    static String getWiFiPassword() { return config.wifiPassword; }
    static String getHAServer() { return config.haServer; }
    static String getHAToken() { return config.haToken; }
    static String getDeviceName() { return config.deviceName; }
    static int getDisplayCount() { return config.displayCount; }
    static bool isDebugMode() { return config.debugMode; }
    static bool isAudioEnabled() { return config.audioEnabled; }
    static String getWeatherAPIKey() { return config.weatherAPIKey; }
    static bool isWeatherEnabled() { return config.weatherEnabled; }
    
    // ==============================================
    // Configuration Setters
    // ==============================================
    
    static void setWiFiSSID(const String& ssid) { config.wifiSSID = ssid; }
    static void setWiFiPassword(const String& password) { config.wifiPassword = password; }
    static void setHAServer(const String& server) { config.haServer = server; }
    static void setHAToken(const String& token) { config.haToken = token; }
    static void setDeviceName(const String& name) { config.deviceName = name; }
    static void setDisplayCount(int count) { 
        if (count >= 1 && count <= 8) {
            config.displayCount = count; 
        }
    }
    static void setDebugMode(bool enabled) { config.debugMode = enabled; }
    static void setAudioEnabled(bool enabled) { config.audioEnabled = enabled; }
    static void setWeatherAPIKey(const String& key) { config.weatherAPIKey = key; }
    static void setWeatherEnabled(bool enabled) { config.weatherEnabled = enabled; }
    
    // ==============================================
    // SPIFFS Configuration File
    // ==============================================
    
    static bool loadFromSPIFFS();
    static bool saveToSPIFFS();
    static bool fileExists(const String& filename);
    
    // ==============================================
    // Preferences (NVS) Storage
    // ==============================================
    
    static bool loadFromPreferences();
    static bool saveToPreferences();
    static void clearPreferences();
    
    // ==============================================
    // Environment Variable Loading
    // ==============================================
    
    static void loadFromEnvironment();
    static String getEnvVar(const String& varName, const String& defaultValue = "");
    static int getEnvVarInt(const String& varName, int defaultValue = 0);
    static bool getEnvVarBool(const String& varName, bool defaultValue = false);
    
    // ==============================================
    // Validation and Utilities
    // ==============================================
    
    static bool validateConfiguration();
    static void printConfiguration();
    static String generateConfigJSON();
    static bool parseConfigJSON(const String& json);
    
    // ==============================================
    // GPIO Pin Configuration Loading
    // ==============================================
    
    static bool loadGPIOConfiguration();
    static void parseGPIOPins(const String& pinString, int* pinArray, int maxPins);
    
    // ==============================================
    // Configuration Source Priority
    // ==============================================
    
    enum ConfigSource {
        SOURCE_DEFAULT = 0,
        SOURCE_ENVIRONMENT = 1,
        SOURCE_SPIFFS = 2,
        SOURCE_PREFERENCES = 3,
        SOURCE_RUNTIME = 4
    };
    
    static ConfigSource getConfigSource() { return currentSource; }
    static void setConfigSource(ConfigSource source) { currentSource = source; }
    
private:
    static ConfigSource currentSource;
    
    // Helper functions
    static void setDefaults();
    static String sanitizeString(const String& input);
    static bool isValidSSID(const String& ssid);
    static bool isValidURL(const String& url);
    static bool isValidToken(const String& token);
};

// ==============================================
// Global Configuration Access
// ==============================================

// Convenience macros for accessing configuration
#define CONFIG_WIFI_SSID() ConfigLoader::getWiFiSSID()
#define CONFIG_WIFI_PASSWORD() ConfigLoader::getWiFiPassword()
#define CONFIG_HA_SERVER() ConfigLoader::getHAServer()
#define CONFIG_HA_TOKEN() ConfigLoader::getHAToken()
#define CONFIG_DEVICE_NAME() ConfigLoader::getDeviceName()
#define CONFIG_DISPLAY_COUNT() ConfigLoader::getDisplayCount()
#define CONFIG_DEBUG_MODE() ConfigLoader::isDebugMode()
#define CONFIG_AUDIO_ENABLED() ConfigLoader::isAudioEnabled()

// ==============================================
// Environment Variable Names
// ==============================================

#define ENV_WIFI_SSID "WIFI_SSID"
#define ENV_WIFI_PASSWORD "WIFI_PASSWORD"
#define ENV_HA_SERVER "HA_SERVER"
#define ENV_HA_TOKEN "HA_TOKEN"
#define ENV_DEVICE_NAME "DEVICE_NAME"
#define ENV_DISPLAY_COUNT "DISPLAY_COUNT"
#define ENV_DEBUG_MODE "DEBUG_MODE"
#define ENV_AUDIO_ENABLED "AUDIO_ENABLED"
#define ENV_WEATHER_API_KEY "WEATHER_API_KEY"
#define ENV_WEATHER_ENABLED "WEATHER_ENABLED"

// ==============================================
// Configuration File Paths
// ==============================================

#define CONFIG_FILE_PATH "/config.json"
#define GPIO_CONFIG_FILE_PATH "/gpio_config.json"
#define BACKUP_CONFIG_PATH "/config_backup.json"

// ==============================================
// Preference Keys
// ==============================================

#define PREF_NAMESPACE "choretracker"
#define PREF_WIFI_SSID "wifi_ssid"
#define PREF_WIFI_PASS "wifi_pass"
#define PREF_HA_SERVER "ha_server"
#define PREF_HA_TOKEN "ha_token"
#define PREF_DEVICE_NAME "device_name"
#define PREF_DISPLAY_COUNT "display_count"
#define PREF_DEBUG_MODE "debug_mode"
#define PREF_AUDIO_ENABLED "audio_enabled" 