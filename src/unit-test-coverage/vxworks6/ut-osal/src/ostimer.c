/* pull in the OSAL configuration */
#include "osconfig.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "ostimer_accessor.h"
#include "stub-map-to-real.h"
#include "os/vxworks6/ostimer.c"


/* Pointers to the static members of ostimer.c so we can inspect or alter them. */
OS_timer_internal_record_t *OS_timer_table_ptr; /* Points to the [0] OS_timer_table element */
uint32            *os_clock_accuracy_ptr;

OS_timer_internal_record_t* getOSTimeTablePtr() {
  return (OS_timer_table_ptr);
}

void setOSTimeTablePtr(OS_timer_internal_record_t* ptr) {
  OS_timer_table_ptr = ptr;
}

uint32* getOSClockAccuracyPtr() {
  return os_clock_accuracy_ptr;
}

uint32 getOSClockAccuracy() {
  return *os_clock_accuracy_ptr;
}

void setOSClockAccuracyPtr(uint32* ptr) {
  os_clock_accuracy_ptr = ptr;
}

void setOSClockAccuracy(uint32 accuracy) {
    *os_clock_accuracy_ptr = accuracy;
}

uint32 getMaxSecsInUSec(void) {
  return (uint32)MAX_SECS_IN_USEC;
}

void initStaticPointers(void) {
  setOSTimeTablePtr(&(OS_timer_table[0]));
  setOSClockAccuracyPtr(&os_clock_accuracy);
}

void Pvt_OS_TimerSignalHandler(int host_timer_id) {
  OS_TimerSignalHandler(host_timer_id);
}
