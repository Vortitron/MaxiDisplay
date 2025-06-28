# 🎮 ChoreTracker Demo Mode

## Overview

The ChoreTracker demo mode provides a fully functional demonstration environment with colourful, fixed tasks and visual elements. This mode is perfect for:

- **Demonstrations** at maker fairs, exhibitions, or presentations
- **Testing** the hardware without requiring Home Assistant setup
- **Development** and debugging of display functionality
- **Training** users on the interface

## Features

### 🌈 Colourful Visual Design
- **Vibrant colour palette** with 11+ colours (RGB565 format)
- **Visual patterns** including stars, hearts, circles, flowers, and more
- **Animated backgrounds** with sparkle effects and colour cycling
- **Progress bars** showing task completion status
- **Theme colours** for each person (Emma: Blue, Jack: Green)

### 📋 Fixed Demo Content
- **Two demo children**: Emma and Jack with 6 tasks each
- **Sample weather data**: Partly cloudy, 22°C with decorative icons
- **Family events**: Swimming lesson, pizza night, movie time, bedtime story
- **Pre-completed tasks** for realistic demonstration

### 🎯 Interactive Elements
- **Task completion simulation** via button presses
- **LED feedback** showing task status (ON = incomplete, OFF = complete)
- **Navigation between columns** using row select buttons
- **Real-time progress tracking** with completion percentages

## Activation

### Method 1: Button Combination (Recommended)
1. **Hold both row select buttons simultaneously**:
   - Row 1 Select Button (GPIO 17)
   - Row 2 Select Button (GPIO 22)
2. **Hold for 5 seconds** - progress will be shown on all displays
3. **Demo mode activates** with rainbow animation
4. **All displays show colourful demo content**

### Method 2: Power-On Sequence
The demo mode can be configured to activate automatically on startup by modifying the configuration.

## Demo Content Structure

### Emma's Tasks (Column 0)
- 🟣 **Make Bed** - Purple heart pattern
- 🔵 **Brush Teeth** - Cyan circle pattern  
- 🟠 **Feed Fluffy** (cat) - Orange flower pattern
- 🟢 **Tidy Room** - Green square pattern
- 🩷 **Piano Practice** - Pink music note pattern
- 🟡 **Read Book** - Gold book pattern

### Jack's Tasks (Column 1)
- 🔴 **Make Bed** - Red diamond pattern
- 🔵 **Brush Teeth** - Blue cross pattern
- 🟡 **Feed Goldfish** - Yellow circle pattern
- 🟣 **Homework** - Purple book pattern
- 🟢 **Football Practice** - Lime sport pattern
- 🟠 **Help Cook** - Orange heart pattern

### Weather Display (Column 2)
- **Current conditions**: Partly Cloudy
- **Temperature**: 22°C display
- **Description**: "Perfect day for outdoor activities!"
- **Visual elements**: Sun and cloud patterns
- **Background**: Cyan theme colour

### Family Events (Column 3)
- 🔵 **Swimming Lesson** (3:00 PM) - Active
- 🔴 **Pizza Night** (6:00 PM) - Active
- 🟣 **Movie Time** (7:30 PM) - Upcoming
- 🟡 **Bedtime Story** (8:30 PM) - Upcoming

## Visual Patterns Available

| Pattern | Description | Use Case |
|---------|-------------|----------|
| ⭐ Star | 5-pointed star | Achievements, highlights |
| ❤️ Heart | Love/caring tasks | Family activities, pets |
| ⭕ Circle | Simple completion | Basic tasks |
| ⬜ Square | Organization | Tidying, organizing |
| 🔺 Triangle | Priority/direction | Important tasks |
| ➕ Cross | Addition/medical | Health, hygiene |
| 💎 Diamond | Special/valuable | Rewards, special tasks |
| 🌸 Flower | Growth/nature | Plant care, outdoor |
| ☀️ Sun | Weather/energy | Weather, active tasks |
| ☁️ Cloud | Weather/calm | Weather conditions |
| 🎵 Music | Audio/practice | Musical instruments |
| 📖 Book | Learning/reading | Homework, reading |
| 🎮 Game | Recreation | Play time, games |
| ⚽ Sport | Physical activity | Exercise, sports |

## Colour Palette

| Colour | Hex RGB565 | Usage |
|--------|------------|-------|
| Red | `0xF800` | High priority, alerts |
| Green | `0x07E0` | Completion, success |
| Blue | `0x001F` | Emma's theme, water |
| Yellow | `0xFFE0` | Highlights, sun |
| Magenta | `0xF81F` | Special events |
| Cyan | `0x07FF` | Weather, cool tasks |
| Orange | `0xFD20` | Energy, food |
| Purple | `0x8010` | Creativity, family |
| Pink | `0xFC18` | Caring, nurturing |
| Lime | `0x87E0` | Jack's theme, nature |
| Gold | `0xFEA0` | Achievements, rewards |

## Demo Mode Operations

### Navigation
- **Row select buttons** cycle through columns (Emma → Jack → Weather → Family)
- **Column titles** display for 2 seconds before showing content
- **Each row operates independently** with separate navigation

### Task Simulation
- **Task buttons 0-7** can toggle demo task completion
- **LEDs show status**: ON = incomplete, OFF = complete
- **Visual feedback** on displays with colour changes
- **Progress bars** update automatically

### Animations
- **Sparkle effects** appear randomly during demo
- **Colour cycling** changes theme colours every 3 seconds
- **Progress animations** during activation
- **Rainbow cascade** during mode transitions

## Exiting Demo Mode

### Method 1: Any Task Button
- **Press any task button** (0-7) to exit immediately
- **Exit animation** shows on all displays
- **Returns to normal operation** mode

### Method 2: Power Cycle
- **Power off/on** the ESP32 to reset to normal mode

## Technical Implementation

### File Structure
```
esp32/src/
├── main.cpp                    # Main application with demo detection
├── core/
│   ├── DemoManager.h/.cpp     # Demo content and logic
│   └── NavigationManager.h/.cpp # Enhanced with demo support  
├── ui/
│   └── DisplayManager.h/.cpp  # TFT display rendering
└── hardware/
    └── GPIOManager.h/.cpp     # Button and LED control
```

### Memory Usage
- **Demo data structures**: ~2KB for all demo content
- **Display buffers**: Managed by Adafruit GFX library
- **Pattern storage**: Minimal, calculated geometrically

### Performance
- **20 FPS display updates** for smooth animations
- **50ms button debouncing** for reliable input
- **100ms animation cycles** for sparkle effects
- **3 second colour cycling** for visual variety

## Customization

### Adding New Tasks
Edit `DemoManager.cpp` to add new demo tasks:
```cpp
// In setupDemoPersons()
demoPersons[0].tasks[6] = {
    "New Task", 
    "Task description", 
    DemoColours::PURPLE, 
    DemoPatterns::STAR, 
    false, 0, 1
};
```

### Custom Patterns
Add new patterns in `DisplayManager.cpp`:
```cpp
case DemoPatterns::CUSTOM: 
    drawCustomPattern(displayIndex, x, y, size, colour); 
    break;
```

### Colour Themes
Modify the `DEMO_COLOURS` array in `DemoManager.cpp`:
```cpp
const uint16_t DemoManager::DEMO_COLOURS[] = {
    0xF800,  // Your custom colours here
    // ...
};
```

## Troubleshooting

### Demo Mode Won't Activate
- Check both row select buttons are working (GPIO 17 and 22)
- Ensure buttons are held simultaneously for full 5 seconds
- Verify button wiring and pullup resistors

### Displays Show Wrong Content
- Check TFT CS pin connections (GPIOs 9,10,1,3,39,36,37,38)
- Verify SPI wiring (SCK=18, MOSI=23, DC=19)
- Ensure backlight is connected (GPIO 8)

### Colours Look Wrong
- Check display orientation setting in `initializeDisplay()`
- Verify RGB565 colour format compatibility
- Test with basic colours first

### LEDs Not Working
- Check LED pin assignments match `LED_PINS` array
- Verify current limiting resistors (220Ω recommended)
- Test LED polarity and connections

## Development Notes

### Adding New Demo Modes
The system is designed to be extensible:
- Create new `DemoMode` enum values
- Add mode-specific content in `DemoManager`
- Implement rendering in `DisplayManager`

### Performance Optimization
- Pattern drawing uses geometric calculations (not bitmap storage)
- Display updates are batched and timed
- CS pin management prevents SPI conflicts

### Future Enhancements
- **Sound effects** using the audio pin (GPIO 11)
- **Touch interaction** if touch displays are used
- **Network demo** showing fake HA integration
- **Multiple demo scenarios** (school day, weekend, holiday)

## Conclusion

The demo mode provides a comprehensive, visually appealing demonstration of the ChoreTracker system without requiring external dependencies. It showcases the hardware capabilities, user interface design, and task management concepts in an engaging, interactive format perfect for demonstrations and testing.

🎮 **Ready to demo? Hold both row select buttons for 5 seconds!** 