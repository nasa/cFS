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
 * \file   osapi-timebase.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *         A "timebase" provides the reference for which "timer" objects are based.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"


/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_TIMEBASES) || (OS_MAX_TIMEBASES <= 0)
#error "osconfig.h must define OS_MAX_TIMEBASES to a valid value"
#endif



enum
{
   LOCAL_NUM_OBJECTS = OS_MAX_TIMEBASES,
   LOCAL_OBJID_TYPE = OS_OBJECT_TYPE_OS_TIMEBASE
};

OS_timebase_internal_record_t    OS_timebase_table           [OS_MAX_TIMEBASES];


/*
 * Limit to the number of times that the OS timebase servicing thread
 * is allowed to spin without achieving external sync.
 */
#define OS_TIMEBASE_SPIN_LIMIT        4

/****************************************************************************************
                                   TimeBase API
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseAPI_Init(void)
{
    memset(OS_timebase_table, 0, sizeof(OS_timebase_table));
    return OS_SUCCESS;
} /* end OS_TimeBaseAPI_Init */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseCreate
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
{
    OS_common_record_t *record;
    int32             return_code;
    uint32            local_id;

    /*
     * Specifying a NULL sync function means the timebase is not externally synchronized.
     * In this case an appropriate OS timer will be used to generate the simulated timer tick.
     */

    /*
     ** Check Parameters
     */
    if ( timer_id == NULL || timebase_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
     ** we don't want to allow names too long
     ** if truncated, two names might be the same
     */
    if (strlen(timebase_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    local_id = OS_TaskGetId_Impl() >> OS_OBJECT_TYPE_SHIFT;
    if (local_id == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TIMEBASE, timebase_name, &local_id, &record);
    if(return_code == OS_SUCCESS)
    {
       /* Save all the data to our own internal timer table */
       memset(&OS_timebase_table[local_id], 0, sizeof(OS_timebase_internal_record_t));

       strncpy(OS_timebase_table[local_id].timebase_name, timebase_name, OS_MAX_API_NAME - 1);
       record->name_entry = OS_timebase_table[local_id].timebase_name;
       OS_timebase_table[local_id].external_sync = external_sync;
       if (external_sync == NULL)
       {
           OS_timebase_table[local_id].accuracy_usec = OS_SharedGlobalVars.MicroSecPerTick;
       }
       else
       {
           OS_timebase_table[local_id].accuracy_usec = 0;
       }

       /* Now call the OS-specific implementation.  This reads info from the timer table. */
       return_code = OS_TimeBaseCreate_Impl(local_id);

       /* Check result, finalize record, and unlock global table. */
       return_code = OS_ObjectIdFinalizeNew(return_code, record, timer_id);
    }

    return return_code;
} /* end OS_TimeBaseCreate */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseSet
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    OS_common_record_t *record;
    int32             return_code;
    uint32            local_id;

    /*
     * Internally the implementation represents the interval as a
     * signed 32-bit integer, but the parameter is unsigned because a negative interval
     * does not make sense.
     *
     * Note that the units are intentionally left unspecified.  The external sync period
     * could be measured in microseconds or hours -- it is whatever the application requires.
     */
    if (interval_time >= 1000000000 || start_time >= 1000000000)
    {
        return OS_TIMER_ERR_INVALID_ARGS;
    }

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    local_id = OS_TaskGetId_Impl() >> OS_OBJECT_TYPE_SHIFT;
    if (local_id == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TIMEBASE, timer_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
        /* Need to take the time base lock to ensure that no ticks are currently being processed */
        OS_TimeBaseLock_Impl(local_id);

        return_code = OS_TimeBaseSet_Impl(local_id, start_time, interval_time);

        if (return_code == OS_SUCCESS)
        {
           /* Save the value since we were successful */
            OS_timebase_table[local_id].nominal_start_time = start_time;
            OS_timebase_table[local_id].nominal_interval_time = interval_time;
        }

        OS_TimeBaseUnlock_Impl(local_id);

        OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TIMEBASE);
    }

    return return_code;
} /* end OS_TimeBaseSet */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseDelete
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseDelete(uint32 timer_id)
{
    OS_common_record_t *record;
    int32             return_code;
    uint32            local_id;

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    local_id = OS_TaskGetId_Impl() >> OS_OBJECT_TYPE_SHIFT;
    if (local_id == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, OS_OBJECT_TYPE_OS_TIMEBASE, timer_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_TimeBaseDelete_Impl(local_id);

        /* Free the entry in the master table now while still locked */
        if (return_code == OS_SUCCESS)
        {
            /* Clear the ID to zero */
            record->active_id = 0;
        }

        OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TIMEBASE);
    }

    return return_code;
} /* end OS_TimeBaseDelete */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
{
    int32 return_code;
    uint32 local_id;

    if (timer_id == NULL || timebase_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    local_id = OS_TaskGetId_Impl() >> OS_OBJECT_TYPE_SHIFT;
    if (local_id == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }


    return_code = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TIMEBASE, timebase_name, timer_id);

    return return_code;
} /* end OS_TimeBaseGetIdByName */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseGetInfo (uint32 timebase_id, OS_timebase_prop_t *timebase_prop)
{
    OS_common_record_t *record;
    int32             return_code;
    uint32            local_id;

    /* Check parameters */
    if (timebase_prop == NULL)
    {
       return OS_INVALID_POINTER;
    }

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    local_id = OS_TaskGetId_Impl() >> OS_OBJECT_TYPE_SHIFT;
    if (local_id == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    memset(timebase_prop,0,sizeof(OS_timebase_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL,LOCAL_OBJID_TYPE, timebase_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
       strncpy(timebase_prop->name, record->name_entry, OS_MAX_API_NAME - 1);
       timebase_prop->creator =    record->creator;
       timebase_prop->nominal_interval_time =   OS_timebase_table[local_id].nominal_interval_time;
       timebase_prop->freerun_time =   OS_timebase_table[local_id].freerun_time;
       timebase_prop->accuracy =   OS_timebase_table[local_id].accuracy_usec;

       return_code = OS_TimeBaseGetInfo_Impl(local_id, timebase_prop);

       OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }

    return return_code;
} /* end OS_TimeBaseGetInfo */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseGetFreeRun
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val)
{
    OS_common_record_t *record;
    int32             return_code;
    uint32            local_id;

    /* Check parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE,LOCAL_OBJID_TYPE, timebase_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
       *freerun_val = OS_timebase_table[local_id].freerun_time;
    }

    return return_code;
} /* end OS_TimeBaseGetFreeRun */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBase_CallbackThread
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Implementation of the time base "helper thread"
 *
 *           This is executed in a dedicated thread context (typically elevated priority)
 *           and performs two basic functions:
 *             1) call the BSP-specified delay routine to sync with the time reference (tick)
 *             2) process the requested Application callbacks each time the tick occurs
 *
 *    Returns: None.
 *
 *    Note: Application callbacks will be done under this thread context.
 *          Doing callbacks directly as an ISR or signal handler can be dangerous, as the
 *          available C library calls are very limited in that context.
 *
 *-----------------------------------------------------------------*/
void OS_TimeBase_CallbackThread(uint32 timebase_id)
{
    OS_TimerSync_t syncfunc;
    OS_timebase_internal_record_t *timebase;
    OS_timecb_internal_record_t *timecb;
    OS_common_record_t *record;
    uint32 local_id;
    uint32 timer_id;
    uint32 curr_cb_local_id;
    uint32 curr_cb_public_id;
    uint32 tick_time;
    uint32 spin_cycles;
    int32 saved_wait_time;

    /*
     * Register this task as a time base handler.
     * Application code MUST NOT attempt to configure timers from the context of a
     * timer callback, otherwise deadlock is possible.  Additional checks prevent this.
     *
     */
    OS_TaskRegister_Impl(timebase_id);

    /* Grab the relevant info from the global structure */
    if (OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id, &local_id, &record) != 0)
    {
        /* Something went wrong - abort this thread */
        return;
    }

    timebase = &OS_timebase_table[local_id];
    syncfunc = timebase->external_sync;
    spin_cycles = 0;

    OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TIMEBASE);

    while (1)
    {
        /*
         * Call the sync function - this will pend for some period of time
         * and return the amount of elapsed time in units of "timebase ticks"
         */
        tick_time = (*syncfunc)(local_id);

        /*
         * The returned tick_time should be nonzero.  If the sync function
         * returns zero, then it means something went wrong and it is not
         * known how many ticks have elapsed.
         *
         * This can validly occur, for instance, if the underlying wait
         * operation was interrupted for some reason, e.g. EINTR or EAGAIN
         * on a POSIX-like OS.
         *
         * In some instances it is appropriate to simply call the
         * function again.  However, since this task typically runs as a
         * high-priority thread, it is prudent to limit such spinning.
         */
        if (tick_time != 0)
        {
            /* nominal case - reset counter */
            spin_cycles = 0;
        }
        else if (spin_cycles < OS_TIMEBASE_SPIN_LIMIT)
        {
            /* off-nominal but OK for now */
            ++spin_cycles;
        }
        else
        {
            /*
             * Spin-loop detected.
             * Just call OS_TaskDelay_Impl() to yield the CPU.
             * generate a debug warning only on the first time
             * so the operator knows this is happening.
             */
            OS_TaskDelay_Impl(10);

            if (spin_cycles == OS_TIMEBASE_SPIN_LIMIT)
            {
                ++spin_cycles;
                OS_DEBUG("WARNING: Timebase Sync Spin Loop detected\n");
            }
        }

        OS_TimeBaseLock_Impl(local_id);

        /*
         * After waiting, check that our ID still matches
         * If not then it means this time base got deleted....
         */
        if (timebase_id != record->active_id)
        {
            OS_TimeBaseUnlock_Impl(local_id);
            break;
        }

        timebase->freerun_time += tick_time;
        if (OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TIMECB, timebase->first_cb, &timer_id) == 0)
        {
            curr_cb_local_id = timer_id;
            do
            {
                curr_cb_public_id = OS_global_timecb_table[curr_cb_local_id].active_id;
                timecb = &OS_timecb_table[curr_cb_local_id];
                saved_wait_time = timecb->wait_time;
                timecb->wait_time -= tick_time;
                while (timecb->wait_time <= 0)
                {
                    timecb->wait_time += timecb->interval_time;

                    /*
                     * Only allow the "wait_time" underflow to go as far negative as one interval time
                     * This prevents a cb "interval_time" of less than the timebase interval_time from
                     * accumulating infinitely
                     */
                    if (timecb->wait_time < -timecb->interval_time)
                    {
                        ++timecb->backlog_resets;
                        timecb->wait_time = -timecb->interval_time;
                    }

                    /*
                     * Only give the callback if the wait_time actually transitioned from positive to negative.
                     * This allows one-shot operation where the API sets the "wait_time" positive but keeps
                     * the "interval_time" at zero.  With the interval_time at zero the wait time will never
                     * go positive again unless the API sets it again.
                     */
                    if (saved_wait_time > 0 && timecb->callback_ptr != NULL)
                    {
                        (*timecb->callback_ptr)(curr_cb_public_id, timecb->callback_arg);
                    }

                    /*
                     * Do not repeat the loop unless interval_time is configured.
                     */
                    if (timecb->interval_time <= 0)
                    {
                        break;
                    }
                }
                curr_cb_local_id = timecb->next_ref;
            }
            while (curr_cb_local_id != timer_id);
        }

        OS_TimeBaseUnlock_Impl(local_id);

    }
} /* end OS_TimeBase_CallbackThread */

/****************************************************************************************
                  Other Time-Related API Implementation
 ***************************************************************************************/

/*
 * This is the OSAL-defined interface to the OS Timer tick -
 * Not sure what it is really useful for since none of the user API timer calls deal with
 * OS ticks directly.
 */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_Tick2Micros
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_Tick2Micros (void)
{
   return (OS_SharedGlobalVars.MicroSecPerTick);
} /* end OS_Tick2Micros */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_Milli2Ticks
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_Milli2Ticks(uint32 milli_seconds)
{
    unsigned long num_of_ticks;

    num_of_ticks = (unsigned long)milli_seconds;
    num_of_ticks *= OS_SharedGlobalVars.TicksPerSecond;
    num_of_ticks = (num_of_ticks + 999) / 1000;

    return((uint32)num_of_ticks);
} /* end OS_Milli2Ticks */



