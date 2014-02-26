// pidbot.c

#include <stdlib.h>

#include "ch.h"  // needs for all ChibiOS programs
#include "hal.h" // hardware abstraction layer header
#include "vex.h" // vex library header

#include "pidbot.h"
#include "apollo.h"
#include "smartmotor.h"
#include "robotc_glue.h" // for ROBOTC-like tasks
#include "pidlib.h"

#define JOYSTICK_DEADZONE 15
#define SMAX 127
#define SMIN (-127)

#define signum(x) ((x > 0) - (x < 0))

// linearizing array, goes to 256 to save CPU cycles; xmax + ymax = 256
const unsigned int TrueSpeed[256] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0, 21, 21, 21, 22, 22, 22, 23, 24, 24,
  25, 25, 25, 25, 26, 27, 27, 28, 28, 28,
  28, 29, 30, 30, 30, 31, 31, 32, 32, 32,
  33, 33, 34, 34, 35, 35, 35, 36, 36, 37,
  37, 37, 37, 38, 38, 39, 39, 39, 40, 40,
  41, 41, 42, 42, 43, 44, 44, 45, 45, 46,
  46, 47, 47, 48, 48, 49, 50, 50, 51, 52,
  52, 53, 54, 55, 56, 57, 57, 58, 59, 60,
  61, 62, 63, 64, 65, 66, 67, 67, 68, 70,
  71, 72, 72, 73, 74, 76, 77, 78, 79, 79,
  80, 81, 83, 84, 84, 86, 86, 87, 87, 88,
  88, 89, 89, 90, 90, 127, 127, 127,

  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127
};

void driveSystemArcadeDrive(void) {
  short y = vexControllerGet(Ch3);
  short x = vexControllerGet(Ch1);

  short l, r;

  if (abs(y) < JOYSTICK_DEADZONE) {
    y = 0;
  }

  if (abs(x) < JOYSTICK_DEADZONE) {
    x = 0;
  }

  l = signum(y - x) * TrueSpeed[abs(y - x)];
  r = signum(y + x) * TrueSpeed[abs(y + x)];

  vexMotorSet(leftFront, l);
  vexMotorSet(rightFront, r);
  vexMotorSet(leftBack, l);
  vexMotorSet(rightBack, r);
}

task driveTask(void *arg) {
  (void)arg; // "use" `arg` to avoid unused variable warning.

  vexTaskRegister("drive");

  while (!chThdShouldTerminate()) {
    driveSystemArcadeDrive();
    vexSleep(25);
  }

  return (task)0;
}

task liftTask(void *arg) {
  (void)arg;

  vexTaskRegister("lift");

  while (!chThdShouldTerminate()) {
    vexSleep(25);
  }

  return (task)0;
}

task liftPIDTask(void *arg) {
  (void)arg;

  vexTaskRegister("lift PID");

  while (!chThdShouldTerminate()) {
    vexSleep(25);
  }

  return (task)0;
}

void intakeSystemSet(short s) {
  vexMotorSet(leftIntake, s);
  vexMotorSet(rightIntake, s);
}

task intakeTask(void *arg) {
  (void)arg;

  vexTaskRegister("intake");

  while (!chThdShouldTerminate()) {
    if (vexControllerGet(Btn5U)) {
      intakeSystemSet(SMAX);
    } else if (vexControllerGet(Btn5D)) {
      intakeSystemSet(SMIN);
    } else {
      intakeSystemSet(0);
    }

    vexSleep(25);
  } 

  return (task)0;
}

task pneumaticsTask(void *arg) {
  (void)arg;

  vexTaskRegister("pneumatics");

  while (!chThdShouldTerminate()) {
    vexSleep(25);
  }

  return (task)0;
}

task apolloTask(void *arg) {
  (void)arg;

  vexTaskRegister("apollo");
  apolloInit();

  while(1) { // Always update apollo output.
    apolloUpdate();
    vexSleep(150); // Slow updates.
  }

  return (task)0;
}