#include "I2CSlave.h"

// Static member initialization
volatile uint8_t* I2CSlave::_registers = nullptr;
volatile uint8_t I2CSlave::_currentRegister = 0;
uint8_t I2CSlave::_numRegs = 0;

I2CSlave::I2CSlave(uint8_t address, uint8_t sdaPin, uint8_t sclPin,
                   uint8_t numRegisters)
    : _address(address),
      _sdaPin(sdaPin),
      _sclPin(sclPin),
      _numRegisters(numRegisters) {
  _numRegs = numRegisters;
  _registers = new uint8_t[numRegisters];
  for (uint8_t i = 0; i < numRegisters; i++) {
    _registers[i] = 0;
  }
}

void I2CSlave::begin() {
  Wire.begin(_address, _sdaPin, _sclPin);
  Wire.onRequest(onRequestHandler);
  Wire.onReceive(onReceiveHandler);

  Serial.print("I2C Slave initialized at address 0x");
  Serial.println(_address, HEX);
}

void I2CSlave::setRegister(uint8_t reg, uint8_t value) {
  if (reg < _numRegisters) {
    _registers[reg] = value;
  }
}

uint8_t I2CSlave::getRegister(uint8_t reg) {
  if (reg < _numRegisters) {
    return _registers[reg];
  }
  return 0;
}

void I2CSlave::onRequestHandler() {
  // Send the current register value
  uint8_t value = _registers[_currentRegister];
  Wire.write(value);

  // Auto-reset register to 0 after reading
  _registers[_currentRegister] = 0;

  Serial.print("I2C Read - Register ");
  Serial.print(_currentRegister);
  Serial.print(": ");
  Serial.println(value);
}

void I2CSlave::onReceiveHandler(int numBytes) {
  if (numBytes > 0) {
    // First byte is the register address
    _currentRegister = Wire.read();

    // Ensure register address is within bounds
    if (_currentRegister >= _numRegs) {
      _currentRegister = 0;
    }

    // If there's a second byte, write it to the register
    if (numBytes > 1 && Wire.available()) {
      uint8_t value = Wire.read();
      _registers[_currentRegister] = value;
      Serial.print("I2C Write - Register ");
      Serial.print(_currentRegister);
      Serial.print(": ");
      Serial.println(value);
    }
  }
}
