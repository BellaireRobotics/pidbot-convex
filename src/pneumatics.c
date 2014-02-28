// pneumatics.c

#include "pidbot.h"

void pneumaticsSystemSet(int b) {
  vexSensorValueSet(solenoid_1, b);
  vexSensorValueSet(solenoid_2, b);
}

task pneumaticsTask(void *arg) {
  (void)arg;
  vexTaskRegister("pneumatics");

  while (!chThdShouldTerminate()) {
    if (vexControllerGet(Btn7U) || vexControllerGet(Btn7UXmtr2)) {
      pneumaticsSystemSet(1);
    } else if (vexControllerGet(Btn7D) || vexControllerGet(Btn7DXmtr2)) {
      pneumaticsSystemSet(0);
    }

    vexSleep(25);
  }

  return (task)0;
}