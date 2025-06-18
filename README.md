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

### Components

| Component | Quantity | Specifications | Estimated Cost |
|-----------|----------|----------------|----------------|
| ESP32 DevKitC V4 | 1 | 38-pin development board | ~£7 |
| ST7789 TFT Display | 8 | 1.9" IPS, 170x320, SPI | ~£3-4 each |
| Latching Switches | 8 | 5-pin with integrated LED, 3-6V | User sourced |
| Momentary Buttons | 4 | Wake + 3 screen selectors | ~£1 each |
| Power Supply | 1 | 1A USB supply (5V to 3.3V) | ~£5 |

### GPIO Pinout

```
ESP32 Pin Assignments (28/30 pins used):
├── Switches (Input):     13, 12, 14, 27, 26, 33, 35, 34
├── LEDs (Output):        15, 2, 0, 4, 5, 25, 32, 21
├── TFT CS (Output):      9, 10, 1, 3, 39, 36, 37, 38
├── TFT Shared:           SCK(18), MOSI(23), DC(19), RST(-1)
├── TFT Backlight:        8 (all displays)
├── Buttons (Input):      16(Wake), 17, 22, 7(Screens 1-3)
└── Spare:                11 (available for buzzer/expansion)
```

### Power Requirements
- **Total Consumption**: ~460mA maximum
- **Supply**: 1A USB (5V), regulated to 3.3V
- **TFT Backlights**: Controlled via GPIO 8 with 100Ω resistors

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

### Basic Operation

1. **Wake Device**: Press wake button to activate displays
2. **Select Screen**: Use screen buttons (1-3) to switch views
3. **Complete Tasks**: Flip latching switches to mark tasks complete
4. **View Progress**: LEDs light up, displays turn green for completed tasks
5. **Earn Rewards**: Completed task sets display reward information

### Screen Navigation
- **Wake Button**: Toggle device on/off, return to Screen 0
- **Button 1**: Switch to Kid 2's tasks (Screen 1)
- **Button 2**: Switch to Weather display (Screen 2)  
- **Button 3**: Switch to Shared family tasks (Screen 3)

### Summer Schedule Mode
On holidays, the device operates in schedule mode:
1. Completed tasks fade to greyscale
2. Remaining tasks shift left automatically
3. New tasks appear from the HA queue
4. Time-based task progression available

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
