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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_TIMEBASE_H_
#define INCLUDE_OS_IMPL_TIMEBASE_H_

#include <osconfig.h>
#include <pthread.h>
#include <signal.h>


typedef struct
{
    pthread_t           handler_thread;
    pthread_mutex_t     handler_mutex;
    timer_t             host_timerid;
    int                 assigned_signal;
    sigset_t            sigset;
    sig_atomic_t        reset_flag;
    struct timespec     softsleep;

} OS_impl_timebase_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES];


#endif  /* INCLUDE_OS_IMPL_TIMEBASE_H_ */

