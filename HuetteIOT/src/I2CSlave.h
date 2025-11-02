#ifndef I2CSLAVE_H
#define I2CSLAVE_H

#include <Arduino.h>
#include <Wire.h>

class I2CSlave {
 private:
  uint8_t _address;
  uint8_t _sdaPin;
  uint8_t _sclPin;
  uint8_t _numRegisters;

  static volatile uint8_t* _registers;
  static volatile uint8_t _currentRegister;
  static uint8_t _numRegs;

  static void onRequestHandler();
  static void onReceiveHandler(int numBytes);

 public:
  I2CSlave(uint8_t address, uint8_t sdaPin, uint8_t sclPin,
           uint8_t numRegisters);
  void begin();
  void setRegister(uint8_t reg, uint8_t value);
  uint8_t getRegister(uint8_t reg);
};

#endif
