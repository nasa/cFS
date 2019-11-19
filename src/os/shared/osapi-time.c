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
 * \file   osapi-time.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
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
#if !defined(OS_MAX_TIMERS) || (OS_MAX_TIMERS <= 0)
#error "osconfig.h must define OS_MAX_TIMERS to a valid value"
#endif



OS_timecb_internal_record_t      OS_timecb_table             [OS_MAX_TIMERS];


/****************************************************************************************
                                   Timer API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_TimerCbAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerCbAPI_Init(void)
{
   memset(OS_timecb_table, 0, sizeof(OS_timecb_table));
   return OS_SUCCESS;
} /* end OS_TimerCbAPI_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_DoTimerAdd
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Adds new OSAL Timer based on an existing timebase
 *           Internal function used by TimerCreate and TimerAdd API calls
 *
 *  Arguments:  flags to specify the internal bits to set in the created record
 *
 *  Return:     OS_SUCCESS or error code
 *
 *-----------------------------------------------------------------*/
static int32 OS_DoTimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_ref_id, OS_ArgCallback_t  callback_ptr, void *callback_arg, uint32 flags)
{
    OS_common_record_t *timebase;
    OS_common_record_t *record;
    OS_timecb_internal_record_t *local;
    int32             return_code;
    uint32            local_id;
    uint32            timebase_local_id;
    uint32            cb_list;
    uint32            attach_id;

    /*
     ** Check Parameters
     */
    if ( timer_id == NULL || timer_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
     ** we don't want to allow names too long
     ** if truncated, two names might be the same
     */
    if (strlen(timer_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    /*
     ** Verify callback parameter
     */
    if (callback_ptr == NULL )
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

    /*
     * Check that the timebase reference is valid
     * If successful, then after this statement, we MUST decrement the refcount
     * if we leave this routine with an error.
     */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_ref_id, &timebase_local_id, &timebase);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TIMECB, timer_name, &local_id, &record);
    if(return_code == OS_SUCCESS)
    {
       /* Save all the data to our own internal timer table */
       local = &OS_timecb_table[local_id];

       memset(local, 0, sizeof(OS_timecb_internal_record_t));

       strncpy(local->timer_name, timer_name, OS_MAX_API_NAME - 1);
       record->name_entry = local->timer_name;
       local->callback_ptr = callback_ptr;
       local->callback_arg = callback_arg;
       local->timebase_ref = timebase_local_id;
       local->flags = flags;
       local->prev_ref = local_id;
       local->next_ref = local_id;

       /*
        * Now we need to add it to the time base callback ring, so take the
        * timebase-specific lock to prevent a tick from being processed at this moment.
        */
       OS_TimeBaseLock_Impl(timebase_local_id);

       cb_list = OS_timebase_table[timebase_local_id].first_cb;
       OS_timebase_table[timebase_local_id].first_cb = record->active_id;

       if (cb_list != 0)
       {
           OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TIMECB, cb_list, &attach_id);
           local->next_ref = attach_id;
           local->prev_ref = OS_timecb_table[attach_id].prev_ref;
           OS_timecb_table[local->prev_ref].next_ref = local_id;
           OS_timecb_table[local->next_ref].prev_ref = local_id;
       }

       OS_TimeBaseUnlock_Impl(timebase_local_id);

       /* Check result, finalize record, and unlock global table. */
       return_code = OS_ObjectIdFinalizeNew(return_code, record, timer_id);
    }
    else
    {
        OS_ObjectIdRefcountDecr(timebase);
    }

    return return_code;
} /* end OS_DoTimerAdd */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimerAdd
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_ref_id, OS_ArgCallback_t  callback_ptr, void *callback_arg)
{
    return (OS_DoTimerAdd(timer_id, timer_name, timebase_ref_id, callback_ptr, callback_arg, 0));
} /* end OS_TimerAdd */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_Timer_NoArgCallback
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
static void OS_Timer_NoArgCallback(uint32 objid, void *arg)
{
    OS_U32ValueWrapper_t Conv;

   /*
    * Note - did not write this as simply *((OS_SimpleCallback_t)arg) because
    * technically you cannot cast a void * to a function pointer.
    */
   Conv.opaque_arg = arg;
   (*Conv.timer_callback_func)(objid);
} /* end OS_Timer_NoArgCallback */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimerCreate
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *accuracy, OS_TimerCallback_t  callback_ptr)
{
    int32             return_code;
    uint32            timebase_ref_id;
    OS_U32ValueWrapper_t Conv;

    /*
    ** Check Parameters.  Although DoTimerAdd will also
    ** check this stuff, also doing it here avoids unnecessarily
    ** creating and deleting a timebase object in case something is bad.
    */
    if (timer_id == NULL || timer_name == NULL || accuracy == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
    ** Verify callback parameter
    */
    if (callback_ptr == NULL )
    {
        return OS_TIMER_ERR_INVALID_ARGS;
    }

    /*
     * Create our dedicated time base object to drive this timer
     */
    return_code = OS_TimeBaseCreate(&timebase_ref_id, timer_name, NULL);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    /*
     * Create the actual timer object based off our dedicated time base
     * The TIMECB_FLAG_DEDICATED_TIMEBASE flag is used to mark this object
     * that the time base object attached to it was automatically created for it
     *
     * Although this passes a function pointer through the "void*" argument
     * to the callback function (technically a no-no), this should be safe
     * because it is already verified by a CompileTimeAssert that
     * sizeof(OS_TimerCallback_t) <= sizeof(void*) on this platform.
     */
    Conv.opaque_arg = NULL;
    Conv.timer_callback_func = callback_ptr;
    return_code = OS_DoTimerAdd(timer_id, timer_name, timebase_ref_id, OS_Timer_NoArgCallback, Conv.opaque_arg, TIMECB_FLAG_DEDICATED_TIMEBASE);

    /*
     * If returning from this call unsuccessfully, then we need to delete the
     * dedicated timebase object that we just created earlier.
     */
    if (return_code != OS_SUCCESS)
    {
        OS_TimeBaseDelete(timebase_ref_id);
    }
    else
    {
        *accuracy = OS_SharedGlobalVars.MicroSecPerTick;
    }

    return return_code;
} /* end OS_TimerCreate */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimerSet
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
   OS_common_record_t *record;
   OS_timecb_internal_record_t *local;
   int32             return_code;
   uint32            local_id;
   uint32            dedicated_timebase_id;

   dedicated_timebase_id = 0;

   if (start_time >= (UINT32_MAX/2) || interval_time >= (UINT32_MAX/2))
   {
       return OS_TIMER_ERR_INVALID_ARGS;
   }
   
   if (start_time == 0 && interval_time == 0)
   {
       return OS_ERROR;
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


   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL,OS_OBJECT_TYPE_OS_TIMECB, timer_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
       local = &OS_timecb_table[local_id];

       OS_TimeBaseLock_Impl(local->timebase_ref);

       if ((local->flags & TIMECB_FLAG_DEDICATED_TIMEBASE) != 0)
       {
           dedicated_timebase_id = OS_global_timebase_table[local->timebase_ref].active_id;
       }

       local->wait_time = (int32)start_time;
       local->interval_time = (int32)interval_time;

       OS_TimeBaseUnlock_Impl(local->timebase_ref);

       /* Unlock the global from OS_ObjectIdCheck() */
       OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TIMECB);
   }

   /*
    * If the timer uses a dedicated time base, then also change that to the same interval.
    *
    * This is not perfect in the sense that the update is not all done atomically.  If this
    * is not the first call to TimerSet, then weirdness can happen:
    *  - a timer tick could occur between setting the callback times above and now
    *  - the call to OS_TimeBaseSet() could fail leaving us in a mixed state
    *
    * However, the notion of multiple TimerSet() calls is a gray area to begin with, since
    * timer ticks can occur at any moment - there is never a guarantee your new TimerSet values
    * will get applied before the old timer expires.  Therefore by definition an application
    * MUST be able to handle a possible "spurious" callback in these circumstances.
    */
   if (return_code == OS_SUCCESS && dedicated_timebase_id != 0)
   {
       return_code = OS_TimeBaseSet(dedicated_timebase_id, start_time, interval_time);
   }
   return return_code;
} /* end OS_TimerSet */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimerDelete
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerDelete(uint32 timer_id)
{
    OS_timecb_internal_record_t *local;
    OS_common_record_t *record;
    OS_common_record_t *timebase;
    int32             return_code;
    uint32            local_id;
    uint32            dedicated_timebase_id;

    dedicated_timebase_id = 0;

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    local_id = OS_TaskGetId_Impl() >> OS_OBJECT_TYPE_SHIFT;
    if (local_id == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }


    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, OS_OBJECT_TYPE_OS_TIMECB, timer_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
        local = &OS_timecb_table[local_id];
        timebase = &OS_global_timebase_table[local->timebase_ref];

        OS_TimeBaseLock_Impl(local->timebase_ref);

        /*
         * If the timer uses a dedicated time base, then also delete that.
         */
        if ((local->flags & TIMECB_FLAG_DEDICATED_TIMEBASE) != 0)
        {
            dedicated_timebase_id = timebase->active_id;
        }

        /*
         * Now we need to remove it from the time base callback ring
         */
        if (OS_timebase_table[local->timebase_ref].first_cb == timer_id)
        {
            if (local->next_ref != local_id)
            {
                OS_ObjectIdMap(OS_OBJECT_TYPE_OS_TIMEBASE, local->next_ref, &OS_timebase_table[local->timebase_ref].first_cb);
            }
            else
            {
                /*
                 * consider the list empty
                 */
                OS_timebase_table[local->timebase_ref].first_cb = 0;
            }
        }

        OS_timecb_table[local->prev_ref].next_ref = local->next_ref;
        OS_timecb_table[local->next_ref].prev_ref = local->prev_ref;
        local->next_ref = local_id;
        local->prev_ref = local_id;

        /* Clear the ID to zero */
        record->active_id = 0;

        OS_TimeBaseUnlock_Impl(local->timebase_ref);

        OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TIMECB);
    }

    /*
     * Remove the reference count against the timebase
     */

    /*
     * If the timer uses a dedicated time base, then also delete it.
     */
    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdRefcountDecr(timebase);
        if (dedicated_timebase_id != 0)
        {
            OS_TimeBaseDelete(dedicated_timebase_id);
        }
    }

    return return_code;
} /* end OS_TimerDelete */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimerGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerGetIdByName (uint32 *timer_id, const char *timer_name)
{
    int32 return_code;
    uint32 local_id;

    if (timer_id == NULL || timer_name == NULL)
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


    return_code = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TIMECB, timer_name, timer_id);

    return return_code;
} /* end OS_TimerGetIdByName */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimerGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerGetInfo (uint32 timer_id, OS_timer_prop_t *timer_prop)
{
    OS_common_record_t *record;
    int32             return_code;
    uint32            local_id;

    /* Check parameters */
    if (timer_prop == NULL)
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

    memset(timer_prop,0,sizeof(OS_timer_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL,OS_OBJECT_TYPE_OS_TIMECB, timer_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
       strncpy(timer_prop->name, record->name_entry, OS_MAX_API_NAME - 1);
       timer_prop->creator =    record->creator;
       timer_prop->interval_time =   (uint32)OS_timecb_table[local_id].interval_time;
       timer_prop->accuracy =   OS_timebase_table[OS_timecb_table[local_id].timebase_ref].accuracy_usec;

       OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TIMECB);
    }

    return return_code;
} /* end OS_TimerGetInfo */

