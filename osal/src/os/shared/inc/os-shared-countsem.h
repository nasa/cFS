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
 * \file     os-shared-countsem.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_COUNTSEM_H_
#define INCLUDE_OS_SHARED_COUNTSEM_H_

#include <os-shared-globaldefs.h>


/* other objects that have only an API name and no other data */
typedef struct
{
   char      obj_name[OS_MAX_API_NAME];
} OS_count_sem_internal_record_t;


/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_count_sem_internal_record_t        OS_count_sem_table[OS_MAX_COUNT_SEMAPHORES];

/****************************************************************************************
                                IMPLEMENTATION FUNCTIONS
  ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_CountSemAPI_Init

   Purpose: Initialize the OS-independent layer for counting semaphores

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_CountSemAPI_Init            (void);


/*----------------------------------------------------------------
   Function: OS_CountSemCreate_Impl

    Purpose: Prepare/allocate OS resources for a counting semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemCreate_Impl          (uint32 sem_id, uint32 sem_initial_value, uint32 options);

/*----------------------------------------------------------------
   Function: OS_CountSemGive_Impl

    Purpose: Increment the semaphore value

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemGive_Impl            (uint32 sem_id);

/*----------------------------------------------------------------
   Function: OS_CountSemTake_Impl

    Purpose: Decrement the semaphore value
             Block the calling task if the semaphore is 0.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemTake_Impl            (uint32 sem_id);

/*----------------------------------------------------------------
   Function: OS_CountSemTimedWait_Impl

    Purpose: Decrement the semaphore value, with a time limit

    Returns: OS_SUCCESS on success, or relevant error code
             OS_SEM_TIMEOUT must be returned if the time limit was reached
 ------------------------------------------------------------------*/
int32 OS_CountSemTimedWait_Impl       (uint32 sem_id, uint32 msecs);

/*----------------------------------------------------------------
   Function: OS_CountSemDelete_Impl

    Purpose: Free the OS resources associated with the counting semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemDelete_Impl          (uint32 sem_id);

/*----------------------------------------------------------------
   Function: OS_CountSemGetInfo_Impl

    Purpose: Obtain OS-specific information about the semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemGetInfo_Impl         (uint32 sem_id, OS_count_sem_prop_t *count_prop);


#endif  /* INCLUDE_OS_SHARED_COUNTSEM_H_ */

