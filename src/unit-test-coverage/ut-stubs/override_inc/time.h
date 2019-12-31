/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
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

