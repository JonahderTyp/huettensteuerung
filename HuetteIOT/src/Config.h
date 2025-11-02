#ifndef CONFIG_H
#define CONFIG_H

#include "secrets.h"

// Device Configuration
#define ESPIOT_DEVICE_NAME "huette_iot"
#define ESPIOT_DEVICE_MODEL "Hütte IoT"
#define ESPIOT_DEVICE_MANUFACTURER "Custom"

// DHT11 Configuration
#define DHTPIN D1
#define DHTTYPE DHT11

// I2C Configuration
#define I2C_SLAVE_ADDRESS 0x8F
#define I2C_SDA_PIN D2
#define I2C_SCL_PIN D3

// MQTT Configuration
#define MQTT_PORT 8883
#define SENSOR_UPDATE_INTERVAL 5000  // milliseconds

// ===================================
// ENTITY CONFIGURATION
// Define your entities here at compile time
// ===================================

// Number of each entity type (configure as needed)
#define NUM_BUTTONS 16
#define NUM_SWITCHES 16
#define NUM_LIGHTS 16
#define NUM_DIMMABLE_LIGHTS 16

// Calculate total number of I2C registers needed
#define NUM_REGISTERS \
  (NUM_BUTTONS + NUM_SWITCHES + NUM_LIGHTS + NUM_DIMMABLE_LIGHTS)

#if NUM_REGISTERS > 255
#error \
    "Total number of I2C registers exceeds 255. Please reduce the number of entities."
#endif

#endif
