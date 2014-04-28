// pidbot.c

#include "pidbot.h"

// Digital I/O configuration
static vexDigiCfg digitalConfig[kVexDigital_Num] = {
  { armLock,        kVexSensorDigitalOutput, kVexConfigOutput, 0 },
  { hangerLock,     kVexSensorDigitalOutput, kVexConfigOutput, 0 },
  { kVexDigital_3,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_4,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_5,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_6,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_7,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_8,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_9,  kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { kVexDigital_10, kVexSensorDigitalInput,  kVexConfigInput,  0 },
  { sonarInput,     kVexConfigSonarIn,       kVexSensorSonarCm, 0 },
  { sonarOutput,    kVexConfigSonarOut,      kVexSensorSonarCm, 0 }
};

// Motor configuration
// Note: 4 amps per motor group (1-5, 6-10).
static vexMotorCfg motorConfig[kVexMotorNum] = {
  { rightIntake,     kVexMotor393S, kVexMotorReversed, kVexSensorNone, 0 },
  { rightFront,      kVexMotor393S, kVexMotorNormal,   kVexSensorNone, 0 },
  { rightBack,       kVexMotor393S, kVexMotorReversed, kVexSensorNone, 0 },
  { rightTopLift,    kVexMotor393T, kVexMotorNormal,   kVexSensorNone, 0 },
  { rightBottomLift, kVexMotor393T, kVexMotorNormal,   kVexSensorNone, 0 },
  { leftTopLift,     kVexMotor393T, kVexMotorReversed, kVexSensorIME,  kImeChannel_1 },
  { leftBottomLift,  kVexMotor393T, kVexMotorReversed, kVexSensorNone, 0 },
  { leftFront,       kVexMotor393S, kVexMotorReversed, kVexSensorNone, 0 },
  { leftBack,        kVexMotor393S, kVexMotorNormal,   kVexSensorNone, 0 },
  { leftIntake,      kVexMotor393S, kVexMotorReversed, kVexSensorNone, 0 }
};

void vexUserSetup(void) {
  vexDigitalConfigure(digitalConfig, DIG_CONFIG_SIZE(digitalConfig));
  vexMotorConfigure(motorConfig, MOT_CONFIG_SIZE(motorConfig));
}

// Pre-autonomous
void vexUserInit(void) {
  // ...
}

task vexOperator(void *arg) {
  (void)arg;
  vexTaskRegister("operator");

  StartTask(driveTask); // drive
  StartTask(armTask); // arm
  StartTask(intakeTask); // intake
  StartTask(armLockTask);
  StartTask(hangerLockTask);

  while (!chThdShouldTerminate()) { // sleep forever
    vexSleep(25);
  }

  return (task)0;
}

task vexAutonomous(void *arg) {
  (void)arg;
  vexTaskRegister("auton");

  StartTask(armTask);
  autonomous();

  while (!chThdShouldTerminate()) {
    vexSleep(25);
  }

  return (task)0;
}
