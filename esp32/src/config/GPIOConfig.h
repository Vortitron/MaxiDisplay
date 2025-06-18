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
