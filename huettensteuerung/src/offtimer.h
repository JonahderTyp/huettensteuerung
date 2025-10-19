#pragma once
#include <Arduino.h>
#include <analog_out.h>
#include <button.h>
#include <digital_out.h>
#include <timer.h>

namespace Huette {
extern DigitalOut K8_Power;
extern AnalogOut L1;
extern AnalogOut L2;
extern ButtonInput DI_OFFTIMER;
}  // namespace Huette

namespace OFFTimer {
Timer OFFtimer((long)1000 * 60 * 5);  // 5 minutes
Timer OFFtimerBlinker(500);

void loop() {
  Huette::DI_OFFTIMER.handle();

  if (Huette::DI_OFFTIMER.isShortPress() || Huette::DI_OFFTIMER.isLongPress()) {
    if (OFFtimer.isRunning()) {
      OFFtimer.reset();
    } else if (Huette::DI_OFFTIMER.isLongPress()) {
      OFFtimer.start();
    }
  }

  if (OFFtimer.hasElapsed()) {
    Huette::K8_Power.on();
  }

  if (OFFtimer.isRunning()) {
    if (OFFtimerBlinker.hasElapsed()) {
      Huette::L1.toggle();
      Huette::L2.toggle();
      OFFtimerBlinker.start();
      OFFtimerBlinker.reset();
    }
  }
}
}  // namespace OFFTimer
