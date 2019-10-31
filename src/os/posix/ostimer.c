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
 * Purpose: This file contains the OSAL Timer API for POSIX systems.
 *
 *          This implementation depends on the POSIX Timer API which may not be available
 *          in older versions of the Linux kernel. It was developed and tested on
 *          RHEL 5 ./ CentOS 5 with Linux kernel 2.6.18
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"

/****************************************************************************************
                                EXTERNAL FUNCTION PROTOTYPES
 ***************************************************************************************/

/****************************************************************************************
                                INTERNAL FUNCTION PROTOTYPES
 ***************************************************************************************/

static void  OS_UsecToTimespec(uint32 usecs, struct timespec *time_spec);

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

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
    pthread_t           handler_thread;
    pthread_mutex_t     handler_mutex;
    timer_t             host_timerid;
    int                 assigned_signal;
    sigset_t            sigset;
    uint32              reset_flag;
    struct timespec     softsleep;

} OS_impl_timebase_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES];

/****************************************************************************************
                                INTERNAL FUNCTIONS
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_UsecToTimespec
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Convert Microseconds to a POSIX timespec structure.
 *
 *-----------------------------------------------------------------*/
static void OS_UsecToTimespec(uint32 usecs, struct timespec *time_spec)
{

   if ( usecs < 1000000 )
   {
      time_spec->tv_nsec = (usecs * 1000);
      time_spec->tv_sec = 0;
   }
   else
   {
      time_spec->tv_sec = usecs / 1000000;
      time_spec->tv_nsec = (usecs % 1000000) * 1000;
   }
} /* end OS_UsecToTimespec */
                        
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
    pthread_mutex_lock(&OS_impl_timebase_table[local_id].handler_mutex);
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
    pthread_mutex_unlock(&OS_impl_timebase_table[local_id].handler_mutex);
} /* end OS_TimeBaseUnlock_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBase_SoftWaitImpl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
static uint32 OS_TimeBase_SigWaitImpl(uint32 timer_id)
{
    int ret;
    OS_impl_timebase_internal_record_t *local;
    uint32 interval_time;
    int sig;

    local = &OS_impl_timebase_table[timer_id];

    ret = sigwait(&local->sigset, &sig);

    if (ret != 0)
    {
        /*
         * the sigwait call failed.
         * returning 0 will cause the process to repeat.
         */
        interval_time = 0;
    }
    else if (local->reset_flag == 0)
    {
        /*
         * Normal steady-state behavior.
         * interval_time reflects the configured interval time.
         */
        interval_time = OS_timebase_table[timer_id].nominal_interval_time;
    }
    else
    {
        /*
         * Reset/First interval behavior.
         * timer_set() was invoked since the previous interval occurred (if any).
         * interval_time reflects the configured start time.
         */
        interval_time = OS_timebase_table[timer_id].nominal_start_time;
        local->reset_flag = 0;
    }

    return interval_time;
} /* end OS_TimeBase_SoftWaitImpl */


/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/******************************************************************************
 *  Function:  OS_Posix_TimeBaseAPI_Impl_Init
 *
 *  Purpose:  Initialize the timer implementation layer
 *
 *  Arguments:
 *
 *  Return:
 */
int32 OS_Posix_TimeBaseAPI_Impl_Init(void)
{
   int    status;
   int    i;
   pthread_mutexattr_t mutex_attr;
   struct timespec clock_resolution;
   int32  return_code;

   return_code = OS_SUCCESS;

   do
   {
       /*
       ** Mark all timers as available
       */
       memset(OS_impl_timebase_table, 0, sizeof(OS_impl_timebase_table));

       /*
       ** get the resolution of the selected clock
       */
       status = clock_getres(OS_PREFERRED_CLOCK, &clock_resolution);
       if ( status != 0 )
       {
          OS_DEBUG("failed in clock_getres: %s\n",strerror(status));
          return_code = OS_ERROR;
          break;
       }

      /*
      ** Convert to microseconds
      ** Note that the resolution MUST be in the sub-second range, if not then
      ** it looks like the POSIX timer API in the C library is broken.
      ** Note for any flavor of RTOS we would expect <= 1ms.  Even a "desktop"
      ** linux or development system should be <= 100ms absolute worst-case.
      */
       if ( clock_resolution.tv_sec > 0 )
       {
           return_code = OS_TIMER_ERR_INTERNAL;
           break;
       }

       /* Round to the nearest microsecond */
       POSIX_GlobalVars.ClockAccuracyNsec = (uint32)(clock_resolution.tv_nsec);

       /*
       ** initialize the attribute with default values
       */
       status = pthread_mutexattr_init(&mutex_attr);
       if ( status != 0 )
       {
          OS_DEBUG("Error: pthread_mutexattr_init failed: %s\n",strerror(status));
          return_code = OS_ERROR;
          break;
       }

       /*
       ** Allow the mutex to use priority inheritance
       */
       status = pthread_mutexattr_setprotocol(&mutex_attr,PTHREAD_PRIO_INHERIT);
       if ( status != 0 )
       {
          OS_DEBUG("Error: pthread_mutexattr_setprotocol failed: %s\n",strerror(status));
          return_code = OS_ERROR;
          break;
       }

       for (i = 0; i < OS_MAX_TIMEBASES; ++i)
       {
           /*
           ** create the timebase sync mutex
           ** This gives a mechanism to synchronize updates to the timer chain with the
           ** expiration of the timer and processing the chain.
           */
           status = pthread_mutex_init(&OS_impl_timebase_table[i].handler_mutex, &mutex_attr);
           if ( status != 0 )
           {
              OS_DEBUG("Error: Mutex could not be created: %s\n",strerror(status));
              return_code = OS_ERROR;
              break;
           }
       }

       /*
        * Pre-calculate the clock tick to microsecond conversion factor.
        * This is used by OS_Tick2Micros(), OS_Milli2Ticks(), etc.
        */
       OS_SharedGlobalVars.TicksPerSecond = sysconf(_SC_CLK_TCK);
       if (OS_SharedGlobalVars.TicksPerSecond <= 0)
       {
          OS_DEBUG("Error: Unable to determine OS ticks per second: %s\n",strerror(errno));
          return_code = OS_ERROR;
          break;
       }

       /*
        * Calculate microseconds per tick - Rounding UP
        *  - If the ratio is not an integer, this will choose the next higher value
        *  - The result is guaranteed not to be zero.
        */
       OS_SharedGlobalVars.MicroSecPerTick = (1000000 + (OS_SharedGlobalVars.TicksPerSecond / 2)) /
             OS_SharedGlobalVars.TicksPerSecond;
    }
    while(0);


   return(return_code);
} /* end OS_Posix_TimeBaseAPI_Impl_Init */

/****************************************************************************************
                                   Time Base API
 ***************************************************************************************/

static void *OS_TimeBasePthreadEntry(void *arg)
{
    OS_U32ValueWrapper_t local_arg;

    local_arg.opaque_arg = arg;
    OS_TimeBase_CallbackThread(local_arg.value);
    return NULL;
}

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
    int    status;
    int    i;
    struct sigevent   evp;
    OS_impl_timebase_internal_record_t *local;
    OS_common_record_t *global;
    OS_U32ValueWrapper_t arg;


    local = &OS_impl_timebase_table[timer_id];
    global = &OS_global_timebase_table[timer_id];

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
    arg.opaque_arg = NULL;
    arg.value = global->active_id;
    return_code = OS_Posix_InternalTaskCreate_Impl(&local->handler_thread, 0, 0, OS_TimeBasePthreadEntry, arg.opaque_arg);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    local->assigned_signal = 0;
    clock_gettime(OS_PREFERRED_CLOCK, &local->softsleep);

    /*
     * Set up the necessary OS constructs
     *
     * If an external sync function is used then there is nothing to do here -
     * we simply call that function and it should synchronize to the time source.
     *
     * If no external sync function is provided then this will set up a POSIX
     * timer to locally simulate the timer tick using the CPU clock.
     */
    if (OS_timebase_table[timer_id].external_sync == NULL)
    {
        sigemptyset(&local->sigset);

        /*
         * find an RT signal that is not used by another time base object.
         * This is all done while the global lock is held so no chance of the
         * underlying tables changing
         */
        for(i = 0; i < OS_MAX_TIMEBASES; ++i)
        {
            if (i != timer_id &&
                    OS_global_timebase_table[i].active_id != 0 &&
                    OS_impl_timebase_table[i].assigned_signal != 0)
            {
                sigaddset(&local->sigset, OS_impl_timebase_table[i].assigned_signal);
            }
        }

        for(i = SIGRTMIN; i <= SIGRTMAX; ++i)
        {
            if (!sigismember(&local->sigset, i))
            {
                local->assigned_signal = i;
                break;
            }
        }

        do
        {
            if (local->assigned_signal == 0)
            {
                OS_DEBUG("No free RT signals to use for simulated time base\n");
                return_code = OS_TIMER_ERR_UNAVAILABLE;
                break;
            }

            sigemptyset(&local->sigset);
            sigaddset(&local->sigset, local->assigned_signal);

            /*
            **  Initialize the sigevent structures for the handler.
            */
            memset((void *)&evp, 0, sizeof(evp));
            evp.sigev_notify = SIGEV_SIGNAL;
            evp.sigev_signo = local->assigned_signal;

            /*
             * Pass the Timer Index value of the object ID to the signal handler --
             *  Note that the upper bits can be safely assumed as a timer ID to recreate the original,
             *  and doing it this way should still work on a system where sizeof(sival_int) < sizeof(uint32)
             *  (as long as sizeof(sival_int) >= number of bits in OS_OBJECT_INDEX_MASK)
             */
            evp.sigev_value.sival_int = (int)(global->active_id & OS_OBJECT_INDEX_MASK);

            /*
            ** Create the timer
            ** Note using the "MONOTONIC" clock here as this will still produce consistent intervals
            ** even if the system clock is stepped (e.g. clock_settime).
            */
            status = timer_create(OS_PREFERRED_CLOCK, &evp, &local->host_timerid);
            if (status < 0)
            {
                return_code = OS_TIMER_ERR_UNAVAILABLE;
                break;
            }

            OS_timebase_table[timer_id].external_sync = OS_TimeBase_SigWaitImpl;
        }
        while (0);

    }

    if (return_code != OS_SUCCESS)
    {
        /*
         * NOTE about the thread cancellation -- this technically is just a backup,
         * we should not need to cancel it because the handler thread will exit automatically
         * if the active ID does not match the expected value.  This check would fail
         * if this function returns non-success (the ID in the global will be set zero)
         */
        pthread_cancel(local->handler_thread);
        local->assigned_signal = 0;
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
    OS_impl_timebase_internal_record_t *local;
    struct itimerspec timeout;
    int32 return_code;
    int status;

    local = &OS_impl_timebase_table[timer_id];
    return_code = OS_SUCCESS;

    /* There is only something to do here if we are generating a simulated tick */
    if (local->assigned_signal != 0)
    {
        /*
        ** Convert from Microseconds to timespec structures
        */
        memset(&timeout, 0, sizeof(timeout));
        OS_UsecToTimespec(start_time, &timeout.it_value);
        OS_UsecToTimespec(interval_time, &timeout.it_interval);

        /*
        ** Program the real timer
        */
        status = timer_settime(local->host_timerid,
                0,              /* Flags field can be zero */
                &timeout,       /* struct itimerspec */
                NULL);         /* Oldvalue */

        if (status < 0)
        {
            OS_DEBUG("Error in timer_settime: %s\n",strerror(errno));
            return_code = OS_TIMER_ERR_INTERNAL;
        }
        else if (interval_time > 0)
        {
            OS_timebase_table[timer_id].accuracy_usec = (uint32)((timeout.it_interval.tv_nsec + 999) / 1000);
        }
        else
        {
            OS_timebase_table[timer_id].accuracy_usec = (uint32)((timeout.it_value.tv_nsec + 999) / 1000);
        }
    }

    local->reset_flag = (return_code == OS_SUCCESS);
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
    OS_impl_timebase_internal_record_t *local;
    int status;

    local = &OS_impl_timebase_table[timer_id];

    pthread_cancel(local->handler_thread);

    /*
    ** Delete the timer
    */
    if (local->assigned_signal != 0)
    {
        status = timer_delete(OS_impl_timebase_table[timer_id].host_timerid);
        if (status < 0)
        {
            OS_DEBUG("Error deleting timer: %s\n",strerror(errno));
            return ( OS_TIMER_ERR_INTERNAL);
        }

        local->assigned_signal = 0;
    }

    return OS_SUCCESS;
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

/* POSIX implements clock_gettime and clock_settime that can be used */
#include "../portable/os-impl-posix-gettime.c"

