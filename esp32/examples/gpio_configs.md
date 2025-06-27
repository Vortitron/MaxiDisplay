# ChoreTracker GPIO Configuration Examples

## Single Display Setup (DISPLAY_COUNT=1)
```
DISPLAY_COUNT=1
TFT_CS_PINS=9
SWITCH_PINS=13
LED_PINS=15
SCREEN_BUTTON_PINS=
```

## Dual Display Setup (DISPLAY_COUNT=2)  
```
DISPLAY_COUNT=2
TFT_CS_PINS=9,10
SWITCH_PINS=13,12
LED_PINS=15,2
SCREEN_BUTTON_PINS=17
```

## Quad Display Setup (DISPLAY_COUNT=4)
```
DISPLAY_COUNT=4
TFT_CS_PINS=9,10,1,3
SWITCH_PINS=13,12,14,27
LED_PINS=15,2,0,4
SCREEN_BUTTON_PINS=17,22,7
```

## Full Setup (DISPLAY_COUNT=8)
```
DISPLAY_COUNT=8
TFT_CS_PINS=9,10,1,3,39,36,37,38
SWITCH_PINS=13,12,14,27,26,33,35,34
LED_PINS=15,2,0,4,5,25,32,21
SCREEN_BUTTON_PINS=17,22,7
```

## Build Commands for Different Configurations

```bash
# Build for 1 display
pio run -e esp32 --build-flag="-DDISPLAY_COUNT=1"

# Build for 4 displays  
pio run -e esp32 --build-flag="-DDISPLAY_COUNT=4"

# Build for 8 displays (default)
pio run -e esp32
```
