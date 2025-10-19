#ifndef BUTTON_INPUT_H
#define BUTTON_INPUT_H

#include <Arduino.h>

#include "../base/button.h"

// ButtonInput class that extends Button and handles digital input reading
class ButtonInput : public Button {
 public:
  // Constructor to initialize the button pin, hold time threshold, active mode,
  // and pullup setting
  ButtonInput(int pin, unsigned long holdTimeThreshold = 1000,
              bool activeLow = true, bool enablePullup = true);

  // Method to update the button state, should be called in the loop
  void handle();

 private:
  int buttonPin;                // Pin number where the button is connected
  unsigned long debounceDelay;  // Debounce delay time
  bool activeLow;               // Active LOW (true) or Active HIGH (false)

  unsigned long
      lastDebounceTime;  // Time when the button state was last changed
  bool lastRawState;     // Previous raw state of the button
};

#endif  // BUTTON_INPUT_H
