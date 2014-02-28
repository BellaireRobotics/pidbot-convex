// drive.c

#include "pidbot.h"

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
  // get joystick values
  short y = vexControllerGet(Ch3); // y vector
  short x = vexControllerGet(Ch1); // x vector

  // accelerometer as joystick! FUN! :)
  //short y = vexControllerGet(AcclY);
  // short x = vexControllerGet(AcclX);

  if (abs(y) < JOYSTICK_DEADZONE) {
    y = 0;
  }

  if (abs(x) < JOYSTICK_DEADZONE) {
    x = 0;
  }

  driveSystemSetVector(y, x);
}

void driveSystemSetL(short l) {
  vexMotorSet(leftFront, l);
  vexMotorSet(leftBack, l);
}

void driveSystemSetR(short r) {
  vexMotorSet(rightFront, r);
  vexMotorSet(rightBack, r);
}

void driveSystemSet(short l, short r) {
  driveSystemSetL(l);
  driveSystemSetR(r);
}

// compute motor speeds with a directional vector
void driveSystemSetVector(short y, short x) {
  driveSystemSetL(sgn(y + x) * TrueSpeed[abs(y + x)]);
  driveSystemSetR(sgn(y - x) * TrueSpeed[abs(y - x)]);
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
