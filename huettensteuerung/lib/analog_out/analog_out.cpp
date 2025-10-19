#include "analog_out.h"

// Constructor
AnalogOut::AnalogOut(uint8_t outputPin, bool invertOutput,
                     uint16_t defaultValue, uint16_t maxValue)
    : pin(outputPin),
      value(defaultValue),
      default_value(defaultValue),
      max_value(maxValue),
      inverted(invertOutput) {
  pinMode(pin, OUTPUT);
  // Set initial value based on default_value and inverted flag
  uint16_t physicalValue =
      inverted ? (max_value - default_value) : default_value;
  analogWrite(pin, physicalValue);
}

// Set the output to maximum value
void AnalogOut::on() {
  value = max_value;
  uint16_t physicalValue = inverted ? 0 : max_value;
  analogWrite(pin, physicalValue);
}

// Set the output to zero
void AnalogOut::off() {
  value = 0;
  uint16_t physicalValue = inverted ? max_value : 0;
  analogWrite(pin, physicalValue);
}

// Toggle between current value and zero
void AnalogOut::toggle() {
  if (value > 0) {
    off();
  } else {
    on();
  }
}

// Set the output to a specific value (0 to max_value)
void AnalogOut::set(uint16_t newValue) {
  // Constrain the value to the valid range
  value = (newValue > max_value) ? max_value : newValue;
  uint16_t physicalValue = inverted ? (max_value - value) : value;
  analogWrite(pin, physicalValue);
}

// Set the output using a percentage (0.0 to 100.0)
void AnalogOut::setPercent(float percent) {
  // Constrain percentage to 0.0 - 100.0
  if (percent < 0.0) percent = 0.0;
  if (percent > 100.0) percent = 100.0;

  uint16_t newValue = (uint16_t)((percent / 100.0) * max_value);
  set(newValue);
}

// Get the current value
uint16_t AnalogOut::getValue() const { return value; }

// Get the current value as percentage (0.0 to 100.0)
float AnalogOut::getPercent() const { return (float)value / max_value * 100.0; }

// Reset to default value
void AnalogOut::reset() { set(default_value); }
