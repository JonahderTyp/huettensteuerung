/*
 * Configuration for I2C Port Extender
 */

#ifndef CONFIG_H
#define CONFIG_H

// ===== I2C Configuration =====
#define I2C_SLAVE_ADDRESS 0x80  // I2C address for this device

// ===== I2C Register Addresses =====
#define REG_LED_UPDATE 0x10  // Register to trigger LED color update

// ===== Input Pin Configuration =====
// Array of input pins to monitor (can be customized to any digital pins)
const uint8_t INPUT_PINS[] = {2, 3, 4, 5, 6, 7};  // D2 to D7
#define NUM_INPUTS (sizeof(INPUT_PINS) / sizeof(INPUT_PINS[0]))

// ===== WS2812B LED Configuration =====
#define WS2812B_PIN 10  // Data pin for WS2812B LED strip
#define NUM_LEDS 12     // Number of LEDs in the strip

#endif  // CONFIG_H
