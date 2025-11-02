#ifndef CONFIG_H
#define CONFIG_H

#include "secrets.h"

// Device Configuration
#define DEVICE_NAME "huette_iot"
#define DEVICE_MODEL "Hütte IoT"
#define DEVICE_MANUFACTURER "Custom"

// DHT11 Configuration
#define DHTPIN D1
#define DHTTYPE DHT11

// I2C Configuration
#define I2C_SLAVE_ADDRESS 0x8F
#define I2C_SDA_PIN D2
#define I2C_SCL_PIN D3
#define NUM_REGISTERS 3

// MQTT Configuration
#define MQTT_PORT 8883
#define SENSOR_UPDATE_INTERVAL 5000  // milliseconds

#endif
