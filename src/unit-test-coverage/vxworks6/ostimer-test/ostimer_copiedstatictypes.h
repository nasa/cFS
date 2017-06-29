/*
 * File: ostimer_copiedstatictypes.h
 *
 * Purpose:
 *   Provides a copy of the types from ostimer.c to support
 *   use of private accessors.
 *
 * History:
 *   06/30/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSTIMER_COPIEDSTATICTYPES_H_
#define _OSTIMER_COPIEDSTATICTYPES_H_

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

#include "uttools.h"
#include "vxworks6-coverage-stubs.h"
#include "common_types.h"
#include "osapi.h"

/* This type is copied directly from ostimer.c
 * so that the tests can access them.  We put
 * it here in a separate header so we don't get
 * multiply-defined types in the same compilation
 * unit as the ostimer.c. */
typedef struct
{
   uint32              free;
   char                name[OS_MAX_API_NAME];
   uint32              creator;
   uint32              start_time;
   uint32              interval_time;
   uint32              accuracy;
   OS_TimerCallback_t  callback_ptr;
   uint32              host_timerid;

} OS_timer_record_t;

#endif /* _OSTIMER_COPIEDSTATICTYPES_H_ */

