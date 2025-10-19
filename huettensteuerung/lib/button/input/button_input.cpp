#include "button_input.h"

#include <Arduino.h>

// ButtonInput class implementation
ButtonInput::ButtonInput(int pin, unsigned long holdTimeThreshold,
                         bool activeLow, bool enablePullup)
    : Button(holdTimeThreshold),
      buttonPin(pin),
      debounceDelay(50),
      activeLow(activeLow),
      lastDebounceTime(0),
      lastRawState(false) {
  if (enablePullup) {
    pinMode(buttonPin, INPUT_PULLUP);
  } else {
    pinMode(buttonPin, INPUT);
  }
}

// Method to update the button state
void ButtonInput::handle() {
  // Read the button pin and apply active mode logic
  bool rawState = digitalRead(buttonPin);
  bool pressed = activeLow ? !rawState : rawState;

  // Handle debouncing
  if (pressed != lastRawState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Call the base class handle method with the debounced pressed state
    Button::handle(pressed);
  }

  lastRawState = pressed;
}
