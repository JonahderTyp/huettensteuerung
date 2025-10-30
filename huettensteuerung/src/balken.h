#pragma once
#include <Arduino.h>
#include <analog_out.h>
#include <digital_out.h>
#include <ton.h>

#include "button.h"
namespace Huette {
extern AnalogOut L1;
extern AnalogOut L2;
namespace bar_controller_buttons {
extern Button VI5_BTN_BL;
}
}  // namespace Huette

namespace {
bool direction = true;
Ton dimmer_timer(10);
}  // namespace

namespace kronleuchter {

void loop() {
  if (Huette::bar_controller_buttons::VI5_BTN_BL.isShortPress()) {
    if (direction) {
      Huette::L1.off();
      Huette::L2.off();
    } else {
      Huette::L1.on();
      Huette::L2.on();
    }
    direction = !direction;
  }

  if (Huette::bar_controller_buttons::VI5_BTN_BL.isLongPress()) {
    dimmer_timer.setIn(true);
    if (dimmer_timer.getQ()) {
      dimmer_timer.setIn(false);
      float val1 = Huette::L1.getPercent();
      float val2 = Huette::L2.getPercent();
      if (direction) {
        Huette::L1.setPercent(val1 + 1.0f);
        Huette::L2.setPercent(val2 + 1.0f);
      } else {
        Huette::L1.setPercent(val1 - 1.0f);
        Huette::L2.setPercent(val2 - 1.0f);
      }
    }
  }
  if (Huette::bar_controller_buttons::VI5_BTN_BL.isLongPressRelease()) {
    direction = !direction;
  }
}
}  // namespace kronleuchter
