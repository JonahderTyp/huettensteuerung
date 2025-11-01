#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#include "secrets.h"  // Include WiFi and MQTT credentials

// Device name for Home Assistant
const char* device_name = "huette_iot";

// MQTT Topics for Home Assistant Auto-Discovery
const char* temp_state_topic =
    "homeassistant/sensor/esp8266_sensor/temperature/state";
const char* humid_state_topic =
    "homeassistant/sensor/esp8266_sensor/humidity/state";
const char* temp_config_topic =
    "homeassistant/sensor/esp8266_sensor/temperature/config";
const char* humid_config_topic =
    "homeassistant/sensor/esp8266_sensor/humidity/config";

// DHT11 Settings
#define DHTPIN D1      // DHT11 connected to D1
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// WiFi and MQTT clients - using WiFiClientSecure for SSL/TLS
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Timing variables
unsigned long lastMsg = 0;
const long interval = 5000;  // Send data every 60 seconds (adjust as needed)

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void publishHomeAssistantDiscovery() {
  // Temperature sensor discovery
  String temp_config = "{";
  temp_config += "\"name\":\"" + String(device_name) + " Temperature\",";
  temp_config += "\"device_class\":\"temperature\",";
  temp_config += "\"state_topic\":\"" + String(temp_state_topic) + "\",";
  temp_config += "\"unit_of_measurement\":\"°C\",";
  temp_config += "\"value_template\":\"{{ value_json.temperature }}\",";
  temp_config += "\"unique_id\":\"" + String(device_name) + "_temp\",";
  temp_config += "\"device\":{";
  temp_config += "\"identifiers\":[\"" + String(device_name) + "\"],";
  temp_config += "\"name\":\"" + String(device_name) + "\",";
  temp_config += "\"model\":\"ESP8266 DHT11\",";
  temp_config += "\"manufacturer\":\"Custom\"";
  temp_config += "}";
  temp_config += "}";

  // Humidity sensor discovery
  String humid_config = "{";
  humid_config += "\"name\":\"" + String(device_name) + " Humidity\",";
  humid_config += "\"device_class\":\"humidity\",";
  humid_config += "\"state_topic\":\"" + String(humid_state_topic) + "\",";
  humid_config += "\"unit_of_measurement\":\"%\",";
  humid_config += "\"value_template\":\"{{ value_json.humidity }}\",";
  humid_config += "\"unique_id\":\"" + String(device_name) + "_humid\",";
  humid_config += "\"device\":{";
  humid_config += "\"identifiers\":[\"" + String(device_name) + "\"],";
  humid_config += "\"name\":\"" + String(device_name) + "\",";
  humid_config += "\"model\":\"ESP8266 DHT11\",";
  humid_config += "\"manufacturer\":\"Custom\"";
  humid_config += "}";
  humid_config += "}";

  // Publish discovery messages
  client.publish(temp_config_topic, temp_config.c_str(), true);
  client.publish(humid_config_topic, humid_config.c_str(), true);

  Serial.println("Home Assistant discovery messages sent");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect with authentication
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");

      // Send Home Assistant discovery messages
      publishHomeAssistantDiscovery();

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void readAndPublishSensorData() {
  // Read humidity and temperature
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Create JSON payloads for Home Assistant
  String temp_payload = "{\"temperature\":" + String(temperature, 1) + "}";
  String humid_payload = "{\"humidity\":" + String(humidity, 1) + "}";

  // Publish to MQTT
  client.publish(temp_state_topic, temp_payload.c_str());
  client.publish(humid_state_topic, humid_payload.c_str());

  Serial.println("Data published to MQTT");
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP8266 DHT11 to MQTT");

  // Initialize DHT sensor
  dht.begin();

  // Configure secure client - disable certificate validation for simplicity
  // WARNING: This skips SSL certificate verification for ease of use
  // For production, you should use proper certificate validation
  espClient.setInsecure();

  // Connect to WiFi
  setup_wifi();

  // Configure MQTT
  client.setServer(mqtt_server, mqtt_port);

  // Initial connection
  reconnect();
}

void loop() {
  // Maintain MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read and publish sensor data periodically
  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;
    readAndPublishSensorData();
  }
}