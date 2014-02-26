// vexuser.c

#include <stdlib.h>

#include "ch.h"  // needs for all ChibiOS programs
#include "hal.h" // hardware abstraction layer header
#include "vex.h" // vex library header

#include "pidbot.h" // bot header
#include "robotc_glue.h" // for ROBOTC-like tasks
#include "pidlib.h"

// Digital I/O configuration
static vexDigiCfg digitalConfig[kVexDigital_Num] = {
  { solenoid_1,     kVexSensorDigitalOutput, kVexConfigOutput, 0 },
  { solenoid_2,     kVexSensorDigitalOutput, kVexConfigOutput, 0 },
  { kVexDigital_3,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_4,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_5,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_6,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_7,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_8,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_9,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_10, kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_11, kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_12, kVexSensorDigitalInput,  kVexConfigInput,  0 }
};

// Motor configuration
// Note: 4 amps per motor group (1-5, 6-10).
static vexMotorCfg motorConfig[kVexMotorNum] = {
  { rightIntake,     kVexMotor393S, kVexMotorNormal,   kVexSensorNone, 0 },
  { rightFront,      kVexMotor393S, kVexMotorNormal,   kVexSensorNone, 0 },
  { rightBack,       kVexMotor393S, kVexMotorReversed, kVexSensorNone, 0 },
  { rightTopLift,    kVexMotor393T, kVexMotorNormal,   kVexSensorNone, 0 },
  { rightBottomLift, kVexMotor393T, kVexMotorNormal,   kVexSensorNone, 0 },
  { leftTopLift,     kVexMotor393T, kVexMotorNormal,   kVexSensorIME,  armEnc },
  { leftBottomLift,  kVexMotor393T, kVexMotorNormal,   kVexSensorNone, 0 },
  { leftFront,       kVexMotor393S, kVexMotorNormal,   kVexSensorNone, 0 },
  { leftBack,        kVexMotor393S, kVexMotorReversed, kVexSensorNone, 0 },
  { leftIntake,      kVexMotor393S, kVexMotorNormal,   kVexSensorNone, 0 }
};

void vexUserSetup() {
  vexDigitalConfigure(digitalConfig, DIG_CONFIG_SIZE(digitalConfig));
  vexMotorConfigure(motorConfig, MOT_CONFIG_SIZE(motorConfig));
}

void vexUserInit() {
  // ...
}

task vexAutonomous(void *arg) {
  (void)arg;

  vexTaskRegister("auton");

  while (!chThdShouldTerminate()) {
    vexSleep(25);
  }

  return (task)0;
}

task vexOperator(void *arg) {
  (void)arg;

  vexTaskRegister("operator");

  StartTask(driveTask);
  //StartTask(armTask);
  //StartTask(pneumaticsTask);
  StartTaskWithPriority(apolloTask, LOWPRIO);

  while (!chThdShouldTerminate()) {
    vexSleep(25);
  }

  return (task)0;
}