/*
 * File: ostimer_stubs.h
 *
 * Purpose:
 *   Provide stubs for unit testing ostimer.c
 *
 * History:
 *   06/29/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSTIMER_STUBS_H_
#define _OSTIMER_STUBS_H_

#include "uttools.h"
#include "vxworks6-coverage-stubs.h"
#include "time.h"
#include "osconfig.h"

/* Define missing types */
typedef struct
{
    int32 Dir;
} __dirstream;

typedef enum
{
    OSTIMER_SEMMCREATE_INDEX,
    OSTIMER_SEMTAKE_INDEX,
    OSTIMER_SEMGIVE_INDEX,
    OSTIMER_CLOCKGETRES_INDEX,
    OSTIMER_OS_FINDCREATOR_INDEX,
    OSTIMER_TIMER_CREATE_INDEX,
    OSTIMER_TIMER_DELETE_INDEX,
    OSTIMER_TIMER_CONNECT_INDEX,
    OSTIMER_TIMER_SETTIME_INDEX,
    OSTIMER_MAX_INDEX
} OsTimer_Index_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} OsTimer_ReturnCodeTable_t;

typedef struct
{
  VCS_SEM_ID  (*semMCreate)(uint16 *);
  VCS_STATUS  (*semTake)(VCS_SEM_ID, int);
  VCS_STATUS  (*semGive)(VCS_SEM_ID);
  int     (*clock_getres)(int clocktype, struct VCS_timespec *clock_resolution);
  uint32  (*OS_FindCreator)(void);
  int     (*timer_create)(VCS_clockid_t, struct VCS_sigevent *, VCS_timer_t *);
  int     (*timer_delete)(VCS_timer_t);
  int     (*timer_connect) (VCS_timer_t, void (*OS_TimerSignalHandler)(int host_timer_id), uint32 possible_id);
  int     (*timer_settime) (VCS_timer_t, int , const struct VCS_itimerspec *, struct VCS_itimerspec *);
} OsTimer_HookTable_t;

extern const char testFileName[];

void OsTimer_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void OsTimer_SetFunctionHook(uint32 Index, void *FunPtr);
void OsTimer_Reset(void);
boolean OsTimer_UseReturnCode(uint32 Index);

int32 getNSemTake(void);
int32 getNSemGive(void);

#if 0
/* The timer_connect protoype is iffy.  The commented-out prototype is more type-correct for the use
 * in ostimer.c.  However, the callback pointer isn't the same as the VOIDFCNPTR as defined in VxWorks.
 * The difference is void (*)() vs. void (*) (int).  In Linux with gcc 4.4.x we get a warning due to 
 * ostimer.c usage, but the test operates as desired.  In VxWorks, this function signature doesn't
 * match well enough to compile.
int timer_connect (timer_t t, void (*OS_TimerSignalHandler)(int host_timer_id), uint32 possible_id);
 * The VxWorks version:
 */
extern int timer_connect (timer_t timerid, VOIDFUNCPTR routine, int arg);
#endif

void FakeTimerCallback(uint32 timer_id);
void ResetFakeTimerCallback(void);
uint32 GetFakeTimerCallbackCount(void);

#endif /* _OSTIMER_STUBS_H_ */
