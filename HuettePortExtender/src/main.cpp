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

void error() {
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  for (int i = 1; i < NUM_LEDS; i++) strip.setPixelColor(i, 0);
  strip.show();
}

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
    uint8_t led_num = Wire.read();
    uint8_t alpha = Wire.read();  // Unused alpha byte
    uint8_t red = Wire.read();
    uint8_t green = Wire.read();
    uint8_t blue = Wire.read();

    strip.setPixelColor(led_num, red, green, blue);
    strip.show();

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

  // Serial.print("[I2C TX] Request for register: 0x");
  // Serial.print(currentRegister, HEX);

  if (currentRegister < NUM_INPUTS) {
    if (inputPressed[currentRegister]) {
      response = 1;                           // Press detected
      inputPressed[currentRegister] = false;  // Clear flag after reading
      // Serial.print(" - Input ");
      // Serial.print(currentRegister);
      // Serial.println(" PRESSED (cleared)");
    } else {
      // Serial.print(" - Input ");
      // Serial.print(currentRegister);
      // Serial.println(" not pressed");
    }
  } else {
    Serial.print("Invalid register-");
    Serial.print(NUM_INPUTS);
    Serial.print(" ");
    Serial.println(currentRegister);
  }

  // Serial.print("[I2C TX] Sending response: 0x");
  // Serial.println(response, HEX);
  Wire.write(response);
  currentRegister = 0xFF;  // Reset register selection
}

/**
 * Setup function - runs once at startup
 */
void setup() {
  Serial.begin(115200);
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
  strip.setBrightness(255);
  strip.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    // Create rainbow effect: distribute hue evenly across all LEDs
    uint16_t hue = (i * 65536L / NUM_LEDS);
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue)));
  }
  strip.show();
  delay(500);
}

/**
 * Main loop - continuously monitors input pins
 */
void loop() { monitorInputs(); }
