// pidbot.h

#include "robotc_glue.h"

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

#ifdef __cplusplus
extern "C" {
#endif

void driveSystemArcadeDrive(void);
task driveTask(void *arg);

void armSystemLift(void);
void armSystemIntakeSet(short s);
void armSystemIntake(void);
task armTask(void *arg);

task pneumaticsTask(void *arg);

// real time display of motor and sensor data
task apolloTask(void *arg);

#ifdef __cplusplus
}
#endif
