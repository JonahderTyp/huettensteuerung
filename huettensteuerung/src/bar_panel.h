#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <analog_out.h>
#include <button.h>
#include <digital_out.h>
#include <timer.h>
#define BAR_PANEL_I2C_ADDRESS 0x20

namespace Huette {
extern DigitalOut K1_KL1_A;
extern DigitalOut K2_KL1_B;
extern AnalogOut L1;
extern AnalogOut L2;
extern ButtonInput DI_DMX_SW;
namespace DMX {
extern bool DMX_Active;
}  // namespace DMX
namespace bar_controller_buttons {
extern Button VI1_BTN_S1;
extern Button VI2_BTN_S2;
extern Button VI3_BTN_S3;
extern Button VI4_BTN_KL;
extern Button VI5_BTN_BL;
extern Button VI6_BTN_DMX;
}  // namespace bar_controller_buttons

}  // namespace Huette

namespace bar_controller {

byte getByte(byte reg) {
  Wire.beginTransmission(BAR_PANEL_I2C_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(BAR_PANEL_I2C_ADDRESS, 1);
  byte data = 0;
  if (Wire.available()) {
    data = Wire.read();
  }
  return data;
}

void loop() {
  Huette::bar_controller_buttons::VI4_BTN_KL.handle(getByte(0x01));
  Huette::bar_controller_buttons::VI6_BTN_DMX.handle(getByte(0x02));
}

uint32_t getLEDcolor(int state) {
  if (state == 0) {
    return 0x00FF0000;
  }
  return 0x0000FF00;  // Default to green
}

void setLeds() {
  uint32_t color;
  Wire.beginTransmission(BAR_PANEL_I2C_ADDRESS);
  Wire.write(0x80);  // Register for LED control

  // Scene 1
  color = 0;
  if (Huette::bar_controller_buttons::VI1_BTN_S1.isPressed())
    color = 0x00FFFFFF;
  Wire.write(color);
  Wire.write(color);

  // Scene 2
  color = 0;
  if (Huette::bar_controller_buttons::VI2_BTN_S2.isPressed())
    color = 0x00FFFFFF;
  Wire.write(color);
  Wire.write(color);

  // Scene 3
  color = 0;
  if (Huette::bar_controller_buttons::VI3_BTN_S3.isPressed())
    color = 0x00FFFFFF;
  Wire.write(color);
  Wire.write(color);

  // Kronleuchter
  color = getLEDcolor(Huette::K1_KL1_A.getState());
  if (Huette::bar_controller_buttons::VI4_BTN_KL.isPressed())
    color = 0x00FFFFFF;
  Wire.write(color);
  color = getLEDcolor(Huette::K2_KL1_B.getState());
  if (Huette::bar_controller_buttons::VI4_BTN_KL.isPressed())
    color = 0x00FFFFFF;
  Wire.write(color);

  // Balken
  color = getLEDcolor(Huette::L1.getValue());
  if (Huette::bar_controller_buttons::VI5_BTN_BL.isPressed())
    color = 0x00FFFFFF;
  Wire.write(color);
  color = getLEDcolor(Huette::L2.getValue());
  if (Huette::bar_controller_buttons::VI5_BTN_BL.isPressed())
    color = 0x00FFFFFF;
  Wire.write(color);

  // DMX
  color = getLEDcolor(Huette::DMX::DMX_Active);
  if (Huette::bar_controller_buttons::VI6_BTN_DMX.isPressed())
    color = 0x00FFFFFF;
  Wire.write(color);
  Wire.write(color);

  Wire.endTransmission();
}

}  // namespace bar_controller
