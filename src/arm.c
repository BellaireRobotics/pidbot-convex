// arm.c

#include "pidbot.h"

pidController *armPID;

void armSystemLiftSet(short s) {
  vexMotorSet(leftTopLift, s);
  vexMotorSet(leftBottomLift, s);
  vexMotorSet(rightTopLift, s);
  vexMotorSet(rightBottomLift, s);
}

void armSystemLift(void) {
  // Lift will approach waypoints while waypoints buttons are pressed.
  // Once the button is released, it stops in place.
  if (vexControllerGet(Btn8UXmtr2) || vexControllerGet(Btn8U)) { // stash waypoint
    armPID->target_value = LIFT_STASH_HEIGHT;
  } else if (vexControllerGet(Btn8LXmtr2) || vexControllerGet(Btn8L)) { // bump waypoint
    armPID->target_value = LIFT_BUMP_HEIGHT;
  } else if (vexControllerGet(Btn8DXmtr2) || vexControllerGet(Btn8D)) { // floor waypoint
    armPID->target_value = LIFT_FLOOR_HEIGHT;
  } else if (vexControllerGet(Btn8RXmtr2) || vexControllerGet(Btn8R)) { // hang waypoint
    armPID->target_value = LIFT_HANG_HEIGHT;
  } else if (vexControllerGet(Btn6UXmtr2) || vexControllerGet(Btn6U)) { // move arm up
    armPID->target_value += 5;
    // if (vexSensorValueGet(armEnc) < LIFT_MAX_HEIGHT) { // arm override up
    //   armSystemLiftSet(LIFT_UP);
    //   //vexSleep(2);
    //   armPID->target_value = vexSensorValueGet(armEnc);
    // }
  } else if (vexControllerGet(Btn6DXmtr2) || vexControllerGet(Btn6D)) { // move arm down
    armPID->target_value -= 10;
    // if (vexSensorValueGet(armEnc) > LIFT_MINIMUM_HEIGHT) { // arm override down
    //   armSystemLiftSet(LIFT_DOWN);
    //   //vexSleep(2);
    //   armPID->target_value = vexSensorValueGet(armEnc);
    // }
  } // else {
  //   armPID->target_value = vexSensorValueGet(armEnc); // set target to current when no action
  //   // armSystemLiftSet(0); // important... don't wanna accidently chew up gears, yeah...? lol
  // }

  // clip lower
  if (armPID->target_value < LIFT_MINIMUM_HEIGHT) {
    armPID->target_value = LIFT_MINIMUM_HEIGHT;
  }

  // clip higher
  if (armPID->target_value > LIFT_MAX_HEIGHT) {
    armPID->target_value = LIFT_MAX_HEIGHT;
  }

  PidControllerUpdate(armPID); // aim...

  // Kill if power is lost
  if (vexSpiGetMainBattery() < 3000) { // Experiment with value. Don't want it dead when needed.
    armPID->drive_cmd = 0;
  }

  armSystemLiftSet(armPID->drive_cmd); // ...and FIRE!!!
}

task armTask(void *arg) {
  (void)arg;
  vexTaskRegister("arm");

  armPID = PidControllerInit(0.5, 0.04, 0.25, armEnc, 1); // Kp, Ki, Kd. Needs tweaking.
  vexSensorValueSet(armEnc, 0);

  while (!chThdShouldTerminate()) {
    armSystemLift();
    vexSleep(25);
  }

  return (task)0;
}
