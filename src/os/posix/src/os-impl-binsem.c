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
 * \file     os-impl-binsem.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains some of the OS APIs abstraction layer
 *    implementation for POSIX
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "os-shared-binsem.h"
#include "os-impl-binsem.h"

/* Tables where the OS object information is stored */
OS_impl_binsem_internal_record_t    OS_impl_bin_sem_table       [OS_MAX_BIN_SEMAPHORES];

/****************************************************************************************
                               BINARY SEMAPHORE API
 ***************************************************************************************/

/*
 * Note that the pthreads world does not provide VxWorks-style binary semaphores that the OSAL API is modeled after.
 * Instead, semaphores are simulated using pthreads mutexes, condition variables, and a bit of internal state.
 *
 * IMPORTANT: the side effect of this is that Binary Semaphores are not usable from signal handlers / ISRs.
 * Use Counting Semaphores instead.
 */

/*---------------------------------------------------------------------------------------
   Name: OS_Posix_BinSemAPI_Impl_Init

   Purpose: Initialize the Binary Semaphore data structures

 ----------------------------------------------------------------------------------------*/
int32 OS_Posix_BinSemAPI_Impl_Init(void)
{
   memset(OS_impl_bin_sem_table, 0, sizeof(OS_impl_bin_sem_table));
   return OS_SUCCESS;
} /* end OS_Posix_BinSemAPI_Impl_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemCreate_Impl (uint32 sem_id, uint32 initial_value, uint32 options)
{
    int ret;
    int attr_created;
    int mutex_created;
    int cond_created;
    int32 return_code;
    pthread_mutexattr_t mutex_attr;
    OS_impl_binsem_internal_record_t *sem;

    /*
     * This preserves a bit of pre-existing functionality that was particular to binary sems:
     * if the initial value is greater than 1 it just silently used 1 without error.
     * (by contrast the counting semaphore will return an error)
     */
    if ( initial_value > 1 )
    {
        initial_value = 1;
    }

    attr_created = 0;
    mutex_created = 0;
    cond_created = 0;
    sem = &OS_impl_bin_sem_table[sem_id];
    memset(sem, 0, sizeof (*sem));

    do
    {
        /*
         ** Initialize the pthread mutex attribute structure with default values
         */
        ret = pthread_mutexattr_init(&mutex_attr);
        if (ret != 0)
        {
            OS_DEBUG("Error: pthread_mutexattr_init failed: %s\n",strerror(ret));
            return_code = OS_SEM_FAILURE;
            break;
        }

        /* After this point, the attr object should be destroyed before return */
        attr_created = 1;

        /*
         ** Use priority inheritance
         */
        ret = pthread_mutexattr_setprotocol(&mutex_attr,PTHREAD_PRIO_INHERIT);
        if (ret != 0)
        {
            OS_DEBUG("Error: pthread_mutexattr_setprotocol failed: %s\n",strerror(ret));
            return_code = OS_SEM_FAILURE;
            break;
        }

        /*
         ** Initialize the mutex that is used with the condition variable
         */
        ret = pthread_mutex_init(&(sem->id), &mutex_attr);
        if (ret !=  0)
        {
            OS_DEBUG("Error: pthread_mutex_init failed: %s\n",strerror(ret));
            return_code = OS_SEM_FAILURE;
            break;
        }

        mutex_created = 1;

        /*
         ** Initialize the condition variable
         */
        ret = pthread_cond_init(&(sem->cv), NULL);
        if (ret != 0)
        {
            OS_DEBUG("Error: pthread_cond_init failed: %s\n",strerror(ret));
            return_code = OS_SEM_FAILURE;
            break;
        }

        cond_created = 1;

        /*
         ** fill out the proper OSAL table fields
         */

        sem->current_value = initial_value;

        return_code = OS_SUCCESS;
    }
    while (0);

    /* Clean up resources if the operation failed */
    if (return_code != OS_SUCCESS)
    {
        if (mutex_created)
        {
            pthread_mutex_destroy(&(sem->id));
        }
        if (cond_created)
        {
            pthread_cond_destroy(&(sem->cv));
        }
    }

    if (attr_created)
    {
        /* Done with the attribute object -
         * this call is a no-op in linux - but for other implementations if
         * the create call allocated something this should free it
         */
        pthread_mutexattr_destroy(&mutex_attr);
    }

    return return_code;

} /* end OS_BinSemCreate_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemDelete_Impl (uint32 sem_id)
{
    OS_impl_binsem_internal_record_t *sem;
    int32 return_code;

    sem = &OS_impl_bin_sem_table[sem_id];

    if (pthread_cond_destroy(&(sem->cv)) != 0)
    {
       /* sem could be busy, i.e. some task is pending on it already.
        * that means it cannot be deleted at this time. */
       return_code = OS_SEM_FAILURE;
    }
    else
    {
       /* Now that the CV is destroyed this sem is unusable,
        * so we must do our best to clean everything else.  Even if cleanup
        * does not fully work, returning anything other than OS_SUCCESS would
        * suggest to the caller that the sem is still usable which it is not.
        */
       return_code = OS_SUCCESS;

       /* destroy the associated mutex --
        * Note that this might fail if the mutex is locked,
        * but there is no sane way to recover from that (see above). */
       pthread_mutex_destroy(&(sem->id));
    }

    return return_code;
} /* end OS_BinSemDelete_Impl */



/*----------------------------------------------------------------
 *
 * Function: OS_BinSemGive_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemGive_Impl ( uint32 sem_id )
{
    OS_impl_binsem_internal_record_t *sem;

    sem = &OS_impl_bin_sem_table[sem_id];

    /*
     * Note there is a possibility that another thread is concurrently taking this sem,
     * and has just checked the current_value but not yet inside the cond_wait call.
     *
     * To address this possibility - the lock must be taken here.  This is unfortunate
     * as it means there may be a task switch when _giving_ a binary semaphore.  But the
     * alternative of having a BinSemGive not wake up the other thread is a bigger issue.
     *
     * Counting sems do not suffer from this, as there is a native POSIX mechanism for those.
     */

    /* Lock the mutex ( not the table! ) */
    if ( pthread_mutex_lock(&(sem->id)) != 0 )
    {
       return(OS_SEM_FAILURE);
    }

    /* Binary semaphores are always set as "1" when given */
    sem->current_value = 1;

    /* unblock one thread that is waiting on this sem */
    pthread_cond_signal(&(sem->cv));

    pthread_mutex_unlock(&(sem->id));

    return OS_SUCCESS;
} /* end OS_BinSemGive_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemFlush_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemFlush_Impl (uint32 sem_id)
{
    OS_impl_binsem_internal_record_t *sem;

    sem = &OS_impl_bin_sem_table[sem_id];

    /* Lock the mutex ( not the table! ) */
    if ( pthread_mutex_lock(&(sem->id)) != 0 )
    {
       return(OS_SEM_FAILURE);
    }

    /* increment the flush counter.  Any other threads that are
     * currently pending in SemTake() will see the counter change and
     * return _without_ modifying the semaphore count.
     */
    ++sem->flush_request;

    /* unblock all threads that are be waiting on this sem */
    pthread_cond_broadcast(&(sem->cv));

    pthread_mutex_unlock(&(sem->id));

    return OS_SUCCESS;
} /* end OS_BinSemFlush_Impl */

/*---------------------------------------------------------------------------------------
   Name: OS_GenericBinSemTake_Impl

   Purpose: Helper function that takes a simulated binary semaphore with a "timespec" timeout
            If the value is zero this will block until either the value
            becomes nonzero (via SemGive) or the semaphore gets flushed.

---------------------------------------------------------------------------------------*/
static int32 OS_GenericBinSemTake_Impl (OS_impl_binsem_internal_record_t *sem, const struct timespec *timeout)
{
   sig_atomic_t flush_count;
   int32 return_code;

   /* Lock the mutex ( not the table! ) */
   if ( pthread_mutex_lock(&(sem->id)) != 0 )
   {
      return(OS_SEM_FAILURE);
   }

   return_code = OS_SUCCESS;

   /*
    * Note that for vxWorks compatibility, we need to stop pending on the semaphore
    * and return from this function under two possible circumstances:
    *
    *  a) the semaphore count was nonzero (may be pre-existing or due to a give)
    *     this is the normal case, we should decrement the count by 1 and return.
    *  b) the semaphore got "flushed"
    *     in this case ALL tasks are un-blocked and we do NOT decrement the count.
    */

   /*
    * first take a local snapshot of the flush request counter,
    * if it changes, we know that someone else called SemFlush.
    */
   flush_count = sem->flush_request;

   /* Note - the condition must be checked in a while loop because
    * even if pthread_cond_wait() returns, it does NOT guarantee that
    * the condition we are looking for has been met.
    *
    * Also if the current_value is already nonzero we will not wait.
    */
   while ( sem->current_value == 0 && sem->flush_request == flush_count )
   {
      /* Must pend until something changes */
      if (timeout == NULL)
      {
         /* wait forever */
         pthread_cond_wait(&(sem->cv),&(sem->id));
      }
      else if (pthread_cond_timedwait(&(sem->cv),&(sem->id),timeout) == ETIMEDOUT)
      {
         return_code = OS_SEM_TIMEOUT;
         break;
      }
   }

   /* If the flush counter did not change, set the value to zero */
   if (return_code == OS_SUCCESS && sem->flush_request == flush_count)
   {
      sem->current_value = 0;
   }

   pthread_mutex_unlock(&(sem->id));

   return return_code;
} /* end OS_GenericBinSemTake_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemTake_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemTake_Impl ( uint32 sem_id )
{
   return (OS_GenericBinSemTake_Impl (&OS_impl_bin_sem_table[sem_id], NULL));
} /* end OS_BinSemTake_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemTimedWait_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
{
   struct timespec ts;

   /*
    ** Compute an absolute time for the delay
    */
   OS_Posix_CompAbsDelayTime(msecs, &ts);

   return (OS_GenericBinSemTake_Impl (&OS_impl_bin_sem_table[sem_id], &ts));
} /* end OS_BinSemTimedWait_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemGetInfo_Impl (uint32 sem_id, OS_bin_sem_prop_t *sem_prop)
{
    /* put the info into the stucture */
    sem_prop -> value = OS_impl_bin_sem_table[sem_id].current_value;
    return OS_SUCCESS;
} /* end OS_BinSemGetInfo_Impl */

