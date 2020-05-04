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
 * \file     os-shared-time.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_TIME_H_
#define INCLUDE_OS_SHARED_TIME_H_

#include <os-shared-globaldefs.h>


#define TIMECB_FLAG_DEDICATED_TIMEBASE      0x1

typedef struct
{
    char                timer_name[OS_MAX_API_NAME];
    uint32              flags;
    uint32              timebase_ref;
    uint32              prev_ref;
    uint32              next_ref;
    uint32              backlog_resets;
    int32               wait_time;
    int32               interval_time;
    OS_ArgCallback_t    callback_ptr;
    void                *callback_arg;
} OS_timecb_internal_record_t;


/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_timecb_internal_record_t         OS_timecb_table[OS_MAX_TIMERS];

/*---------------------------------------------------------------------------------------
   Name: OS_TimerCbAPI_Init

   Purpose: Initialize the OS-independent layer for timer callback objects

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_TimerCbAPI_Init             (void);


#endif  /* INCLUDE_OS_SHARED_TIME_H_ */

