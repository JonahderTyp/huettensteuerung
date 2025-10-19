#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

#include <Arduino.h>

class DigitalOut {
 private:
  uint8_t pin;
  bool state;
  bool default_state;
  bool active_low;

 public:
  // Constructor
  DigitalOut(uint8_t outputPin, bool activeLow = false,
             bool defaultState = false);

  // Set the output high
  void on();

  // Set the output low
  void off();

  // Toggle the output state
  void toggle();

  // Set the output to a specific state
  void set(bool value);

  // Get the current state
  bool getState() const;
};

#endif  // DIGITAL_OUT_H
