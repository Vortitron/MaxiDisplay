# ChoreTracker Project Structure

## Overview

The ChoreTracker project consists of two main components:
1. **ESP32 Firmware** - Device-side code for the physical chore tracker
2. **Home Assistant Integration** - Server-side integration for task management

## Complete Project Structure

```
ChoreTracker/
├── README.md                           # Main project documentation
├── LICENSE                             # MIT License
├── .gitignore                         # Git ignore patterns
├── DEVELOPMENT_PLAN.md                # Development roadmap
├── IMPROVEMENTS_SUMMARY.md            # Project improvements summary
├── project_outline.md                 # Project specifications
├── CONTRIBUTING.md                    # Contribution guidelines
├── CHANGELOG.md                       # Version history
│
├── esp32/                             # ESP32 Firmware Side
│   ├── platformio.ini                 # PlatformIO configuration
│   ├── src/                           # Main source code
│   │   ├── main.cpp                   # Application entry point
│   │   ├── config/
│   │   │   ├── Config.h               # Main configuration
│   │   │   ├── GPIOConfig.h           # Hardware pin definitions
│   │   │   ├── NetworkConfig.h        # Network settings
│   │   │   └── DisplayConfig.h        # Display settings
│   │   ├── core/
│   │   │   ├── TaskManager.cpp/.h     # Task logic and scheduling
│   │   │   ├── StateMachine.cpp/.h    # Application state management
│   │   │   ├── Scheduler.cpp/.h       # Task scheduling engine
│   │   │   └── RewardManager.cpp/.h   # Reward system management
│   │   ├── hardware/
│   │   │   ├── DisplayManager.cpp/.h  # TFT display management
│   │   │   ├── InputHandler.cpp/.h    # Button/switch handling
│   │   │   ├── PowerManager.cpp/.h    # Power management
│   │   │   └── AudioManager.cpp/.h    # Audio feedback (GPIO 11)
│   │   ├── network/
│   │   │   ├── HAClient.cpp/.h        # Home Assistant API client
│   │   │   ├── WeatherClient.cpp/.h   # Weather API client
│   │   │   ├── OTAManager.cpp/.h      # OTA update handling
│   │   │   └── MQTTClient.cpp/.h      # MQTT communication
│   │   ├── storage/
│   │   │   ├── StorageManager.cpp/.h  # SPIFFS persistent storage
│   │   │   ├── ConfigManager.cpp/.h   # Configuration management
│   │   │   └── TaskCache.cpp/.h       # Task caching for offline mode
│   │   ├── ui/
│   │   │   ├── Screen.cpp/.h          # Base screen class
│   │   │   ├── TaskScreen.cpp/.h      # Task display screen
│   │   │   ├── WeatherScreen.cpp/.h   # Weather information screen
│   │   │   ├── StatusScreen.cpp/.h    # System status screen
│   │   │   └── themes/
│   │   │       ├── DefaultTheme.h     # Default colour scheme
│   │   │       ├── SummerTheme.h      # Summer mode colours
│   │   │       └── DarkTheme.h        # Dark mode theme
│   │   └── utils/
│   │       ├── Logger.cpp/.h          # Logging system
│   │       ├── TimeUtils.cpp/.h       # Time and date utilities
│   │       ├── StringUtils.cpp/.h     # String manipulation
│   │       ├── JsonParser.cpp/.h      # JSON parsing utilities
│   │       └── Diagnostics.cpp/.h     # Hardware diagnostics
│   ├── lib/                           # Custom libraries
│   │   ├── ChoreTrackerDisplay/       # Display abstraction library
│   │   │   ├── library.json
│   │   │   ├── src/
│   │   │   │   ├── ChoreDisplay.cpp/.h
│   │   │   │   └── DisplayEffects.cpp/.h
│   │   │   └── examples/
│   │   │       └── BasicDisplay.cpp
│   │   └── ChoreTrackerTasks/         # Task management library
│   │       ├── library.json
│   │       ├── src/
│   │       │   ├── Task.cpp/.h
│   │       │   └── TaskQueue.cpp/.h
│   │       └── examples/
│   │           └── BasicTaskManager.cpp
│   ├── test/                          # ESP32 Testing
│   │   ├── native/                    # Native unit tests (PlatformIO)
│   │   │   ├── test_task_manager/
│   │   │   │   └── test_main.cpp
│   │   │   ├── test_display_manager/
│   │   │   │   └── test_main.cpp
│   │   │   ├── test_config_manager/
│   │   │   │   └── test_main.cpp
│   │   │   └── test_utils/
│   │   │       └── test_main.cpp
│   │   ├── embedded/                  # Hardware integration tests
│   │   │   ├── test_gpio/
│   │   │   │   └── test_main.cpp
│   │   │   ├── test_displays/
│   │   │   │   └── test_main.cpp
│   │   │   └── test_switches/
│   │   │       └── test_main.cpp
│   │   └── mocks/                     # Mock objects for testing
│   │       ├── MockHAClient.h
│   │       ├── MockDisplay.h
│   │       └── MockWiFi.h
│   ├── tools/                         # ESP32 Development Tools
│   │   ├── simulator/                 # Device simulator
│   │   │   ├── ChoreTrackerSim.py
│   │   │   ├── requirements.txt
│   │   │   └── README.md
│   │   ├── config_generator/          # Configuration generator
│   │   │   ├── generate_config.py
│   │   │   ├── config_template.json
│   │   │   └── README.md
│   │   ├── test_harness/             # Automated testing
│   │   │   ├── run_tests.py
│   │   │   ├── hardware_test.py
│   │   │   └── performance_test.py
│   │   └── flash_tool/               # Firmware flashing utility
│   │       ├── flash_choretracker.py
│   │       └── README.md
│   ├── docs/                         # ESP32 Documentation
│   │   ├── hardware/
│   │   │   ├── assembly_guide.md
│   │   │   ├── gpio_pinout.md
│   │   │   ├── power_requirements.md
│   │   │   └── pcb_design/
│   │   │       ├── schematic.pdf
│   │   │       ├── pcb_layout.pdf
│   │   │       └── bom.csv
│   │   ├── software/
│   │   │   ├── architecture.md
│   │   │   ├── api_reference.md
│   │   │   ├── configuration.md
│   │   │   └── troubleshooting.md
│   │   └── testing/
│   │       ├── unit_test_guide.md
│   │       ├── integration_test_guide.md
│   │       └── performance_testing.md
│   └── examples/                     # ESP32 Examples
│       ├── basic_display/
│       │   └── basic_display.ino
│       ├── simple_task_tracker/
│       │   └── simple_tracker.ino
│       └── weather_display/
│           └── weather_display.ino
│
├── homeassistant/                    # Home Assistant Integration Side
│   ├── custom_components/            # Custom HA integration
│   │   └── choretracker/
│   │       ├── __init__.py           # Integration entry point
│   │       ├── manifest.json         # Integration manifest
│   │       ├── config_flow.py        # Configuration flow
│   │       ├── const.py              # Constants
│   │       ├── coordinator.py        # Data update coordinator
│   │       ├── device.py             # Device representation
│   │       ├── sensor.py             # Sensor entities
│   │       ├── switch.py             # Switch entities
│   │       ├── binary_sensor.py      # Binary sensor entities
│   │       ├── calendar.py           # Calendar integration
│   │       ├── services.yaml         # Service definitions
│   │       ├── strings.json          # Localisation strings
│   │       └── translations/         # Translation files
│   │           ├── en.json
│   │           └── de.json
│   ├── config/                       # HA Configuration
│   │   ├── configuration.yaml        # Main HA config
│   │   ├── automations/
│   │   │   ├── choretracker_school_day.yaml
│   │   │   ├── choretracker_holiday.yaml
│   │   │   ├── choretracker_weather.yaml
│   │   │   └── choretracker_rewards.yaml
│   │   ├── scripts/
│   │   │   ├── create_daily_tasks.yaml
│   │   │   ├── handle_task_completion.yaml
│   │   │   └── send_rewards.yaml
│   │   ├── templates/
│   │   │   ├── school_day_tasks.yaml
│   │   │   ├── holiday_tasks.yaml
│   │   │   ├── weather_tasks.yaml
│   │   │   └── reward_templates.yaml
│   │   ├── sensors/
│   │   │   ├── choretracker_sensors.yaml
│   │   │   └── task_completion_sensors.yaml
│   │   └── dashboards/
│   │       ├── choretracker_admin.yaml
│   │       └── family_dashboard.yaml
│   ├── tests/                        # HA Integration Testing
│   │   ├── conftest.py               # Test configuration
│   │   ├── test_config_flow.py       # Config flow tests
│   │   ├── test_coordinator.py       # Coordinator tests
│   │   ├── test_sensor.py            # Sensor tests
│   │   ├── test_switch.py            # Switch tests
│   │   ├── test_calendar.py          # Calendar tests
│   │   ├── test_automations.py       # Automation tests
│   │   └── fixtures/                 # Test fixtures
│   │       ├── sample_events.json
│   │       ├── sample_responses.json
│   │       └── mock_devices.json
│   ├── tools/                        # HA Development Tools
│   │   ├── setup_dev_env.py          # Development environment setup
│   │   ├── validate_config.py        # Configuration validator
│   │   ├── generate_entities.py      # Entity generator
│   │   └── test_runner.py            # Test execution utility
│   └── docs/                         # HA Documentation
│       ├── installation.md           # Installation guide
│       ├── configuration.md          # Configuration guide
│       ├── automation_examples.md    # Automation examples
│       ├── template_guide.md         # Template system guide
│       ├── api_integration.md        # API integration guide
│       └── troubleshooting.md        # HA-specific troubleshooting
│
├── shared/                           # Shared Components
│   ├── api/                          # API Definitions
│   │   ├── choretracker_api.yaml     # OpenAPI specification
│   │   ├── schemas/
│   │   │   ├── task_schema.json
│   │   │   ├── device_schema.json
│   │   │   └── config_schema.json
│   │   └── examples/
│   │       ├── api_requests.json
│   │       └── api_responses.json
│   ├── protocols/                    # Communication Protocols
│   │   ├── mqtt_topics.md            # MQTT topic definitions
│   │   ├── webhook_spec.md           # Webhook specifications
│   │   └── calendar_format.md        # Calendar event format
│   └── utils/                        # Shared Utilities
│       ├── calendar_parser.py        # Calendar parsing utility
│       ├── task_validator.py         # Task validation utility
│       └── api_client.py             # Generic API client
│
├── tests/                            # Integration Tests
│   ├── e2e/                          # End-to-end tests
│   │   ├── test_full_workflow.py     # Complete system test
│   │   ├── test_task_lifecycle.py    # Task management test
│   │   └── test_device_integration.py # Device integration test
│   ├── performance/                  # Performance tests
│   │   ├── test_response_times.py
│   │   ├── test_memory_usage.py
│   │   └── test_concurrent_users.py
│   └── fixtures/                     # Shared test fixtures
│       ├── sample_calendar_events.json
│       ├── test_configurations.json
│       └── mock_responses.json
│
├── tools/                            # Project-wide Tools
│   ├── ci/                           # CI/CD Scripts
│   │   ├── build_esp32.py            # ESP32 build script
│   │   ├── test_ha_integration.py    # HA integration test
│   │   ├── deploy.py                 # Deployment script
│   │   └── release.py                # Release automation
│   ├── development/                  # Development utilities
│   │   ├── setup_workspace.py        # Workspace setup
│   │   ├── lint_code.py              # Code linting
│   │   └── generate_docs.py          # Documentation generator
│   └── monitoring/                   # Monitoring tools
│       ├── health_check.py           # System health checker
│       ├── performance_monitor.py    # Performance monitoring
│       └── log_analyzer.py           # Log analysis tool
│
├── docs/                             # Project Documentation
│   ├── getting_started.md            # Quick start guide
│   ├── architecture/
│   │   ├── system_overview.md
│   │   ├── data_flow.md
│   │   └── security.md
│   ├── deployment/
│   │   ├── production_setup.md
│   │   ├── staging_environment.md
│   │   └── monitoring.md
│   ├── api/
│   │   ├── rest_api.md
│   │   ├── mqtt_api.md
│   │   └── webhook_api.md
│   └── examples/
│       ├── sample_configurations/
│       ├── automation_examples/
│       └── calendar_templates/
│
├── scripts/                          # Build and Setup Scripts
│   ├── setup.sh                      # Initial project setup
│   ├── build.sh                      # Build both components
│   ├── test.sh                       # Run all tests
│   ├── deploy.sh                     # Deploy to production
│   └── clean.sh                      # Clean build artifacts
│
└── .github/                          # GitHub Configuration
    ├── workflows/                    # GitHub Actions
    │   ├── esp32_build.yml           # ESP32 build workflow
    │   ├── ha_integration_test.yml   # HA integration test
    │   ├── e2e_test.yml              # End-to-end testing
    │   └── release.yml               # Release workflow
    ├── ISSUE_TEMPLATE/               # Issue templates
    │   ├── bug_report.md
    │   ├── feature_request.md
    │   └── esp32_hardware_issue.md
    └── PULL_REQUEST_TEMPLATE.md      # PR template
```

## Component Descriptions

### ESP32 Firmware Side (`esp32/`)

#### Core Structure
- **`src/`**: Main application code with modular architecture
- **`lib/`**: Custom libraries for reusable components
- **`test/`**: Comprehensive testing including native and embedded tests
- **`tools/`**: Development utilities and simulators

#### Key Features
- **Modular Architecture**: Separated concerns for hardware, network, storage, and UI
- **Theme System**: Customisable display themes for different modes
- **Testing Framework**: Unit tests, integration tests, and hardware mocks
- **Development Tools**: Simulator, config generator, and test harness

### Home Assistant Integration Side (`homeassistant/`)

#### Core Structure
- **`custom_components/choretracker/`**: Full HA custom integration
- **`config/`**: Ready-to-use HA configurations
- **`tests/`**: Comprehensive HA integration testing
- **`tools/`**: HA-specific development utilities

#### Key Features
- **Custom Integration**: Full-featured HA integration with config flow
- **Automation Templates**: Ready-made automations for different scenarios
- **Entity Management**: Sensors, switches, and binary sensors
- **Dashboard Integration**: Pre-built dashboards for family management

### Shared Components (`shared/`)

#### API Definitions
- **OpenAPI Specification**: Complete API documentation
- **Schema Definitions**: JSON schemas for validation
- **Protocol Documentation**: MQTT, webhook, and calendar specifications

### Testing Strategy

#### ESP32 Testing
```bash
# Native unit tests
pio test -e native

# Embedded integration tests  
pio test -e embedded

# Hardware validation tests
python tools/test_harness/hardware_test.py
```

#### Home Assistant Testing
```bash
# Integration tests
python -m pytest homeassistant/tests/

# Configuration validation
python homeassistant/tools/validate_config.py

# Automation testing
python homeassistant/tools/test_runner.py
```

#### End-to-End Testing
```bash
# Full system test
python tests/e2e/test_full_workflow.py

# Performance testing
python tests/performance/test_response_times.py
```

## Development Workflow

### Initial Setup
```bash
# Clone and setup workspace
git clone <repository>
cd ChoreTracker
./scripts/setup.sh
```

### ESP32 Development
```bash
# Build firmware
cd esp32
pio run

# Run tests
pio test

# Upload to device
pio run -t upload
```

### Home Assistant Development
```bash
# Install in development mode
cd homeassistant
python tools/setup_dev_env.py

# Run tests
python -m pytest tests/

# Validate configuration
python tools/validate_config.py
```

### Continuous Integration

#### GitHub Actions Workflows
1. **ESP32 Build**: Compile firmware for different ESP32 variants
2. **HA Integration Test**: Test HA custom integration
3. **E2E Test**: Full system integration testing
4. **Release**: Automated release and deployment

This structure provides:
- **Clear Separation**: ESP32 and HA components are properly separated
- **Comprehensive Testing**: Multiple testing levels for both sides
- **Development Tools**: Utilities for efficient development
- **Documentation**: Complete documentation for all components
- **CI/CD Ready**: Automated testing and deployment workflows
- **Scalable**: Structure supports future expansion and features 