// pneumatics.c

#include "pidbot.h"

void pneumaticsSystemSet(tVexDigitalState s) {
  vexDigitalPinSet(solenoid_1, s);
  vexDigitalPinSet(solenoid_2, s);
}

task pneumaticsTask(void *arg) {
  (void)arg;
  vexTaskRegister("pneumatics");

  pneumaticsSystemSet(kVexDigitalLow);

  while (!chThdShouldTerminate()) {
    if (vexControllerGet(Btn7U) || vexControllerGet(Btn7UXmtr2)) { // lock
      pneumaticsSystemSet(kVexDigitalHigh);
    } else if (vexControllerGet(Btn7D) || vexControllerGet(Btn7DXmtr2)) { // unlock
      pneumaticsSystemSet(kVexDigitalLow);
    }

    vexSleep(25);
  }

  return (task)0;
}