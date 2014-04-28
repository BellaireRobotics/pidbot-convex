// arm.c

#include "pidbot.h"

// Uncomment to enable manual lift override.
#define LIFT_OVERRIDE

pidController *armPID;

void armSet(short s) {
  vexMotorSet(leftTopLift, s);
  vexMotorSet(leftBottomLift, s);
  vexMotorSet(rightTopLift, s);
  vexMotorSet(rightBottomLift, s);
}

void armRun(void) {
  // if current encoder count is negative, then set that to zero
  // safety net for if bot is init'd w/o lift all the way down
  // alternative to limit switch
  //if (vexMotorPositionGet(leftTopLift) < -30)
  //  vexSensorValueSet(armEnc, 0);

  if (vexControllerGet(BTN_LIFT_STASH) || vexControllerGet(Btn8RXmtr2)) { // stash waypoint
    armPID->target_value = LIFT_STASH_HEIGHT;
  } else if (vexControllerGet(BTN_LIFT_BUMP) || vexControllerGet(Btn8LXmtr2)) { // bump waypoint
    armPID->target_value = LIFT_BUMP_HEIGHT;
  } else if (vexControllerGet(BTN_LIFT_FLOOR) || vexControllerGet(Btn8DXmtr2)) { // floor waypoint
    armPID->target_value = LIFT_FLOOR_HEIGHT;
  } else if (vexControllerGet(BTN_AUTO_HANG) || vexControllerGet(Btn8UXmtr2)) { // hang waypoint
    /*
    armPID->target_value = LIFT_HANG_HEIGHT;
    if (((LIFT_HANG_HEIGHT - vexMotorPositionGet(leftTopLift))) < 20) {

    }
    */
  } else if (vexControllerGet(BTN_LIFT_UP) || vexControllerGet(Btn6UXmtr2)) { // move arm up
#ifdef LIFT_OVERRIDE
    armPID->enabled = 0;
    armSet(LIFT_UP * SMAX);
    armPID->target_value = vexMotorPositionGet(leftTopLift);
#else
    armPID->target_value += 15;
#endif
  } else if (vexControllerGet(BTN_LIFT_DOWN) || vexControllerGet(Btn6DXmtr2)) { // move arm down
#ifdef LIFT_OVERRIDE
    armPID->enabled = 0;
    armSet(LIFT_DOWN * SMAX);
    armPID->target_value = vexMotorPositionGet(leftTopLift);
#else
    armPID->target_value -= 25;
#endif
  }
#ifdef LIFT_OVERRIDE
  else {
    armPID->enabled = 1;
  }
#endif

  // clip
  // if (armPID->target_value < LIFT_MINIMUM_HEIGHT) {
  //   armPID->target_value = LIFT_MINIMUM_HEIGHT;
  // }
  //if (armPID->target_value > LIFT_MAX_HEIGHT) {
  //  armPID->target_value = LIFT_MAX_HEIGHT;
  //}

  // Kill if power is lost
  // if (vexSpiGetMainBattery() < 3000) { // Experiment with value. Don't want it dead when needed.
  //   armPID->drive_cmd = 0;
  // }

  if (armPID->enabled) {
    armSet(PidControllerUpdate(armPID)); // ...and FIRE!!!
  }
}

void armInit(void) {
  armPID = PidControllerInit(0.01, 0, 0, armEnc, 1); // Kp, Ki, Kd. Needs tweaking.
  vexSensorValueSet(armEnc, 0);
  armPID->target_value = vexMotorPositionGet(leftTopLift);
}

task armTask(void *arg) {
  (void)arg;
  vexTaskRegister("arm");

  armInit();

  while (!chThdShouldTerminate()) {

    if (vexDigitalPinGet(armLock) == kVexDigitalLow)
      armRun();
    else
      armSet(0);

    vexSleep(25);
  }

  return (task)0;
}
