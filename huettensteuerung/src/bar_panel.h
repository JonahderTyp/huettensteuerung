#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <analog_out.h>
#include <button.h>
#include <digital_out.h>
#include <timer.h>
#define BAR_PANEL_I2C_ADDRESS 0x70

namespace Huette {
extern DigitalOut K1_KL1_A;
extern DigitalOut K2_KL1_B;
extern AnalogOut L1;
extern AnalogOut L2;
extern AnalogOut L3;
extern ButtonInput DI_DMX_SW;
namespace DMX {
extern bool DMX_Active;
}  // namespace DMX
namespace bar_controller_buttons {
extern Button VI1_BTN_S1;
extern Button VI2_BTN_S2;
extern Button VI3_BTN_KL;
extern Button VI4_BTN_BLK;
extern Button VI5_BTN_BAR;
extern Button VI6_BTN_DMX;
}  // namespace bar_controller_buttons

}  // namespace Huette

namespace bar_controller {

uint32_t getLEDcolor(int state) {
  if (state == 0) {
    return 0x00FF0000;
  }
  return 0x0000FF00;  // Default to green
}

void setPixelColor(byte pixel, uint32_t color) {
  Wire.beginTransmission(BAR_PANEL_I2C_ADDRESS);
  Wire.write(0x10);  // Register for the specific pixel
  Wire.write(pixel);

  Wire.write((color >> 24) & 0xFF);  // Not used
  Wire.write((color >> 16) & 0xFF);  // Red
  Wire.write((color >> 8) & 0xFF);   // Green
  Wire.write(color & 0xFF);          // Blue

  Wire.endTransmission();
}

void setLeds() {
  uint32_t color;
  // Scene 1
  color = 0;
  setPixelColor(0, color);
  if (Huette::bar_controller_buttons::VI1_BTN_S1.isPressed())
    color = 0x00FFFFFF;
  setPixelColor(1, color);

  // Scene 2
  color = 0;
  setPixelColor(2, color);
  if (Huette::bar_controller_buttons::VI2_BTN_S2.isPressed())
    color = 0x00FFFFFF;
  setPixelColor(3, color);

  // Kronleuchter
  color = getLEDcolor(Huette::K1_KL1_A.getState());
  setPixelColor(4, color);
  color = getLEDcolor(Huette::K2_KL1_B.getState());
  if (Huette::bar_controller_buttons::VI3_BTN_KL.isPressed())
    color = 0x00FFFFFF;
  setPixelColor(5, color);

  // Balken
  color = getLEDcolor(Huette::L1.getValue());
  setPixelColor(6, color);
  color = getLEDcolor(Huette::L2.getValue());
  if (Huette::bar_controller_buttons::VI4_BTN_BLK.isPressed())
    color = 0x00FFFFFF;
  setPixelColor(7, color);

  // Bar
  color = getLEDcolor(Huette::L3.getValue());
  setPixelColor(8, color);
  if (Huette::bar_controller_buttons::VI5_BTN_BAR.isPressed())
    color = 0x00FFFFFF;
  setPixelColor(9, color);

  // DMX
  color = getLEDcolor(Huette::DMX::DMX_Active);
  setPixelColor(10, color);
  if (Huette::bar_controller_buttons::VI6_BTN_DMX.isPressed())
    color = 0x00FFFFFF;
  setPixelColor(11, color);
}

byte getByte(byte reg) {
  // Serial.print("Reading I2C reg 0x");
  // Serial.print(reg, HEX);
  // Serial.print(" from BAR_PANEL_I2C_ADDRESS 0x");
  // Serial.println(BAR_PANEL_I2C_ADDRESS, HEX);
  // Request 1 byte from the specified register
  uint8_t len = Wire.requestFrom(BAR_PANEL_I2C_ADDRESS, 1, reg, 1, true);
  if (len == 1) {
    byte data = Wire.read();
    // Serial.print("Received byte: 0x");
    // Serial.println(data, HEX);
    return data;
  }
  return 0;
}

void loop() {
  Huette::bar_controller_buttons::VI1_BTN_S1.handle(getByte(0));
  Huette::bar_controller_buttons::VI2_BTN_S2.handle(getByte(1));
  Huette::bar_controller_buttons::VI3_BTN_KL.handle(getByte(2));
  Huette::bar_controller_buttons::VI4_BTN_BLK.handle(getByte(3));
  Huette::bar_controller_buttons::VI5_BTN_BAR.handle(getByte(4));
  Huette::bar_controller_buttons::VI6_BTN_DMX.handle(getByte(5));
  setLeds();
}

}  // namespace bar_controller