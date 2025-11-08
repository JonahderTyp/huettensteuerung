#include "switch.h"

#include <Arduino.h>

// Base Switch class implementation
Switch::Switch()
    : switchState(false),
      previousState(false),
      onDetected(false),
      offDetected(false) {}

// Method to update the switch state with a boolean input (active high)
void Switch::handle(bool active) {
  // Reset detection flags at the start of each loop
  onDetected = false;
  offDetected = false;

  previousState = switchState;
  switchState = active;

  // Switch turned on (transition from off to on)
  if (switchState && !previousState) {
    onDetected = true;
  }
  // Switch turned off (transition from on to off)
  else if (!switchState && previousState) {
    offDetected = true;
  }
}

// Method to get the current state of the switch
bool Switch::state() { return switchState; }

// Method to check if the switch has been turned on (single trigger)
bool Switch::on_single() { return onDetected; }

// Method to check if the switch has been turned off (single trigger)
bool Switch::off_single() { return offDetected; }

// Method to check if the switch has been toggled
bool Switch::toggled() { return onDetected || offDetected; }
