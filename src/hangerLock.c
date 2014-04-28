// hangerLock.c

#include "pidbot.h"

int hangerLockPressed = 0;

void hangerLockInit(void) {
  vexDigitalPinSet(hangerLock, kVexDigitalLow);
}

void hangerLockRun(void) {
  if(!hangerLockPressed && (vexControllerGet(BTN_HANGER_LOCK_TOGGLE) || vexControllerGet(Btn7UXmtr2)))
    vexDigitalPinSet(hangerLock, 1 - vexDigitalPinGet(hangerLock));
  hangerLockPressed = vexControllerGet(BTN_HANGER_LOCK_TOGGLE) || vexControllerGet(Btn7UXmtr2);
}

task hangerLockTask(void *arg) {
  (void)arg;
  vexTaskRegister("hanger lock");

  while (!chThdShouldTerminate()) {
    hangerLockRun();
    vexSleep(25);
  }

  return (task)0;
}