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

