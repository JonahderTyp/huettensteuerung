#ifndef BUTTON_BASE_H
#define BUTTON_BASE_H

#include <Arduino.h>

// Base Button class that handles button logic with boolean input (active high)
class Button {
 public:
  // Constructor to initialize the hold time threshold
  Button(unsigned long holdTimeThreshold = 1000);

  // Method to update the button state with a boolean input (active high)
  // Should be called in the loop
  void handle(bool pressed);

  // Method to check if a short press has been detected
  // Returns true every time queried until handle() is called again
  bool isShortPress();

  // Method to check if a long press has been detected
  // Returns true every time queried until handle() is called again
  bool isLongPress();

  // Method to check if a long press has been detected (single trigger)
  // Returns true only once when the threshold is first reached
  bool isLongPressSingle();

  // Method to check if a long press has been released
  // Returns true only once when a long press is released
  bool isLongPressRelease();

  // Method to check if the button is currently being pressed
  bool isPressed();

 protected:
  unsigned long holdTimeThreshold;  // Time threshold to differentiate between
                                    // short and long press
  unsigned long buttonPressTime;    // Time when the button was pressed

  bool buttonState;         // Current debounced state of the button
  bool shortPressDetected;  // Flag for short press detection
  bool longPressDetected;   // Flag for long press detection
  bool longPressTriggered;  // Flag to track if long press has been triggered
                            // once
  bool longPressReleaseDetected;  // Flag for long press release detection
};

#endif  // BUTTON_BASE_H
