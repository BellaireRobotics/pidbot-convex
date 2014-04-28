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
// Port Constants

#define armLock kVexDigital_1
#define hangerLock kVexDigital_2
#define sonarInput kVexDigital_11
#define sonarOutput kVexDigital_12

#define autonSelect kVexAnalog_1

#define armEnc kVexSensorIme_1

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
// Controller Mappings

#define BTN_LIFT_UP Btn6U
#define BTN_LIFT_DOWN Btn6D
#define BTN_LIFT_FLOOR Btn8D
#define BTN_LIFT_BUMP Btn8L
#define BTN_LIFT_STASH Btn8R
#define BTN_AUTO_HANG Btn8U

#define BTN_INTAKE_IN Btn5U
#define BTN_INTAKE_OUT Btn5D

#define BTN_ARM_LOCK_TOGGLE Btn7D
#define BTN_HANGER_LOCK_TOGGLE Btn7U

#define CH_Y Ch3
#define CH_X Ch1

// ---------------------------------
// Constants

#define JOYSTICK_DEADZONE 15
#define SMAX 127
#define SMIN (-127)

#define LIFT_UP 1
#define LIFT_DOWN (-1)
#define LIFT_MINIMUM_HEIGHT 0
#define LIFT_FLOOR_HEIGHT 0
#define LIFT_BUMP_HEIGHT 300
#define LIFT_STASH_HEIGHT 1230
#define LIFT_HANG_HEIGHT 1230
#define LIFT_MAX_HEIGHT 1230

#define INTAKE_IN 1
#define INTAKE_OUT (-1)

#define LOW 0
#define HIGH 1

// sign function. returns -1 or 1.
#define sgn(x) ((x > 0) - (x < 0))

// ----------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern pidController *armPID;

void driveRun(void);
void driveSetLeft(short l);
void driveSetRight(short r);
void driveSet(short l, short r);
void driveSetVector(short y, short x);
task driveTask(void *arg);

void armSet(short s);
void armInit(void);
void armRun(void);
task armTask(void *arg);

void intakeSet(short s);
void intakeRun(void);
task intakeTask(void *arg);

void armLockInit(void);
void armLockRun(void);
task armLockTask(void *arg);

void hangerLockInit(void);
void hangerLockRun(void);
task hangerLockTask(void *arg);

task apolloTask(void *arg);
task safetyTask(void *arg);

void autonomous(void);

#ifdef __cplusplus
}
#endif

#endif // _PIDBOT_H_
