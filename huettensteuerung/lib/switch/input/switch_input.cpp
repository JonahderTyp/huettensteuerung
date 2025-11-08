#include "switch_input.h"

#include <Arduino.h>

// SwitchInput class implementation
SwitchInput::SwitchInput(int pin, bool activeLow, bool enablePullup)
    : Switch(),
      switchPin(pin),
      debounceDelay(50),
      activeLow(activeLow),
      lastDebounceTime(0),
      lastRawState(false) {
  if (enablePullup) {
    pinMode(switchPin, INPUT_PULLUP);
  } else {
    pinMode(switchPin, INPUT);
  }

  // Read initial state to prevent false triggers on power-up
  delay(10);  // Small delay to let the pin stabilize
  bool initialState = digitalRead(switchPin);
  lastRawState = initialState;

  // Initialize the base class state without triggering detection flags
  switchState = initialState;
  previousState = initialState;
}

// Method to update the switch state
void SwitchInput::handle() {
  // Read the switch pin and apply active mode logic
  bool active = digitalRead(switchPin);

  // Handle debouncing
  if (active != lastRawState) {
    lastDebounceTime = millis();
    lastRawState = active;
  }

  // Determine the debounced state
  bool debouncedActive = lastRawState;
  if ((millis() - lastDebounceTime) > debounceDelay) {
    debouncedActive = active;
    lastRawState = active;
  }

  // Always call the base class handle method (required for single-per-loop
  // detection)
  Switch::handle(debouncedActive ^ activeLow);
}
