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
 * \file     ut-adaptor-tasks.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-tasks.h"

#include <os-vxworks.h>
#include <os-impl-tasks.h>

void*  const UT_Ref_OS_impl_task_table = OS_impl_task_table;
size_t const UT_Ref_OS_impl_task_table_SIZE = sizeof(OS_impl_task_table);


int32 UT_Call_OS_VxWorks_TaskAPI_Impl_Init(void)
{
    return OS_VxWorks_TaskAPI_Impl_Init();
}

void UT_TaskTest_SetImplTaskId(uint32 local_id, OCS_TASK_ID TaskId)
{
    OS_impl_task_table[local_id].vxid = TaskId;
}

/*
 * Because the task entry point is declared "static",
 * in order for the UT to invoke it there must be a non-static
 * way to get access to it.
 */
int UT_TaskTest_CallEntryPoint(int arg)
{
    return OS_VxWorks_TaskEntry(arg);
}

OCS_WIND_TCB* UT_TaskTest_GetTaskTcb(uint32 local_id)
{
    return &OS_impl_task_table[local_id].tcb;
}

