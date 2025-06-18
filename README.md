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

If there's a way to show the "prize" which they get once the tasks are complete and also a time this would be good too... eg. at 15:00 it could show chores like "Empty Dishwasher", "Tidy Room" and they win "Screen Time" at 16:00.

Please write all this up into a readme.md so I can start a git repo and get some developers on the case whilist I start building the device
