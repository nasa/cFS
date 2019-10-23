/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   ostimer.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains the OSAL Timer API for RTEMS
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#define _USING_RTEMS_INCLUDES_

#include "os-rtems.h"

/****************************************************************************************
                                INTERNAL FUNCTION PROTOTYPES
 ***************************************************************************************/

void  OS_UsecsToTicks(uint32 usecs, rtems_interval *ticks);

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define OSAL_TABLE_MUTEX_ATTRIBS \
 (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | \
  RTEMS_INHERIT_PRIORITY | RTEMS_NO_PRIORITY_CEILING | RTEMS_LOCAL)

/*
 * Prefer to use the MONOTONIC clock if available, as it will not get distrupted by setting
 * the time like the REALTIME clock will.
 */
#ifndef OS_PREFERRED_CLOCK
#ifdef  _POSIX_MONOTONIC_CLOCK
#define OS_PREFERRED_CLOCK      CLOCK_MONOTONIC
#else
#define OS_PREFERRED_CLOCK      CLOCK_REALTIME
#endif
#endif



/****************************************************************************************
                                    LOCAL TYPEDEFS
 ***************************************************************************************/

typedef struct
{
    rtems_id            rtems_timer_id;
    rtems_id            tick_sem;
    rtems_id            handler_mutex;
    rtems_id            handler_task;
    uint8               simulate_flag;
    uint8               reset_flag;
    rtems_interval      interval_ticks;
    uint32              configured_start_time;
    uint32              configured_interval_time;
} OS_impl_timebase_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES];
                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseLock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_TimeBaseLock_Impl(uint32 local_id)
{
    rtems_semaphore_obtain(OS_impl_timebase_table[local_id].handler_mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
} /* end OS_TimeBaseLock_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseUnlock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_TimeBaseUnlock_Impl(uint32 local_id)
{
    rtems_semaphore_release(OS_impl_timebase_table[local_id].handler_mutex);
} /* end OS_TimeBaseUnlock_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBase_ISR
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           An ISR to service a timer tick interrupt, which in turn
 *           posts a semaphore so the user task can execute
 *
 *-----------------------------------------------------------------*/
static rtems_timer_service_routine OS_TimeBase_ISR(rtems_id rtems_timer_id, void *arg)
{
    OS_U32ValueWrapper_t user_data;
    uint32 local_id;
    OS_impl_timebase_internal_record_t *local;

    user_data.opaque_arg = arg;
    OS_ConvertToArrayIndex(user_data.value, &local_id);
    local = &OS_impl_timebase_table[local_id];
    if (OS_global_timebase_table[local_id].active_id == user_data.value)
    {
        /*
         * Reset the timer, but only if an interval was selected
         */
        if (local->interval_ticks > 0)
        {
            rtems_timer_fire_after(rtems_timer_id, local->interval_ticks,
                    OS_TimeBase_ISR, user_data.opaque_arg);
        }

        /*
         * RTEMS OS timers implemented with an ISR callback
         * this must be downgraded to an ordinary task context
         *
         * This is accomplished by just releasing a semaphore here.
         */
        rtems_semaphore_release(local->tick_sem);
    }

} /* end OS_TimeBase_ISR */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBase_WaitImpl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Pends on the semaphore for the next timer tick
 *
 *-----------------------------------------------------------------*/
static uint32 OS_TimeBase_WaitImpl(uint32 local_id)
{
    OS_impl_timebase_internal_record_t *local;
    uint32 tick_time;

    local = &OS_impl_timebase_table[local_id];

    /*
     * Pend for the tick arrival
     */
    rtems_semaphore_obtain(local->tick_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    /*
     * Determine how long this tick was.
     * Note that there are plenty of ways this become wrong if the timer
     * is reset right around the time a tick comes in.  However, it is
     * impossible to guarantee the behavior of a reset if the timer is running.
     * (This is not an expected use-case anyway; the timer should be set and forget)
     */
    if (local->reset_flag == 0)
    {
        tick_time = local->configured_interval_time;
    }
    else
    {
        tick_time = local->configured_start_time;
        local->reset_flag = 0;
    }


    return tick_time;
} /* end OS_TimeBase_WaitImpl */


/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/
                        
/*----------------------------------------------------------------
 *
 * Function: OS_Rtems_TimeBaseAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32  OS_Rtems_TimeBaseAPI_Impl_Init ( void )
{
   /*
   ** Store the clock accuracy for 1 tick.
   */
   rtems_interval ticks_per_sec = rtems_clock_get_ticks_per_second();

   if (ticks_per_sec <= 0)
   {
      return OS_ERROR;
   }

   /*
    * For the global ticks per second, use the value direct from RTEMS
    */
   OS_SharedGlobalVars.TicksPerSecond = (int32)ticks_per_sec;

   /*
    * Compute the clock accuracy in Nanoseconds (ns per tick)
    * This really should be an exact/whole number result; otherwise this
    * will round to the nearest nanosecond.
    */
   RTEMS_GlobalVars.ClockAccuracyNsec = (1000000000 + (OS_SharedGlobalVars.TicksPerSecond / 2)) /
         OS_SharedGlobalVars.TicksPerSecond;


   /*
    * Finally compute the Microseconds per tick that is used for OS_Tick2Micros() call
    * This must further round again to the nearest microsecond, so it is undesirable to use
    * this for time computations if the result is not exact.
    */
   OS_SharedGlobalVars.MicroSecPerTick = (RTEMS_GlobalVars.ClockAccuracyNsec + 500) / 1000;

   return(OS_SUCCESS);
} /* end OS_Rtems_TimeBaseAPI_Impl_Init */

/****************************************************************************************
                                INTERNAL FUNCTIONS
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_UsecsToTicks
 *
 *  Purpose:  Convert Microseconds to a number of ticks.
 *
 *-----------------------------------------------------------------*/
void  OS_UsecsToTicks(uint32 usecs, rtems_interval *ticks)
{
   uint32 result;

   /*
    * In order to compute without overflowing a 32 bit integer,
    * this is done in 2 parts -
    * the fractional seconds first then add any whole seconds.
    * the fractions are rounded UP so that this is guaranteed to produce
    * a nonzero number of ticks for a nonzero number of microseconds.
    */

   result = (1000 * (usecs % 1000000) + RTEMS_GlobalVars.ClockAccuracyNsec - 1) /
         RTEMS_GlobalVars.ClockAccuracyNsec;

   if (usecs >= 1000000)
   {
      result += (usecs / 1000000) * OS_SharedGlobalVars.TicksPerSecond;
   }

   *ticks = (rtems_interval)result;
} /* end OS_UsecsToTicks */



/****************************************************************************************
                                   Time Base API
 ***************************************************************************************/

/* The user may specify whether to use priority inheritance on mutexes via osconfig.h */
#define OSAL_TIMEBASE_MUTEX_ATTRIBS            RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseCreate_Impl(uint32 timer_id)
{
    int32  return_code;
    rtems_status_code rtems_sc;
    OS_impl_timebase_internal_record_t *local;
    OS_common_record_t *global;


    return_code = OS_SUCCESS;
    local = &OS_impl_timebase_table[timer_id];
    global = &OS_global_timebase_table[timer_id];

    /*
     * Set up the necessary OS constructs
     *
     * If an external sync function is used then there is nothing to do here -
     * we simply call that function and it should synchronize to the time source.
     *
     * If no external sync function is provided then this will set up an RTEMS
     * timer to locally simulate the timer tick using the CPU clock.
     */
    local->simulate_flag = (OS_timebase_table[timer_id].external_sync == NULL);
    if (local->simulate_flag)
    {
        OS_timebase_table[timer_id].external_sync = OS_TimeBase_WaitImpl;

        /*
         * The tick_sem is a simple semaphore posted by the ISR and taken by the
         * timebase helper task (created later).
         */
        rtems_sc = rtems_semaphore_create (global->active_id, 0, RTEMS_SIMPLE_BINARY_SEMAPHORE | RTEMS_PRIORITY, 0,
                                           &local->tick_sem);
        if ( rtems_sc != RTEMS_SUCCESSFUL )
        {
            OS_DEBUG("Error: Tick Sem could not be created: %d\n",(int)rtems_sc);
            return_code = OS_TIMER_ERR_INTERNAL;
        }

        /*
         * The handler_mutex is deals with access to the callback list for this timebase
         */
        rtems_sc = rtems_semaphore_create (global->active_id, 1, OSAL_TIMEBASE_MUTEX_ATTRIBS, 0,
                                           &local->handler_mutex);

        if ( rtems_sc != RTEMS_SUCCESSFUL )
        {
            OS_DEBUG("Error: Handler Mutex could not be created: %d\n",(int)rtems_sc);
            rtems_semaphore_delete (local->tick_sem);
            return_code = OS_TIMER_ERR_INTERNAL;
        }

        rtems_sc = rtems_timer_create(global->active_id, &local->rtems_timer_id);
        if ( rtems_sc != RTEMS_SUCCESSFUL )
        {
            OS_DEBUG("Error: Timer object could not be created: %d\n",(int)rtems_sc);
            rtems_semaphore_delete (local->handler_mutex);
            rtems_semaphore_delete (local->tick_sem);
            return_code = OS_TIMER_ERR_UNAVAILABLE;
        }
    }

    /*
     * Spawn a dedicated time base handler thread
     *
     * This alleviates the need to handle expiration in the context of a signal handler -
     * The handler thread can call a BSP synchronized delay implementation as well as the
     * application callback function.  It should run with elevated priority to reduce latency.
     *
     * Note the thread will not actually start running until this function exits and releases
     * the global table lock.
     */
    if (return_code == OS_SUCCESS)
    {
        /* note on the priority - rtems is inverse (like vxworks) so that the lowest numeric
         * value will preempt other threads in the ready state.
         * Using "RTEMS_MINIMUM_PRIORITY + 1" because rtems seems to not schedule it at all if
         * the priority is set to RTEMS_MINIMUM_PRIORITY.
         */
        rtems_sc = rtems_task_create(
                     global->active_id,
                     RTEMS_MINIMUM_PRIORITY + 1,
                     0,
                     RTEMS_PREEMPT | RTEMS_NO_ASR | RTEMS_NO_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0),
                     RTEMS_LOCAL,
                     &local->handler_task);

        /* check if task_create failed */
        if (rtems_sc != RTEMS_SUCCESSFUL )
        {
            /* Provide some freedback as to why this failed */
            OS_printf("rtems_task_create failed: %s\n", rtems_status_text(rtems_sc));
            return_code = OS_TIMER_ERR_INTERNAL;
        }
        else
        {
            /* will place the task in 'ready for scheduling' state */
            rtems_sc = rtems_task_start (local->handler_task, /*rtems task id*/
                         (rtems_task_entry) OS_TimeBase_CallbackThread, /* task entry point */
                         (rtems_task_argument) global->active_id );  /* passed argument  */

            if (rtems_sc != RTEMS_SUCCESSFUL )
            {
                OS_printf("rtems_task_start failed: %s\n", rtems_status_text(rtems_sc));
                rtems_task_delete(local->handler_task);
                return_code = OS_TIMER_ERR_INTERNAL;
            }
        }

        if (return_code != OS_SUCCESS)
        {
            /* Also delete the resources we allocated earlier */
            rtems_timer_delete(local->rtems_timer_id);
            rtems_semaphore_delete (local->handler_mutex);
            rtems_semaphore_delete (local->tick_sem);
            return return_code;
        }
    }

    return return_code;
} /* end OS_TimeBaseCreate_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseSet_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseSet_Impl(uint32 timer_id, int32 start_time, int32 interval_time)
{
    OS_U32ValueWrapper_t user_data;
    OS_impl_timebase_internal_record_t *local;
    int32 return_code;
    int status;
    rtems_interval start_ticks;

    local = &OS_impl_timebase_table[timer_id];
    return_code = OS_SUCCESS;

    /* There is only something to do here if we are generating a simulated tick */
    if (local->simulate_flag)
    {
        /*
        ** Note that UsecsToTicks() already protects against intervals
        ** less than os_clock_accuracy -- no need for extra checks which
        ** would actually possibly make it less accurate.
        **
        ** Still want to preserve zero, since that has a special meaning.
        */

        if (start_time <= 0)
        {
            interval_time = 0;  /* cannot have interval without start */
        }

        if (interval_time <= 0)
        {
            local->interval_ticks = 0;
        }
        else
        {
            OS_UsecsToTicks(interval_time, &local->interval_ticks);
        }

        /*
        ** The defined behavior is to not arm the timer if the start time is zero
        ** If the interval time is zero, then the timer will not be re-armed.
        */
        if ( start_time > 0 )
        {
           /*
           ** Convert from Microseconds to the timeout
           */
           OS_UsecsToTicks(start_time, &start_ticks);

           user_data.opaque_arg = NULL;
           user_data.value = OS_global_timebase_table[timer_id].active_id;

           status = rtems_timer_fire_after(local->rtems_timer_id, start_ticks,
                   OS_TimeBase_ISR, user_data.opaque_arg );
           if ( status != RTEMS_SUCCESSFUL )
           {
               return_code = OS_TIMER_ERR_INTERNAL;
           }
           else
           {
               local->configured_start_time = (10000 * start_ticks) / OS_SharedGlobalVars.TicksPerSecond;
               local->configured_interval_time = (10000 * local->interval_ticks) / OS_SharedGlobalVars.TicksPerSecond;
               local->configured_start_time *= 100;
               local->configured_interval_time *= 100;

               if (local->configured_start_time != start_time)
               {
                   OS_DEBUG("WARNING: timer %lu start_time requested=%luus, configured=%luus\n",
                           (unsigned long)timer_id,
                           (unsigned long)start_time,
                           (unsigned long)local->configured_start_time);
               }
               if (local->configured_interval_time != interval_time)
               {
                   OS_DEBUG("WARNING: timer %lu interval_time requested=%luus, configured=%luus\n",
                           (unsigned long)timer_id,
                           (unsigned long)interval_time,
                           (unsigned long)local->configured_interval_time);
               }

               if (local->interval_ticks > 0)
               {
                   OS_timebase_table[timer_id].accuracy_usec = local->configured_interval_time;
               }
               else
               {
                   OS_timebase_table[timer_id].accuracy_usec = local->configured_start_time;
               }
           }
        }
    }

    if (local->reset_flag == 0 && return_code == OS_SUCCESS)
    {
        local->reset_flag = 1;
    }
    return return_code;
} /* end OS_TimeBaseSet_Impl */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseDelete_Impl(uint32 timer_id)
{
    rtems_status_code rtems_sc;
    OS_impl_timebase_internal_record_t *local;
    int32 return_code;

    local = &OS_impl_timebase_table[timer_id];
    return_code = OS_SUCCESS;

    /*
    ** Delete the tasks and timer OS constructs first, then delete the
    ** semaphores.  If the task/timer is running it might try to use them.
    */
    if (local->simulate_flag)
    {
        rtems_sc = rtems_timer_delete(local->rtems_timer_id);
        if (rtems_sc != RTEMS_SUCCESSFUL)
        {
            OS_DEBUG("Error deleting rtems timer: %s\n", rtems_status_text(rtems_sc));
            return_code = OS_TIMER_ERR_INTERNAL;
        }
    }

    rtems_sc = rtems_task_delete(local->handler_task);
    if (rtems_sc != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Error deleting timebase helper task: %s\n", rtems_status_text(rtems_sc));
        return_code = OS_TIMER_ERR_INTERNAL;
    }

    /*
     * If any delete/cleanup calls fail, unfortunately there is no recourse.
     * Just report the error via OS_DEBUG and the resource will be leaked.
     */
    if (return_code == OS_SUCCESS)
    {
        rtems_sc = rtems_semaphore_delete (local->handler_mutex);
        if (rtems_sc != RTEMS_SUCCESSFUL)
        {
            OS_DEBUG("Error deleting handler mutex: %s\n", rtems_status_text(rtems_sc));
        }

        if (local->simulate_flag)
        {
            rtems_sc = rtems_semaphore_delete (local->tick_sem);
            if (rtems_sc != RTEMS_SUCCESSFUL)
            {
                OS_DEBUG("Error deleting tick semaphore: %s\n", rtems_status_text(rtems_sc));
            }
            local->simulate_flag = 0;
        }
    }

    return return_code;
} /* end OS_TimeBaseDelete_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseGetInfo_Impl (uint32 timer_id, OS_timebase_prop_t *timer_prop)
{
    return OS_SUCCESS;

} /* end OS_TimeBaseGetInfo_Impl */

/****************************************************************************************
                  Other Time-Related API Implementation
 ***************************************************************************************/

/* RTEMS implements POSIX-style clock_gettime and clock_settime calls */
#include "../portable/os-impl-posix-gettime.c"

