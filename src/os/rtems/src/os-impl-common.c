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
 * \file     os-impl-common.c
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 *
 *      NOTE: This uses only the "Classic" RTEMS API.  It is intended to
 *      work on RTEMS targets that do not provide the POSIX API, i.e.
 *      when "--disable-posix" is given during the configuration stage.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-shared-common.h"

RTEMS_GlobalVars_t RTEMS_GlobalVars = { 0 };

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_API_Init

   Purpose: Initialize the tables that the OS API uses to keep track of information
            about objects

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_API_Impl_Init(uint32 idtype)
{
   int32               return_code;

   return_code = OS_Rtems_TableMutex_Init(idtype);
   if (return_code != OS_SUCCESS)
   {
       return return_code;
   }

   switch(idtype)
   {
   case OS_OBJECT_TYPE_OS_TASK:
       return_code = OS_Rtems_TaskAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_QUEUE:
       return_code = OS_Rtems_QueueAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_BINSEM:
       return_code = OS_Rtems_BinSemAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_COUNTSEM:
       return_code = OS_Rtems_CountSemAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_MUTEX:
       return_code = OS_Rtems_MutexAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_MODULE:
       return_code = OS_Rtems_ModuleAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_TIMEBASE:
       return_code = OS_Rtems_TimeBaseAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_STREAM:
       return_code = OS_Rtems_StreamAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_DIR:
       return_code = OS_Rtems_DirAPI_Impl_Init();
       break;
   case OS_OBJECT_TYPE_OS_FILESYS:
       return_code = OS_Rtems_FileSysAPI_Impl_Init();
       break;
   default:
       break;
   }

   return(return_code);
} /* end OS_API_Impl_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_IdleLoop_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_IdleLoop_Impl()
{
    RTEMS_GlobalVars.IdleTaskId = rtems_task_self();
    rtems_task_suspend(RTEMS_SELF);
} /* end OS_IdleLoop_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_ApplicationShutdown_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_ApplicationShutdown_Impl()
{
   /* Note that setting the IdleTaskId and suspending
    * the idle task is not an atomic operation, so there
    * is a remote chance that this could attempt to
    * resume a task that is not yet suspended. */
   rtems_task_resume(RTEMS_GlobalVars.IdleTaskId);
} /* end OS_ApplicationShutdown_Impl */


