#pragma once
#include <Arduino.h>
#include <analog_out.h>
#include <button.h>
#include <digital_out.h>
#include <timer.h>
#include <ton.h>

namespace Huette {
extern DigitalOut K1_KL1_A;
extern DigitalOut K2_KL1_B;
extern DigitalOut K3_AB1;
extern DigitalOut K4_AB2;
extern DigitalOut K8_Power;
extern AnalogOut L1;
extern AnalogOut L2;
extern ButtonInput DI_OFFTIMER;
}  // namespace Huette

namespace OFFTimer {
Timer OFFtimer((long)1000 * 60 * 5);  // 5 minutes
Ton blink(500);

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
    Huette::K1_KL1_A.on();
    Huette::K2_KL1_B.on();
    Huette::K3_AB1.on();
    Huette::K4_AB2.on();

    blink.setIn(true);
    if (blink.getQ()) {
      blink.setIn(false);
      Huette::L1.toggle();
      Huette::L2.toggle();
    }
  }
}
}  // namespace OFFTimer
