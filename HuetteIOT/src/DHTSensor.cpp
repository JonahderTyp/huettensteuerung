#include "DHTSensor.h"

DHTSensor::DHTSensor(uint8_t pin, uint8_t type)
    : _dht(pin, type), _lastTemperature(0), _lastHumidity(0) {}

void DHTSensor::begin() {
  _dht.begin();
  Serial.println("DHT sensor initialized");
}

bool DHTSensor::readSensor() {
  float humidity = _dht.readHumidity();
  float temperature = _dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }

  _lastTemperature = temperature;
  _lastHumidity = humidity;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  return true;
}

float DHTSensor::getTemperature() const { return _lastTemperature; }

float DHTSensor::getHumidity() const { return _lastHumidity; }
