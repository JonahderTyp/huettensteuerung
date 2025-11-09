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
}

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

uint32_t getLEDcolor(int state, bool brightness_adjust = true,
                     uint8_t brightnessLow = 31, uint8_t brightnessHigh = 255) {
  uint32_t color;
  if (state == 0)
    color = 0x00FF0000;
  else
    color = 0x0000FF00;  // Default to green

  // reduce brightness
  uint8_t brightness = brightness_adjust ? brightnessHigh : brightnessLow;
  uint32_t a = (color >> 24) & 0xFF;
  uint32_t r = (color >> 16) & 0xFF;
  uint32_t g = (color >> 8) & 0xFF;
  uint32_t b = color & 0xFF;
  a = (a * brightness) / 255;
  r = (r * brightness) / 255;
  g = (g * brightness) / 255;
  b = (b * brightness) / 255;
  color = (a << 24) | (r << 16) | (g << 8) | b;

  return color;
}

void setPixelColor(byte pixel, uint32_t color) {
  Wire.beginTransmission(BAR_PANEL_I2C_ADDRESS);
  Wire.write(0x10);  // Pixel register
  Wire.write(pixel);
  Wire.write(0x00);                  // Alpha (unused)
  Wire.write((color >> 16) & 0xFF);  // Red
  Wire.write((color >> 8) & 0xFF);   // Green
  Wire.write(color & 0xFF);          // Blue
  Wire.endTransmission();
}

void setLeds() {
  using namespace Huette::bar_controller_buttons;

  // Scene buttons (S1 & S2)
  setPixelColor(0, VI1_BTN_S1.isPressed() ? 0 : 0);
  setPixelColor(1, VI1_BTN_S1.isPressed() ? 0 : 0);
  setPixelColor(2, VI2_BTN_S2.isPressed() ? 0 : 0);
  setPixelColor(3, VI2_BTN_S2.isPressed() ? 0 : 0);

  // Kronleuchter
  setPixelColor(
      4, getLEDcolor(Huette::K1_KL1_A.getState(), VI3_BTN_KL.isPressed()));
  setPixelColor(
      5, getLEDcolor(Huette::K2_KL1_B.getState(), VI3_BTN_KL.isPressed()));

  // Balken
  setPixelColor(6, getLEDcolor(Huette::L1.getValue(), VI4_BTN_BLK.isPressed()));
  setPixelColor(7, getLEDcolor(Huette::L2.getValue(), VI4_BTN_BLK.isPressed()));

  // Bar
  uint32_t barColor =
      getLEDcolor(Huette::L3.getValue(), VI5_BTN_BAR.isPressed());
  setPixelColor(8, barColor);
  setPixelColor(9, barColor);

  // DMX
  uint32_t dmxColor =
      getLEDcolor(Huette::DMX::DMX_Active, VI6_BTN_DMX.isPressed());
  setPixelColor(10, dmxColor);
  setPixelColor(11, dmxColor);
}

byte getByte(byte reg) {
  // Manual I2C transaction - explicit byte by byte
  Wire.beginTransmission(BAR_PANEL_I2C_ADDRESS);
  Wire.write(reg);              // Send register address
  Wire.endTransmission(false);  // Send restart condition

  // Request one byte
  Wire.requestFrom(BAR_PANEL_I2C_ADDRESS, 1);

  // Read the byte if available
  if (Wire.available()) {
    return Wire.read();
  }
  return 0;
}

void loop() {
  using namespace Huette::bar_controller_buttons;

  VI1_BTN_S1.handle(getByte(0));
  VI2_BTN_S2.handle(getByte(1));
  VI3_BTN_KL.handle(getByte(2));
  VI4_BTN_BLK.handle(getByte(3));
  VI5_BTN_BAR.handle(getByte(4));
  VI6_BTN_DMX.handle(getByte(5));

  setLeds();
}
}  // namespace bar_controller