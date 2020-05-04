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

