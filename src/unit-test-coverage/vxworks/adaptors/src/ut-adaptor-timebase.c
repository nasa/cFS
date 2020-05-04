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
 * \file     ut-adaptor-timebase.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-timebase.h"

#include <os-vxworks.h>
#include <os-impl-timebase.h>


void*  const UT_Ref_OS_impl_timebase_table = OS_impl_timebase_table;
size_t const UT_Ref_OS_impl_timebase_table_SIZE = sizeof(OS_impl_timebase_table);

int32 UT_Call_OS_VxWorks_TimeBaseAPI_Impl_Init(void)
{
    return OS_VxWorks_TimeBaseAPI_Impl_Init();
}


int32 UT_TimeBaseTest_CallSigWaitFunc(uint32 local_id)
{
    return OS_VxWorks_SigWait(local_id);
}

int UT_TimeBaseTest_CallHelperTaskFunc(int arg)
{
    return OS_VxWorks_TimeBaseTask(arg);
}

void UT_TimeBaseTest_CallRegisterTimer(uint32 local_id)
{
    OS_VxWorks_RegisterTimer(local_id);
}

bool UT_TimeBaseTest_CheckTimeBaseRegisteredState(uint32 local_id)
{
    return (OS_impl_timebase_table[local_id].timer_state == OS_TimerRegState_SUCCESS);
}

bool UT_TimeBaseTest_CheckTimeBaseErrorState(uint32 local_id)
{
    return (OS_impl_timebase_table[local_id].timer_state == OS_TimerRegState_ERROR);
}

void UT_TimeBaseTest_ClearTimeBaseRegState(uint32 local_id)
{
    OS_impl_timebase_table[local_id].timer_state = OS_TimerRegState_INIT;
}

void UT_TimeBaseTest_SetTimeBaseRegState(uint32 local_id, bool is_success)
{
    /* Mimic the setting of the Reg state global, which
     * is typically done by the task after spawning
     */
    if (is_success)
    {
        OS_impl_timebase_table[local_id].timer_state = OS_TimerRegState_SUCCESS;
    }
    else
    {
        OS_impl_timebase_table[local_id].timer_state = OS_TimerRegState_ERROR;
    }
}

void UT_TimeBaseTest_UsecToTimespec(uint32 usecs, struct OCS_timespec *time_spec)
{
    OS_VxWorks_UsecToTimespec(usecs, time_spec);
}

void UT_TimeBaseTest_Setup(uint32 local_id, int signo, bool reset_flag)
{
    static OCS_WIND_TCB FAKE_TASK;
    static OCS_SEM FAKE_SEM;

    OS_impl_timebase_table[local_id].assigned_signal = signo;
    OS_impl_timebase_table[local_id].handler_task = &FAKE_TASK;
    OS_impl_timebase_table[local_id].handler_mutex = &FAKE_SEM;
    OS_impl_timebase_table[local_id].reset_flag = reset_flag;
}

