#include "HAEntity.h"

#include "Config.h"

HAEntity::HAEntity(const char* name, EntityType type, uint8_t registerIndex)
    : _name(name), _type(type), _registerIndex(registerIndex) {
  _uniqueId = String(ESPIOT_DEVICE_NAME) + "_" + String(name);
  _state.state = false;
  _state.brightness = 0;
}

String HAEntity::getStateTopic() const {
  String domain = getEntityTypeName();
  return "homeassistant/" + domain + "/" + String(ESPIOT_DEVICE_NAME) + "/" +
         _name + "/state";
}

String HAEntity::getCommandTopic() const {
  String domain = getEntityTypeName();
  return "homeassistant/" + domain + "/" + String(ESPIOT_DEVICE_NAME) + "/" +
         _name + "/set";
}

String HAEntity::getConfigTopic() const {
  String domain = getEntityTypeName();
  return "homeassistant/" + domain + "/" + String(ESPIOT_DEVICE_NAME) + "/" +
         _name + "/config";
}

String HAEntity::getBrightnessStateTopic() const {
  return "homeassistant/light/" + String(ESPIOT_DEVICE_NAME) + "/" + _name +
         "/brightness/state";
}

String HAEntity::getBrightnessCommandTopic() const {
  return "homeassistant/light/" + String(ESPIOT_DEVICE_NAME) + "/" + _name +
         "/brightness/set";
}

void HAEntity::setState(bool state) { _state.state = state; }

void HAEntity::setBrightness(uint8_t brightness) {
  _state.brightness = brightness;
}

String HAEntity::getStatePayload() const {
  if (_type == EntityType::DIMMABLE_LIGHT) {
    String payload = "{\"state\":\"";
    payload += _state.state ? "ON" : "OFF";
    payload += "\",\"brightness\":";
    payload += String(_state.brightness);
    payload += "}";
    return payload;
  } else if (_type == EntityType::LIGHT || _type == EntityType::SWITCH) {
    return _state.state ? "ON" : "OFF";
  }
  return "";
}

uint8_t HAEntity::getRegisterValue() const {
  switch (_type) {
    case EntityType::BUTTON:
      return _state.state ? 1 : 0;
    case EntityType::SWITCH:
    case EntityType::LIGHT:
      return _state.state ? 1 : 0;
    case EntityType::DIMMABLE_LIGHT:
      return _state.brightness;
    default:
      return 0;
  }
}

const char* HAEntity::getEntityTypeName() const {
  switch (_type) {
    case EntityType::BUTTON:
      return "button";
    case EntityType::SWITCH:
      return "switch";
    case EntityType::LIGHT:
    case EntityType::DIMMABLE_LIGHT:
      return "light";
    default:
      return "unknown";
  }
}
