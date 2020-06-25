/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

