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

#define LIFT_MINIMUM_HEIGHT 0
#define LIFT_FLOOR_HEIGHT   0
#define LIFT_BUMP_HEIGHT    150
#define LIFT_STASH_HEIGHT   1100
#define LIFT_HANG_HEIGHT    1280
#define LIFT_MAX_HEIGHT     1280

#define signum(x) ((x > 0) - (x < 0))

pidController *armPID;

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

void armSystemLiftSet(short s) {
  vexMotorSet(leftTopLift, s);
  vexMotorSet(leftBottomLift, s);
  vexMotorSet(rightTopLift, s);
  vexMotorSet(rightBottomLift, s);
}

void armSystemLift(void) {
  if (vexControllerGet(Btn8UXmtr2) || vexControllerGet(Btn8U)) { // stash waypoint
    armPID->target_value = LIFT_STASH_HEIGHT;
  } else if (vexControllerGet(Btn8LXmtr2) || vexControllerGet(Btn8L)) { // bump waypoint
    armPID->target_value = LIFT_BUMP_HEIGHT;
  } else if (vexControllerGet(Btn8DXmtr2) || vexControllerGet(Btn8D)) { // floor waypoint
    armPID->target_value = LIFT_FLOOR_HEIGHT;
  } else if (vexControllerGet(Btn8RXmtr2) || vexControllerGet(Btn8R)) { // hang waypoint
    armPID->target_value = LIFT_HANG_HEIGHT;
  } else if (vexControllerGet(Btn6UXmtr2) || vexControllerGet(Btn6U)) {
    //armPID->target_value += 5;
    if (vexSensorValueGet(armEnc) < LIFT_MAX_HEIGHT) { // arm override up
      armSystemLiftSet(127);
      //vexSleep(2);
      armPID->target_value = vexSensorValueGet(armEnc);
    }
  } else if (vexControllerGet(Btn6DXmtr2) || vexControllerGet(Btn6D)) {
    //armPID->target_value -= 10;
    if (vexSensorValueGet(armEnc) > LIFT_MINIMUM_HEIGHT) { // arm override down
      armSystemLiftSet(-127);
      //vexSleep(2);
      armPID->target_value = vexSensorValueGet(armEnc);
    }
  } else {
    armSystemLiftSet(0); // important... don't wanna accidently chew up gears, yeah...? lol
  }

  if (armPID->target_value < LIFT_MINIMUM_HEIGHT) {
    armPID->target_value = LIFT_MINIMUM_HEIGHT;
  }

  if (armPID->target_value > LIFT_MAX_HEIGHT) {
    armPID->target_value = LIFT_MAX_HEIGHT;
  }

  PidControllerUpdate(armPID); // aim...

  // Kill if power is lost
  if (vexSpiGetMainBattery() < 3000) {
    armPID->drive_cmd = 0;
  }

  armSystemLiftSet(armPID->drive_cmd); // ...and fire!
}

void armSystemIntakeSet(short s) {
  vexMotorSet(leftIntake, s);
  vexMotorSet(rightIntake, s);
}

void armSystemIntake(void) {
  if (vexControllerGet(Btn5U)) {
    armSystemIntakeSet(SMAX);
  } else if (vexControllerGet(Btn5D)) {
    armSystemIntakeSet(SMIN);
  } else {
    armSystemIntakeSet(0);
  }
}

task armTask(void *arg) {
  (void)arg;
  vexTaskRegister("arm");

  armPID = PidControllerInit(1, 0.04, 0.0, armEnc, 0); // Kp, Ki, Kd
  vexSensorValueSet(armEnc, 0);

  while (!chThdShouldTerminate()) {
    armSystemLift();
    armSystemIntake();

    vexSleep(25);
  }

  return (task)0;
}

task pneumaticsTask(void *arg) {
  (void)arg;
  vexTaskRegister("pneumatics");

  while (!chThdShouldTerminate()) {
    if (vexControllerGet(Btn7U) || vexControllerGet(Btn7UXmtr2)) {
      vexSensorValueSet(solenoid_1, 1);
      vexSensorValueSet(solenoid_2, 1);
    } else if (vexControllerGet(Btn7D) || vexControllerGet(Btn7DXmtr2)) {
      vexSensorValueSet(solenoid_1, 0);
      vexSensorValueSet(solenoid_2, 0);
    }

    vexSleep(25);
  }

  return (task)0;
}

task apolloTask(void *arg) {
  (void)arg;
  vexTaskRegister("apollo");

  apolloInit();

  while (1) { // Always update apollo output.
    apolloUpdate();
    vexSleep(150); // Slow updates.
  }

  return (task)0;
}