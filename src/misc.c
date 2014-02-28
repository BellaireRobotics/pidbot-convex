// misc.c

#include "pidbot.h"

task apolloTask(void *arg) {
  (void)arg;
  vexTaskRegister("apollo");

  apolloInit();

  while (1) { // Always update apollo output.
    apolloUpdate();
    vexSleep(15); // Slow updates.
  }

  return (task)0;
}

task safetyTask(void *arg) {
  (void)arg;
  vexTaskRegister("safety");

  while (!chThdShouldTerminate()) {
    // detect if controller is face down.
    if (vexControllerGet(AcclY) < 0) { // figure this out
      vexMotorStopAll(); // kill all motors (for now)
      //vexTaskEmergencyStop(); // emergency stop, reboot required
    }

    vexSleep(10);
  }

  return (task)0;
}