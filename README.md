# ChoreTracker

A Wi-Fi-enabled chore tracking device built with ESP32 and Home Assistant integration. Features 8 ST7789 TFT displays, latching switches with LEDs, and dynamic task management for family chore coordination.

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-orange.svg)

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware](#hardware)
- [Software Architecture](#software-architecture)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [API Documentation](#api-documentation)
- [Development](#development)
- [Contributing](#contributing)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## Overview

ChoreTracker is designed to gamify household chores for children through a dynamic, interactive display system. It integrates with Home Assistant to manage tasks based on school schedules, holidays, weather conditions, and calendar events. Children can race to complete tasks, earn rewards, and view their progress on dedicated TFT displays.

### Key Concepts

- **Dynamic Task Grid**: Tasks organised as rows (per person) and columns (tasks)
- **Schedule Mode**: For holidays - tasks fade out when completed/timed-out, new tasks appear
- **Race Mode**: Children compete to complete their assigned tasks
- **Reward System**: Display prizes and times (e.g., "Screen Time at 16:00")
- **Smart Integration**: Tasks update based on school days, weather, and calendar events

## Features

### Core Functionality
- **8 TFT Displays**: Individual task displays with visual feedback
- **Latching Switches**: Physical task completion confirmation with LED feedback
- **Home Assistant Integration**: Dynamic task management via HA API
- **Weather Integration**: OpenWeatherMap API for weather-based task adjustments
- **OTA Updates**: Wireless firmware updates
- **Power Management**: Automatic sleep/wake functionality

### Display Modes
- **Screen 0**: Child 1's task row
- **Screen 1**: Child 2's task row  
- **Screen 2**: Weather information
- **Screen 3**: Shared family tasks

### Smart Features
- **Context-Aware Tasks**: Different tasks for school days vs holidays
- **Weather-Responsive**: Add indoor tasks when raining
- **Event-Triggered**: Special tasks for calendar events (e.g., swimming)
- **Reward Display**: Show earned rewards and availability times
- **Task Progression**: Summer schedule mode with automatic task shifting

## Hardware

### Physical Layout
ChoreTracker features a **2×4 display grid** with intuitive navigation:
- **8 TFT Displays**: Arranged as 2 rows × 4 columns
- **8 Task Buttons**: Non-momentary (latching) switches, one per display
- **8 Task LEDs**: Independent LED control, one per button
- **2 Row Select Buttons**: Momentary buttons for cycling through columns
- **LED Logic**: ON = Task Incomplete, OFF = Task Complete

### Components

| Component | Quantity | Specifications | Estimated Cost |
|-----------|----------|----------------|----------------|
| ESP32 DevKitC V4 | 1 | 38-pin development board | ~£7 |
| ST7789 TFT Display | 8 | 1.9" IPS, 170×320, SPI | ~£3-4 each |
| Task Buttons | 8 | Non-momentary with integrated LED | User sourced |
| Row Select Buttons | 2 | Momentary buttons for navigation | ~£1 each |
| Power Supply | 1 | 1A USB supply (5V to 3.3V) | ~£5 |

### GPIO Pinout

```
ESP32 Pin Configuration:
├── Task Buttons (Input):     13, 12, 14, 27, 26, 33, 35, 34  # 8 non-momentary switches
├── Task LEDs (Output):       15, 2, 0, 4, 5, 25, 32, 21     # 8 independent LEDs
├── Row Select (Input):       17, 22                          # 2 momentary navigation buttons
├── TFT CS (Output):          9, 10, 1, 3, 39, 36, 37, 38    # 8 display CS pins
├── TFT Shared:               SCK(18), MOSI(23), DC(19), RST(-1)
├── TFT Backlight:            8 (all displays)
└── Spare:                    11, 16, 7 (buzzer, expansion)
```

### Display Layout

```
Physical Layout:
┌─────────┬─────────┬─────────┬─────────┐ Row 1 [Select Button: GPIO 17]
│ Disp 0  │ Disp 1  │ Disp 2  │ Disp 3  │
│ Child 1 │ Child 2 │ Weather │ Family  │
└─────────┴─────────┴─────────┴─────────┘
┌─────────┬─────────┬─────────┬─────────┐ Row 2 [Select Button: GPIO 22]
│ Disp 4  │ Disp 5  │ Disp 6  │ Disp 7  │
│ Tasks   │ Tasks   │ Info    │ Tasks   │
└─────────┴─────────┴─────────┴─────────┘

Task Buttons (One per Display):
[Btn0/Disp0: GPIO13+LED15] [Btn1/Disp1: GPIO12+LED2] [Btn2/Disp2: GPIO14+LED0] [Btn3/Disp3: GPIO27+LED4]
[Btn4/Disp4: GPIO26+LED5]  [Btn5/Disp5: GPIO33+LED25] [Btn6/Disp6: GPIO35+LED32] [Btn7/Disp7: GPIO34+LED21]
```

### Hardware Operation

#### Button Types
1. **Task Buttons (Non-Momentary)**:
   - Latching switches that change state when pressed
   - We detect **state changes**, not button presses
   - LEDs work independently from button position
   - Used for task completion confirmation

2. **Row Select Buttons (Momentary)**:
   - Traditional push buttons
   - Cycle through columns for their respective row
   - Show column title on screen 1 of the row for 2 seconds

### Power Requirements
- **Total Consumption**: ~460mA maximum
- **Supply**: 1A USB (5V), regulated to 3.3V
- **TFT Backlights**: Controlled via GPIO 8

## Software Architecture

### Platform & Libraries
- **Platform**: Arduino IDE
- **Core Libraries**:
  - `Adafruit_GFX` & `Adafruit_ST7789` - Display control
  - `WiFi` & `HTTPClient` - Network communication
  - `ArduinoOTA` - Over-the-air updates
  - `ArduinoJson` - JSON parsing for HA API

### System Design

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Home Assistant │    │   ChoreTracker  │    │  OpenWeatherMap │
│                 │    │      ESP32      │    │       API       │
│  ┌─────────────┐│    │ ┌─────────────┐ │    │                 │
│  │  Calendar   ││◄──►│ │ Task Logic  │ │    │                 │
│  │  Switches   ││    │ │ Display Mgr │ │◄──►│                 │
│  │  Templates  ││    │ │ Input Hdlr  │ │    │                 │
│  └─────────────┘│    │ └─────────────┘ │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Task Management Logic

1. **HA Calendar Query**: Daily fetch of calendar events
2. **Task Parsing**: Extract tasks, rewards, and times from event descriptions
3. **Context Processing**: Apply templates based on day type, weather, events
4. **Display Update**: Render tasks on appropriate screens
5. **Interaction Handling**: Process switch presses, update HA states
6. **Summer Mode**: Task fading and progression on holidays

## Installation

### Prerequisites
- Arduino IDE (latest version)
- ESP32 board package
- Home Assistant instance with API access
- OpenWeatherMap API key (optional, for weather features)

### Hardware Assembly

1. **Wiring**: Follow the GPIO pinout diagram
2. **Power**: Connect 1A USB supply
3. **TFT Backlights**: Wire all 8 displays to GPIO 8 via 100Ω resistors
4. **Testing**: Verify all connections before powering on

### Software Setup

1. **Install Libraries**:
   ```bash
   # Via Arduino IDE Library Manager
   - Adafruit GFX Library
   - Adafruit ST7789 Library
   - ArduinoJson
   ```

2. **Board Configuration**:
   - Board: "ESP32 Dev Module"
   - Upload Speed: 921600
   - Flash Frequency: 80MHz

3. **Initial Flash**:
   ```bash
   # Update configuration in src/config.h
   - WiFi credentials
   - Home Assistant URL and token
   - OpenWeatherMap API key
   ```

## Configuration

### Home Assistant Setup

#### 1. Enable API Access
```yaml
# configuration.yaml
http:
  use_x_forwarded_for: true
  trusted_proxies:
    - 192.168.1.0/24
```

#### 2. Create Calendar Integration
```yaml
# Add Google Calendar or local calendar
calendar:
  - platform: google
    client_id: YOUR_CLIENT_ID
    client_secret: YOUR_CLIENT_SECRET
```

#### 3. Create Switch Entities
```yaml
# configuration.yaml
switch:
  - platform: template
    switches:
      kid1_task1:
        friendly_name: "Kid 1 Task 1"
        turn_on:
        turn_off:
      kid1_task2:
        friendly_name: "Kid 1 Task 2"
        turn_on:
        turn_off:
      # ... repeat for all tasks
```

#### 4. Generate API Token
1. Navigate to Profile → Long-Lived Access Tokens
2. Create new token for ChoreTracker
3. Update `haToken` in device configuration

### Calendar Event Format

The system supports three types of tasks that can be mixed within any day:

#### Task Types
1. **Untimed Tasks**: No time constraint, completed via button press only
2. **Fixed Time Tasks**: Auto-appear/disappear at set times, no button interaction
3. **Timed with Early Completion**: Scheduled times but can be completed early

#### Example Calendar Events

```yaml
# Summer Day with Mixed Task Types
Event 1:
  summary: "Kid 1: Summer Schedule"
  description: "Brush Teeth,8:00-9:00 Free Play,9:00-10:00 Do Some Sums*,Tidy Room,10:30-11:30 Screen Time,Help with Lunch*,Reward:Ice Cream,Time:15:00"
  start: "2025-07-15T00:00:00Z"
  end: "2025-07-16T00:00:00Z"

# School Day Example  
Event 2:
  summary: "Kid 2: School Day"
  description: "Get Dressed,Pack Bag,7:30-8:00 Breakfast*,Homework,Read,Reward:TV Time,Time:18:00"
  start: "2025-05-28T00:00:00Z"
  end: "2025-05-29T00:00:00Z"
```

#### Task Format Legend
- **Plain text** (e.g., "Brush Teeth"): Untimed task, button completion
- **Time range** (e.g., "8:00-9:00 Free Play"): Fixed time, auto-appear/disappear
- **Time range with \*** (e.g., "9:00-10:00 Do Some Sums*"): Timed with early completion

#### Parsing Logic
```
Task Format Rules:
├── "Task Name" → UNTIMED (button required)
├── "HH:MM-HH:MM Task Name" → FIXED_TIME (no button)
├── "HH:MM-HH:MM Task Name*" → TIMED_WITH_EARLY (button optional)
└── "Reward:Name,Time:HH:MM" → Reward information
```

### Task Templates

#### School Day Template
```
Standard Tasks: Get Dressed, Pack Bag, Homework, Read
Weather Additions:
  - Rain: Board Games, Indoor Exercise
  - Snow: Build Snowman, Hot Chocolate Help
Event Additions:
  - Swimming: Pack Swim Kit, Towel Ready
  - Doctor: Prepare Questions, Brush Teeth Extra
```

#### Holiday Template
```
Schedule Mode: Tasks fade when complete/timeout
Base Tasks: Free Play, Tidy Room, Help with Meal, Quiet Time
Progressive Tasks: Art Project, Garden Help, Letter Writing
```

## Usage

### Navigation System

#### Column Navigation
Each row has its own independent navigation:

1. **Press Row Select Button**: Cycles to next column
2. **Column Title Display**: Shows on first screen of row for 2 seconds
3. **Content Display**: Returns to normal content view
4. **Column Order**: Child 1 → Child 2 → Weather → Family → (repeat)

#### Task Completion
1. **Toggle Task Button**: Physical switch changes state (one button per display)
2. **System Detection**: Monitors for state changes (not button presses)  
3. **LED Feedback**: LED turns OFF when task completed, ON when incomplete
4. **HA Integration**: Updates corresponding Home Assistant switch

### Example Usage Flow

```
Initial State: Row 1 showing "Child 1" column, Row 2 showing "Child 1" column

User presses Row 1 select button:
├── Row 1 Display 0 shows "Child 2" for 2 seconds
└── Then shows Child 2's actual tasks

User presses Row 2 select button:
├── Row 2 Display 4 shows "Child 2" for 2 seconds  
└── Then shows Child 2's tasks on Row 2

User toggles Task Button 0 (for Display 0):
├── System detects button state change
├── Task LED 0 turns OFF (task complete)
├── Marks corresponding task complete
└── Updates Home Assistant switch
```

### Hardware Behaviour

#### Non-Momentary Buttons
Unlike traditional push buttons, these latching switches:
- **Stay in position** when pressed
- **Change state** rather than generate press events
- **Are monitored continuously** for state changes
- **Provide physical feedback** of completion status

#### Independent LEDs
The LEDs operate separately from button position:
- **Software controlled** via ESP32 GPIO
- **Can indicate** different states than button position
- **Provide visual feedback** for task completion
- **Can be programmed** for animations or patterns

### Navigation Logic

#### Row-Based Operation
Each row operates independently:
- **Row 1** (Displays 0-3): Controlled by select button on GPIO 17
- **Row 2** (Displays 4-7): Controlled by select button on GPIO 22

#### Column Cycling
```cpp
Column 0: "Child 1"    → Display child 1's tasks
Column 1: "Child 2"    → Display child 2's tasks  
Column 2: "Weather"    → Display weather information
Column 3: "Family"     → Display shared family tasks
```

#### Title Display Sequence
1. **Select button pressed** → Cycle to next column
2. **Show title** on screen 1 of row for 2000ms
3. **Transition to content** → Show actual tasks/information
4. **Ready for next** select button press

## API Documentation

### Home Assistant Endpoints

#### Calendar API
```http
GET /api/calendars/calendar.family?start=YYYY-MM-DDTHH:mm:ssZ&end=YYYY-MM-DDTHH:mm:ssZ
Authorization: Bearer YOUR_TOKEN

Response:
[
  {
    "summary": "Kid 1: School Day",
    "description": "Get Dressed,Pack,Math,Read,Reward:Screen Time,Time:16:00",
    "start": "2025-05-28T00:00:00Z",
    "end": "2025-05-29T00:00:00Z"
  }
]
```

#### Switch Toggle
```http
POST /api/services/switch/toggle
Authorization: Bearer YOUR_TOKEN
Content-Type: application/json

{
  "entity_id": "switch.kid1_task1"
}
```

### Device Configuration API

The device exposes configuration endpoints for runtime adjustments:

```http
# Update task refresh interval
POST /config/refresh_interval
{
  "interval_ms": 3600000
}

# Force task refresh
POST /api/refresh_tasks

# Get device status
GET /api/status
```

## Development

### Project Structure
```
ChoreTracker/
├── src/
│   ├── main.cpp              # Main application logic
│   ├── config.h              # Configuration constants
│   ├── display_manager.cpp   # TFT display handling
│   ├── ha_client.cpp         # Home Assistant API client
│   ├── task_manager.cpp      # Task logic and parsing
│   └── weather_client.cpp    # Weather API integration
├── docs/
│   ├── hardware_guide.md     # Assembly instructions
│   ├── api_reference.md      # Complete API documentation
│   └── troubleshooting.md    # Common issues and solutions
├── examples/
│   ├── ha_templates/         # Home Assistant configuration examples
│   └── calendar_events/      # Sample calendar event formats
├── tests/
│   ├── unit_tests/           # Unit tests for core functions
│   └── integration_tests/    # Hardware integration tests
└── tools/
    ├── simulator/            # Device simulator for development
    └── config_generator/     # Configuration file generator
```

### Building and Testing

```bash
# Clone repository
git clone https://github.com/yourusername/ChoreTracker.git
cd ChoreTracker

# Install dependencies
# (via Arduino IDE Library Manager)

# Build and upload
# (via Arduino IDE or PlatformIO)

# Run tests
# See tests/README.md for testing instructions
```

### OTA Updates

Once the initial firmware is flashed via USB:

1. Device appears as "ChoreTracker" on network
2. Use Arduino IDE → Tools → Port → ChoreTracker (Network)
3. Upload new firmware wirelessly
4. Device automatically reboots with new code

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details.

### Development Setup

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Test on ESP32 DevKitC V4 with ST7789 displays
4. Ensure OTA compatibility
5. Document any HA template changes
6. Submit a pull request

### Coding Standards

- Use tabs for indentation (as per user preference)
- Follow British English in comments and documentation
- Reuse functions where possible for code cleanliness
- Include error handling and logging
- Add unit tests for new functionality
- Replace hardcoded values with named constants

## Troubleshooting

### Common Issues

#### WiFi Connection Problems
```
Symptoms: Device doesn't connect to WiFi
Solutions:
- Verify SSID and password in config.h
- Check 2.4GHz network availability
- Restart router and device
- Check signal strength at device location
```

#### Home Assistant API Errors
```
Symptoms: Tasks not updating from HA
Solutions:
- Verify HA URL and port (default 8123)
- Check API token validity
- Confirm calendar entity exists
- Test API with curl command
```

#### Display Issues
```
Symptoms: TFT displays not working correctly
Solutions:
- Check SPI wiring (CS, SCK, MOSI, DC pins)
- Verify power supply (1A minimum)
- Test backlight connection (GPIO 8)
- Check for loose connections
```

#### Switch/LED Problems
```
Symptoms: Switches not registering or LEDs not lighting
Solutions:
- Verify pullup resistors enabled in code
- Check switch wiring and connections
- Test LED polarity and current limiting
- Confirm GPIO pin assignments
```

### Debug Mode

Enable debug output by setting `DEBUG_MODE true` in config.h:
```cpp
#define DEBUG_MODE true
```

Monitor serial output at 115200 baud for detailed logging.

### Support

- **Issues**: Report bugs via GitHub Issues
- **Discussions**: Use GitHub Discussions for questions
- **Documentation**: Check the docs/ folder for detailed guides

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Adafruit for excellent hardware libraries
- Home Assistant community for integration inspiration
- OpenWeatherMap for weather API services

---

**Happy Chore Tracking! 🏠✨**
