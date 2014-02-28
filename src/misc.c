// misc.c

#include "pidbot.h"

// real time display of motor and sensor data (UNUSED; done in main shell task)
task apolloTask(void *arg) {
  (void)arg;
  vexTaskRegister("apollo");

  apolloInit();

  while (!chThdShouldTerminate()) {
    apolloUpdate();
    vexSleep(15);
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