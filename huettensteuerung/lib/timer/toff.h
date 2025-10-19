#include <Arduino.h>
#ifndef TOFF_H
#define TOFF_H

// TOFF (Timer Off-Delay) - IEC 61131-3 Timer
// Output Q remains TRUE for the duration PT after input IN goes FALSE
class Toff {
 private:
  unsigned long startTime;
  unsigned long presetTime;  // PT - Preset Time
  bool lastInput;            // Previous input state for edge detection
  bool output;               // Q - Output signal
  bool timerRunning;

 public:
  // Constructor to initialize the timer with preset time (in milliseconds)
  Toff(unsigned long presetTimeMs) {
    presetTime = presetTimeMs;
    startTime = 0;
    lastInput = false;
    output = false;
    timerRunning = false;
  }

  // Set the input state and process timer logic immediately
  void setIn(bool inputState) {
    // Rising edge detection - input goes from false to true
    if (inputState && !lastInput) {
      // Stop any running timer and set output to true immediately
      timerRunning = false;
      output = true;
    }
    // Falling edge detection - input goes from true to false
    else if (!inputState && lastInput) {
      // Start the off-delay timer
      startTime = millis();
      timerRunning = true;
      output = true;  // Output stays true during delay
    }
    // Input remains false - check if time has elapsed
    else if (!inputState && timerRunning) {
      if (millis() - startTime >= presetTime) {
        output = false;
        timerRunning = false;  // Timer done
      }
    }
    // Input remains true - output stays true
    else if (inputState) {
      output = true;
    }

    lastInput = inputState;
  }

  // Get the output state
  bool getQ() { return output; }

  // Get the elapsed time (ET)
  unsigned long getElapsedTime() {
    if (timerRunning) {
      return millis() - startTime;
    } else if (!output && !lastInput) {
      return presetTime;
    }
    return 0;
  }

  // Check if timer is currently running
  bool isRunning() { return timerRunning; }

  // Reset the timer
  void reset() {
    lastInput = false;
    output = false;
    timerRunning = false;
    startTime = 0;
  }

  // Change preset time
  void setPresetTime(unsigned long newPresetTimeMs) {
    presetTime = newPresetTimeMs;
  }
};

#endif  // TOFF_H
