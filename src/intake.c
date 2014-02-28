// intake.c

#include "pidbot.h"

void intakeSystemIntakeSet(short s) {
  vexMotorSet(leftIntake, s);
  vexMotorSet(rightIntake, s);
}

void intakeSystemIntake(void) {
  if (vexControllerGet(Btn5U)) {
    intakeSystemIntakeSet(SMAX);
  } else if (vexControllerGet(Btn5D)) {
    intakeSystemIntakeSet(SMIN);
  } else {
    intakeSystemIntakeSet(0);
  }
}

task intakeTask(void *arg) {
  (void)arg;
  vexTaskRegister("intake");

  while(!chThdShouldTerminate()) {
    intakeSystemIntake();
    vexSleep(25);
  }

  return (task)0;
}