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
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define OSAL_TABLE_MUTEX_ATTRIBS    (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY)

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

rtems_id            OS_task_table_sem;
rtems_id            OS_queue_table_sem;
rtems_id            OS_bin_sem_table_sem;
rtems_id            OS_mutex_table_sem;
rtems_id            OS_count_sem_table_sem;
rtems_id            OS_stream_table_mut;
rtems_id            OS_dir_table_mut;
rtems_id            OS_timebase_table_mut;
rtems_id            OS_module_table_mut;
rtems_id            OS_filesys_table_mut;
rtems_id            OS_console_mut;

static rtems_id * const MUTEX_TABLE[] =
      {
            [OS_OBJECT_TYPE_UNDEFINED] = NULL,
            [OS_OBJECT_TYPE_OS_TASK] = &OS_task_table_sem,
            [OS_OBJECT_TYPE_OS_QUEUE] = &OS_queue_table_sem,
            [OS_OBJECT_TYPE_OS_COUNTSEM] = &OS_count_sem_table_sem,
            [OS_OBJECT_TYPE_OS_BINSEM] = &OS_bin_sem_table_sem,
            [OS_OBJECT_TYPE_OS_MUTEX] = &OS_mutex_table_sem,
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
   rtems_id *mut;

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

   if (rtems_semaphore_obtain(*mut, RTEMS_WAIT, RTEMS_NO_TIMEOUT) != 0)
   {
      return OS_ERROR;
   }

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
   rtems_id *mut;

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

   if (rtems_semaphore_release(*mut) != 0)
   {
      return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_Unlock_Global_Impl */



/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_Rtems_TableMutex_Init

   Purpose: Initialize the tables that the OS API uses to keep track of information
            about objects

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_Rtems_TableMutex_Init(uint32 idtype)
{
    int32               return_code = OS_SUCCESS;
    rtems_status_code   rtems_sc;

    /* Initialize the table mutex for the given idtype */
    if (idtype < MUTEX_TABLE_SIZE && MUTEX_TABLE[idtype] != NULL)
    {
        rtems_sc = rtems_semaphore_create (idtype,
                1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                MUTEX_TABLE[idtype]);

        if ( rtems_sc != RTEMS_SUCCESSFUL )
        {
            OS_DEBUG("Error: rtems_semaphore_create failed: %s\n", rtems_status_text(rtems_sc));
            return_code = OS_ERROR;
        }
    }

    return(return_code);
} /* end OS_Rtems_TableMutex_Init */
