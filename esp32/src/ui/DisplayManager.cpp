#include "DisplayManager.h"
#include "../core/DemoManager.h"
#include "../config/GPIOConfig.h"
#include <Arduino.h>

// Static member definitions
Adafruit_ST7789* DisplayManager::displays[8];
bool DisplayManager::displaysInitialized = false;
uint8_t DisplayManager::currentBrightness = 200;
unsigned long DisplayManager::lastUpdate = 0;

// Hardware pin assignments from GPIOConfig.h
const int DisplayManager::CS_PINS[8] = {9, 10, 1, 3, 39, 36, 37, 38};
const int DisplayManager::TFT_DC = 19;
const int DisplayManager::TFT_RST = -1;  // Connected to ESP32 reset
const int DisplayManager::TFT_SCK = 18;
const int DisplayManager::TFT_MOSI = 23;
const int DisplayManager::TFT_BL = 8;

void DisplayManager::initialize() {
	Serial.println("DisplayManager: Initializing TFT displays...");
	
	// Initialize SPI pins
	pinMode(TFT_SCK, OUTPUT);
	pinMode(TFT_MOSI, OUTPUT);
	pinMode(TFT_DC, OUTPUT);
	pinMode(TFT_BL, OUTPUT);
	
	// Turn on backlight
	digitalWrite(TFT_BL, HIGH);
	
	// Initialize all CS pins
	for (int i = 0; i < 8; i++) {
		pinMode(CS_PINS[i], OUTPUT);
		digitalWrite(CS_PINS[i], HIGH);  // CS is active low
	}
	
	// Initialize display objects
	for (int i = 0; i < 8; i++) {
		displays[i] = new Adafruit_ST7789(CS_PINS[i], TFT_DC, TFT_RST);
		initializeDisplay(i);
	}
	
	displaysInitialized = true;
	Serial.println("DisplayManager: All displays initialized with colourful demo support!");
	
	// Show startup pattern on all displays
	for (int i = 0; i < 8; i++) {
		clearDisplay(i, DemoColours::BLACK);
		drawCenteredText(i, "ChoreTracker", 100, DemoColours::WHITE, 2);
		drawCenteredText(i, "Ready!", 140, DemoColours::GREEN, 1);
		drawPattern(i, DemoPatterns::STAR, DemoColours::YELLOW, DISPLAY_WIDTH/2, 200, 30);
	}
}

void DisplayManager::initializeDisplay(int displayIndex) {
	if (displayIndex < 0 || displayIndex >= 8) return;
	
	selectDisplay(displayIndex);
	displays[displayIndex]->init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	displays[displayIndex]->setRotation(1);  // Landscape orientation
	displays[displayIndex]->fillScreen(ST77XX_BLACK);
	deselectAllDisplays();
	
	Serial.printf("Display %d initialized (CS: GPIO %d)\n", displayIndex, CS_PINS[displayIndex]);
}

void DisplayManager::selectDisplay(int displayIndex) {
	if (displayIndex < 0 || displayIndex >= 8) return;
	
	// Deselect all displays first
	deselectAllDisplays();
	
	// Select the target display
	digitalWrite(CS_PINS[displayIndex], LOW);
}

void DisplayManager::deselectAllDisplays() {
	for (int i = 0; i < 8; i++) {
		digitalWrite(CS_PINS[i], HIGH);
	}
}

void DisplayManager::update() {
	if (!displaysInitialized) return;
	
	// Update displays periodically
	unsigned long now = millis();
	if (now - lastUpdate >= 50) {  // 20 FPS update rate
		
		if (DemoManager::isDemoActive()) {
			// Update demo mode animations
			for (int i = 0; i < 8; i++) {
				// Add subtle animation effects in demo mode
				if ((now / 100) % 20 == i) {  // Staggered updates
					showSparkleEffect(i);
				}
			}
		}
		
		lastUpdate = now;
	}
}

void DisplayManager::showDemoActivationProgress(int progress) {
	if (!displaysInitialized) return;
	
	// Show progress on all displays
	for (int i = 0; i < 8; i++) {
		clearDisplay(i, DemoColours::BLACK);
		
		drawCenteredText(i, "DEMO MODE", 60, DemoColours::YELLOW, 2);
		drawCenteredText(i, "Hold buttons...", 90, DemoColours::WHITE, 1);
		
		// Progress bar
		int barWidth = DISPLAY_WIDTH - 40;
		int barHeight = 20;
		int barX = 20;
		int barY = 120;
		
		drawProgressBar(i, progress, DemoColours::GREEN, barX, barY, barWidth, barHeight);
		
		// Percentage text
		String progressText = String(progress) + "%";
		drawCenteredText(i, progressText, 160, DemoColours::CYAN, 1);
		
		// Animated pattern
		int patternSize = 15 + (progress / 10);
		drawPattern(i, DemoPatterns::STAR, DemoColours::GOLD, DISPLAY_WIDTH/2, 200, patternSize);
	}
}

void DisplayManager::clearDemoActivationProgress() {
	if (!displaysInitialized) return;
	
	// Clear progress display and return to normal content
	for (int i = 0; i < 8; i++) {
		clearDisplay(i, DemoColours::BLACK);
		drawCenteredText(i, "Ready", 160, DemoColours::GREEN, 1);
	}
}

void DisplayManager::showDemoModeActivation() {
	if (!displaysInitialized) return;
	
	Serial.println("🎮 Showing demo mode activation animation!");
	
	// Rainbow cascade animation
	for (int phase = 0; phase < 8; phase++) {
		for (int i = 0; i < 8; i++) {
			uint16_t colour = DemoManager::DEMO_COLOURS[i % DemoManager::DEMO_COLOUR_COUNT];
			
			clearDisplay(i, colour);
			drawCenteredText(i, "DEMO", 80, DemoColours::BLACK, 3);
			drawCenteredText(i, "MODE", 120, DemoColours::BLACK, 3);
			drawCenteredText(i, "ACTIVE!", 160, DemoColours::WHITE, 2);
			
			// Add sparkle pattern
			drawPattern(i, DemoPatterns::STAR, DemoColours::WHITE, 50, 200, 20);
			drawPattern(i, DemoPatterns::HEART, DemoColours::PINK, DISPLAY_WIDTH-50, 200, 20);
		}
		delay(200);
	}
}

void DisplayManager::showExitingDemoMode() {
	if (!displaysInitialized) return;
	
	// Show exit animation
	for (int i = 0; i < 8; i++) {
		clearDisplay(i, DemoColours::PURPLE);
		drawCenteredText(i, "Exiting", 100, DemoColours::WHITE, 2);
		drawCenteredText(i, "Demo Mode", 130, DemoColours::WHITE, 2);
		drawPattern(i, DemoPatterns::CIRCLE, DemoColours::CYAN, DISPLAY_WIDTH/2, 200, 25);
	}
	delay(1000);
	
	// Clear to normal display
	for (int i = 0; i < 8; i++) {
		clearDisplay(i, DemoColours::BLACK);
	}
}

void DisplayManager::showDemoContent(int displayIndex, int columnIndex, int position) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	clearDisplay(displayIndex, DemoColours::BLACK);
	
	// Show different content based on column
	switch (columnIndex) {
		case 0:  // Emma
			showDemoPersonSummary(displayIndex, 0);
			break;
		case 1:  // Jack
			showDemoPersonSummary(displayIndex, 1);
			break;
		case 2:  // Weather
			showDemoWeather(displayIndex);
			break;
		case 3:  // Family
			showDemoFamily(displayIndex);
			break;
		default:
			drawCenteredText(displayIndex, "Demo", 160, DemoColours::WHITE, 2);
			break;
	}
}

void DisplayManager::showDemoPersonSummary(int displayIndex, int personIndex) {
	DemoPerson* person = DemoManager::getDemoPerson(personIndex);
	if (!person) return;
	
	// Header with person name and theme colour
	clearDisplay(displayIndex, DemoColours::BLACK);
	
	// Name banner
	fillDisplay(displayIndex, person->themeColour);
	drawCenteredText(displayIndex, person->name, 25, DemoColours::WHITE, 3);
	
	// Avatar pattern
	drawPattern(displayIndex, person->avatar, DemoColours::WHITE, 30, 25, 25);
	
	// Task summary area
	int y = 70;
	selectDisplay(displayIndex);
	displays[displayIndex]->fillRect(0, y, DISPLAY_WIDTH, DISPLAY_HEIGHT-y, DemoColours::BLACK);
	
	// Show first few tasks with colours and patterns
	for (int i = 0; i < min(4, person->taskCount); i++) {
		DemoTask& task = person->tasks[i];
		int taskY = y + 15 + i * 35;
		
		// Task status indicator
		uint16_t statusColour = task.isCompleted ? DemoColours::GREEN : task.colour;
		drawPattern(displayIndex, task.iconPattern, statusColour, 20, taskY, 15);
		
		// Task name
		uint16_t textColour = task.isCompleted ? DemoColours::GREY : DemoColours::WHITE;
		String taskText = task.name;
		if (task.isCompleted) taskText += " ✓";
		
		selectDisplay(displayIndex);
		displays[displayIndex]->setCursor(45, taskY - 5);
		displays[displayIndex]->setTextColor(textColour);
		displays[displayIndex]->setTextSize(1);
		displays[displayIndex]->print(taskText);
	}
	
	// Progress summary
	int completed = DemoManager::getCompletedTaskCount(personIndex);
	int total = DemoManager::getTotalTaskCount(personIndex);
	String summary = String(completed) + "/" + String(total) + " done";
	
	drawCenteredText(displayIndex, summary, DISPLAY_HEIGHT - 30, DemoColours::CYAN, 1);
	
	// Progress bar
	int progress = (completed * 100) / total;
	drawProgressBar(displayIndex, progress, DemoColours::GREEN, 20, DISPLAY_HEIGHT - 15, DISPLAY_WIDTH - 40, 8);
	
	deselectAllDisplays();
}

void DisplayManager::showDemoWeather(int displayIndex) {
	DemoWeather* weather = DemoManager::getDemoWeather();
	if (!weather) return;
	
	clearDisplay(displayIndex, DemoColours::CYAN);
	
	// Weather header
	drawCenteredText(displayIndex, "Weather", 25, DemoColours::WHITE, 2);
	
	// Weather icon
	drawPattern(displayIndex, weather->icon, DemoColours::WHITE, DISPLAY_WIDTH/2, 80, 40);
	
	// Temperature
	String tempStr = String(weather->temperature) + "°C";
	drawCenteredText(displayIndex, tempStr, 140, DemoColours::WHITE, 3);
	
	// Condition
	drawCenteredText(displayIndex, weather->condition, 170, DemoColours::BLACK, 1);
	
	// Description
	drawWrappedText(displayIndex, weather->description, 10, 200, DISPLAY_WIDTH-20, DemoColours::DARK_BLUE, 1);
	
	// Decorative elements
	drawPattern(displayIndex, DemoPatterns::SUN, DemoColours::YELLOW, 30, 250, 20);
	drawPattern(displayIndex, DemoPatterns::CLOUD, DemoColours::WHITE, DISPLAY_WIDTH-50, 250, 20);
}

void DisplayManager::showDemoFamily(int displayIndex) {
	clearDisplay(displayIndex, DemoColours::PURPLE);
	
	// Family header
	drawCenteredText(displayIndex, "Family", 25, DemoColours::WHITE, 2);
	
	// Show family events
	DemoFamily* events = DemoManager::getDemoFamily();
	int y = 70;
	
	for (int i = 0; i < 4 && i < 4; i++) {  // Show up to 4 events
		DemoFamily& event = events[i];
		int eventY = y + i * 50;
		
		// Event icon
		uint16_t iconColour = event.isActive ? event.colour : DemoColours::GREY;
		drawPattern(displayIndex, event.icon, iconColour, 20, eventY, 20);
		
		// Event text
		uint16_t textColour = event.isActive ? DemoColours::WHITE : DemoColours::GREY;
		
		selectDisplay(displayIndex);
		displays[displayIndex]->setCursor(50, eventY - 10);
		displays[displayIndex]->setTextColor(textColour);
		displays[displayIndex]->setTextSize(1);
		displays[displayIndex]->print(event.event);
		
		displays[displayIndex]->setCursor(50, eventY + 5);
		displays[displayIndex]->setTextColor(DemoColours::CYAN);
		displays[displayIndex]->print(event.time);
		deselectAllDisplays();
	}
	
	// Decorative hearts
	drawPattern(displayIndex, DemoPatterns::HEART, DemoColours::PINK, DISPLAY_WIDTH/2, 280, 25);
}

void DisplayManager::drawPattern(int displayIndex, uint8_t patternId, uint16_t colour, int x, int y, int size) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	switch (patternId) {
		case DemoPatterns::STAR: drawStar(displayIndex, x, y, size, colour); break;
		case DemoPatterns::HEART: drawHeart(displayIndex, x, y, size, colour); break;
		case DemoPatterns::CIRCLE: drawCircle(displayIndex, x, y, size, colour); break;
		case DemoPatterns::SQUARE: drawSquare(displayIndex, x, y, size, colour); break;
		case DemoPatterns::TRIANGLE: drawTriangle(displayIndex, x, y, size, colour); break;
		case DemoPatterns::CROSS: drawCross(displayIndex, x, y, size, colour); break;
		case DemoPatterns::DIAMOND: drawDiamond(displayIndex, x, y, size, colour); break;
		case DemoPatterns::FLOWER: drawFlower(displayIndex, x, y, size, colour); break;
		case DemoPatterns::SUN: drawSun(displayIndex, x, y, size, colour); break;
		case DemoPatterns::CLOUD: drawCloud(displayIndex, x, y, size, colour); break;
		case DemoPatterns::RAIN: drawRain(displayIndex, x, y, size, colour); break;
		case DemoPatterns::SNOW: drawSnow(displayIndex, x, y, size, colour); break;
		case DemoPatterns::MUSIC: drawMusic(displayIndex, x, y, size, colour); break;
		case DemoPatterns::BOOK: drawBook(displayIndex, x, y, size, colour); break;
		case DemoPatterns::GAME: drawGame(displayIndex, x, y, size, colour); break;
		case DemoPatterns::SPORT: drawSport(displayIndex, x, y, size, colour); break;
		default: drawCircle(displayIndex, x, y, size, colour); break;
	}
}

void DisplayManager::drawProgressBar(int displayIndex, int progress, uint16_t colour, int x, int y, int width, int height) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	selectDisplay(displayIndex);
	
	// Background
	displays[displayIndex]->fillRect(x, y, width, height, DemoColours::GREY);
	
	// Progress fill
	int fillWidth = (progress * width) / 100;
	displays[displayIndex]->fillRect(x, y, fillWidth, height, colour);
	
	// Border
	displays[displayIndex]->drawRect(x, y, width, height, DemoColours::WHITE);
	
	deselectAllDisplays();
}

void DisplayManager::drawCenteredText(int displayIndex, const String& text, int y, uint16_t colour, uint8_t textSize) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	selectDisplay(displayIndex);
	
	displays[displayIndex]->setTextSize(textSize);
	displays[displayIndex]->setTextColor(colour);
	
	// Calculate text width for centering
	int textWidth = text.length() * 6 * textSize;  // Approximate
	int x = (DISPLAY_WIDTH - textWidth) / 2;
	
	displays[displayIndex]->setCursor(x, y);
	displays[displayIndex]->print(text);
	
	deselectAllDisplays();
}

void DisplayManager::drawWrappedText(int displayIndex, const String& text, int x, int y, int maxWidth, uint16_t colour, uint8_t textSize) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	selectDisplay(displayIndex);
	displays[displayIndex]->setTextSize(textSize);
	displays[displayIndex]->setTextColor(colour);
	displays[displayIndex]->setCursor(x, y);
	displays[displayIndex]->print(text);  // Simple implementation for now
	deselectAllDisplays();
}

void DisplayManager::clearDisplay(int displayIndex, uint16_t backgroundColour) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	selectDisplay(displayIndex);
	displays[displayIndex]->fillScreen(backgroundColour);
	deselectAllDisplays();
}

void DisplayManager::fillDisplay(int displayIndex, uint16_t colour) {
	clearDisplay(displayIndex, colour);
}

// Simple pattern implementations (basic geometric shapes)
void DisplayManager::drawStar(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Simple 5-pointed star approximation using triangles
	for (int i = 0; i < 5; i++) {
		float angle = i * 2 * PI / 5;
		int x1 = x + cos(angle) * size;
		int y1 = y + sin(angle) * size;
		displays[displayIndex]->drawLine(x, y, x1, y1, colour);
	}
	deselectAllDisplays();
}

void DisplayManager::drawHeart(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Simple heart using circles and triangle
	int halfSize = size / 2;
	displays[displayIndex]->fillCircle(x - halfSize/2, y - halfSize/2, halfSize/2, colour);
	displays[displayIndex]->fillCircle(x + halfSize/2, y - halfSize/2, halfSize/2, colour);
	displays[displayIndex]->fillTriangle(x - halfSize, y, x + halfSize, y, x, y + halfSize, colour);
	deselectAllDisplays();
}

void DisplayManager::drawCircle(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	displays[displayIndex]->fillCircle(x, y, size/2, colour);
	deselectAllDisplays();
}

void DisplayManager::drawSquare(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	displays[displayIndex]->fillRect(x - size/2, y - size/2, size, size, colour);
	deselectAllDisplays();
}

void DisplayManager::drawTriangle(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	displays[displayIndex]->fillTriangle(x, y - size/2, x - size/2, y + size/2, x + size/2, y + size/2, colour);
	deselectAllDisplays();
}

void DisplayManager::drawCross(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	int halfSize = size / 2;
	displays[displayIndex]->drawLine(x - halfSize, y, x + halfSize, y, colour);
	displays[displayIndex]->drawLine(x, y - halfSize, x, y + halfSize, colour);
	deselectAllDisplays();
}

void DisplayManager::drawDiamond(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	int halfSize = size / 2;
	displays[displayIndex]->fillTriangle(x, y - halfSize, x - halfSize, y, x + halfSize, y, colour);
	displays[displayIndex]->fillTriangle(x - halfSize, y, x, y + halfSize, x + halfSize, y, colour);
	deselectAllDisplays();
}

void DisplayManager::drawFlower(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Simple flower with petals
	int petalSize = size / 3;
	displays[displayIndex]->fillCircle(x, y, petalSize, colour);  // Center
	for (int i = 0; i < 6; i++) {
		float angle = i * PI / 3;
		int px = x + cos(angle) * petalSize;
		int py = y + sin(angle) * petalSize;
		displays[displayIndex]->fillCircle(px, py, petalSize/2, colour);
	}
	deselectAllDisplays();
}

void DisplayManager::drawSun(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Sun with rays
	displays[displayIndex]->fillCircle(x, y, size/3, colour);
	for (int i = 0; i < 8; i++) {
		float angle = i * PI / 4;
		int x1 = x + cos(angle) * size/2;
		int y1 = y + sin(angle) * size/2;
		int x2 = x + cos(angle) * size;
		int y2 = y + sin(angle) * size;
		displays[displayIndex]->drawLine(x1, y1, x2, y2, colour);
	}
	deselectAllDisplays();
}

void DisplayManager::drawCloud(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Cloud made of overlapping circles
	int cloudSize = size / 4;
	displays[displayIndex]->fillCircle(x - cloudSize, y, cloudSize, colour);
	displays[displayIndex]->fillCircle(x, y - cloudSize/2, cloudSize, colour);
	displays[displayIndex]->fillCircle(x + cloudSize, y, cloudSize, colour);
	displays[displayIndex]->fillCircle(x, y + cloudSize/2, cloudSize*2, colour);
	deselectAllDisplays();
}

// Implement remaining pattern functions with simple geometric approximations
void DisplayManager::drawRain(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	for (int i = 0; i < 5; i++) {
		int rx = x + (i - 2) * size/4;
		displays[displayIndex]->drawLine(rx, y - size/2, rx, y + size/2, colour);
	}
	deselectAllDisplays();
}

void DisplayManager::drawSnow(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Snowflake pattern
	int halfSize = size / 2;
	displays[displayIndex]->drawLine(x - halfSize, y, x + halfSize, y, colour);
	displays[displayIndex]->drawLine(x, y - halfSize, x, y + halfSize, colour);
	displays[displayIndex]->drawLine(x - halfSize/2, y - halfSize/2, x + halfSize/2, y + halfSize/2, colour);
	displays[displayIndex]->drawLine(x - halfSize/2, y + halfSize/2, x + halfSize/2, y - halfSize/2, colour);
	deselectAllDisplays();
}

void DisplayManager::drawMusic(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Musical note
	displays[displayIndex]->fillCircle(x, y + size/3, size/4, colour);
	displays[displayIndex]->drawLine(x + size/4, y + size/3, x + size/4, y - size/2, colour);
	deselectAllDisplays();
}

void DisplayManager::drawBook(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Book representation
	displays[displayIndex]->fillRect(x - size/2, y - size/3, size, size*2/3, colour);
	displays[displayIndex]->drawLine(x, y - size/3, x, y + size/3, DemoColours::BLACK);
	deselectAllDisplays();
}

void DisplayManager::drawGame(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Game controller approximation
	displays[displayIndex]->fillRect(x - size/2, y - size/4, size, size/2, colour);
	displays[displayIndex]->fillCircle(x - size/3, y, size/6, DemoColours::BLACK);
	displays[displayIndex]->fillCircle(x + size/3, y, size/6, DemoColours::BLACK);
	deselectAllDisplays();
}

void DisplayManager::drawSport(int displayIndex, int x, int y, int size, uint16_t colour) {
	selectDisplay(displayIndex);
	// Simple ball
	displays[displayIndex]->fillCircle(x, y, size/2, colour);
	displays[displayIndex]->drawCircle(x, y, size/2, DemoColours::BLACK);
	deselectAllDisplays();
}

void DisplayManager::showSparkleEffect(int displayIndex) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	// Add small random sparkles
	for (int i = 0; i < 3; i++) {
		int x = random(DISPLAY_WIDTH);
		int y = random(DISPLAY_HEIGHT);
		uint16_t colour = DemoManager::DEMO_COLOURS[random(DemoManager::DEMO_COLOUR_COUNT)];
		
		selectDisplay(displayIndex);
		displays[displayIndex]->fillCircle(x, y, 2, colour);
		deselectAllDisplays();
	}
}

// Colour helper implementations
namespace DisplayColours {
	uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}
	
	uint16_t interpolateColour(uint16_t colour1, uint16_t colour2, float ratio) {
		// Simple linear interpolation - could be improved
		return colour1;  // Simplified for now
	}
	
	uint16_t dimColour(uint16_t colour, float dimFactor) {
		// Simple dimming - could be improved
		return colour;  // Simplified for now
	}
	
	uint16_t brightenColour(uint16_t colour, float brightFactor) {
		// Simple brightening - could be improved
		return colour;  // Simplified for now
	}
}

// Additional display functions for normal mode
void DisplayManager::showColumnTitle(int displayIndex, const String& title, int columnIndex) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	// Show column title with theme colours
	uint16_t themeColour = DemoManager::DEMO_COLOURS[columnIndex % DemoManager::DEMO_COLOUR_COUNT];
	
	clearDisplay(displayIndex, themeColour);
	drawCenteredText(displayIndex, title, DISPLAY_HEIGHT/2 - 10, DemoColours::WHITE, 3);
	
	// Add decorative pattern
	drawPattern(displayIndex, DemoPatterns::STAR, DemoColours::WHITE, DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 + 40, 30);
}

void DisplayManager::showTaskContent(int displayIndex, int columnIndex, int position) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	if (DemoManager::isDemoActive()) {
		showDemoContent(displayIndex, columnIndex, position);
	} else {
		// Normal mode content
		clearDisplay(displayIndex, DemoColours::BLACK);
		String content = "Task " + String(displayIndex);
		drawCenteredText(displayIndex, content, DISPLAY_HEIGHT/2, DemoColours::WHITE, 2);
	}
}

void DisplayManager::updateTaskDisplay(int displayIndex, bool taskCompleted) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	// Update display based on task completion
	uint16_t statusColour = taskCompleted ? DemoColours::GREEN : DemoColours::RED;
	drawTaskStatus(displayIndex, taskCompleted, statusColour);
}

void DisplayManager::drawTaskStatus(int displayIndex, bool completed, uint16_t colour) {
	if (!displaysInitialized || displayIndex < 0 || displayIndex >= 8) return;
	
	// Draw status indicator in corner
	selectDisplay(displayIndex);
	if (completed) {
		displays[displayIndex]->fillCircle(DISPLAY_WIDTH - 20, 20, 10, DemoColours::GREEN);
		displays[displayIndex]->drawLine(DISPLAY_WIDTH - 25, 20, DISPLAY_WIDTH - 20, 25, DemoColours::WHITE);
		displays[displayIndex]->drawLine(DISPLAY_WIDTH - 20, 25, DISPLAY_WIDTH - 15, 15, DemoColours::WHITE);
	} else {
		displays[displayIndex]->fillCircle(DISPLAY_WIDTH - 20, 20, 10, DemoColours::RED);
	}
	deselectAllDisplays();
}

void DisplayManager::setDisplayBrightness(uint8_t brightness) {
	currentBrightness = brightness;
	// Note: This would require PWM control of the backlight pin for true brightness control
	// For now, we just store the value
}

void DisplayManager::showAnimatedBackground(int displayIndex, uint16_t colour) {
	// Simple animated background - could be enhanced with more complex patterns
	clearDisplay(displayIndex, colour);
} 