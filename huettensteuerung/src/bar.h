#pragma once
#include <Arduino.h>
#include <analog_out.h>
#include <digital_out.h>
#include <ton.h>

#include "button.h"
namespace Huette {
extern AnalogOut L3;
namespace bar_controller_buttons {
extern Button VI5_BTN_BAR;
}
}  // namespace Huette

namespace bar {
bool direction = false;
Ton dimmer_timer(10);

void loop() {
  if (Huette::bar_controller_buttons::VI5_BTN_BAR.isShortPress()) {
    if (direction) {
      Huette::L3.off();
    } else {
      Huette::L3.on();
    }
    direction = !direction;
  }

  if (Huette::bar_controller_buttons::VI5_BTN_BAR.isLongPress()) {
    dimmer_timer.setIn(true);
    if (dimmer_timer.getQ()) {
      dimmer_timer.setIn(false);
      float val1 = Huette::L3.getPercent();
      if (direction) {
        Huette::L3.setPercent(val1 + 1.0f);
        if (val1 >= 100.0f) direction = !direction;

      } else {
        Huette::L3.setPercent(val1 - 1.0f);
        if (val1 <= 0.0f) direction = !direction;
      }
    }
  }
  if (Huette::bar_controller_buttons::VI5_BTN_BAR.isLongPressRelease()) {
    direction = !direction;
  }
}
}  // namespace bar
