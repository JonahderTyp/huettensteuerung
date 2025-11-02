#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : _ssid(ssid), _password(password) {}

void WiFiManager::connect() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

bool WiFiManager::isConnected() { return WiFi.status() == WL_CONNECTED; }
