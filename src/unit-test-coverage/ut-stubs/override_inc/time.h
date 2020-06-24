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

/* OSAL coverage stub replacement for time.h */
#ifndef _OSAL_OVERRIDE_TIME_H_
#define _OSAL_OVERRIDE_TIME_H_

#include <OCS_time.h>

/* ----------------------------------------- */
/* mappings for declarations in time.h */
/* ----------------------------------------- */

#define CLOCK_REALTIME       OCS_CLOCK_REALTIME
#define CLOCK_MONOTONIC      OCS_CLOCK_MONOTONIC
#define TIMER_ABSTIME        OCS_TIMER_ABSTIME

#define clockid_t            OCS_clockid_t
#define timer_t              OCS_timer_t
#define time_t               OCS_time_t
#define suseconds_t          OCS_suseconds_t
#define timespec             OCS_timespec
#define timeval              OCS_timeval
#define itimerspec           OCS_itimerspec
#define TIMER_CONNECT_FUNC   OCS_TIMER_CONNECT_FUNC

#define clock_getres         OCS_clock_getres
#define clock_gettime        OCS_clock_gettime
#define clock_nanosleep      OCS_clock_nanosleep
#define clock_settime        OCS_clock_settime
#define timer_create         OCS_timer_create
#define timer_delete         OCS_timer_delete
#define timer_gettime        OCS_timer_gettime
#define timer_settime        OCS_timer_settime
#define timer_connect        OCS_timer_connect


#endif /* _OSAL_OVERRIDE_TIME_H_ */

