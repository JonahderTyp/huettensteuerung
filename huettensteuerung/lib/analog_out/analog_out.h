#ifndef ANALOG_OUT_H
#define ANALOG_OUT_H

#include <Arduino.h>

class AnalogOut {
 private:
  uint8_t pin;
  uint16_t value;
  uint16_t default_value;
  uint16_t max_value;
  bool inverted;

 public:
  AnalogOut(uint8_t outputPin, bool invertOutput = false,
            uint16_t defaultValue = 0, uint16_t maxValue = 255);

  void on();
  void off();
  void toggle();
  void set(uint16_t newValue);
  void setPercent(float percent);
  uint16_t getValue() const;
  float getPercent() const;
  void reset();
};

#endif  // ANALOG_OUT_H
