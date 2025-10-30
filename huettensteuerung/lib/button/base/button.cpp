#include "button.h"

#include <Arduino.h>

// Base Button class implementation
Button::Button(unsigned long holdTimeThreshold)
    : holdTimeThreshold(holdTimeThreshold),
      buttonPressTime(0),
      buttonState(false),
      shortPressDetected(false),
      longPressDetected(false),
      longPressTriggered(false),
      longPressReleaseDetected(false) {}

// Method to update the button state with a boolean input (active high)
void Button::handle(bool pressed) {
  // Reset detection flags at the start of each loop
  shortPressDetected = false;
  longPressDetected = false;
  longPressReleaseDetected = false;

  bool previousState = buttonState;
  buttonState = pressed;

  // Button just pressed
  if (buttonState && !previousState) {
    buttonPressTime = millis();
    longPressTriggered = false;  // Reset the single trigger flag
  }
  // Button just released
  else if (!buttonState && previousState) {
    unsigned long pressDuration = millis() - buttonPressTime;
    if (pressDuration < holdTimeThreshold) {
      shortPressDetected = true;
    } else {
      longPressReleaseDetected = true;
    }
    longPressTriggered = false;  // Reset when button is released
  }
  // Button held down
  else if (buttonState && previousState) {
    unsigned long pressDuration = millis() - buttonPressTime;
    if (pressDuration >= holdTimeThreshold) {
      longPressDetected = true;
    }
  }
}

// Method to check if a short press has been detected
// Returns the same value when queried multiple times in one loop
bool Button::isShortPress() { return shortPressDetected; }

// Method to check if a long press has been detected
// Returns the same value when queried multiple times in one loop
bool Button::isLongPress() { return longPressDetected; }

// Method to check if a long press has been detected (single trigger)
// Returns true only once when the threshold is first reached
bool Button::isLongPressSingle() {
  if (longPressDetected && !longPressTriggered) {
    longPressTriggered = true;
    return true;
  }
  return false;
}

// Method to check if a long press has been released
// Returns true only once when a long press is released
bool Button::isLongPressRelease() { return longPressReleaseDetected; }

// Method to check if the button is currently being pressed
bool Button::isPressed() { return buttonState; }
