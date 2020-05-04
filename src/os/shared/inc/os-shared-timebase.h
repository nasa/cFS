/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-shared-timebase.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_TIMEBASE_H_
#define INCLUDE_OS_SHARED_TIMEBASE_H_

#include <os-shared-globaldefs.h>


typedef struct
{
    char                timebase_name[OS_MAX_API_NAME];
    OS_TimerSync_t      external_sync;
    uint32              accuracy_usec;
    uint32              first_cb;
    uint32              freerun_time;
    uint32              nominal_start_time;
    uint32              nominal_interval_time;
} OS_timebase_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_timebase_internal_record_t       OS_timebase_table[OS_MAX_TIMEBASES];

/****************************************************************************************
                 CLOCK / TIME API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_TimeBaseAPI_Init

   Purpose: Initialize the OS-independent layer for timebase objects

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_TimeBaseAPI_Init            (void);


/*
 * TimeBase routines implement a low-level timer tick/interrupt;
 * no callbacks to user code here.  All application callbacks are
 * done in the shared layer timer API.
 */

/*----------------------------------------------------------------
   Function: OS_TimeBaseCreate_Impl

    Purpose: Prepare OS resources for a time base

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_TimeBaseCreate_Impl        (uint32 timebase_id);

/*----------------------------------------------------------------
   Function: OS_TimeBaseSet_Impl

    Purpose: Configure the OS resources to provide a timer tick

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_TimeBaseSet_Impl           (uint32 timebase_id, int32 start_time, int32 interval_time);

/*----------------------------------------------------------------
   Function: OS_TimeBaseDelete_Impl

    Purpose: Free the OS resources associated with the time base

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_TimeBaseDelete_Impl        (uint32 timebase_id);


/****************************************************************************************
                                INTERNAL FUNCTIONS
****************************************************************************************/


/*----------------------------------------------------------------
   Function: OS_TimeBaseLock_Impl

    Purpose: Get exclusive access to the given timebase
             Add/remove of application callbacks is prevented
 ------------------------------------------------------------------*/
void  OS_TimeBaseLock_Impl          (uint32 timebase_id);

/*----------------------------------------------------------------
   Function: OS_TimeBaseLock_Impl

    Purpose: Release exclusive access to the given timebase
             Add/remove of application callbacks is allowed
 ------------------------------------------------------------------*/
void  OS_TimeBaseUnlock_Impl        (uint32 timebase_id);

/*----------------------------------------------------------------
   Function: OS_TimeBaseGetInfo_Impl

    Purpose: Obtain the OS-specific time base information, if any

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_TimeBaseGetInfo_Impl       (uint32 timer_id, OS_timebase_prop_t *timer_prop);

/*----------------------------------------------------------------
   Function: OS_TimeBase_CallbackThread

    Purpose: Implement the time base helper thread
             This is the context for providing application callbacks
 ------------------------------------------------------------------*/
void  OS_TimeBase_CallbackThread    (uint32 timebase_id);


#endif  /* INCLUDE_OS_SHARED_TIMEBASE_H_ */

