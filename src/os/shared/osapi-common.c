/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osapi-common.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *         Instantiates the global object tables and the overall OSAL
 *         init/teardown logic such as OS_API_Init() and OS_ApplicationExit().
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false,
            .PrintfEnabled = false,
            .ShutdownFlag = 0,
            .MicroSecPerTick = 0, /* invalid, _must_ be set by implementation init */
            .TicksPerSecond = 0,  /* invalid, _must_ be set by implementation init */
      };

/*
 *********************************************************************************
 *          PUBLIC API (application-callable functions)
 *********************************************************************************
 */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_API_Init
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_API_Init(void)
{
   int32  return_code = OS_SUCCESS;
   uint32 idtype;

   if (OS_SharedGlobalVars.Initialized != false)
   {
       OS_DEBUG("WARNING: BUG - initialization function called multiple times\n");
       return OS_ERROR;
   }

   OS_SharedGlobalVars.Initialized = true;

   /* Initialize the common table that everything shares */
   return_code = OS_ObjectIdInit();
   if (return_code != OS_SUCCESS)
   {
       return return_code;
   }

   for (idtype = 0; idtype < OS_OBJECT_TYPE_USER; ++idtype)
   {
       /* Initialize the implementation first, as the shared layer depends on it */
       return_code = OS_API_Impl_Init(idtype);
       if (return_code != OS_SUCCESS)
       {
           OS_DEBUG("OS_API_Impl_Init(0x%x) failed to initialize: %d\n",(unsigned int)idtype,(int)return_code);
           break;
       }

       switch(idtype)
       {
       case OS_OBJECT_TYPE_OS_TASK:
           return_code = OS_TaskAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_QUEUE:
           return_code = OS_QueueAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_BINSEM:
           return_code = OS_BinSemAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_COUNTSEM:
           return_code = OS_CountSemAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_MUTEX:
           return_code = OS_MutexAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_MODULE:
           return_code = OS_ModuleAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_TIMEBASE:
           return_code = OS_TimeBaseAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_TIMECB:
           return_code = OS_TimerCbAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_STREAM:
           return_code = OS_FileAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_DIR:
           return_code = OS_DirAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_FILESYS:
           return_code = OS_FileSysAPI_Init();
           break;
       case OS_OBJECT_TYPE_OS_CONSOLE:
           return_code = OS_ConsoleAPI_Init();
           break;
       default:
           break;
       }
       if (return_code != OS_SUCCESS)
       {
           OS_DEBUG("ID type 0x%x shared layer failed to initialize: %d\n",(unsigned int)idtype,(int)return_code);
           break;
       }

   }

   if (return_code == OS_SUCCESS)
   {
       return_code = OS_NetworkAPI_Init();
   }

   if (return_code == OS_SUCCESS)
   {
       return_code = OS_SocketAPI_Init();
   }

   /*
    * Confirm that somewhere during initialization,
    * the time variables got set to something valid
    */
   if (return_code == OS_SUCCESS &&
         (OS_SharedGlobalVars.MicroSecPerTick == 0 ||
         OS_SharedGlobalVars.TicksPerSecond == 0))
   {
      OS_DEBUG("Implementation failed to initialize tick time globals\n");
      return_code = OS_ERROR;
   }

   return(return_code);
} /* end OS_API_Init */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ApplicationExit
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_ApplicationExit(int32 Status)
{
   if (Status == OS_SUCCESS)
   {
      exit(EXIT_SUCCESS);
   }
   else
   {
      exit(EXIT_FAILURE);
   }
} /* end OS_ApplicationExit */

/*---------------------------------------------------------------------------------------
   Name: OS_CleanUpObject

   Purpose: Implements a single API call that can delete ANY object
            Will dispatch to the correct delete implementation for that object type

   Returns: None

---------------------------------------------------------------------------------------*/
                        
/*----------------------------------------------------------------
 *
 * Function: OS_CleanUpObject
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
void OS_CleanUpObject(uint32 object_id, void *arg)
{
    uint32 *ObjectCount;

    ObjectCount = (uint32 *)arg;
    ++(*ObjectCount);
    switch(OS_IdentifyObject(object_id))
    {
    case OS_OBJECT_TYPE_OS_TASK:
        OS_TaskDelete(object_id);
        break;
    case OS_OBJECT_TYPE_OS_QUEUE:
        OS_QueueDelete(object_id);
        break;
    case OS_OBJECT_TYPE_OS_BINSEM:
        OS_BinSemDelete(object_id);
        break;
    case OS_OBJECT_TYPE_OS_COUNTSEM:
        OS_CountSemDelete(object_id);
        break;
    case OS_OBJECT_TYPE_OS_MUTEX:
        OS_MutSemDelete(object_id);
        break;
    case OS_OBJECT_TYPE_OS_MODULE:
        OS_ModuleUnload(object_id);
        break;
    case OS_OBJECT_TYPE_OS_TIMEBASE:
        OS_TimeBaseDelete(object_id);
        break;
    case OS_OBJECT_TYPE_OS_TIMECB:
        OS_TimerDelete(object_id);
        break;
    case OS_OBJECT_TYPE_OS_STREAM:
        OS_close(object_id);
        break;
    case OS_OBJECT_TYPE_OS_DIR:
        OS_DirectoryClose(object_id);
        break;
    default:
        break;
    }
} /* end OS_CleanUpObject */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_DeleteAllObjects
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_DeleteAllObjects(void)
{
    uint32 ObjectCount;
    uint32 TryCount;

    /*
     * Note - this is done in a loop because some objects depend on other objects
     * and you will not be able to delete the object until the ref count becomes zero.
     */
    TryCount = 0;
    while(true)
    {
        ObjectCount = 0;
        ++TryCount;
        OS_ForEachObject(0, OS_CleanUpObject, &ObjectCount);
        if (ObjectCount == 0 || TryCount > 4)
        {
           break;
        }
        OS_TaskDelay(5);
    }
    while (ObjectCount > 0 && TryCount < 5);
} /* end OS_DeleteAllObjects */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_IdleLoop
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_IdleLoop()
{
    /*
     * Wait until the shutdown value is set to the shutdown number
     * In most "real" embedded systems, this will never happen.
     * However it will happen in debugging situations (CTRL+C, etc).
     */
   while (OS_SharedGlobalVars.ShutdownFlag != OS_SHUTDOWN_MAGIC_NUMBER)
   {
      OS_IdleLoop_Impl();
   }
} /* end OS_IdleLoop */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_ApplicationShutdown
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_ApplicationShutdown(uint8 flag)
{
   if (flag == true)
   {
      OS_SharedGlobalVars.ShutdownFlag = OS_SHUTDOWN_MAGIC_NUMBER;
   }

   /*
    * Hook to allow the underlying implementation to do something.
    * Assuming the main task is sitting in OS_IdleLoop(), this implementation
    * should do whatever is needed to wake that task up.
    */
   OS_ApplicationShutdown_Impl();
} /* end OS_ApplicationShutdown */

