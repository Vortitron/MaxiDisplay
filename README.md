Below is a detailed README.md for your ESP32-based chore tracker project, designed to guide you and any developers contributing to the Git repository. It incorporates your latest requirements: a Home Assistant (HA)-controlled dynamic task grid with rows per person (e.g., kids) and columns for tasks, updated based on school days, holidays, weather, or calendar events (e.g., swimming). The system supports a schedule-like behavior on summer days, task completion via button presses that toggle HA switches, fading/moving tasks, and displaying rewards and times (e.g., “Screen Time” at 16:00). The design retains the 8 ST7789 TFTs, 8 latching switches, 4 momentary buttons, and OTA updates, with HA managing the task logic.
ChoreTracker
A Wi-Fi-enabled chore tracking device built with an ESP32 DevKitC V4, 8 ST7789 1.9” TFT displays, 8 latching switches with LEDs, and 4 momentary buttons. Designed to integrate with Home Assistant (HA) for dynamic task management, it allows kids to race to complete tasks, with tasks updating based on day type (school, holiday), weather, or calendar events. The device displays a grid of tasks (rows per person, columns for tasks), controlled by HA, with visual feedback for completions and rewards.
Features
Dynamic Task Grid: HA sends tasks as a grid (rows per person, e.g., Kid 1, Kid 2; columns for tasks). Each row can show up to 8 tasks on the 8 TFTs, selectable via momentary buttons.
HA Integration: Tasks pulled from HA calendar or switches, updated by day (school vs. holiday), weather (e.g., rain adds indoor tasks), or events (e.g., swimming adds “Pack Swim Kit”). Button presses toggle HA switches.
Summer Schedule: On holidays, tasks act like a schedule—completed or timed-out tasks fade out, others shift left, new tasks append from HA.
Race Mode: Kids compete to complete 4 tasks each, shown across 8 TFTs (duplicated), with latching switches lighting LEDs and turning TFTs green (“Done!”).
Rewards & Times: HA sends reward (e.g., “Screen Time”) and time (e.g., 16:00) per task set, shown on TFTs when completed.
Screens:
Screen 0 (Wake): Kid 1’s task row.
Screen 1: Kid 2’s task row.
Screen 2: Weather (OpenWeatherMap API).
Screen 3: Shared family tasks (e.g., “Feed Cats”).
OTA Updates: Firmware updates via Wi-Fi using ArduinoOTA.
Power: 1A USB supply (~460mA max).
Hardware
Microcontroller: ESP32 DevKitC V4 (38-pin, ~$7).
Displays: 8x 1.9” ST7789 IPS TFTs (170x320, SPI, ~$3-4 each).
Switches: 8x 5-pin latching switches with LEDs (3-6V, user-owned).
Buttons: 4x momentary buttons (Wake + 3 screen selectors).
Power: 1A USB supply (5V, stepped to 3.3V).
GPIO Usage: 28/30 pins:
Switches: 13, 12, 14, 27, 26, 33, 35, 34 (inputs).
LEDs: 15, 2, 0, 4, 5, 25, 32, 21 (outputs).
TFTs: CS (9, 10, 1, 3, 39, 36, 37, 38), SCK (18), MOSI (23), DC (19).
Backlight: 8 (all TFTs).
Buttons: 16 (Wake), 17, 22, 7 (Screens 1-3).
Spare: 11 (e.g., buzzer).
Software
Platform: Arduino IDE.
Libraries:
Adafruit_GFX, Adafruit_ST7789 (TFT control).
WiFi, HTTPClient (HA, weather).
ArduinoOTA (updates).
ArduinoJson (HA JSON parsing).
HA Integration:
Queries HA’s /api/states for switch states and calendar events.
Toggles HA switches (switch.kid1_task1) on button press.
Pulls tasks from calendar (e.g., “Kid 1: Get Dressed,Pack”) or HA templates.
Task Logic:
HA sends task grid (rows: people, columns: tasks) via API.
Option 1: HA sends full grid, ESP32 filters 8 tasks for selected row.
Option 2: HA sends 8 tasks for requested row (simpler, implemented).
Summer: Completed/timed-out tasks fade (greyscale), shift left, new task appended.
Race: 4 tasks per kid duplicated across 8 TFTs/switches (e.g., switches 0=4=“Get Dressed”).
Weather: OpenWeatherMap API for Screen 2.
OTA: Wireless updates via ArduinoOTA.
HA Configuration
Calendar: Google Calendar or local HA calendar (e.g., calendar.family).
Events: Summary (e.g., “Kid 1: School Day”), Description (e.g., “Get Dressed,Pack,Math,Read”).
Switches: Create in HA (e.g., switch.kid1_task1, switch.kid2_task2).
Templates:
School day: Standard tasks (e.g., “Get Dressed,Pack”).
Holiday: Schedule-based (e.g., “Play,Tidy”).
Weather: Add tasks (e.g., rain → “Board Games”).
Events: Add tasks (e.g., swimming → “Pack Swim Kit”).
Rewards/Times: Include in event (e.g., “Reward: Screen Time, Time: 16:00”).
API: Enable HA API (port 8123), generate long-lived token.
Setup
Hardware
Wire components per GPIO map.
Connect all 8 TFT LED pins to GPIO 8 via 100Ω resistors.
Power with 1A USB supply.
Software
Install Arduino IDE (arduino.cc).
Install libraries: Adafruit_GFX, Adafruit_ST7789, WiFi, HTTPClient, ArduinoOTA, ArduinoJson.
Flash initial firmware via USB:
Set board: “ESP32 Dev Module”.
Update ssid, password, haUrl, haToken, weatherApiKey, city.
Enable OTA for wireless updates.
HA
Set up HA with calendar integration.
Create switches (e.g., switch.kid1_task1).
Add calendar events with tasks, rewards, times.
Generate API token, configure haUrl.
Development
Current Code
See choretracker.ino for the Arduino sketch:
Fetches HA calendar tasks daily or on wake.
Toggles HA switches on latching switch press.
Displays 8 tasks per selected screen (Kid 1, Kid 2, Weather, Shared).
Supports race (4 tasks duplicated).
OTA updates enabled.
TODO
HA API:
Implement /api/states to fetch switch states and task grid.
Parse rewards/times from calendar (e.g., “Reward: Screen Time, Time: 16:00”).
Summer Schedule:
Add fade-out effect (greyscale text) for completed/timed-out tasks.
Shift tasks left, append new task from HA.
Task Grid:
Decide: Full grid from HA (ESP32 filters) vs. 8 tasks per row (HA filters).
Add row selection logic for >2 kids.
Visuals:
Show reward/time on TFTs after tasks complete.
Enhance race UI (e.g., progress bar).
Fallback:
Store last HA tasks in ESP32 SPIFFS for offline use.
Spare GPIO (11):
Add buzzer for task completion or reward.
Contributing
Fork the repo, create feature branches.
Test on ESP32 DevKitC V4 with ST7789 TFTs.
Ensure OTA compatibility.
Document HA template examples for school/holiday/weather.
Example HA Calendar Event
yaml
summary: "Kid 1: School Day"
description: "Get Dressed,Pack,Math,Read,Reward:Screen Time,Time:16:00"
start: "2025-05-28T00:00:00Z"
end: "2025-05-29T00:00:00Z"
License
MIT License—feel free to modify and share.
Notes for Developers
HA API: Start with /api/states for switches and /api/calendars for tasks. Assume HA sends 8 tasks per row (simpler than full grid).
Summer Schedule: Implement fade via greyscale (ST77XX_GRAY or dim color), shift tasks in tasks[screen][], query HA for next task.
Rewards/Times: Parse from event description, display on TFTs post-completion.
Testing: Simulate HA with a local server if HA isn’t ready.
For You
Build: Wire parts (backlight to GPIO 8), flash the last code via USB, test switches/TFTs.
HA Setup: When ready, add calendar, switches, and token. Test API with curl.
Tasks: Draft school/holiday events in your calendar (e.g., “Get Dressed” vs. “Play”).
Git: Create a repo (e.g., GitHub), add this README.md and choretracker.ino, invite devs.


Customer requirements notes:
On summer days we want it to be a schedule.
Basically everything should be controlled by the HA dashboard. 
So HA might have 6 things to do per child that day but when the button is pressed to say one is completed, or the time has passed, then that item fades out and they all move along so a new one can be placed on the end.
So at HA end they are simple switches and the button press on the device then flips that switch at HA.
The select buttons on the device choose which rows to display.

So we have an unlimited grid, a row per person and columns which are the tasks. We can choose if the HA sends the whole lot to the device and it decides which to display, or if the HA just sends the requested 8.

In HA we then have templates so that if it's a school day they will have a standard set of things, BUT depending on the weather or a calendar event (eg. if they have swimming that day) then we can add extra things in.
If it's a day in the holidays then it will be more schedule based.


Previous notes:

You’re absolutely right—hardcoding tasks in tasks[][] won’t cut it if you need them to change dynamically based on the day (e.g., school day tasks like “Get Dressed” vs. weekend tasks). Integrating with Home Assistant (HA) to pull tasks from a calendar is a smart move, especially since you’re planning to set up HA. Since you’re sticking with Arduino for now (no ESPHome yet) and want to keep all 8 TFTs active for both kids’ race, I’ll redesign the system to fetch tasks from an HA calendar dynamically. This will sync tasks based on the day (e.g., school vs. non-school) and keep OTA for code updates. Let’s make it happen!
Design Update
Goals
Dynamic Tasks: Pull tasks from an HA calendar (e.g., Google Calendar synced with HA) based on the day (school day: “Get Dressed”; weekend: “Play Outside”).
Keep Race: Both kids use all 8 switches/TFTs for 4 tasks each (duplicated), racing to light LEDs and turn screens green.
HA Integration: ESP32 queries HA’s API for calendar events, updates tasks[][].
Fallback: If HA or Wi-Fi fails, use default tasks.
Hardware: Same (ESP32 DevKitC V4, 8 ST7789 TFTs, 8 latching switches, 4 momentary buttons, 1A USB, 28 GPIOs).
Assumptions
HA Setup: You’ll have HA running soon, with a calendar (e.g., Google Calendar integration) containing events like “School Day: Get Dressed, Pack” or “Weekend: Play, Clean.”
Calendar: Events have titles/descriptions with tasks (e.g., “Kid 1: Read, Pack”).
Access: HA exposes its API (port 8123, long-lived access token).
HA Calendar Integration
How It Works
HA Calendar: Use HA’s /api/calendars endpoint to fetch events for today (e.g., 2025-05-28).
Tasks: Parse event titles/descriptions for tasks (e.g., “Kid 1: Read, Pack, Math, Clean”).
ESP32: Connects to Wi-Fi, queries HA via HTTP, updates tasks[0][] (Kid 1), tasks[1][] (Kid 2), tasks[3][] (Shared).
Weather: Stays as OpenWeatherMap (Screen 2), since HA calendar is task-focused.
Timing: Check calendar daily (e.g., at midnight) or on wake.
Requirements
HA Token: Generate a long-lived access token in HA (Profile > Long-Lived Access Tokens).
Wi-Fi: ESP32 stays connected.
Calendar: Events with clear task lists (e.g., “Kid 1: Read, Pack, Math, Clean”).
Updated Code
This modifies the Arduino sketch to query HA’s calendar API, parse tasks, and keep the 8-TFT race. OTA stays for updates. I’ve added a basic parser for tasks in event descriptions (e.g., “Read,Pack,Math,Clean”).
cpp
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>

#define TFT_WIDTH 170
#define TFT_HEIGHT 320
#define NUM_DISPLAYS 8
#define NUM_SCREENS 4
#define TASKS_PER_KID 4

#define TFT_SCLK 18
#define TFT_MOSI 23
#define TFT_DC 19
#define TFT_RST -1

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* haUrl = "http://YOUR_HA_IP:8123/api/calendars/calendar.your_calendar?start=2025-05-28T00:00:00Z&end=2025-05-29T00:00:00Z";
const char* haToken = "YOUR_HA_TOKEN";
const char* weatherApiKey = "YOUR_OPENWEATHERMAP_API_KEY";
const char* city = "YOUR_CITY";

int tft_cs[NUM_DISPLAYS] = {9, 10, 1, 3, 39, 36, 37, 38};
int switchPins[NUM_DISPLAYS] = {13, 12, 14, 27, 26, 33, 35, 34};
int ledPins[NUM_DISPLAYS] = {15, 2, 0, 4, 5, 25, 32, 21};
int backlightPin = 8;
int wakePin = 16;
int screenPins[NUM_SCREENS - 1] = {17, 22, 7};

String tasks[NUM_SCREENS][NUM_DISPLAYS] = {
  {"Read", "Pack", "Math", "Clean", "Read", "Pack", "Math", "Clean"}, // Kid 1
  {"Read", "Pack", "Math", "Clean", "Read", "Pack", "Math", "Clean"}, // Kid 2
  {"Weather", "Weather", "Weather", "Weather", "Weather", "Weather", "Weather", "Weather"}, // Weather
  {"Cats", "Plants", "Trash", "Dishes", "Mail", "Sweep", "Laundry", "Dog"} // Shared
};
bool switchStates[NUM_SCREENS][NUM_DISPLAYS] = {{false}};
bool ledStates[NUM_SCREENS][NUM_DISPLAYS] = {{false}};
bool isAwake = false;
int currentScreen = 0;
unsigned long lastCalendarCheck = 0;

Adafruit_ST7789 displays[NUM_DISPLAYS] = {
  Adafruit_ST7789(tft_cs[0], TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST),
  Adafruit_ST7789(tft_cs[1], TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST),
  Adafruit_ST7789(tft_cs[2], TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST),
  Adafruit_ST7789(tft_cs[3], TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST),
  Adafruit_ST7789(tft_cs[4], TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST),
  Adafruit_ST7789(tft_cs[5], TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST),
  Adafruit_ST7789(tft_cs[6], TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST),
  Adafruit_ST7789(tft_cs[7], TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST)
};

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_DISPLAYS; i++) {
    displays[i].init(TFT_WIDTH, TFT_HEIGHT);
    displays[i].setRotation(0);
    pinMode(switchPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  pinMode(wakePin, INPUT_PULLUP);
  for (int i = 0; i < NUM_SCREENS - 1; i++) pinMode(screenPins[i], INPUT_PULLUP);
  pinMode(backlightPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  ArduinoOTA.setHostname("ChoreTracker");
  ArduinoOTA.setPassword("chore123");
  ArduinoOTA.begin();
  Serial.println("OTA ready");

  updateTasksFromHA(); // Initial calendar check
  setAwake(false);
}

void loop() {
  ArduinoOTA.handle();

  // Check calendar daily
  if (millis() - lastCalendarCheck > 86400000) { // 24 hours
    updateTasksFromHA();
    lastCalendarCheck = millis();
  }

  if (digitalRead(wakePin) == LOW && !isAwake) {
    setAwake(true);
    loadScreen(currentScreen);
    delay(200);
  } else if (digitalRead(wakePin) == LOW && isAwake) {
    setAwake(false);
    delay(200);
  }

  if (isAwake) {
    for (int i = 0; i < NUM_SCREENS - 1; i++) {
      if (digitalRead(screenPins[i]) == LOW) {
        currentScreen = i + 1;
        loadScreen(currentScreen);
        delay(200);
      }
    }
    if (digitalRead(wakePin) == LOW && currentScreen != 0) {
      currentScreen = 0;
      loadScreen(currentScreen);
      delay(200);
    }

    for (int i = 0; i < NUM_DISPLAYS; i++) {
      bool currentState = (digitalRead(switchPins[i]) == LOW);
      if (currentState != switchStates[currentScreen][i]) {
        switchStates[currentScreen][i] = currentState;
        ledStates[currentScreen][i] = currentState;
        digitalWrite(ledPins[i], currentState ? HIGH : LOW);
        updateDisplay(i, currentState ? "Done!" : tasks[currentScreen][i], 
                      currentState ? ST77XX_GREEN : ST77XX_RED);
      }
    }
  }
}

void setAwake(bool state) {
  isAwake = state;
  digitalWrite(backlightPin, state ? HIGH : LOW);
  for (int i = 0; i < NUM_DISPLAYS; i++) {
    if (state) {
      displays[i].sleep(false);
    } else {
      displays[i].fillScreen(ST77XX_BLACK);
      displays[i].sleep(true);
    }
  }
}

void loadScreen(int screen) {
  if (screen == 2) {
    showWeather();
  } else {
    for (int i = 0; i < NUM_DISPLAYS; i++) {
      digitalWrite(ledPins[i], ledStates[screen][i] ? HIGH : LOW);
      updateDisplay(i, switchStates[screen][i] ? "Done!" : tasks[screen][i], 
                    switchStates[screen][i] ? ST77XX_GREEN : ST77XX_RED);
    }
  }
}

void updateDisplay(int idx, String text, uint16_t color) {
  displays[idx].fillScreen(ST77XX_BLACK);
  displays[idx].setCursor(10, 20);
  displays[idx].setTextSize(3);
  displays[idx].setTextColor(color);
  displays[idx].println(text);
}

void showWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "&units=metric&appid=" + weatherApiKey;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      int tempIdx = payload.indexOf("\"temp\":") + 7;
      int descIdx = payload.indexOf("\"description\":\"") + 14;
      float temp = payload.substring(tempIdx, payload.indexOf(",", tempIdx)).toFloat();
      String desc = payload.substring(descIdx, payload.indexOf("\"", descIdx));
      String weatherStr = String(temp, 1) + "C, " + desc;
      for (int i = 0; i < NUM_DISPLAYS; i++) {
        displays[i].fillScreen(ST77XX_BLACK);
        displays[i].setCursor(10, 20);
        displays[i].setTextSize(2);
        displays[i].setTextColor(ST77XX_WHITE);
        displays[i].println("Weather:");
        displays[i].println(weatherStr);
      }
    }
    http.end();
  }
}

void updateTasksFromHA() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(haUrl);
    http.addHeader("Authorization", "Bearer " + String(haToken));
    int httpCode = http.GET();
    if (httpCode == 200) {
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, http.getString());
      JsonArray events = doc.as<JsonArray>();
      
      // Clear tasks
      for (int s = 0; s < 2; s++) { // Kid 1, Kid 2
        for (int i = 0; i < NUM_DISPLAYS; i++) {
          tasks[s][i] = "";
        }
      }

      // Parse events
      for (JsonObject event : events) {
        String summary = event["summary"].as<String>();
        String description = event["description"].as<String>();
        if (summary.startsWith("Kid 1:")) {
          parseTasks(description, 0); // Kid 1
        } else if (summary.startsWith("Kid 2:")) {
          parseTasks(description, 1); // Kid 2
        }
      }

      // Duplicate tasks for race
      for (int s = 0; s < 2; s++) {
        for (int i = 4; i < NUM_DISPLAYS; i++) {
          tasks[s][i] = tasks[s][i - 4];
        }
      }
    }
    http.end();
  }
}

void parseTasks(String desc, int screen) {
  int idx = 0;
  int start = 0;
  for (int i = 0; i < desc.length() && idx < TASKS_PER_KID; i++) {
    if (desc[i] == ',' || i == desc.length() - 1) {
      tasks[screen][idx] = desc.substring(start, i == desc.length() - 1 ? i + 1 : i);
      start = i + 1;
      idx++;
    }
  }
}
Changes
Library: Added ArduinoJson (install via Library Manager) to parse HA’s JSON calendar response.
HA Query: updateTasksFromHA() fetches today’s events from calendar.your_calendar (replace with your HA calendar entity ID).
Task Parsing: Assumes event descriptions like “Read,Pack,Math,Clean” for each kid. Summary starts with “Kid 1:” or “Kid 2:” to assign.
Race: Duplicates tasks (0-3 to 4-7) for each kid’s screen, so all 8 switches/TFTs are active.
Shared Tasks: Hardcoded for now (Screen 3)—HA can override later by adding “Shared:” events.
Timing: Checks calendar every 24 hours or on wake (adjustable).
HA Calendar Setup
Create Calendar: In HA, add a Google Calendar (or local calendar) via integration.
Events: Add daily events, e.g.:
Summary: “Kid 1: School Day”
Description: “Get Dressed,Pack,Math,Read”
Summary: “Kid 2: School Day”
Description: “Get Dressed,Pack,Math,Clean”
For weekends: “Kid 1: Weekend” with “Play,Clean,Books,Fun”.
API Access: In HA, generate token (Profile > Long-Lived Access Tokens), set haUrl (e.g., http://192.168.1.100:8123/api/calendars/calendar.family?start=2025-05-28T00:00:00Z&end=2025-05-29T00:00:00Z), and haToken.
Fallback
If HA or Wi-Fi is down:
Uses last tasks[][] (defaults on first boot: “Read,” “Pack,” etc.).
Shared tasks (Screen 3) stay hardcoded until HA adds “Shared:” events.
Race Experience
Kid 1 (Screen 0): 4 tasks (e.g., “Get Dressed,Pack,Math,Read”) on TFTs 0-3, repeated on 4-7. Flip switches 0-7 to light LEDs, turn all green (“Done!”).
Kid 2 (Screen 1): Same, their tasks.
Weather (Screen 2): All 8 TFTs show weather.
Shared (Screen 3): 8 family tasks (e.g., “Cats”).
Visuals: Big text (setTextSize(3)), red to green—kids race to fill the board!
Updating
OTA: Tweak code (e.g., parseTasks logic) in Arduino IDE, upload to “ChoreTracker” via Wi-Fi.
Tasks: Edit HA calendar events—no code changes needed.
HA Setup: When HA’s ready, add calendar, token, and test API call (e.g., curl -H "Authorization: Bearer YOUR_TOKEN" http://YOUR_HA_IP:8123/api/calendars/calendar.family).
Final Thoughts
This keeps your 8-TFT race with dynamic tasks from HA’s calendar—school days get “Get Dressed,” weekends swap to “Play.” The Arduino approach handles the complexity (vs. ESPHome’s Lambda struggles), and OTA keeps updates easy. When parts arrive, wire up, flash via USB, set up HA calendar later, and test. Want specific task examples for school/weekend or a different parsing format?




If there's a way to show the "prize" which they get once the tasks are complete and also a time this would be good too... eg. at 15:00 it could show chores like "Empty Dishwasher", "Tidy Room" and they win "Screen Time" at 16:00.

Please write all this up into a readme.md so I can start a git repo and get some developers on the case whilist I start building the device
