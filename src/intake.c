// intake.c

#include "pidbot.h"

void intakeSystemIntakeSet(short s) {
  vexMotorSet(leftIntake, s);
  vexMotorSet(rightIntake, s);
}

void intakeSystemIntake(void) {
  if (vexControllerGet(Btn5U)) {
    intakeSystemIntakeSet(INTAKE_IN * SMAX);
  } else if (vexControllerGet(Btn5D)) {
    intakeSystemIntakeSet(INTAKE_OUT * SMAX);
  } else {
    intakeSystemIntakeSet(0);
  }
}

task intakeTask(void *arg) {
  (void)arg;
  vexTaskRegister("intake");

  while (!chThdShouldTerminate()) {
    intakeSystemIntake();
    vexSleep(25);
  }

  return (task)0;
}