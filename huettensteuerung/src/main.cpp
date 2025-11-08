#include <Arduino.h>
#include <analog_out.h>
#include <button.h>
#include <digital_out.h>
#include <switch.h>
#include <timer.h>

#include "aussenbel.h"
#include "balken.h"
#include "bar_panel.h"
#include "dmx.h"
#include "kronleuchter.h"
#include "offtimer.h"

namespace Huette {
DigitalOut K1_KL1_A(53, true);
DigitalOut K2_KL1_B(52, true);
DigitalOut K3_AB1(51, true);
DigitalOut K4_AB2(50, true);
DigitalOut K5(49, true);
DigitalOut K6(48, true);
DigitalOut K7(47, true);
DigitalOut K8_Power(46, true);

AnalogOut L1(4, false, 255, 255);
AnalogOut L2(5, false, 255, 255);
AnalogOut L3(2, false, 0, 255);
AnalogOut L4(3, false, 0, 255);

namespace DMX {
bool DMX_Active = false;
}  // namespace DMX

namespace bar_controller_buttons {
Button VI1_BTN_S1;
Button VI2_BTN_S2;
Button VI3_BTN_S3;
Button VI4_BTN_KL;
Button VI5_BTN_BL;
Button VI6_BTN_DMX;
}  // namespace bar_controller_buttons

ButtonInput DI_OFFTIMER(1, 1000);
SwitchInput SW_AB_1(22);
SwitchInput SW_AB_2(23);

void handlers() {
  DI_OFFTIMER.handle();
  SW_AB_1.handle();
  SW_AB_2.handle();
}

};  // namespace Huette

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Huettensteuerung starting...");
  delay(100);
  Wire.begin();
}

void loop() {
  Huette::handlers();
  OFFTimer::loop();
  DMX::loop();
  kronleuchter::loop();
  bar_controller::loop();
  balken::loop();
  aussenbel::loop();
}
