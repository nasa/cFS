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
 * \file     ut-adaptor-tasks.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_TASKS_H_
#define INCLUDE_UT_ADAPTOR_TASKS_H_

#include <common_types.h>
#include <OCS_taskLib.h>

extern void*  const UT_Ref_OS_impl_task_table;
extern size_t const UT_Ref_OS_impl_task_table_SIZE;


/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

int32 UT_Call_OS_VxWorks_TaskAPI_Impl_Init(void);
void UT_TaskTest_SetImplTaskId(uint32 local_id, OCS_TASK_ID TaskId);
int UT_TaskTest_CallEntryPoint(int arg);
OCS_WIND_TCB* UT_TaskTest_GetTaskTcb(uint32 local_id);


#endif  /* INCLUDE_UT_ADAPTOR_TASKS_H_ */

