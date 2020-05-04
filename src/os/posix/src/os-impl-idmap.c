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
 * \file     os-impl-idmap.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "bsp-impl.h"
#include <sched.h>

#include "os-shared-idmap.h"

typedef struct
{
   pthread_mutex_t mutex;
   sigset_t sigmask;
} POSIX_GlobalLock_t;

static POSIX_GlobalLock_t OS_global_task_table_mut;
static POSIX_GlobalLock_t OS_queue_table_mut;
static POSIX_GlobalLock_t OS_bin_sem_table_mut;
static POSIX_GlobalLock_t OS_mutex_table_mut;
static POSIX_GlobalLock_t OS_count_sem_table_mut;
static POSIX_GlobalLock_t OS_stream_table_mut;
static POSIX_GlobalLock_t OS_dir_table_mut;
static POSIX_GlobalLock_t OS_timebase_table_mut;
static POSIX_GlobalLock_t OS_module_table_mut;
static POSIX_GlobalLock_t OS_filesys_table_mut;
static POSIX_GlobalLock_t OS_console_mut;

static POSIX_GlobalLock_t * const MUTEX_TABLE[] =
      {
            [OS_OBJECT_TYPE_UNDEFINED] = NULL,
            [OS_OBJECT_TYPE_OS_TASK] = &OS_global_task_table_mut,
            [OS_OBJECT_TYPE_OS_QUEUE] = &OS_queue_table_mut,
            [OS_OBJECT_TYPE_OS_COUNTSEM] = &OS_count_sem_table_mut,
            [OS_OBJECT_TYPE_OS_BINSEM] = &OS_bin_sem_table_mut,
            [OS_OBJECT_TYPE_OS_MUTEX] = &OS_mutex_table_mut,
            [OS_OBJECT_TYPE_OS_STREAM] = &OS_stream_table_mut,
            [OS_OBJECT_TYPE_OS_DIR] = &OS_dir_table_mut,
            [OS_OBJECT_TYPE_OS_TIMEBASE] = &OS_timebase_table_mut,
            [OS_OBJECT_TYPE_OS_MODULE] = &OS_module_table_mut,
            [OS_OBJECT_TYPE_OS_FILESYS] = &OS_filesys_table_mut,
            [OS_OBJECT_TYPE_OS_CONSOLE] = &OS_console_mut,
      };

enum
{
   MUTEX_TABLE_SIZE = (sizeof(MUTEX_TABLE) / sizeof(MUTEX_TABLE[0]))
};

/*----------------------------------------------------------------
 *
 * Function: OS_Lock_Global_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_Lock_Global_Impl(uint32 idtype)
{
   POSIX_GlobalLock_t *mut;
   sigset_t previous;

   if (idtype < MUTEX_TABLE_SIZE)
   {
      mut = MUTEX_TABLE[idtype];
   }
   else
   {
      mut = NULL;
   }

   if (mut == NULL)
   {
      return OS_ERROR;
   }

   if (pthread_sigmask(SIG_SETMASK, &POSIX_GlobalVars.MaximumSigMask, &previous) != 0)
   {
      return OS_ERROR;
   }

   if (pthread_mutex_lock(&mut->mutex) != 0)
   {
      return OS_ERROR;
   }

   /* Only set values inside the GlobalLock _after_ it is locked */
   mut->sigmask = previous;

   return OS_SUCCESS;
} /* end OS_Lock_Global_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_Unlock_Global_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_Unlock_Global_Impl(uint32 idtype)
{
   POSIX_GlobalLock_t *mut;
   sigset_t previous;

   if (idtype < MUTEX_TABLE_SIZE)
   {
      mut = MUTEX_TABLE[idtype];
   }
   else
   {
      mut = NULL;
   }

   if (mut == NULL)
   {
      return OS_ERROR;
   }

   /* Only get values inside the GlobalLock _before_ it is unlocked */
   previous = mut->sigmask;

   if (pthread_mutex_unlock(&mut->mutex) != 0)
   {
      return OS_ERROR;
   }

   pthread_sigmask(SIG_SETMASK, &previous, NULL);

   return OS_SUCCESS;
} /* end OS_Unlock_Global_Impl */


/*---------------------------------------------------------------------------------------
   Name: OS_Posix_TableMutex_Init

   Purpose: Initialize the mutex that the OS API uses for the shared state tables

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_Posix_TableMutex_Init(uint32 idtype)
{
    int                 ret;
    int32               return_code = OS_SUCCESS;
    pthread_mutexattr_t mutex_attr;

    do
    {
        if (idtype >= MUTEX_TABLE_SIZE)
        {
            break;
        }

        /* Initialize the table mutex for the given idtype */
        if (MUTEX_TABLE[idtype] == NULL)
        {
            break;
        }

        /*
         ** initialize the pthread mutex attribute structure with default values
         */
        ret = pthread_mutexattr_init(&mutex_attr);
        if ( ret != 0 )
        {
            OS_DEBUG("Error: pthread_mutexattr_init failed: %s\n",strerror(ret));
            return_code = OS_ERROR;
            break;
        }

        /*
         ** Allow the mutex to use priority inheritance
         */
        ret = pthread_mutexattr_setprotocol(&mutex_attr,PTHREAD_PRIO_INHERIT) ;
        if ( ret != 0 )
        {
            OS_DEBUG("Error: pthread_mutexattr_setprotocol failed: %s\n",strerror(ret));
            return_code = OS_ERROR;
            break;
        }

        /*
         **  Set the mutex type to RECURSIVE so a thread can do nested locks
         **  TBD - not sure if this is really desired, but keep it for now.
         */
        ret = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
        if ( ret != 0 )
        {
            OS_DEBUG("Error: pthread_mutexattr_settype failed: %s\n",strerror(ret));
            return_code = OS_ERROR;
            break;
        }

        ret = pthread_mutex_init(&MUTEX_TABLE[idtype]->mutex, &mutex_attr);
        if ( ret != 0 )
        {
            OS_DEBUG("Error: pthread_mutex_init failed: %s\n",strerror(ret));
            return_code = OS_ERROR;
            break;
        }
   }
   while (0);


   return(return_code);
} /* end OS_Posix_TableMutex_Init */

