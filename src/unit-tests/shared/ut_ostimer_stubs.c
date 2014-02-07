/*================================================================================*
** File:  ut_ostimer_stubs.c
** Owner: Tam Ngo
** Date:  March 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_stubs.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/* As defined in ostimer.c */
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

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

UT_OsReturnCode_t  g_timerInit = {0,0};
UT_OsReturnCode_t  g_timerCreate = {0,0};
UT_OsReturnCode_t  g_timerDelete = {0,0};
UT_OsReturnCode_t  g_timerSet = {0,0};
UT_OsReturnCode_t  g_timerGetId = {0,0};
UT_OsReturnCode_t  g_timerGetInfo = {0,0};

/* As defined in ostimer.c */
OS_timer_record_t  OS_timer_table[OS_MAX_TIMERS];
uint32             os_clock_accuracy;

#ifdef _LINUX_OS_
pthread_mutex_t    OS_timer_table_mut;
#endif

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/

int32 OS_TimerAPIInit(void)
{
    return (g_timerInit.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerCreate(uint32* timer_id, const char* timer_name,
                     uint32* clock_accuracy, OS_TimerCallback_t callback_ptr)
{
    return (g_timerCreate.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerDelete(uint32 timer_id)
{
    return (g_timerDelete.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerSet(uint32 timer_id, uint32 start_msec, uint32 interval_msec)
{
    return (g_timerSet.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerGetIdByName(uint32* timer_id, const char* timer_name)
{
    return (g_timerGetId.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerGetInfo(uint32 timer_id, OS_timer_prop_t* timer_prop)
{
    return (g_timerGetInfo.value);
}

/*================================================================================*
** End of File: ut_ostimer_stubs.c
**================================================================================*/
