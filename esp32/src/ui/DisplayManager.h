#pragma once
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "../core/DemoManager.h"

class DisplayManager {
public:
	// Initialisation and control
	static void initialize();
	static void update();
	static void setDisplayBrightness(uint8_t brightness);
	
	// Normal mode display functions
	static void showColumnTitle(int displayIndex, const String& title, int columnIndex);
	static void showTaskContent(int displayIndex, int columnIndex, int position);
	static void updateTaskDisplay(int displayIndex, bool taskCompleted);
	
	// Demo mode display functions
	static void showDemoActivationProgress(int progress);
	static void clearDemoActivationProgress();
	static void showDemoModeActivation();
	static void showExitingDemoMode();
	static void showDemoContent(int displayIndex, int columnIndex, int position);
	
	// Visual effects and patterns
	static void drawPattern(int displayIndex, uint8_t patternId, uint16_t colour, int x, int y, int size);
	static void drawProgressBar(int displayIndex, int progress, uint16_t colour, int x, int y, int width, int height);
	static void drawTaskStatus(int displayIndex, bool completed, uint16_t colour);
	static void showAnimatedBackground(int displayIndex, uint16_t colour);
	
	// Text and layout helpers
	static void drawCenteredText(int displayIndex, const String& text, int y, uint16_t colour, uint8_t textSize = 1);
	static void drawWrappedText(int displayIndex, const String& text, int x, int y, int maxWidth, uint16_t colour, uint8_t textSize = 1);
	static void clearDisplay(int displayIndex, uint16_t backgroundColour = 0x0000);
	static void fillDisplay(int displayIndex, uint16_t colour);
	
	// Demo-specific display functions
	static void showDemoPersonSummary(int displayIndex, int personIndex);
	static void showDemoTaskDetail(int displayIndex, int personIndex, int taskIndex);
	static void showDemoWeather(int displayIndex);
	static void showDemoFamily(int displayIndex);
	
private:
	static Adafruit_ST7789* displays[8];
	static bool displaysInitialized;
	static uint8_t currentBrightness;
	static unsigned long lastUpdate;
	
	// Display configuration
	static const int DISPLAY_WIDTH = 170;
	static const int DISPLAY_HEIGHT = 320;
	static const int CS_PINS[8];
	static const int TFT_DC;
	static const int TFT_RST;
	static const int TFT_SCK;
	static const int TFT_MOSI;
	static const int TFT_BL;
	
	// Layout constants
	static const int MARGIN = 5;
	static const int LINE_HEIGHT = 12;
	static const int TITLE_HEIGHT = 40;
	static const int PATTERN_SIZE = 20;
	
	// Helper methods
	static void initializeDisplay(int displayIndex);
	static void selectDisplay(int displayIndex);
	static void deselectAllDisplays();
	static uint16_t scaleColour(uint16_t colour, float brightness);
	static int getTextWidth(const String& text, uint8_t textSize = 1);
	static int getTextHeight(uint8_t textSize = 1);
	
	// Pattern drawing functions
	static void drawStar(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawHeart(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawCircle(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawSquare(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawTriangle(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawCross(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawDiamond(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawFlower(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawSun(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawCloud(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawRain(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawSnow(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawMusic(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawBook(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawGame(int displayIndex, int x, int y, int size, uint16_t colour);
	static void drawSport(int displayIndex, int x, int y, int size, uint16_t colour);
	
	// Animation helpers
	static void showRainbowEffect(int displayIndex);
	static void showPulseEffect(int displayIndex, uint16_t colour);
	static void showSparkleEffect(int displayIndex);
};

// Colour helper functions
namespace DisplayColours {
	uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);
	uint16_t interpolateColour(uint16_t colour1, uint16_t colour2, float ratio);
	uint16_t dimColour(uint16_t colour, float dimFactor);
	uint16_t brightenColour(uint16_t colour, float brightFactor);
} 