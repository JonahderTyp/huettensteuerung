#pragma once
#include <Arduino.h>
#include <digital_out.h>

#include "button.h"
namespace Huette {
extern DigitalOut K1_KL1_A;
extern DigitalOut K2_KL1_B;
namespace bar_controller_buttons {
extern Button VI6_BTN_DMX;
}  // namespace bar_controller_buttons

}  // namespace Huette

namespace {
int KL_State = 0;
}

namespace kronleuchter {

void loop() {
  if (Huette::bar_controller_buttons::VI4_BTN_KL.isShortPress()) KL_State++;
  if (Huette::bar_controller_buttons::VI4_BTN_KL.isLongPress()) KL_State = 0;

  switch (KL_State) {
    case 0:
      Huette::K1_KL1_A.off();
      Huette::K2_KL1_B.off();
      break;
    case 1:
      Huette::K1_KL1_A.on();
      Huette::K2_KL1_B.off();
      break;
    case 2:
      Huette::K1_KL1_A.on();
      Huette::K2_KL1_B.on();
      break;
    default:
      KL_State = 0;
      break;
  }
}
}  // namespace kronleuchter
