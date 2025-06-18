# ChoreTracker Project Outline & Development Plan

## Project Analysis

### Current State
The ChoreTracker project is a sophisticated ESP32-based chore management system with Home Assistant integration. After reviewing the design, several improvements and development phases have been identified.

### Key Design Improvements Needed

#### 1. Code Architecture
- **Current Issue**: Monolithic code structure in single Arduino file
- **Improvement**: Modular architecture with separate modules for different concerns
- **Benefits**: Better maintainability, testability, and scalability

#### 2. Task Management System
- **Current Issue**: Basic task parsing and display
- **Improvement**: Advanced task queue system with scheduling, priorities, and progression
- **Benefits**: Better summer schedule mode, reward system, and task intelligence

#### 3. Integration Robustness
- **Current Issue**: Basic HA API calls without error handling
- **Improvement**: Robust bidirectional integration with fallback mechanisms
- **Benefits**: Reliable operation even with network issues

#### 4. User Experience
- **Current Issue**: Basic display and interaction
- **Improvement**: Enhanced animations, feedback, and customisation
- **Benefits**: More engaging experience for children

## Development Phases

### Phase 1: Foundation (Weeks 1-3)
**Goal**: Establish solid architectural foundation

#### Week 1: Project Setup & Code Restructure
- [ ] **Repository Setup**
  - Create proper Git repository with branching strategy
  - Implement CI/CD pipeline for automated builds
  - Set up development environment documentation
  - Create contributor guidelines and issue templates

- [ ] **Code Modularisation**
  - Extract display management into `DisplayManager` class
  - Create `TaskManager` for task logic and state
  - Implement `HAClient` for Home Assistant integration
  - Create `WeatherClient` for weather API integration
  - Add `ConfigManager` for configuration handling

#### Week 2: Enhanced Task System
- [ ] **Task Queue Implementation**
  - Design task data structures with state management
  - Implement task scheduling for summer mode
  - Add task priority and dependency systems
  - Create task template engine

- [ ] **Reward System**
  - Parse reward information from calendar events
  - Implement reward display and countdown timers
  - Add reward notification system
  - Create reward achievement tracking

#### Week 3: Robust HA Integration
- [ ] **Enhanced API Client**
  - Implement error handling and retry logic
  - Add bidirectional switch synchronisation
  - Create webhook support for real-time updates
  - Implement automatic entity discovery

### Phase 2: Features & UX (Weeks 4-6)
**Goal**: Enhance user experience and add advanced features

#### Week 4: Display & Animation System
- [ ] **Enhanced Visuals**
  - Implement smooth task transitions and animations
  - Add progress indicators and completion effects
  - Create customisable themes and layouts
  - Implement visual task grouping

- [ ] **Summer Schedule Mode**
  - Add task fading effects (greyscale transition)
  - Implement task shifting animations
  - Create automatic task progression
  - Add time-based task management

#### Week 5: Smart Features
- [ ] **Intelligence Layer**
  - Add weather-based task modification
  - Implement day-type detection (school vs holiday)
  - Create event-triggered task additions
  - Add adaptive scheduling based on completion patterns

- [ ] **Multi-Child Support**
  - Create scalable person management
  - Implement dynamic screen allocation
  - Add person-specific configuration
  - Create fair task distribution algorithms

#### Week 6: Audio & Feedback
- [ ] **Audio System** (using spare GPIO 11)
  - Implement task completion sounds
  - Add voice feedback for task announcements
  - Create configurable audio alerts
  - Implement text-to-speech integration

### Phase 3: Reliability & Production (Weeks 7-9)
**Goal**: Ensure production-ready reliability and performance

#### Week 7: Error Handling & Persistence
- [ ] **Robust Error Handling**
  - Implement comprehensive error catching
  - Add automatic recovery mechanisms
  - Create fallback modes for network failures
  - Add watchdog timer integration

- [ ] **Data Persistence**
  - Implement SPIFFS for configuration and task storage
  - Add data backup and recovery
  - Create offline mode with cached tasks
  - Implement data integrity checking

#### Week 8: Testing & Validation
- [ ] **Comprehensive Testing**
  - Create unit tests for all modules
  - Implement hardware integration tests
  - Add system-level workflow tests
  - Create automated testing pipeline

- [ ] **Performance Optimisation**
  - Profile memory usage and optimise
  - Implement efficient display rendering
  - Optimise network communication
  - Add performance monitoring

#### Week 9: Documentation & Tools
- [ ] **Complete Documentation**
  - Create detailed hardware assembly guide
  - Write comprehensive software setup instructions
  - Document all API endpoints
  - Create troubleshooting guide

- [ ] **Development Tools**
  - Create configuration generator tool
  - Implement device simulator for testing
  - Add diagnostic and monitoring tools
  - Create deployment automation

## Technical Implementation Strategy

### Code Architecture

```
src/
├── main.cpp                      # Application entry point
├── config/
│   ├── Config.h                 # Configuration constants
│   ├── GPIOConfig.h             # Hardware pin definitions
│   └── NetworkConfig.h          # Network settings
├── core/
│   ├── TaskManager.cpp/.h       # Task logic and scheduling
│   ├── StateMachine.cpp/.h      # Application state management
│   └── Scheduler.cpp/.h         # Task scheduling engine
├── hardware/
│   ├── DisplayManager.cpp/.h    # TFT display management
│   ├── InputHandler.cpp/.h      # Button/switch handling
│   └── PowerManager.cpp/.h      # Power management
├── network/
│   ├── HAClient.cpp/.h          # Home Assistant integration
│   ├── WeatherClient.cpp/.h     # Weather API client
│   └── OTAManager.cpp/.h        # OTA update handling
├── storage/
│   ├── StorageManager.cpp/.h    # Persistent storage
│   └── ConfigManager.cpp/.h     # Configuration management
└── utils/
    ├── Logger.cpp/.h            # Logging system
    ├── TimeUtils.cpp/.h         # Time utilities
    └── StringUtils.cpp/.h       # String manipulation
```

### Key Design Patterns

#### 1. State Machine Pattern
- Manage device states (sleeping, active, configuration)
- Handle transitions between display modes
- Manage task states (pending, active, completed, expired)

#### 2. Observer Pattern
- Notify display updates when tasks change
- Handle button press events
- Manage network status changes

#### 3. Template Method Pattern
- Task processing pipeline
- Display rendering pipeline
- Network communication patterns

### Data Structures

#### Task Structure
```cpp
enum TaskType {
    UNTIMED,              // No time constraint, button completion only
    FIXED_TIME,           // Auto appear/disappear, no button interaction
    TIMED_WITH_EARLY      // Scheduled time but can complete early with button
};

enum TaskState {
    PENDING,              // Not yet active
    ACTIVE,               // Currently displayed and available
    COMPLETED,            // Marked complete by button press
    EXPIRED,              // Time passed without completion
    AUTO_REMOVED          // Automatically removed (fixed time tasks)
};

struct Task {
    String id;
    String title;
    String description;
    TaskState state;
    TaskType type;
    Priority priority;
    DateTime startTime;       // When task becomes available (null for untimed)
    DateTime endTime;         // When task auto-expires (null for untimed)
    DateTime completedTime;   // When marked complete
    String reward;
    DateTime rewardTime;
    String personId;
    bool allowEarlyCompletion; // Can be completed before endTime
    bool requiresSwitch;      // Whether task needs switch interaction
};
```

#### Three-Tier Task System Examples

**Untimed Tasks:**
```cpp
Task brushTeeth = {
    .title = "Brush Teeth",
    .type = UNTIMED,
    .startTime = null,
    .endTime = null,
    .requiresSwitch = true,
    .allowEarlyCompletion = false
};
```

**Fixed Time Tasks:**
```cpp
Task freePlay = {
    .title = "Free Play",
    .type = FIXED_TIME,
    .startTime = DateTime("08:00"),
    .endTime = DateTime("09:00"),
    .requiresSwitch = false,
    .allowEarlyCompletion = false
};
```

**Timed with Early Completion:**
```cpp
Task doSums = {
    .title = "Do Some Sums",
    .type = TIMED_WITH_EARLY,
    .startTime = DateTime("09:00"),
    .endTime = DateTime("10:00"),
    .requiresSwitch = true,
    .allowEarlyCompletion = true
};
```

#### Person Structure
```cpp
struct Person {
    String id;
    String name;
    int displayOffset;
    vector<Task> tasks;
    PersonConfig config;
    Statistics stats;
};
```

## Hardware Improvements

### Enhanced GPIO Usage
- **GPIO 11**: Implement buzzer/speaker for audio feedback
- **Power Management**: Add deep sleep modes for battery operation
- **Expansion Connectors**: Prepare for additional displays/switches
- **Status LEDs**: Add system status indicators

### PCB Design Considerations
- **Modular Design**: Separate display and control boards
- **Connector Standards**: Use standard connectors for easy assembly
- **EMI Considerations**: Proper grounding and shielding
- **Manufacturing**: Design for automated assembly

## Home Assistant Integration Enhancements

### Enhanced Templates
```yaml
# School day template
automation:
  - alias: "ChoreTracker School Day Tasks"
    trigger:
      - platform: time
        at: "06:00:00"
    condition:
      - condition: state
        entity_id: binary_sensor.workday_sensor
        state: 'on'
    action:
      - service: calendar.create_event
        data:
          summary: "Kid 1: School Day"
          description: "Get Dressed,Pack Bag,Homework,Read,Reward:Screen Time,Time:16:00"

# Weather-based task addition
automation:
  - alias: "ChoreTracker Rainy Day Tasks"
    trigger:
      - platform: state
        entity_id: weather.home
        attribute: condition
        to: "rainy"
    action:
      - service: calendar.create_event
        data:
          summary: "Rainy Day Addition"
          description: "Board Games,Indoor Exercise,Art Project"
```

### Advanced Switch Management
```yaml
# Bidirectional switch synchronisation
automation:
  - alias: "ChoreTracker Switch Sync"
    trigger:
      - platform: mqtt
        topic: "choretracker/switch/+"
    action:
      - service: switch.turn_{{ trigger.payload }}
        data:
          entity_id: "switch.{{ trigger.topic.split('/')[-1] }}"
```

## API Design

### RESTful API Endpoints
```
GET    /api/status              # Device status and health
GET    /api/tasks               # Current task list
POST   /api/tasks/refresh       # Force task refresh
GET    /api/config              # Current configuration
PUT    /api/config              # Update configuration
POST   /api/switch/{id}/toggle  # Toggle switch state
GET    /api/persons             # Person list and stats
POST   /api/restart             # Restart device
POST   /api/ota                 # Trigger OTA update
```

### MQTT Integration
```
# Topics for real-time communication
choretracker/status/online       # Device online status
choretracker/task/completed/{id} # Task completion events
choretracker/switch/{id}/state   # Switch state changes
choretracker/person/{id}/stats   # Person statistics
choretracker/config/update       # Configuration updates
```

## Testing Strategy

### Unit Testing
- Test individual functions and classes
- Mock hardware interactions
- Validate data structures and algorithms
- Test error handling and edge cases

### Integration Testing
- Test hardware component interactions
- Validate display rendering
- Test network communication
- Verify HA integration

### System Testing
- End-to-end user workflows
- Performance under load
- Network failure scenarios
- Long-term reliability testing

### Hardware Testing
- GPIO functionality validation
- Power consumption measurement
- Thermal characteristics
- EMI compliance testing

## Quality Assurance

### Code Quality
- Static analysis with cppcheck
- Code coverage measurement
- Consistent coding standards
- Peer code reviews

### Performance Metrics
- Response time < 500ms for task updates
- Memory usage < 80% of available
- Power consumption < 500mA average
- 99%+ uptime during operation

### User Experience
- Setup time < 30 minutes
- Task completion rate > 80%
- User satisfaction > 4.5/5
- Support requests < 5% of users

## Risk Management

### Technical Risks
| Risk | Impact | Mitigation |
|------|--------|------------|
| Memory constraints | High | Memory profiling and optimisation |
| GPIO conflicts | High | Comprehensive pin validation |
| Network reliability | Medium | Robust retry mechanisms |
| Component availability | Medium | Multiple supplier sources |

### Project Risks
| Risk | Impact | Mitigation |
|------|--------|------------|
| Scope creep | High | Clear phase definitions |
| Timeline delays | Medium | Agile development approach |
| Team availability | Medium | Cross-training and documentation |
| Hardware issues | Low | Thorough testing and validation |

## Success Metrics

### Technical
- **Reliability**: >99% uptime
- **Performance**: <500ms response time
- **Memory**: <80% usage
- **Power**: <500mA consumption
- **Updates**: >95% successful OTA

### Business
- **Adoption**: >1000 active devices
- **Satisfaction**: >4.5/5 user rating
- **Support**: <5% support requests
- **Community**: Active contributor base
- **Revenue**: Cost recovery within 12 months

## Resource Requirements

### Development Team
- **Lead Developer**: Full-time, ESP32/Arduino expert
- **UI/UX Developer**: Part-time, display and interaction design
- **Hardware Engineer**: Consultant, PCB and validation
- **QA Engineer**: Part-time, testing and validation
- **Technical Writer**: Part-time, documentation

### Infrastructure
- **Development**: Arduino IDE, PlatformIO, Git
- **Testing**: Hardware test rigs, automated pipelines
- **Documentation**: GitHub Wiki, video tools
- **Communication**: Project management tools

### Budget Estimate
- **Development**: £50,000 (6 months)
- **Hardware**: £5,000 (prototypes and testing)
- **Infrastructure**: £2,000 (tools and services)
- **Total**: £57,000

## Timeline Summary

| Phase | Duration | Key Deliverables |
|-------|----------|------------------|
| Phase 1 | 3 weeks | Modular architecture, enhanced task system |
| Phase 2 | 3 weeks | Advanced features, UX improvements |
| Phase 3 | 3 weeks | Production readiness, documentation |
| **Total** | **9 weeks** | **Production-ready ChoreTracker** |

## Next Steps

1. **Immediate (Week 1)**
   - Set up development environment
   - Begin code restructuring
   - Create project repository

2. **Short-term (Month 1)**
   - Complete Phase 1 development
   - Begin hardware prototyping
   - Establish testing framework

3. **Medium-term (Month 2-3)**
   - Complete Phases 2 and 3
   - Finalise documentation
   - Prepare for production

4. **Long-term (Month 4+)**
   - Community engagement
   - Feature expansion
   - Commercial considerations

---

This development plan transforms the ChoreTracker from a functional prototype into a robust, production-ready system that will delight families and contribute to the smart home ecosystem. 

### Task Management Logic

1. **HA Calendar Query**: Daily fetch of calendar events
2. **Task Parsing**: Extract tasks, rewards, and times from event descriptions
3. **Context Processing**: Apply templates based on day type, weather, events
4. **Task Scheduling**: Handle three-tier task system:
   - **Untimed Tasks**: Always available when added to queue
   - **Fixed Time Tasks**: Auto-appear at startTime, auto-remove at endTime
   - **Timed with Early Completion**: Appear at startTime, can complete early or auto-expire
5. **Display Update**: Render tasks on appropriate screens based on time and state
6. **Interaction Handling**: Process switch presses based on task type and timing
7. **Summer Mode**: Task fading, shifting, and progression with time-aware scheduling

#### Task Lifecycle Management

```cpp
class TaskScheduler {
private:
    vector<Task> taskQueue;
    vector<Task> activeTasks;
    
public:
    void updateTaskStates() {
        DateTime now = getCurrentTime();
        
        // Check for tasks that should become active
        for (auto& task : taskQueue) {
            if (task.state == PENDING) {
                if (task.type == UNTIMED || 
                   (task.startTime.isValid() && now >= task.startTime)) {
                    activateTask(task);
                }
            }
        }
        
        // Check for tasks that should auto-expire or remove
        for (auto& task : activeTasks) {
            if (task.endTime.isValid() && now >= task.endTime) {
                if (task.type == FIXED_TIME) {
                    task.state = AUTO_REMOVED;
                } else if (task.type == TIMED_WITH_EARLY && task.state != COMPLETED) {
                    task.state = EXPIRED;
                }
                removeFromDisplay(task);
            }
        }
    }
    
    bool handleSwitchPress(int switchId) {
        Task* task = getTaskForSwitch(switchId);
        if (task && task->requiresSwitch && task->state == ACTIVE) {
            if (task->type == UNTIMED || 
               (task->type == TIMED_WITH_EARLY && task->allowEarlyCompletion)) {
                task->state = COMPLETED;
                task->completedTime = getCurrentTime();
                updateHASwitch(task->id, true);
                return true;
            }
        }
        return false;
    }
};
``` 