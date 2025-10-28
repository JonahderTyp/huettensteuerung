#include <Arduino.h>
#include <analog_out.h>
#include <button.h>
#include <digital_out.h>
#include <timer.h>

#include "bar_panel.h"
#include "dmx.h"
#include "kronleuchter.h"
#include "offtimer.h"

namespace Huette {
DigitalOut K1_KL1_A(1, true);
DigitalOut K2_KL1_B(2, true);
DigitalOut K3_AB1(3, true);
DigitalOut K4_AB2(4, true);
DigitalOut K5(5, true);
DigitalOut K6(6, true);
DigitalOut K7(7, true);
DigitalOut K8_Power(8, true);

AnalogOut L1(9, false, 0, 255);
AnalogOut L2(10, false, 0, 255);

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

};  // namespace Huette

void setup() {}

void loop() {
  Huette::DI_OFFTIMER.handle();

  OFFTimer::loop();
  DMX::loop();
  kronleuchter::loop();
  bar_controller::loop();
}
