#include "EntityManager.h"

#include "MQTTHandler.h"

EntityManager::EntityManager(I2CSlave& i2cSlave) : _i2cSlave(i2cSlave) {}

EntityManager::~EntityManager() {
  for (auto entity : _entities) {
    delete entity;
  }
}

void EntityManager::addButton(const char* name, uint8_t registerIndex) {
  _entities.push_back(new HAEntity(name, EntityType::BUTTON, registerIndex));
  Serial.print("Added Button: ");
  Serial.print(name);
  Serial.print(" -> Register ");
  Serial.println(registerIndex);
}

void EntityManager::addSwitch(const char* name, uint8_t registerIndex) {
  _entities.push_back(new HAEntity(name, EntityType::SWITCH, registerIndex));
  Serial.print("Added Switch: ");
  Serial.print(name);
  Serial.print(" -> Register ");
  Serial.println(registerIndex);
}

void EntityManager::addLight(const char* name, uint8_t registerIndex) {
  _entities.push_back(new HAEntity(name, EntityType::LIGHT, registerIndex));
  Serial.print("Added Light: ");
  Serial.print(name);
  Serial.print(" -> Register ");
  Serial.println(registerIndex);
}

void EntityManager::addDimmableLight(const char* name, uint8_t registerIndex) {
  _entities.push_back(
      new HAEntity(name, EntityType::DIMMABLE_LIGHT, registerIndex));
  Serial.print("Added Dimmable Light: ");
  Serial.print(name);
  Serial.print(" -> Register ");
  Serial.println(registerIndex);
}

HAEntity* EntityManager::findEntityByTopic(const char* topic) {
  for (auto entity : _entities) {
    if (entity->getCommandTopic() == String(topic)) {
      return entity;
    }
    if (entity->getType() == EntityType::DIMMABLE_LIGHT &&
        entity->getBrightnessCommandTopic() == String(topic)) {
      return entity;
    }
  }
  return nullptr;
}

HAEntity* EntityManager::getEntity(size_t index) {
  if (index < _entities.size()) {
    return _entities[index];
  }
  return nullptr;
}

void EntityManager::handleCommand(HAEntity* entity, const String& payload) {
  if (!entity) return;

  Serial.print("Handling command for ");
  Serial.print(entity->getName());
  Serial.print(": ");
  Serial.println(payload);

  EntityType type = entity->getType();

  if (type == EntityType::BUTTON) {
    // Buttons trigger on any command
    entity->setState(true);
  } else if (type == EntityType::SWITCH || type == EntityType::LIGHT) {
    // Parse ON/OFF commands
    if (payload == "ON") {
      entity->setState(true);
    } else if (payload == "OFF") {
      entity->setState(false);
    }
  } else if (type == EntityType::DIMMABLE_LIGHT) {
    // Check if it's a brightness command or state command
    if (payload.toInt() > 0 && payload.toInt() <= 255) {
      // Brightness value
      entity->setBrightness(payload.toInt());
      if (payload.toInt() > 0) {
        entity->setState(true);
      }
    } else if (payload == "ON") {
      entity->setState(true);
      if (entity->getBrightness() == 0) {
        entity->setBrightness(255);
      }
    } else if (payload == "OFF") {
      entity->setState(false);
    }
  }

  // Update I2C register
  _i2cSlave.setRegister(entity->getRegisterIndex(), entity->getRegisterValue());
}

void EntityManager::updateI2CRegisters() {
  for (auto entity : _entities) {
    _i2cSlave.setRegister(entity->getRegisterIndex(),
                          entity->getRegisterValue());
  }
}

void EntityManager::publishStates(MQTTHandler& mqtt) {
  for (auto entity : _entities) {
    if (entity->getType() != EntityType::BUTTON) {
      String payload = entity->getStatePayload();
      if (payload.length() > 0) {
        mqtt.publish(entity->getStateTopic().c_str(), payload.c_str());
      }
    }
  }
}
