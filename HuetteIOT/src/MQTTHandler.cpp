#include "MQTTHandler.h"

#include "Config.h"

MQTTHandler::MQTTHandler(WiFiClientSecure& wifiClient, const char* server,
                         uint16_t port, const char* user, const char* password,
                         const char* deviceName)
    : _wifiClient(wifiClient),
      _client(wifiClient),
      _server(server),
      _port(port),
      _user(user),
      _password(password),
      _deviceName(deviceName) {}

void MQTTHandler::begin() {
  _client.setServer(_server, _port);
  Serial.print("MQTT configured for ");
  Serial.print(_server);
  Serial.print(":");
  Serial.println(_port);
}

void MQTTHandler::setCallback(MQTT_CALLBACK_SIGNATURE) {
  _client.setCallback(callback);
}

bool MQTTHandler::connect() {
  if (_client.connected()) {
    return true;
  }

  Serial.print("Attempting MQTT connection...");

  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);

  if (_client.connect(clientId.c_str(), _user, _password)) {
    Serial.println("connected");
    return true;
  } else {
    Serial.print("failed, rc=");
    Serial.println(_client.state());
    return false;
  }
}

void MQTTHandler::loop() { _client.loop(); }

bool MQTTHandler::isConnected() { return _client.connected(); }

void MQTTHandler::subscribe(const char* topic) {
  _client.subscribe(topic);
  Serial.print("Subscribed to: ");
  Serial.println(topic);
}

bool MQTTHandler::publish(const char* topic, const char* payload) {
  return _client.publish(topic, payload);
}

void MQTTHandler::publishSensorData(const char* topic, float value,
                                    const char* name) {
  String payload = "{\"" + String(name) + "\":" + String(value, 1) + "}";
  _client.publish(topic, payload.c_str());
}

void MQTTHandler::publishDiscoveryMessage(
    const char* domain, const char* component, const char* name,
    const char* configTopic, const char* stateTopic, const char* commandTopic,
    const char* deviceClass, const char* unit, const char* valueTemplate,
    const char* brightnessStateTopic, const char* brightnessCommandTopic) {
  String config = "{";
  config += "\"name\":\"" + String(_deviceName) + " " + String(name) + "\",";

  if (deviceClass != nullptr) {
    config += "\"device_class\":\"" + String(deviceClass) + "\",";
  }

  if (stateTopic != nullptr) {
    config += "\"state_topic\":\"" + String(stateTopic) + "\",";
  }

  if (commandTopic != nullptr) {
    config += "\"command_topic\":\"" + String(commandTopic) + "\",";
  }

  if (unit != nullptr) {
    config += "\"unit_of_measurement\":\"" + String(unit) + "\",";
  }

  if (valueTemplate != nullptr) {
    config += "\"value_template\":\"" + String(valueTemplate) + "\",";
  }

  // Brightness support for dimmable lights
  if (brightnessStateTopic != nullptr) {
    config +=
        "\"brightness_state_topic\":\"" + String(brightnessStateTopic) + "\",";
    config += "\"brightness_scale\":255,";
  }

  if (brightnessCommandTopic != nullptr) {
    config += "\"brightness_command_topic\":\"" +
              String(brightnessCommandTopic) + "\",";
  }

  config += "\"unique_id\":\"" + String(_deviceName) + "_" + String(component) +
            "\",";
  config += "\"device\":{";
  config += "\"identifiers\":[\"" + String(_deviceName) + "\"],";
  config += "\"name\":\"" + String(_deviceName) + "\",";
  config += "\"model\":\"" + String(ESPIOT_DEVICE_MODEL) + "\",";
  config += "\"manufacturer\":\"" + String(ESPIOT_DEVICE_MANUFACTURER) + "\"";
  config += "}";
  config += "}";

  _client.publish(configTopic, config.c_str(), true);
}

void MQTTHandler::publishHomeAssistantDiscovery() {
  // Temperature sensor
  publishDiscoveryMessage(
      "sensor", "temp", "Temperature",
      "homeassistant/sensor/esp8266_sensor/temperature/config",
      "homeassistant/sensor/esp8266_sensor/temperature/state", nullptr,
      "temperature", "°C", "{{ value_json.temperature }}");

  // Humidity sensor
  publishDiscoveryMessage("sensor", "humid", "Humidity",
                          "homeassistant/sensor/esp8266_sensor/humidity/config",
                          "homeassistant/sensor/esp8266_sensor/humidity/state",
                          nullptr, "humidity", "%",
                          "{{ value_json.humidity }}");

  // Button 1
  publishDiscoveryMessage("button", "button1", "Button 1",
                          "homeassistant/button/huette_iot/button1/config",
                          nullptr,
                          "homeassistant/button/huette_iot/button1/set");

  // Button 2
  publishDiscoveryMessage("button", "button2", "Button 2",
                          "homeassistant/button/huette_iot/button2/config",
                          nullptr,
                          "homeassistant/button/huette_iot/button2/set");

  // Button 3
  publishDiscoveryMessage("button", "button3", "Button 3",
                          "homeassistant/button/huette_iot/button3/config",
                          nullptr,
                          "homeassistant/button/huette_iot/button3/set");

  Serial.println("Home Assistant discovery messages sent");
}

void MQTTHandler::publishEntityDiscovery(HAEntity* entity) {
  if (!entity) return;

  const char* domain = entity->getEntityTypeName();
  EntityType type = entity->getType();

  if (type == EntityType::BUTTON) {
    publishDiscoveryMessage(domain, entity->getName(), entity->getName(),
                            entity->getConfigTopic().c_str(), nullptr,
                            entity->getCommandTopic().c_str());
  } else if (type == EntityType::SWITCH) {
    publishDiscoveryMessage(domain, entity->getName(), entity->getName(),
                            entity->getConfigTopic().c_str(),
                            entity->getStateTopic().c_str(),
                            entity->getCommandTopic().c_str());
  } else if (type == EntityType::LIGHT) {
    publishDiscoveryMessage(domain, entity->getName(), entity->getName(),
                            entity->getConfigTopic().c_str(),
                            entity->getStateTopic().c_str(),
                            entity->getCommandTopic().c_str());
  } else if (type == EntityType::DIMMABLE_LIGHT) {
    publishDiscoveryMessage(
        domain, entity->getName(), entity->getName(),
        entity->getConfigTopic().c_str(), entity->getStateTopic().c_str(),
        entity->getCommandTopic().c_str(), nullptr, nullptr,
        "{{ value_json.state }}", entity->getBrightnessStateTopic().c_str(),
        entity->getBrightnessCommandTopic().c_str());
  }

  Serial.print("Published discovery for ");
  Serial.print(entity->getEntityTypeName());
  Serial.print(": ");
  Serial.println(entity->getName());
}
