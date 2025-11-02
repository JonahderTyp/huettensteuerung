#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

class MQTTHandler {
 private:
  WiFiClientSecure& _wifiClient;
  PubSubClient _client;
  const char* _server;
  uint16_t _port;
  const char* _user;
  const char* _password;
  const char* _deviceName;

  void publishDiscoveryMessage(const char* domain, const char* component,
                               const char* name, const char* configTopic,
                               const char* stateTopic = nullptr,
                               const char* commandTopic = nullptr,
                               const char* deviceClass = nullptr,
                               const char* unit = nullptr,
                               const char* valueTemplate = nullptr);

 public:
  MQTTHandler(WiFiClientSecure& wifiClient, const char* server, uint16_t port,
              const char* user, const char* password, const char* deviceName);

  void begin();
  void setCallback(MQTT_CALLBACK_SIGNATURE);
  bool connect();
  void loop();
  bool isConnected();
  void publishSensorData(const char* topic, float value, const char* name);
  void subscribe(const char* topic);
  bool publish(const char* topic, const char* payload);
  void publishHomeAssistantDiscovery();
};

#endif
