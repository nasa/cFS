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

