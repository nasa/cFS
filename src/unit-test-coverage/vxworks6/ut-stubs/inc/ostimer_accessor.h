/*
 * File: ostimer_accessor.h
 *
 * Purpose:
 *   Provide private accessors for getting/manipulating ostimer.c's static data.
 *
 * History:
 *   06/30/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSTIMER_ACCESSOR_H_
#define _OSTIMER_ACCESSOR_H_

/*
 * File: ostimer_accessor.h
 *
 * Purpose:
 *   Provide accesssor functions for unit testing ostimer.c
 *
 * History:
 *   06/30/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#include "vxworks6-coverage-stubs.h"

/* --------------- Prototypes for non-exported functions -------------------- */
void OS_UsecToTimespec(uint32 usecs, struct VCS_timespec *time_spec);
void OS_TimespecToUsec(struct VCS_timespec time_spec, uint32 *usecs);

/* -------------- Delegation function for static functions ------------------ */
void Pvt_OS_TimerSignalHandler(int host_timer_id);

/* --------------------- Custom Accessors ----------------------------------- */
/* Note, the types that support these prototypes
 * are in ostimer_copiedstatictypes.h */

/* Setters & getters for pointers to the OS_timer_table and 
 * os_clock_accuracy. */
#ifdef JPHFIX
OS_timer_record_t* getOSTimeTablePtr(void);
void setOSTimeTablePtr(OS_timer_record_t* ptr);
#endif

uint32* getOSClockAccuracyPtr(void);
uint32 getOSClockAccuracy(void);
void setOSClockAccuracyPtr(uint32* ptr);
void setOSClockAccuracy(uint32 accuracy);

uint32 getMaxSecsInUSec(void);

/* An initialization function to set the pointers before any
* ostimer.c calls occur. */
void initStaticPointers(void);

#endif /* _OSTIMER_ACCESSOR_H_ */

