/*
 * I2C Slave Port Extender with WS2812B LED Control
 *
 * Features:
 * - 6 input ports (D2-D7) with latching press detection
 * - WS2812B LED strip control (12 LEDs on D10)
 * - I2C slave interface for input polling and LED updates
 *
 * I2C Protocol:
 * - Input Query: Master writes input ID (0-5), reads 1 byte response (0=no
 * press, 1=press detected)
 * - LED Update: Master writes 0x10 followed by 36 bytes (RGB data for 12 LEDs)
 */

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Wire.h>

#include "config.h"

// WS2812B LED strip object
Adafruit_NeoPixel strip(NUM_LEDS, WS2812B_PIN, NEO_GRB + NEO_KHZ800);

// I2C communication state
volatile uint8_t currentRegister =
    0xFF;  // Currently selected register/input ID
volatile uint8_t ledBuffer[NUM_LEDS * 3];  // Buffer for incoming LED data (RGB)
volatile uint8_t ledBufferIndex = 0;       // Current position in LED buffer

// Input state tracking - latches HIGH state until read via I2C
volatile bool inputPressed[NUM_INPUTS] = {false};

// Function declarations
void monitorInputs();
void receiveEvent(int bytes);
void requestEvent();

/**
 * Continuously monitor all input pins and latch HIGH states
 * Once an input goes HIGH, it remains flagged until cleared by I2C read
 */
void monitorInputs() {
  for (unsigned int i = 0; i < NUM_INPUTS; i++) {
    if (digitalRead(INPUT_PINS[i]) == HIGH) {
      inputPressed[i] = true;
    }
  }
}

/**
 * I2C receive event handler
 * Called when master sends data to this slave
 *
 * Protocol:
 * - First byte = 0x10: LED update mode, followed by 36 bytes (12 LEDs * 3 RGB)
 * - First byte = 0-5: Select input ID for subsequent read request
 */
void receiveEvent(int bytes) {
  if (bytes < 1) return;

  uint8_t firstByte = Wire.read();

  if (firstByte == REG_LED_UPDATE) {
    // LED update: read RGB data for all LEDs
    ledBufferIndex = 0;

    while (Wire.available() && ledBufferIndex < NUM_LEDS * 3) {
      ledBuffer[ledBufferIndex++] = Wire.read();
    }

    // Update strip if complete data received
    if (ledBufferIndex == NUM_LEDS * 3) {
      for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i,
                            ledBuffer[i * 3],       // R
                            ledBuffer[i * 3 + 1],   // G
                            ledBuffer[i * 3 + 2]);  // B
      }
      strip.show();
    }
  } else {
    // Input selection: store input ID for next read request
    currentRegister = firstByte;
  }
}

/**
 * I2C request event handler
 * Called when master requests data from this slave
 *
 * Returns:
 * - 0x00: No press detected since last read
 * - 0x01: Press was detected (flag is cleared after reading)
 */
void requestEvent() {
  uint8_t response = 0;

  if (currentRegister < NUM_INPUTS) {
    if (inputPressed[currentRegister]) {
      response = 1;                           // Press detected
      inputPressed[currentRegister] = false;  // Clear flag after reading
    }
  }

  Wire.write(response);
  currentRegister = 0xFF;  // Reset register selection
}

/**
 * Setup function - runs once at startup
 */
void setup() {
  // Configure I2C slave with callbacks
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  // Configure input pins
  for (unsigned int i = 0; i < NUM_INPUTS; i++) {
    pinMode(INPUT_PINS[i], INPUT);
  }

  // Initialize WS2812B LED strip (all LEDs off)
  strip.begin();
  strip.clear();
  strip.show();
}

/**
 * Main loop - continuously monitors input pins
 */
void loop() { monitorInputs(); }
