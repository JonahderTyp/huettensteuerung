# Hütte IoT - Entity Configuration Guide

## Overview
This project supports dynamic configuration of Home Assistant entities at compile time. You can easily configure any number of Buttons, Switches, Lights, and Dimmable Lights.

## Supported Entity Types

### 1. **Button**
- Triggers on press (sets I2C register to 1)
- Auto-resets to 0 after I2C master reads it
- No state feedback in Home Assistant

### 2. **Switch**
- ON/OFF control
- State feedback
- I2C register: 0 = OFF, 1 = ON

### 3. **Light**
- ON/OFF control (like switch but shows as light in HA)
- State feedback
- I2C register: 0 = OFF, 1 = ON

### 4. **Dimmable Light**
- ON/OFF control + brightness (0-255)
- State feedback with brightness level
- I2C register: 0 = OFF, 1-255 = brightness value

## Configuration

### Step 1: Edit `Config.h`

Open `src/Config.h` and modify these lines to set the number of each entity type:

```cpp
// Number of each entity type (configure as needed)
#define NUM_BUTTONS 2          // Number of buttons
#define NUM_SWITCHES 1         // Number of switches
#define NUM_LIGHTS 1           // Number of lights
#define NUM_DIMMABLE_LIGHTS 1  // Number of dimmable lights
```

### Step 2: Compile and Upload

The code will automatically:
- Calculate the total number of I2C registers needed
- Create entities with names like `button1`, `button2`, `switch1`, `light1`, `dimlight1`, etc.
- Assign sequential I2C register indices
- Publish Home Assistant discovery messages

## Examples

### Example 1: 3 Buttons only
```cpp
#define NUM_BUTTONS 3
#define NUM_SWITCHES 0
#define NUM_LIGHTS 0
#define NUM_DIMMABLE_LIGHTS 0
```
Result: 3 I2C registers (0, 1, 2) mapped to button1, button2, button3

### Example 2: Mixed entities
```cpp
#define NUM_BUTTONS 2
#define NUM_SWITCHES 1
#define NUM_LIGHTS 2
#define NUM_DIMMABLE_LIGHTS 1
```
Result: 6 I2C registers:
- Register 0: button1
- Register 1: button2
- Register 2: switch1
- Register 3: light1
- Register 4: light2
- Register 5: dimlight1

### Example 3: Lights only
```cpp
#define NUM_BUTTONS 0
#define NUM_SWITCHES 0
#define NUM_LIGHTS 3
#define NUM_DIMMABLE_LIGHTS 2
```
Result: 5 I2C registers mapped to light1, light2, light3, dimlight1, dimlight2

## I2C Register Mapping

Registers are assigned in this order:
1. Buttons (starting at register 0)
2. Switches (after buttons)
3. Lights (after switches)
4. Dimmable Lights (after lights)

## I2C Communication Protocol

### Reading from ESP8266 (I2C Master reads register)
```cpp
Wire.beginTransmission(0x8F);
Wire.write(registerIndex);  // Select register (0-N)
Wire.endTransmission();
Wire.requestFrom(0x8F, 1);
uint8_t value = Wire.read();  // Read value (auto-resets buttons to 0)
```

### Writing to ESP8266 (I2C Master writes register)
```cpp
Wire.beginTransmission(0x8F);
Wire.write(registerIndex);   // Select register
Wire.write(value);            // Write value
Wire.endTransmission();
```

## Advanced Customization

If you want custom names instead of `button1`, `button2`, etc., you can modify the `setupEntities()` function in `main.cpp`:

```cpp
void setupEntities() {
  uint8_t registerIndex = 0;

  // Custom names
  entityManager.addButton("doorbell", registerIndex++);
  entityManager.addButton("alarm_trigger", registerIndex++);
  entityManager.addSwitch("main_power", registerIndex++);
  entityManager.addLight("bedroom", registerIndex++);
  entityManager.addDimmableLight("living_room", registerIndex++);
}
```

## Home Assistant Integration

After the ESP8266 boots and connects to MQTT:
1. All entities automatically appear in Home Assistant (via MQTT discovery)
2. Entities are grouped under the device "huette_iot"
3. No manual configuration needed in Home Assistant

## Troubleshooting

### Check Serial Output
Monitor the serial output to see:
- Which entities were created
- Register assignments
- MQTT connection status
- Discovery message publishing

### Verify Register Count
Make sure `NUM_REGISTERS` (automatically calculated) doesn't exceed your I2C buffer size limits.

## File Structure

```
src/
├── Config.h              # CONFIGURE HERE - Entity counts
├── HAEntity.h/cpp        # Entity class implementation
├── EntityManager.h/cpp   # Manages all entities
├── MQTTHandler.h/cpp     # MQTT & HA discovery
├── I2CSlave.h/cpp        # I2C slave implementation
├── DHTSensor.h/cpp       # DHT11 sensor
├── WiFiManager.h/cpp     # WiFi connection
└── main.cpp              # Main application
```
