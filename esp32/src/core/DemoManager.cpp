#include "DemoManager.h"
#include "../hardware/GPIOManager.h"
#include <Arduino.h>

// Static member definitions
bool DemoManager::demoActive = false;
unsigned long DemoManager::demoStartTime = 0;
unsigned long DemoManager::lastAnimationUpdate = 0;
int DemoManager::animationPhase = 0;
uint16_t DemoManager::currentThemeColour = DemoColours::BLUE;

DemoPerson DemoManager::demoPersons[2];
DemoWeather DemoManager::demoWeather;
DemoFamily DemoManager::demoFamilyEvents[4];
int DemoManager::familyEventCount = 0;

unsigned long DemoManager::lastColourCycle = 0;
int DemoManager::colourPhase = 0;
bool DemoManager::animationDirection = true;

// Demo colour palette
const uint16_t DemoManager::DEMO_COLOURS[] = {
	DemoColours::RED, DemoColours::GREEN, DemoColours::BLUE, DemoColours::YELLOW,
	DemoColours::MAGENTA, DemoColours::CYAN, DemoColours::ORANGE, DemoColours::PURPLE,
	DemoColours::PINK, DemoColours::LIME, DemoColours::GOLD
};
const int DemoManager::DEMO_COLOUR_COUNT = 11;

// Demo task templates
const String DemoManager::DEMO_TASK_NAMES[] = {
	"Make Bed", "Brush Teeth", "Feed Pet", "Tidy Room", "Homework", "Practice Piano",
	"Water Plants", "Help Cook", "Read Book", "Exercise", "Art Project", "Clean Desk"
};

const String DemoManager::DEMO_TASK_DESCRIPTIONS[] = {
	"Make your bed neat", "Brush for 2 minutes", "Fill food & water", "Put toys away",
	"Complete school work", "30 minutes practice", "Give plants water", "Help with dinner",
	"Read 20 minutes", "10 minutes activity", "Creative time", "Organise workspace"
};

const int DemoManager::DEMO_TASK_COUNT = 12;

void DemoManager::initialize() {
	Serial.println("DemoManager: Initializing demo system...");
	
	demoActive = false;
	demoStartTime = 0;
	lastAnimationUpdate = 0;
	animationPhase = 0;
	currentThemeColour = DemoColours::BLUE;
	
	lastColourCycle = 0;
	colourPhase = 0;
	animationDirection = true;
	
	// Initialize demo content
	initializeDemoContent();
	
	Serial.println("DemoManager: Demo system ready");
}

void DemoManager::startDemo() {
	Serial.println("🎮 DemoManager: Starting demo mode with colourful content!");
	
	demoActive = true;
	demoStartTime = millis();
	lastAnimationUpdate = millis();
	animationPhase = 0;
	
	// Reset demo content to initial state
	initializeDemoContent();
	
	// Start with some tasks already completed for variety
	completeDemoTask(0, 0);  // Emma: Make Bed (completed)
	completeDemoTask(0, 2);  // Emma: Feed Pet (completed)
	completeDemoTask(1, 1);  // Jack: Brush Teeth (completed)
	
	// Set initial LED pattern
	GPIOManager::setTaskLED(0, true);   // LED 0 OFF (task complete)
	GPIOManager::setTaskLED(1, false);  // LED 1 ON (task incomplete)
	GPIOManager::setTaskLED(2, true);   // LED 2 OFF (task complete)
	GPIOManager::setTaskLED(3, false);  // LED 3 ON (task incomplete)
	GPIOManager::setTaskLED(4, false);  // LED 4 ON (task incomplete)
	GPIOManager::setTaskLED(5, true);   // LED 5 OFF (task complete)
	GPIOManager::setTaskLED(6, false);  // LED 6 ON (task incomplete)
	GPIOManager::setTaskLED(7, false);  // LED 7 ON (task incomplete)
	
	Serial.println("Demo mode started with sample completed tasks!");
}

void DemoManager::stopDemo() {
	Serial.println("DemoManager: Stopping demo mode");
	
	demoActive = false;
	demoStartTime = 0;
	
	// Reset all LEDs
	GPIOManager::setAllTasksIncomplete();
	
	Serial.println("Demo mode stopped - returned to normal operation");
}

void DemoManager::update() {
	if (!demoActive) return;
	
	unsigned long now = millis();
	
	// Update animations every 100ms
	if (now - lastAnimationUpdate >= 100) {
		updateAnimations();
		lastAnimationUpdate = now;
	}
	
	// Cycle colours every 3 seconds
	if (now - lastColourCycle >= 3000) {
		cycleDemoColours();
		lastColourCycle = now;
	}
	
	// Show periodic demo stats
	if ((now - demoStartTime) % 10000 < 100) {  // Every 10 seconds
		showDemoStats();
	}
}

void DemoManager::initializeDemoContent() {
	setupDemoPersons();
	setupDemoWeather();
	setupDemoFamily();
	
	Serial.println("Demo content initialized with colourful tasks and data");
}

void DemoManager::setupDemoPersons() {
	// Emma (Person 0) - Blue theme
	demoPersons[0].name = "Emma";
	demoPersons[0].themeColour = DemoColours::BLUE;
	demoPersons[0].avatar = DemoPatterns::STAR;
	demoPersons[0].taskCount = 6;
	
	// Emma's tasks
	demoPersons[0].tasks[0] = {"Make Bed", "Make your bed neat & tidy", DemoColours::PURPLE, DemoPatterns::HEART, false, 0, 2};
	demoPersons[0].tasks[1] = {"Brush Teeth", "Brush for 2 minutes", DemoColours::CYAN, DemoPatterns::CIRCLE, false, 0, 1};
	demoPersons[0].tasks[2] = {"Feed Fluffy", "Fill cat food & water", DemoColours::ORANGE, DemoPatterns::FLOWER, false, 0, 1};
	demoPersons[0].tasks[3] = {"Tidy Room", "Put toys away neatly", DemoColours::GREEN, DemoPatterns::SQUARE, false, 0, 2};
	demoPersons[0].tasks[4] = {"Piano Practice", "30 minutes practice", DemoColours::PINK, DemoPatterns::MUSIC, false, 0, 3};
	demoPersons[0].tasks[5] = {"Read Book", "Read for 20 minutes", DemoColours::GOLD, DemoPatterns::BOOK, false, 0, 3};
	
	// Jack (Person 1) - Green theme
	demoPersons[1].name = "Jack";
	demoPersons[1].themeColour = DemoColours::GREEN;
	demoPersons[1].avatar = DemoPatterns::TRIANGLE;
	demoPersons[1].taskCount = 6;
	
	// Jack's tasks
	demoPersons[1].tasks[0] = {"Make Bed", "Make your bed nice", DemoColours::RED, DemoPatterns::DIAMOND, false, 0, 2};
	demoPersons[1].tasks[1] = {"Brush Teeth", "Clean teeth well", DemoColours::BLUE, DemoPatterns::CROSS, false, 0, 1};
	demoPersons[1].tasks[2] = {"Feed Goldfish", "Give fish food", DemoColours::YELLOW, DemoPatterns::CIRCLE, false, 0, 1};
	demoPersons[1].tasks[3] = {"Homework", "Complete maths work", DemoColours::PURPLE, DemoPatterns::BOOK, false, 0, 1};
	demoPersons[1].tasks[4] = {"Football Practice", "Kick ball outside", DemoColours::LIME, DemoPatterns::SPORT, false, 0, 3};
	demoPersons[1].tasks[5] = {"Help Cook", "Assist with dinner", DemoColours::ORANGE, DemoPatterns::HEART, false, 0, 2};
}

void DemoManager::setupDemoWeather() {
	demoWeather.condition = "Partly Cloudy";
	demoWeather.temperature = 22;
	demoWeather.colour = DemoColours::CYAN;
	demoWeather.icon = DemoPatterns::CLOUD;
	demoWeather.description = "Perfect day for outdoor activities!";
}

void DemoManager::setupDemoFamily() {
	familyEventCount = 4;
	
	demoFamilyEvents[0] = {"Swimming Lesson", "3:00 PM", DemoColours::BLUE, DemoPatterns::SPORT, true};
	demoFamilyEvents[1] = {"Pizza Night", "6:00 PM", DemoColours::RED, DemoPatterns::HEART, true};
	demoFamilyEvents[2] = {"Movie Time", "7:30 PM", DemoColours::PURPLE, DemoPatterns::STAR, false};
	demoFamilyEvents[3] = {"Bedtime Story", "8:30 PM", DemoColours::GOLD, DemoPatterns::BOOK, false};
}

void DemoManager::toggleDemoTask(int personIndex, int taskIndex) {
	if (personIndex < 0 || personIndex >= 2 || taskIndex < 0 || taskIndex >= 6) return;
	
	DemoTask& task = demoPersons[personIndex].tasks[taskIndex];
	task.isCompleted = !task.isCompleted;
	
	if (task.isCompleted) {
		task.completedTime = millis();
		Serial.printf("Demo: %s completed '%s' - great job! ⭐\n", 
		              demoPersons[personIndex].name.c_str(), task.name.c_str());
	} else {
		task.completedTime = 0;
		Serial.printf("Demo: %s reset '%s'\n", 
		              demoPersons[personIndex].name.c_str(), task.name.c_str());
	}
}

void DemoManager::completeDemoTask(int personIndex, int taskIndex) {
	if (personIndex < 0 || personIndex >= 2 || taskIndex < 0 || taskIndex >= 6) return;
	
	DemoTask& task = demoPersons[personIndex].tasks[taskIndex];
	if (!task.isCompleted) {
		task.isCompleted = true;
		task.completedTime = millis();
		Serial.printf("Demo: %s completed '%s'! 🎉\n", 
		              demoPersons[personIndex].name.c_str(), task.name.c_str());
	}
}

void DemoManager::resetDemoTask(int personIndex, int taskIndex) {
	if (personIndex < 0 || personIndex >= 2 || taskIndex < 0 || taskIndex >= 6) return;
	
	DemoTask& task = demoPersons[personIndex].tasks[taskIndex];
	task.isCompleted = false;
	task.completedTime = 0;
	Serial.printf("Demo: %s reset '%s'\n", 
	              demoPersons[personIndex].name.c_str(), task.name.c_str());
}

void DemoManager::updateAnimations() {
	animationPhase = (animationPhase + 1) % 100;  // 0-99 cycle
	
	// Update theme colour with smooth transitions
	currentThemeColour = getNextAnimationColour();
}

void DemoManager::cycleDemoColours() {
	if (animationDirection) {
		colourPhase = (colourPhase + 1) % DEMO_COLOUR_COUNT;
	} else {
		colourPhase = (colourPhase - 1 + DEMO_COLOUR_COUNT) % DEMO_COLOUR_COUNT;
	}
	
	// Change direction occasionally for variety
	if (random(100) < 10) {  // 10% chance to change direction
		animationDirection = !animationDirection;
	}
}

uint16_t DemoManager::getNextAnimationColour() {
	// Smoothly cycle through demo colours
	return DEMO_COLOURS[colourPhase];
}

void DemoManager::showDemoStats() {
	Serial.println("📊 Demo Mode Stats:");
	
	for (int person = 0; person < 2; person++) {
		int completed = getCompletedTaskCount(person);
		int total = getTotalTaskCount(person);
		float percentage = getCompletionPercentage(person);
		
		Serial.printf("   %s: %d/%d tasks (%.0f%%) 🌟\n", 
		              demoPersons[person].name.c_str(), completed, total, percentage);
	}
	
	Serial.printf("   Weather: %s, %d°C ☀️\n", 
	              demoWeather.condition.c_str(), demoWeather.temperature);
	
	unsigned long runtime = (millis() - demoStartTime) / 1000;
	Serial.printf("   Demo runtime: %lu seconds 🕐\n", runtime);
}

bool DemoManager::isDemoActive() {
	return demoActive;
}

unsigned long DemoManager::getDemoStartTime() {
	return demoStartTime;
}

DemoPerson* DemoManager::getDemoPerson(int personIndex) {
	if (personIndex >= 0 && personIndex < 2) {
		return &demoPersons[personIndex];
	}
	return nullptr;
}

DemoWeather* DemoManager::getDemoWeather() {
	return &demoWeather;
}

DemoFamily* DemoManager::getDemoFamily() {
	return demoFamilyEvents;
}

String DemoManager::getPersonTaskSummary(int personIndex) {
	if (personIndex < 0 || personIndex >= 2) return "Invalid person";
	
	int completed = getCompletedTaskCount(personIndex);
	int total = getTotalTaskCount(personIndex);
	
	return String(completed) + "/" + String(total) + " tasks done";
}

int DemoManager::getCompletedTaskCount(int personIndex) {
	if (personIndex < 0 || personIndex >= 2) return 0;
	
	int count = 0;
	for (int i = 0; i < demoPersons[personIndex].taskCount; i++) {
		if (demoPersons[personIndex].tasks[i].isCompleted) {
			count++;
		}
	}
	return count;
}

int DemoManager::getTotalTaskCount(int personIndex) {
	if (personIndex >= 0 && personIndex < 2) {
		return demoPersons[personIndex].taskCount;
	}
	return 0;
}

float DemoManager::getCompletionPercentage(int personIndex) {
	int total = getTotalTaskCount(personIndex);
	if (total == 0) return 0.0;
	
	int completed = getCompletedTaskCount(personIndex);
	return (completed * 100.0) / total;
}

uint16_t DemoManager::getCurrentThemeColour() {
	return currentThemeColour;
}

void DemoManager::resetDemo() {
	Serial.println("Resetting demo content to initial state");
	initializeDemoContent();
} 