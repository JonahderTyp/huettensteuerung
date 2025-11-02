#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

#include "HAEntity.h"
#include "I2CSlave.h"

class EntityManager {
 private:
  std::vector<HAEntity*> _entities;
  I2CSlave& _i2cSlave;

 public:
  EntityManager(I2CSlave& i2cSlave);
  ~EntityManager();

  void addButton(const char* name, uint8_t registerIndex);
  void addSwitch(const char* name, uint8_t registerIndex);
  void addLight(const char* name, uint8_t registerIndex);
  void addDimmableLight(const char* name, uint8_t registerIndex);

  HAEntity* findEntityByTopic(const char* topic);
  HAEntity* getEntity(size_t index);
  size_t getEntityCount() const { return _entities.size(); }

  void handleCommand(HAEntity* entity, const String& payload);
  void updateI2CRegisters();
  void publishStates(class MQTTHandler& mqtt);
};

#endif
