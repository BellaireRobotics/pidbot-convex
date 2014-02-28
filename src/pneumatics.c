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
    if (vexControllerGet(Btn7U) || vexControllerGet(Btn7UXmtr2)) { // lock
      pneumaticsSystemSet(HIGH);
    } else if (vexControllerGet(Btn7D) || vexControllerGet(Btn7DXmtr2)) { // unlock
      pneumaticsSystemSet(LOW);
    }

    vexSleep(25);
  }

  return (task)0;
}