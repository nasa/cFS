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
 * \file     os-impl-timebase.h
 * \ingroup  rtems
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

