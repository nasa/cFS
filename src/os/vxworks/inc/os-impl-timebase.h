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
 * \file     os-impl-timebase.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_TIMEBASE_H_
#define INCLUDE_OS_IMPL_TIMEBASE_H_

#include <osconfig.h>
#include <common_types.h>
#include <signal.h>
#include <taskLib.h>
#include <semLib.h>

enum OS_TimerState
{
    OS_TimerRegState_INIT = 0,
    OS_TimerRegState_SUCCESS,
    OS_TimerRegState_ERROR
};

typedef struct
{
    VX_MUTEX_SEMAPHORE(mmem);
    SEM_ID              handler_mutex;
    int                 assigned_signal;
    sigset_t            timer_sigset;
    TASK_ID             handler_task;
    timer_t             host_timerid;
    enum OS_TimerState  timer_state;
    uint32              configured_start_time;
    uint32              configured_interval_time;
    bool                reset_flag;
} OS_impl_timebase_internal_record_t;


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES];


#endif  /* INCLUDE_OS_IMPL_TIMEBASE_H_ */

