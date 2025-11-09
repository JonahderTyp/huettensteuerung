#pragma once
#include <Arduino.h>
#include <analog_out.h>
#include <digital_out.h>
#include <switch.h>
#include <ton.h>

#include "button.h"
namespace Huette {
extern DigitalOut K3_AB1;
extern DigitalOut K4_AB2;
// extern SwitchInput SW_AB_1;
// extern SwitchInput SW_AB_2;
}  // namespace Huette

namespace aussenbel {

void loop() {
  // if (Huette::SW_AB_1.toggled()) {
  //   Huette::K3_AB1.set(Huette::SW_AB_1.state());
  // }

  // if (Huette::SW_AB_2.toggled()) {
  //   Huette::K4_AB2.set(Huette::SW_AB_2.state());
  // }
}
}  // namespace aussenbel