#include "digital_out.h"

// Constructor
DigitalOut::DigitalOut(uint8_t outputPin, bool activeLow, bool defaultState)
    : pin(outputPin),
      state(defaultState),
      default_state(defaultState),
      active_low(activeLow) {
  pinMode(pin, OUTPUT);
  // Set initial state based on default_state and active_low
  uint8_t physicalLevel = (default_state != active_low) ? HIGH : LOW;
  digitalWrite(pin, physicalLevel);
}

// Set the output high
void DigitalOut::on() {
  state = true;
  digitalWrite(pin, active_low ? LOW : HIGH);
}

// Set the output low
void DigitalOut::off() {
  state = false;
  digitalWrite(pin, active_low ? HIGH : LOW);
}

// Toggle the output state
void DigitalOut::toggle() {
  state = !state;
  uint8_t physicalLevel = (state != active_low) ? HIGH : LOW;
  digitalWrite(pin, physicalLevel);
}

// Set the output to a specific state
void DigitalOut::set(bool value) {
  state = value;
  uint8_t physicalLevel = (state != active_low) ? HIGH : LOW;
  digitalWrite(pin, physicalLevel);
}

// Get the current state
bool DigitalOut::getState() const { return state; }
