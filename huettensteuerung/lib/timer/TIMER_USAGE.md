# Timer Classes Usage Guide

## TON (Timer On-Delay)

**Purpose:** Delays turning ON a signal. The output becomes TRUE only after the input has been TRUE for the specified duration.

**Use Case:** Running a function every second when a condition is true.

### Example 1: Using setIn() and handle() (Recommended)
```cpp
#include "ton.h"

Ton timer(1000);  // 1 second delay

void setup() {
  Serial.begin(9600);
}

void loop() {
  bool trigger = digitalRead(BUTTON_PIN);
  
  timer.setIn(trigger);  // Set input state
  timer.handle();        // Process timer logic
  
  if (timer.getQ()) {
    // This executes after button held for 1 second
    myFunction();
  }
}

void myFunction() {
  Serial.println("Function executed!");
}
```

### Example 2: Using update() (Legacy style)
```cpp
#include "ton.h"

Ton buttonTimer(2000);  // 2 second delay

void loop() {
  bool buttonPressed = digitalRead(BUTTON_PIN);
  buttonTimer.update(buttonPressed);  // Combines setIn() and handle()
  
  if (buttonTimer.getQ()) {
    // Button was held for 2 seconds
    Serial.println("Long press detected!");
  }
}
```

### Example 3: Periodic function execution
```cpp
#include "ton.h"

Ton periodicTimer(1000);  // 1 second

void loop() {
  periodicTimer.setIn(true);  // Always active
  periodicTimer.handle();
  
  if (periodicTimer.getQ()) {
    myPeriodicFunction();
    periodicTimer.reset();  // Reset to trigger again
  }
}
```

## TOFF (Timer Off-Delay)

**Purpose:** Delays turning OFF a signal. The output remains TRUE for the specified duration after the input goes FALSE.

**Use Case:** Keep a light/motor running for some time after a sensor deactivates.

### Example 1: Staircase light timer (using setIn/handle)
```cpp
#include "toff.h"

Toff lightTimer(30000);  // 30 seconds

void loop() {
  bool motionDetected = digitalRead(MOTION_SENSOR_PIN);
  
  lightTimer.setIn(motionDetected);  // Set input
  lightTimer.handle();               // Process timer
  
  // Light stays on for 30 seconds after motion stops
  digitalWrite(LIGHT_PIN, lightTimer.getQ());
}
```

### Example 2: Fan run-on timer (using update)
```cpp
#include "toff.h"

Toff fanTimer(60000);  // 1 minute

void loop() {
  bool heaterActive = digitalRead(HEATER_PIN);
  fanTimer.update(heaterActive);  // Combines setIn() and handle()
  
  // Fan continues running for 1 minute after heater turns off
  digitalWrite(FAN_PIN, fanTimer.getQ());
}
```

## Running a Function Every Second - Best Approach

For a simple periodic task, use the existing `Timer` class:

```cpp
#include "timer.h"

Timer periodicTimer(1000);  // 1 second

void setup() {
  Serial.begin(9600);
  periodicTimer.start();
}

void loop() {
  if (periodicTimer.hasElapsed()) {
    myPeriodicFunction();
    periodicTimer.start();  // Restart for next cycle
  }
}

void myPeriodicFunction() {
  Serial.println("This runs every second!");
}
```

## Comparison

| Timer Type | Purpose | Output Behavior |
|------------|---------|-----------------|
| **Timer**  | One-shot or manual repeat | Goes true once, must restart manually |
| **TON**    | On-delay | Goes true after input is true for PT time |
| **TOFF**   | Off-delay | Stays true for PT time after input goes false |

## Methods Available

### TON and TOFF classes:
- `setIn(bool inputState)` - Set the input state
- `handle()` - Process timer logic (call every loop)
- `update(bool inputState)` - Legacy method, combines setIn() and handle()
- `getQ()` - Get output state
- `getElapsedTime()` - Get elapsed time in ms
- `isRunning()` - Check if timing
- `reset()` - Reset timer
- `setPresetTime(unsigned long ms)` - Change duration

### Timer class:
- `start()` - Start the timer
- `hasElapsed()` - Check if time has elapsed
- `isRunning()` - Check if timing
- `reset()` - Reset timer
