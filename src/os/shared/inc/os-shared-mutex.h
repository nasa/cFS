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
 * \file     os-shared-mutex.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_MUTEX_H_
#define INCLUDE_OS_SHARED_MUTEX_H_

#include <os-shared-globaldefs.h>


typedef struct
{
   char      obj_name[OS_MAX_API_NAME];
} OS_mutex_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_mutex_internal_record_t        OS_mutex_table[OS_MAX_MUTEXES];

/*---------------------------------------------------------------------------------------
   Name: OS_MutexAPI_Init

   Purpose: Initialize the OS-independent layer for mutex objects

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_MutexAPI_Init               (void);


/*----------------------------------------------------------------
   Function: OS_MutSemCreate_Impl

    Purpose: Prepare/allocate OS resources for a mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemCreate_Impl           (uint32 sem_id, uint32 options);

/*----------------------------------------------------------------
   Function: OS_MutSemGive_Impl

    Purpose: Release the mutex, which must be owned by the caller

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemGive_Impl             (uint32 sem_id);

/*----------------------------------------------------------------
   Function: OS_MutSemTake_Impl

    Purpose: Acquire the mutex, blocking the caller as necessary

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemTake_Impl             (uint32 sem_id);

/*----------------------------------------------------------------
   Function: OS_MutSemDelete_Impl

    Purpose: Free the OS resources associated with a mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemDelete_Impl           (uint32 sem_id);

/*----------------------------------------------------------------
   Function: OS_MutSemGetInfo_Impl

    Purpose: Obtain OS-specific information about the mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemGetInfo_Impl          (uint32 sem_id, OS_mut_sem_prop_t *mut_prop);


#endif  /* INCLUDE_OS_SHARED_MUTEX_H_ */

