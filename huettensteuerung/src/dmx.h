#pragma once
#include <Arduino.h>
#include <DMXSerial.h>
#include <analog_out.h>
#include <button.h>
#include <digital_out.h>
#include <timer.h>

namespace Huette {
extern DigitalOut K1_KL1_A;
extern DigitalOut K2_KL1_B;
extern AnalogOut L1;
extern AnalogOut L2;
extern ButtonInput DI_DMX_SW;
}  // namespace Huette

namespace DMX {

// DMX512 Configuration
#define DMX_MAX_CHANNELS 512  // DMX512 standard
#define DMX_START_ADDRESS 1   // Default DMX start address (1-512)
#define LAMP_OFFSET 16
bool DMX_Active = false;

// Initialize DMX receiver
void init() {
  // Initialize DMXSerial in receive mode
  // The library automatically handles Serial1 on ATmega1280
  DMXSerial.init(DMXReceiver);
}

// Get DMX channel value (1-based addressing like DMX standard)
uint8_t getChannel(uint16_t channel) {
  if (channel > 0 && channel <= DMX_MAX_CHANNELS) {
    return DMXSerial.read(channel);
  }
  return 0;
}

// Check if DMX data is being received
bool isConnected() {
  // Returns true if valid DMX data has been received recently
  return (DMXSerial.noDataSince() < 1000);  // 1 second timeout
}

// Main loop function
void loop() {
  // Handle button press to toggle DMX active mode
  if (Huette::DI_DMX_SW.isShortPress()) {
    DMX_Active = !DMX_Active;

    if (DMX_Active) {
      init();  // Initialize DMX when activated
    }
  }

  if (DMX_Active && isConnected()) {
    // The DMXSerial library handles receiving in the background
    // Just read the channel values as needed

    Huette::L1.set(getChannel(LAMP_OFFSET * 4 + 0));
    Huette::L2.set(getChannel(LAMP_OFFSET * 4 + 1));

    Huette::K1_KL1_A.set(getChannel(LAMP_OFFSET * 5 + 0) > 127);
    Huette::K2_KL1_B.set(getChannel(LAMP_OFFSET * 5 + 1) > 127);
  }
}

}  // namespace DMX
