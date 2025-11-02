#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "Config.h"
#include "DHTSensor.h"
#include "EntityManager.h"
#include "HAEntity.h"
#include "I2CSlave.h"
#include "MQTTHandler.h"
#include "WiFiManager.h"

// Global objects
WiFiClientSecure espClient;
WiFiManager wifiManager(ESPIOT_SSID, ESPIOT_PASSWORD);
MQTTHandler mqttHandler(espClient, ESPIOT_MQTT_SERVER, MQTT_PORT,
                        ESPIOT_MQTT_USER, ESPIOT_MQTT_PASSWORD,
                        ESPIOT_DEVICE_NAME);
DHTSensor dhtSensor(DHTPIN, DHTTYPE);
I2CSlave i2cSlave(I2C_SLAVE_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN, NUM_REGISTERS);
EntityManager entityManager(i2cSlave);

// Timing
unsigned long lastSensorUpdate = 0;

// ===================================
// ENTITY SETUP - Configure your entities here
// ===================================
void setupEntities() {
  Serial.println("\n--- Configuring Entities ---");

  uint8_t registerIndex = 0;

  for (int i = 0; i < NUM_BUTTONS; i++) {
    String name = "button" + String(i + 1);
    entityManager.addButton(name.c_str(), registerIndex++);
  }

  for (int i = 0; i < NUM_SWITCHES; i++) {
    String name = "switch" + String(i + 1);
    entityManager.addSwitch(name.c_str(), registerIndex++);
  }
  for (int i = 0; i < NUM_LIGHTS; i++) {
    String name = "light" + String(i + 1);
    entityManager.addLight(name.c_str(), registerIndex++);
  }
  for (int i = 0; i < NUM_DIMMABLE_LIGHTS; i++) {
    String name = "dimlight" + String(i + 1);
    entityManager.addDimmableLight(name.c_str(), registerIndex++);
  }

  Serial.print("Total entities configured: ");
  Serial.println(entityManager.getEntityCount());
  Serial.print("Total I2C registers used: ");
  Serial.println(NUM_REGISTERS);
  Serial.println("----------------------------\n");
}

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

  // Find entity by topic and handle command
  HAEntity* entity = entityManager.findEntityByTopic(topic);
  if (entity) {
    entityManager.handleCommand(entity, message);

    // Publish state update for non-button entities
    if (entity->getType() != EntityType::BUTTON) {
      String payload = entity->getStatePayload();
      if (payload.length() > 0) {
        mqttHandler.publish(entity->getStateTopic().c_str(), payload.c_str());
      }
    }
  } else {
    Serial.println("Unknown topic, no entity found");
  }
}

void subscribeToAllEntities() {
  for (size_t i = 0; i < entityManager.getEntityCount(); i++) {
    HAEntity* entity = entityManager.getEntity(i);
    if (entity) {
      mqttHandler.subscribe(entity->getCommandTopic().c_str());

      // Subscribe to brightness topic for dimmable lights
      if (entity->getType() == EntityType::DIMMABLE_LIGHT) {
        mqttHandler.subscribe(entity->getBrightnessCommandTopic().c_str());
      }
    }
  }
}

void publishAllEntityDiscovery() {
  // Publish sensor discovery
  mqttHandler.publishHomeAssistantDiscovery();

  // Publish entity discovery
  for (size_t i = 0; i < entityManager.getEntityCount(); i++) {
    HAEntity* entity = entityManager.getEntity(i);
    if (entity) {
      mqttHandler.publishEntityDiscovery(entity);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n========================================");
  Serial.println("Hütte IoT - ESP8266 Smart Home Controller");
  Serial.println("========================================");

  // Setup entities first
  setupEntities();

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
  publishAllEntityDiscovery();

  // Subscribe to all entity topics
  subscribeToAllEntities();

  Serial.println("\n========================================");
  Serial.println("Setup complete!");
  Serial.println("========================================\n");
}

void loop() {
  // Maintain MQTT connection
  if (!mqttHandler.isConnected()) {
    Serial.println("MQTT disconnected, reconnecting...");
    if (mqttHandler.connect()) {
      publishAllEntityDiscovery();
      subscribeToAllEntities();
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