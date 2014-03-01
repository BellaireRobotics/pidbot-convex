// autonomous.c

#include "pidbot.h"

void autonomous(void) {
  // jiggle intake and move forward
  driveSystemSetVector(SMAX, 0);
  vexSleep(300);
  driveSystemSetVector(SMIN, 0);
  vexSleep(200);
  driveSystemSetVector(0, 0);

  // intake spin
  intakeSystemIntakeSet(INTAKE_IN * SMAX);

  // move forward
  driveSystemSetVector(SMAX, 0);
  vexSleep(600);

  // turn back and left or back and right
  intakeSystemIntakeSet(0);
  driveSystemSetVector(0, 0);
  vexSleep(150);
  driveSystemSetVector(0, -100); // TURNING LINE, BLUE IS RIGHT, RED IS LEFT.
  vexSleep(240);
  driveSystemSetVector(0, 0);

  // lift all the way
  armPID->target_value = LIFT_HANG_HEIGHT;
  vexSleep(1500);

  // move all the way back
  driveSystemSetVector(-70, 0);
  vexSleep(2100);
  driveSystemSetVector(0, 0);

  // set PID floor
  armPID->target_value = LIFT_FLOOR_HEIGHT;
  vexSleep(1000);
  armSystemLiftSet(LIFT_UP * SMAX);
  vexSleep(3000);

  // lock pneumatics
  pneumaticsSystemSet(kVexDigitalHigh);
  armSystemLiftSet(0);
}