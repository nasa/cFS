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
 * \file     osapi-task-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-task.h"


/*
** Task API
*/
UT_DEFAULT_STUB(OS_TaskMatch_Impl,(uint32 task_id))
UT_DEFAULT_STUB(OS_TaskCreate_Impl,(uint32 task_id, uint32 flags))
UT_DEFAULT_STUB(OS_TaskDelete_Impl,(uint32 task_id))
void OS_TaskExit_Impl               (void)
{
    UT_DEFAULT_IMPL(OS_TaskExit_Impl);
}

UT_DEFAULT_STUB(OS_TaskDelay_Impl,(uint32 millisecond))
UT_DEFAULT_STUB(OS_TaskSetPriority_Impl,(uint32 task_id, uint32 new_priority))
uint32 OS_TaskGetId_Impl            (void)
{
    return UT_DEFAULT_IMPL(OS_TaskGetId_Impl);
}
UT_DEFAULT_STUB(OS_TaskGetInfo_Impl,(uint32 task_id, OS_task_prop_t *task_prop))
UT_DEFAULT_STUB(OS_TaskRegister_Impl,(uint32 global_task_id))

bool OS_TaskIdMatchSystemData_Impl(void *ref, uint32 local_id, const OS_common_record_t *obj)
{
    return UT_DEFAULT_IMPL(OS_TaskIdMatchSystemData_Impl);
}

UT_DEFAULT_STUB(OS_TaskValidateSystemData_Impl,(const void *sysdata, uint32 sysdata_size))

