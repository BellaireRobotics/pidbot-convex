// arm.c

#include "pidbot.h"

// Uncomment to enable manual lift override.
// #define LIFT_OVERRIDE

pidController *armPID;

void armSystemLiftSet(short s) {
  vexMotorSet(leftTopLift, s);
  vexMotorSet(leftBottomLift, s);
  vexMotorSet(rightTopLift, s);
  vexMotorSet(rightBottomLift, s);
}

void armSystemLift(void) {
  if (vexControllerGet(Btn8U) || vexControllerGet(Btn8UXmtr2)) { // stash waypoint
    armPID->target_value = LIFT_STASH_HEIGHT;
  } else if (vexControllerGet(Btn8L) || vexControllerGet(Btn8LXmtr2)) { // bump waypoint
    armPID->target_value = LIFT_BUMP_HEIGHT;
  } else if (vexControllerGet(Btn8D) || vexControllerGet(Btn8DXmtr2)) { // floor waypoint
    armPID->target_value = LIFT_FLOOR_HEIGHT;
  } else if (vexControllerGet(Btn8R) || vexControllerGet(Btn8RXmtr2)) { // hang waypoint
    armPID->target_value = LIFT_HANG_HEIGHT;
  } else if (vexControllerGet(Btn6U) || vexControllerGet(Btn6UXmtr2)) { // move arm up
#ifdef LIFT_OVERRIDE
    if (vexSensorValueGet(armEnc) < LIFT_MAX_HEIGHT) { // arm override up
      armSystemLiftSet(LIFT_UP * SMAX);
      vexSleep(2);
    }
#else
      armPID->target_value += 20;
#endif
  } else if (vexControllerGet(Btn6D) || vexControllerGet(Btn6DXmtr2)) { // move arm down
#ifdef LIFT_OVERRIDE
    if (vexSensorValueGet(armEnc) > LIFT_MINIMUM_HEIGHT) { // arm override down
      armSystemLiftSet(LIFT_DOWN * SMAX);
      vexSleep(2);
    }
#else
    armPID->target_value -= 25;
#endif
  } else {
#ifdef LIFT_OVERRIDE
    armSystemLiftSet(0); // important... don't wanna accidently chew up gears, yeah...? lol
    armPID->target_value = vexSensorValueGet(armEnc);
#else
    // armPID->target_value = vexSensorValueGet(armEnc); // set target to current when no action
#endif
  }

  // clip
  if (armPID->target_value < LIFT_MINIMUM_HEIGHT) {
    armPID->target_value = LIFT_MINIMUM_HEIGHT;
  }
  if (armPID->target_value > LIFT_MAX_HEIGHT) {
    armPID->target_value = LIFT_MAX_HEIGHT;
  }

  PidControllerUpdate(armPID); // aim...

  // Kill if power is lost
  // if (vexSpiGetMainBattery() < 3000) { // Experiment with value. Don't want it dead when needed.
  //   armPID->drive_cmd = 0;
  // }

  armSystemLiftSet(armPID->drive_cmd); // ...and FIRE!!!
}


task armTask(void *arg) {
  (void)arg;
  vexTaskRegister("arm");

  armPID = PidControllerInit(0.01, 0, 0, armEnc, 1); // Kp, Ki, Kd. Needs tweaking.
  vexSensorValueSet(armEnc, 0);

  while (!chThdShouldTerminate()) {
    armSystemLift();
    vexSleep(25);
  }

  return (task)0;
}
