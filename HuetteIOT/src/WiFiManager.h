#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ESP8266WiFi.h>

class WiFiManager {
 private:
  const char* _ssid;
  const char* _password;

 public:
  WiFiManager(const char* ssid, const char* password);
  void connect();
  bool isConnected();
};

#endif
