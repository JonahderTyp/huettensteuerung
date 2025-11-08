#ifndef SWITCH_BASE_H
#define SWITCH_BASE_H

#include <Arduino.h>

// Base Switch class that handles switch logic with boolean input (active high)
class Switch {
 public:
  // Constructor
  Switch();

  // Method to update the switch state with a boolean input (active high)
  // Should be called in the loop
  void handle(bool active);

  // Method to get the current state of the switch
  bool state();

  // Method to check if the switch has been turned on (single trigger)
  // Returns true only once when the switch transitions from off to on
  bool on_single();

  // Method to check if the switch has been turned off (single trigger)
  // Returns true only once when the switch transitions from on to off
  bool off_single();

  // Method to check if the switch has been toggled
  // Returns true when either on_single or off_single would return true
  bool toggled();

 protected:
  bool switchState;    // Current debounced state of the switch
  bool previousState;  // Previous state to detect transitions
  bool onDetected;     // Flag for on transition detection
  bool offDetected;    // Flag for off transition detection
};

#endif  // SWITCH_BASE_H
