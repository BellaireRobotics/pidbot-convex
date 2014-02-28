// pidbot.h

#ifndef _PIDBOT_H_
#define _PIDBOT_H_

#include <stdlib.h>

#include "ch.h"  // needs for all ChibiOS programs
#include "hal.h" // hardware abstraction layer header
#include "vex.h" // vex library header

#include "apollo.h"
#include "smartmotor.h"
#include "robotc_glue.h" // for ROBOTC-like tasks
#include "pidlib.h"

// ---------------------------------
#define solenoid_1 kVexDigital_1
#define solenoid_2 kVexDigital_2

// #define autonomousSelect kVexAnalog_2

#define armEnc kImeChannel_1

#define rightIntake kVexMotor_1
#define leftIntake kVexMotor_10
#define rightFront kVexMotor_2
#define rightBack kVexMotor_3
#define leftFront kVexMotor_8
#define leftBack kVexMotor_9
#define rightTopLift kVexMotor_4
#define rightBottomLift kVexMotor_5
#define leftTopLift kVexMotor_6
#define leftBottomLift kVexMotor_7
// ---------------------------------

#define JOYSTICK_DEADZONE 15
#define SMAX 127
#define SMIN (-127)

#define LIFT_MINIMUM_HEIGHT 0
#define LIFT_FLOOR_HEIGHT   0
#define LIFT_BUMP_HEIGHT    150
#define LIFT_STASH_HEIGHT   1100
#define LIFT_HANG_HEIGHT    1280
#define LIFT_MAX_HEIGHT     1280

#define sgn(x) ((x > 0) - (x < 0))

// ----------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern pidController *armPID;

void driveSystemArcadeDrive(void);
void driveSystemSetL(short l);
void driveSystemSetR(short r);
void driveSystemSet(short l, short r);
void driveSystemSetVector(short y, short x);
task driveTask(void *arg);

void armSystemLiftSet(short s);
void armSystemLift(void);
task armTask(void *arg);

void intakeSystemIntakeSet(short s);
void intakeSystemIntake(void);
task intakeTask(void *arg);

void pneumaticsSystemSet(int b);
task pneumaticsTask(void *arg);

task apolloTask(void *arg); // // real time display of motor and sensor data
task safetyTask(void *arg);

void autonomous(void);

#ifdef __cplusplus
}
#endif

#endif // _PIDBOT_H_
