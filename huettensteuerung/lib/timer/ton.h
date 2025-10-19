#include <Arduino.h>
#ifndef TON_H
#define TON_H

// TON (Timer On-Delay) - IEC 61131-3 Timer
// Output Q becomes TRUE after the input IN has been TRUE for the duration PT
class Ton {
 private:
  unsigned long startTime;
  unsigned long presetTime;  // PT - Preset Time
  bool lastInput;            // Previous input state for edge detection
  bool output;               // Q - Output signal
  bool timerRunning;

 public:
  // Constructor to initialize the timer with preset time (in milliseconds)
  Ton(unsigned long presetTimeMs) {
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
      // Start timing
      startTime = millis();
      timerRunning = true;
      output = false;
    }
    // Falling edge detection - input goes from true to false
    else if (!inputState && lastInput) {
      // Reset timer
      timerRunning = false;
      output = false;
    }
    // Input remains true - check if time has elapsed
    else if (inputState && timerRunning) {
      if (millis() - startTime >= presetTime) {
        output = true;
        timerRunning = false;  // Timer done
      }
    }

    lastInput = inputState;
  }

  // Get the output state
  bool getQ() { return output; }

  // Get the elapsed time (ET)
  unsigned long getElapsedTime() {
    if (timerRunning) {
      return millis() - startTime;
    } else if (output) {
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

#endif  // TON_H
