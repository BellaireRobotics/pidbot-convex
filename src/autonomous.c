// autonomous.c

#include "pidbot.h"

void autonomous(void) {
  // jiggle intake and move forward
  intakeSet(INTAKE_IN * SMAX);
  driveSetVector(127, 0);
  vexSleep(300);
  driveSetVector(-127, 0);
  vexSleep(200);
  driveSetVector(0, 0);

  // move forward
  driveSetVector(127, 0);
  vexSleep(900);

  // turn back and left or back and right
  intakeSet(0);
  driveSetVector(0, 0);
  vexSleep(150);

  // TURNING LINE, BLUE IS RIGHT, RED IS LEFT.
  driveSetVector(0, 100);

  vexSleep(290);
  driveSetVector(0, 0);

  // lift all the way and hanger deploy
  // do you even lift?
  vexDigitalPinSet(hangerLock, kVexDigitalHigh);
  armPID->target_value = LIFT_HANG_HEIGHT;
  vexSleep(1500);

  // move all the way back
  driveSetVector(-70, 0);
  vexSleep(1600);
  driveSetVector(0, 0);

  // set PID floor
  armPID->target_value = LIFT_FLOOR_HEIGHT;
  vexSleep(1000);
  armSet(127);
  vexSleep(3000);

  // lock pneumatics
  vexDigitalPinSet(armLock, kVexDigitalHigh);
  armSet(0);
}