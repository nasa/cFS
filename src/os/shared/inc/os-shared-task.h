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
 * \file     os-shared-task.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_TASK_H_
#define INCLUDE_OS_SHARED_TASK_H_

#include <os-shared-globaldefs.h>

/*tasks */
typedef struct
{
   char      task_name[OS_MAX_API_NAME];
   uint32    stack_size;
   uint32    priority;
   osal_task_entry entry_function_pointer;
   osal_task_entry delete_hook_pointer;
   void      *entry_arg;
   uint32    *stack_pointer;
}OS_task_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_task_internal_record_t           OS_task_table[OS_MAX_TASKS];



/****************************************************************************************
                          TASK API LOW-LEVEL IMPLEMENTATION FUNCTIONS
  ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_TaskAPI_Init

   Purpose: Initialize the OS-independent layer for tasks

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_TaskAPI_Init                (void);


/*----------------------------------------------------------------
   Function: OS_TaskEntryPoint

    Purpose: Entry point for all newly created tasks

   The "OS_TaskEntryPoint" is a generic method implemented in the
   shared layer that performs housekeeping and then calls the user-specified
   entry point.  It should be the first thing called in any new task.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
void   OS_TaskEntryPoint              (uint32 global_task_id);

/*----------------------------------------------------------------
   Function: OS_TaskMatch_Impl

    Purpose: Determines if the caller matches the given task_id

    Returns: OS_SUCCESS on match, any other code on non-match
 ------------------------------------------------------------------*/
int32  OS_TaskMatch_Impl             (uint32 task_id);

/*----------------------------------------------------------------

   Function: OS_TaskCreate_Impl

    Purpose: Prepare/Allocate OS resources for a new task and start
             running it, based on configuration in the global object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32  OS_TaskCreate_Impl            (uint32 task_id, uint32 flags);

/*----------------------------------------------------------------
   Function: OS_TaskDelete_Impl

    Purpose: Free the OS resources associated with the specified task

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32  OS_TaskDelete_Impl            (uint32 task_id);

/*----------------------------------------------------------------
   Function: OS_TaskExit_Impl

    Purpose: Exits the calling task

    This function does not return
 ------------------------------------------------------------------*/
void   OS_TaskExit_Impl              (void);

/*----------------------------------------------------------------
   Function: OS_TaskDelay_Impl

    Purpose: Blocks the calling task for the specified number of milliseconds

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32  OS_TaskDelay_Impl             (uint32 millisecond);

/*----------------------------------------------------------------
   Function: OS_TaskSetPriority_Impl

    Purpose: Set the scheduling priority of the specified task

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32  OS_TaskSetPriority_Impl       (uint32 task_id, uint32 new_priority);

/*----------------------------------------------------------------
   Function: OS_TaskGetId_Impl

    Purpose: Obtain the OSAL task ID of the caller

    Returns: The OSAL ID of the calling task, or zero if not registered
 ------------------------------------------------------------------*/
uint32 OS_TaskGetId_Impl             (void);

/*----------------------------------------------------------------
   Function: OS_TaskGetInfo_Impl

    Purpose: Obtain OS-specific information about a task

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32  OS_TaskGetInfo_Impl           (uint32 task_id, OS_task_prop_t *task_prop);

/*----------------------------------------------------------------

   Function: OS_TaskRegister_Impl

    Purpose: Perform registration actions after new task creation

        NOTE: This is invoked via the OS_TaskEntryPoint() immediately
              after new task creation, not through OS_TaskRegister() API

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32  OS_TaskRegister_Impl          (uint32 global_task_id);


/*----------------------------------------------------------------

   Function: OS_TaskIdMatchSystemData_Impl

    Purpose: A helper "match" function to find an OSAL task ID based on system ID
             Compatible with the "OS_ObjectIdFindBySearch" routine

 ------------------------------------------------------------------*/
bool OS_TaskIdMatchSystemData_Impl(void *ref, uint32 local_id, const OS_common_record_t *obj);

/*----------------------------------------------------------------

   Function: OS_TaskValidateSystemData_Impl

    Purpose: Checks that the supplied sysdata pointer and sysdata_size are
             compatible/reasonable for the underlying OS.

 ------------------------------------------------------------------*/
int32 OS_TaskValidateSystemData_Impl(const void *sysdata, uint32 sysdata_size);


#endif  /* INCLUDE_OS_SHARED_TASK_H_ */

