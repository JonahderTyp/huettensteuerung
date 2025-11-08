#ifndef SWITCH_INPUT_H
#define SWITCH_INPUT_H

#include <Arduino.h>

#include "../base/switch.h"

// SwitchInput class that extends Switch and handles digital input reading
class SwitchInput : public Switch {
 public:
  // Constructor to initialize the switch pin, active mode, and pullup setting
  SwitchInput(int pin, bool activeLow = true, bool enablePullup = true);

  // Method to update the switch state, should be called in the loop
  void handle();

 private:
  int switchPin;                // Pin number where the switch is connected
  unsigned long debounceDelay;  // Debounce delay time
  bool activeLow;               // Active LOW (true) or Active HIGH (false)

  unsigned long
      lastDebounceTime;  // Time when the switch state was last changed
  bool lastRawState;     // Previous raw state of the switch
};

#endif  // SWITCH_INPUT_H
