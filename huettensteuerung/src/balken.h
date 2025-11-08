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
extern Button VI4_BTN_BLK;
}
}  // namespace Huette

namespace {
bool direction = true;
Ton dimmer_timer(10);
}  // namespace

namespace balken {

void loop() {
  if (Huette::bar_controller_buttons::VI4_BTN_BLK.isShortPress()) {
    if (direction) {
      Huette::L1.off();
      Huette::L2.off();
    } else {
      Huette::L1.on();
      Huette::L2.on();
    }
    direction = !direction;
  }

  if (Huette::bar_controller_buttons::VI4_BTN_BLK.isLongPress()) {
    dimmer_timer.setIn(true);
    if (dimmer_timer.getQ()) {
      dimmer_timer.setIn(false);
      float val1 = Huette::L1.getPercent();
      float val2 = Huette::L2.getPercent();
      if (direction) {
        Huette::L1.setPercent(val1 + 1.0f);
        Huette::L2.setPercent(val2 + 1.0f);
        if (val1 >= 100.0f && val2 >= 100.0f) direction = !direction;

      } else {
        Huette::L1.setPercent(val1 - 1.0f);
        Huette::L2.setPercent(val2 - 1.0f);
        if (val1 <= 0.0f && val2 <= 0.0f) direction = !direction;
      }
    }
  }
  if (Huette::bar_controller_buttons::VI4_BTN_BLK.isLongPressRelease()) {
    direction = !direction;
  }
}
}  // namespace balken
