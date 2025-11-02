#ifndef HAENTITY_H
#define HAENTITY_H

#include <Arduino.h>

// Entity types supported by Home Assistant
enum class EntityType { BUTTON, SWITCH, LIGHT, DIMMABLE_LIGHT };

// Entity state
struct EntityState {
  bool state;          // On/Off state
  uint8_t brightness;  // For dimmable lights (0-255)
};

// Base class for Home Assistant entities
class HAEntity {
 protected:
  String _name;
  String _uniqueId;
  EntityType _type;
  uint8_t _registerIndex;
  EntityState _state;

 public:
  HAEntity(const char* name, EntityType type, uint8_t registerIndex);

  const char* getName() const { return _name.c_str(); }
  const char* getUniqueId() const { return _uniqueId.c_str(); }
  EntityType getType() const { return _type; }
  uint8_t getRegisterIndex() const { return _registerIndex; }

  String getStateTopic() const;
  String getCommandTopic() const;
  String getConfigTopic() const;
  String getBrightnessStateTopic() const;
  String getBrightnessCommandTopic() const;

  void setState(bool state);
  void setBrightness(uint8_t brightness);
  bool getState() const { return _state.state; }
  uint8_t getBrightness() const { return _state.brightness; }

  String getStatePayload() const;
  uint8_t getRegisterValue() const;

  const char* getEntityTypeName() const;
};

#endif
