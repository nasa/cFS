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
 * \file     os-impl-mutex.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "os-shared-mutex.h"
#include "os-impl-mutex.h"


/* Tables where the OS object information is stored */
OS_impl_mutex_internal_record_t   OS_impl_mutex_table       [OS_MAX_MUTEXES];


/****************************************************************************************
                                  MUTEX API
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_Posix_MutexAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Posix_MutexAPI_Impl_Init(void)
{
   memset(OS_impl_mutex_table, 0, sizeof(OS_impl_mutex_table));
   return OS_SUCCESS;
} /* end OS_Posix_MutexAPI_Impl_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemCreate_Impl (uint32 sem_id, uint32 options)
{
    int                 return_code;
    pthread_mutexattr_t mutex_attr;

    /*
    ** initialize the attribute with default values
    */
    return_code = pthread_mutexattr_init(&mutex_attr);
    if ( return_code != 0 )
    {
       OS_DEBUG("Error: Mutex could not be created. pthread_mutexattr_init failed ID = %u: %s\n",
             (unsigned int)sem_id,strerror(return_code));
       return OS_SEM_FAILURE;
    }

    /*
    ** Allow the mutex to use priority inheritance
    */
    return_code = pthread_mutexattr_setprotocol(&mutex_attr,PTHREAD_PRIO_INHERIT);
    if ( return_code != 0 )
    {
       OS_DEBUG("Error: Mutex could not be created. pthread_mutexattr_setprotocol failed ID = %u: %s\n",
             (unsigned int)sem_id,strerror(return_code));
       return OS_SEM_FAILURE;
    }

    /*
    **  Set the mutex type to RECURSIVE so a thread can do nested locks
    */
    return_code = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    if ( return_code != 0 )
    {
       OS_DEBUG("Error: Mutex could not be created. pthread_mutexattr_settype failed ID = %u: %s\n",
             (unsigned int)sem_id,strerror(return_code));
       return OS_SEM_FAILURE;
    }

    /*
    ** create the mutex
    ** upon successful initialization, the state of the mutex becomes initialized and unlocked
    */
    return_code = pthread_mutex_init(&OS_impl_mutex_table[sem_id].id,&mutex_attr);
    if ( return_code != 0 )
    {
       OS_DEBUG("Error: Mutex could not be created. ID = %u: %s\n",
             (unsigned int)sem_id,strerror(return_code));
       return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
} /* end OS_MutSemCreate_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemDelete_Impl (uint32 sem_id)
{
    int status;

    status = pthread_mutex_destroy( &(OS_impl_mutex_table[sem_id].id)); /* 0 = success */

    if (status != 0)
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_MutSemDelete_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemGive_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemGive_Impl ( uint32 sem_id )
{
   int status;

   /*
    ** Unlock the mutex
    */
   status = pthread_mutex_unlock(&(OS_impl_mutex_table[sem_id].id));
   if(status != 0)
   {
      return OS_SEM_FAILURE;
   }

   return OS_SUCCESS;
} /* end OS_MutSemGive_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemTake_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemTake_Impl ( uint32 sem_id )
{
    int status;

    /*
    ** Lock the mutex
    */
    status = pthread_mutex_lock(&(OS_impl_mutex_table[sem_id].id));
    if( status != 0 )
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
} /* end OS_MutSemTake_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemGetInfo_Impl (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
{
    return OS_SUCCESS;

} /* end OS_MutSemGetInfo_Impl */

