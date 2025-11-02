#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "Config.h"
#include "DHTSensor.h"
#include "I2CSlave.h"
#include "MQTTHandler.h"
#include "WiFiManager.h"

// Global objects
WiFiClientSecure espClient;
WiFiManager wifiManager(ssid, password);
MQTTHandler mqttHandler(espClient, mqtt_server, MQTT_PORT, mqtt_user,
                        mqtt_password, DEVICE_NAME);
DHTSensor dhtSensor(DHTPIN, DHTTYPE);
I2CSlave i2cSlave(I2C_SLAVE_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN, NUM_REGISTERS);

// Timing
unsigned long lastSensorUpdate = 0;

// MQTT callback function
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Handle button commands and set I2C registers
  if (strcmp(topic, "homeassistant/button/huette_iot/button1/set") == 0) {
    i2cSlave.setRegister(0, 1);
    Serial.println("Button 1 pressed - Register 0 set to 1");
  } else if (strcmp(topic, "homeassistant/button/huette_iot/button2/set") ==
             0) {
    i2cSlave.setRegister(1, 1);
    Serial.println("Button 2 pressed - Register 1 set to 1");
  } else if (strcmp(topic, "homeassistant/button/huette_iot/button3/set") ==
             0) {
    i2cSlave.setRegister(2, 1);
    Serial.println("Button 3 pressed - Register 2 set to 1");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nHütte IoT - ESP8266 Smart Home Controller");
  Serial.println("==========================================");

  // Initialize I2C slave
  i2cSlave.begin();

  // Initialize DHT sensor
  dhtSensor.begin();

  // Configure secure WiFi client
  espClient.setInsecure();

  // Connect to WiFi
  wifiManager.connect();

  // Setup MQTT
  mqttHandler.begin();
  mqttHandler.setCallback(onMqttMessage);

  // Connect to MQTT and publish discovery
  while (!mqttHandler.connect()) {
    Serial.println("Retrying MQTT connection in 5 seconds...");
    delay(5000);
  }

  // Publish Home Assistant discovery messages
  mqttHandler.publishHomeAssistantDiscovery();

  // Subscribe to button topics
  mqttHandler.subscribe("homeassistant/button/huette_iot/button1/set");
  mqttHandler.subscribe("homeassistant/button/huette_iot/button2/set");
  mqttHandler.subscribe("homeassistant/button/huette_iot/button3/set");

  Serial.println("Setup complete!");
}

void loop() {
  // Maintain MQTT connection
  if (!mqttHandler.isConnected()) {
    Serial.println("MQTT disconnected, reconnecting...");
    if (mqttHandler.connect()) {
      mqttHandler.publishHomeAssistantDiscovery();
      mqttHandler.subscribe("homeassistant/button/huette_iot/button1/set");
      mqttHandler.subscribe("homeassistant/button/huette_iot/button2/set");
      mqttHandler.subscribe("homeassistant/button/huette_iot/button3/set");
    }
  }
  mqttHandler.loop();

  // Read and publish sensor data periodically
  unsigned long now = millis();
  if (now - lastSensorUpdate > SENSOR_UPDATE_INTERVAL) {
    lastSensorUpdate = now;

    if (dhtSensor.readSensor()) {
      mqttHandler.publishSensorData(
          "homeassistant/sensor/esp8266_sensor/temperature/state",
          dhtSensor.getTemperature(), "temperature");
      mqttHandler.publishSensorData(
          "homeassistant/sensor/esp8266_sensor/humidity/state",
          dhtSensor.getHumidity(), "humidity");
      Serial.println("Sensor data published to MQTT");
    }
  }
}