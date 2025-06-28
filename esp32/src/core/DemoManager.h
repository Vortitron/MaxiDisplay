#pragma once
#include <Arduino.h>

// Demo task structure for fixed demo content
struct DemoTask {
	String name;
	String description;
	uint16_t colour;        // RGB565 colour for display
	uint8_t iconPattern;    // Simple pattern ID for visual representation
	bool isCompleted;
	unsigned long completedTime;
	int priority;           // 1-3 (1=high, 3=low)
};

// Demo person structure
struct DemoPerson {
	String name;
	uint16_t themeColour;
	DemoTask tasks[6];      // Up to 6 tasks per person
	int taskCount;
	uint8_t avatar;         // Simple avatar pattern ID
};

// Demo weather data
struct DemoWeather {
	String condition;
	int temperature;
	uint16_t colour;
	uint8_t icon;
	String description;
};

// Demo family data
struct DemoFamily {
	String event;
	String time;
	uint16_t colour;
	uint8_t icon;
	bool isActive;
};

class DemoManager {
public:
	// Initialisation and control
	static void initialize();
	static void startDemo();
	static void stopDemo();
	static void update();
	
	// Demo state management
	static bool isDemoActive();
	static unsigned long getDemoStartTime();
	static void resetDemo();
	
	// Demo content access
	static DemoPerson* getDemoPerson(int personIndex);    // 0=Emma, 1=Jack
	static DemoWeather* getDemoWeather();
	static DemoFamily* getDemoFamily();
	
	// Task manipulation for demo
	static void toggleDemoTask(int personIndex, int taskIndex);
	static void completeDemoTask(int personIndex, int taskIndex);
	static void resetDemoTask(int personIndex, int taskIndex);
	
	// Visual effects and animations
	static void runDemoAnimation();
	static void showDemoStats();
	static uint16_t getCurrentThemeColour();
	
	// Demo content helpers
	static String getPersonTaskSummary(int personIndex);
	static int getCompletedTaskCount(int personIndex);
	static int getTotalTaskCount(int personIndex);
	static float getCompletionPercentage(int personIndex);
	
private:
	static bool demoActive;
	static unsigned long demoStartTime;
	static unsigned long lastAnimationUpdate;
	static int animationPhase;
	static uint16_t currentThemeColour;
	
	// Demo data storage
	static DemoPerson demoPersons[2];    // Emma and Jack
	static DemoWeather demoWeather;
	static DemoFamily demoFamilyEvents[4];  // Up to 4 family events
	static int familyEventCount;
	
	// Animation and visual state
	static unsigned long lastColourCycle;
	static int colourPhase;
	static bool animationDirection;
	
	// Private helper methods
	static void initializeDemoContent();
	static void setupDemoPersons();
	static void setupDemoWeather();
	static void setupDemoFamily();
	static void updateAnimations();
	static void cycleDemoColours();
	static uint16_t getNextAnimationColour();
	
	// Demo content constants
	static const uint16_t DEMO_COLOURS[];
	static const int DEMO_COLOUR_COUNT;
	static const String DEMO_TASK_NAMES[];
	static const String DEMO_TASK_DESCRIPTIONS[];
	static const int DEMO_TASK_COUNT;
};

// Colour constants for demo mode (RGB565 format)
namespace DemoColours {
	constexpr uint16_t RED = 0xF800;
	constexpr uint16_t GREEN = 0x07E0;
	constexpr uint16_t BLUE = 0x001F;
	constexpr uint16_t YELLOW = 0xFFE0;
	constexpr uint16_t MAGENTA = 0xF81F;
	constexpr uint16_t CYAN = 0x07FF;
	constexpr uint16_t ORANGE = 0xFD20;
	constexpr uint16_t PURPLE = 0x8010;
	constexpr uint16_t PINK = 0xFC18;
	constexpr uint16_t LIME = 0x87E0;
	constexpr uint16_t WHITE = 0xFFFF;
	constexpr uint16_t BLACK = 0x0000;
	constexpr uint16_t GREY = 0x8410;
	constexpr uint16_t DARK_GREEN = 0x03E0;
	constexpr uint16_t DARK_BLUE = 0x000F;
	constexpr uint16_t GOLD = 0xFEA0;
}

// Icon/Pattern constants for demo mode
namespace DemoPatterns {
	constexpr uint8_t STAR = 0;
	constexpr uint8_t HEART = 1;
	constexpr uint8_t CIRCLE = 2;
	constexpr uint8_t SQUARE = 3;
	constexpr uint8_t TRIANGLE = 4;
	constexpr uint8_t CROSS = 5;
	constexpr uint8_t DIAMOND = 6;
	constexpr uint8_t FLOWER = 7;
	constexpr uint8_t SUN = 8;
	constexpr uint8_t CLOUD = 9;
	constexpr uint8_t RAIN = 10;
	constexpr uint8_t SNOW = 11;
	constexpr uint8_t MUSIC = 12;
	constexpr uint8_t BOOK = 13;
	constexpr uint8_t GAME = 14;
	constexpr uint8_t SPORT = 15;
} 