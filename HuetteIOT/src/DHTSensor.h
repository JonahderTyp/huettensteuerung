#ifndef DHTSENSOR_H
#define DHTSENSOR_H

#include <DHT.h>

class DHTSensor {
 private:
  DHT _dht;
  float _lastTemperature;
  float _lastHumidity;

 public:
  DHTSensor(uint8_t pin, uint8_t type);
  void begin();
  bool readSensor();
  float getTemperature() const;
  float getHumidity() const;
};

#endif
