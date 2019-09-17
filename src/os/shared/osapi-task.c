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
 * \file   osapi-task.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *         This code only uses very basic C library calls that are expected
 *         to be available on every sane C-language compiler.  For everything else,
 *         a platform-specific implementation function is used.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"


/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_TASKS) || (OS_MAX_TASKS <= 0)
#error "osconfig.h must define OS_MAX_TASKS to a valid value"
#endif


enum
{
   LOCAL_NUM_OBJECTS = OS_MAX_TASKS,
   LOCAL_OBJID_TYPE = OS_OBJECT_TYPE_OS_TASK
};

OS_task_internal_record_t    OS_task_table          [LOCAL_NUM_OBJECTS];


/*----------------------------------------------------------------
 *
 * Function: OS_TaskPrepare
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Helper function for registering new tasks in the global database.
 *           This maps the given task_id back to the array entry (OS_task_internal_record_t)
 *           so that the caller can call the real entry point.
 *
 *           In the process, this also verifies that the task_id is valid and
 *           it matches the expected entry, and this calls the implementation's
 *           "Register" function to make sure that the appropriate thread-specific
 *           variables are set - this guarantees that GetTaskId will work.
 *
 *
 *-----------------------------------------------------------------*/
static int32 OS_TaskPrepare(uint32 task_id, osal_task_entry *entrypt)
{
   int32 return_code;
   uint32 local_id;

   return_code = OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TASK, task_id, &local_id);
   if (return_code == OS_SUCCESS)
   {
      /*
       * Take our own task table lock.
       *
       * This ensures that the parent thread's OS_TaskCreate() call is fully completed,
       * and that nobody can call OS_TaskDelete() and possibly overwrite this data.
       */
      OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_TASK);

      /*
       * Verify that we still appear to own the table entry
       */
      if (OS_global_task_table[local_id].active_id != task_id)
      {
          return_code = OS_ERR_INVALID_ID;
      }
      else
      {
          return_code = OS_TaskMatch_Impl(local_id);
          *entrypt = OS_task_table[local_id].entry_function_pointer;
      }

      OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TASK);
   }

   if (return_code == OS_SUCCESS)
   {
      OS_TaskRegister_Impl(task_id);
   }
   else
   {
      *entrypt = NULL;
   }

   return return_code;
} /* end OS_TaskPrepare */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskEntryPoint
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           The entry point for all OSAL tasks
 *           This function is called from the OS-specific layers after a task is spawned
 *           and is the first thing to run under the context of the task itself.
 *           This will register the task appropriately in the global data structures and
 *           call the user's intended entry point function.
 *
 *-----------------------------------------------------------------*/
void OS_TaskEntryPoint(uint32 task_id)
{
    osal_task_entry task_entry;

   if (OS_TaskPrepare(task_id, &task_entry) == OS_SUCCESS)
   {
      if (task_entry != NULL)
      {
          (*task_entry)();
      }
   }

   /* If the function returns, treat as a normal exit and do the proper cleanup */
   OS_TaskExit();
} /* end OS_TaskEntryPoint */

/*
 *********************************************************************************
 *          TASK API
 *********************************************************************************
 */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskAPI_Init(void)
{
   memset(OS_task_table, 0, sizeof(OS_task_table));
   return OS_SUCCESS;
} /* end OS_TaskAPI_Init */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskCreate
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskCreate (uint32 *task_id, const char *task_name, osal_task_entry function_pointer,
                      uint32 *stack_pointer, uint32 stack_size, uint32 priority, uint32 flags)
{
   OS_common_record_t *record;
   int32             return_code;
   uint32            local_id;

   /* Check for bad priority */
   if (priority > OS_MAX_TASK_PRIORITY)
   {
       return OS_ERR_INVALID_PRIORITY;
   }

   /* Check for NULL pointers */
   if (task_name == NULL || task_id == NULL || function_pointer == NULL)
   {
      return OS_INVALID_POINTER;
   }

   /* we don't want to allow names too long*/
   /* if truncated, two names might be the same */
   if (strlen(task_name) >= OS_MAX_API_NAME)
   {
      return OS_ERR_NAME_TOO_LONG;
   }

   /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
   return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, task_name, &local_id, &record);
   if(return_code == OS_SUCCESS)
   {
      /* Save all the data to our own internal task table */
      memset(&OS_task_table[local_id], 0, sizeof(OS_task_internal_record_t));

      strncpy(OS_task_table[local_id].task_name, task_name, OS_MAX_API_NAME);
      record->name_entry = OS_task_table[local_id].task_name;
      OS_task_table[local_id].stack_size = stack_size;
      OS_task_table[local_id].priority = priority;
      OS_task_table[local_id].entry_function_pointer = function_pointer;
      OS_task_table[local_id].stack_pointer = stack_pointer;

      /* Now call the OS-specific implementation.  This reads info from the task table. */
      return_code = OS_TaskCreate_Impl(local_id, flags);

      /* Check result, finalize record, and unlock global table. */
      return_code = OS_ObjectIdFinalizeNew(return_code, record, task_id);
   }



   return return_code;
} /* end OS_TaskCreate */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskDelete
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelete (uint32 task_id)
{
   OS_common_record_t *record;
   int32             return_code;
   uint32            local_id;
   osal_task_entry   delete_hook;

   delete_hook = NULL;
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, task_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      /* Save the delete hook, as we do not want to call it while locked */
      delete_hook = OS_task_table[local_id].delete_hook_pointer;

      return_code = OS_TaskDelete_Impl(local_id);

      /* Free the entry in the master table now while still locked */
      if (return_code == OS_SUCCESS)
      {
         /* Only need to clear the ID as zero is the "unused" flag */
         record->active_id = 0;
      }
      else
      {
         delete_hook = NULL;
      }

      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   /*
   ** Call the thread Delete hook if there is one.
   */
   if (delete_hook != NULL)
   {
      delete_hook();
   }

   return return_code;
} /* end OS_TaskDelete */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskExit
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_TaskExit()
{
   OS_common_record_t *record;
   uint32 task_id;
   uint32 local_id;

   task_id = OS_TaskGetId_Impl();
   if (OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, task_id, &local_id, &record) == OS_SUCCESS)
   {
      /* Only need to clear the ID as zero is the "unused" flag */
      record->active_id = 0;
      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   /* call the implementation */
   OS_TaskExit_Impl();

   /* Impl function never returns */
} /* end OS_TaskExit */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskDelay
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelay(uint32 millisecond)
{
   /* just call the implementation */
   return OS_TaskDelay_Impl(millisecond);
} /* end OS_TaskDelay */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskSetPriority
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskSetPriority (uint32 task_id, uint32 new_priority)
{
   OS_common_record_t *record;
   int32             return_code;
   uint32            local_id;

   if (new_priority > OS_MAX_TASK_PRIORITY)
   {
      return_code = OS_ERR_INVALID_PRIORITY;
   }
   else
   {
      return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL,LOCAL_OBJID_TYPE, task_id, &local_id, &record);
      if (return_code == OS_SUCCESS)
      {
         return_code = OS_TaskSetPriority_Impl(local_id, new_priority);

         if (return_code == OS_SUCCESS)
         {
            /* Use the abstracted priority, not the OS one */
            /* Change the priority in the table as well */
            OS_task_table[local_id].priority = new_priority;
         }

         /* Unlock the global from OS_ObjectIdGetAndLock() */
         OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
      }
   }

   return return_code;
} /* end OS_TaskSetPriority */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskRegister
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskRegister (void)
{
    OS_common_record_t *record;
    uint32 local_id;

    /*
     * Just to retain compatibility (really, only the unit test cares)
     * this will return NON success when called from a non-task context
     */
    return OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, OS_TaskGetId_Impl(), &local_id, &record);
} /* end OS_TaskRegister */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetId
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
uint32 OS_TaskGetId (void)
{
   OS_common_record_t *record;
   uint32 local_id;
   uint32 task_id;

   task_id = OS_TaskGetId_Impl();

   /* Confirm the task master table entry matches the expected.
    * If not it means we have some stale/leftover value */
   if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, task_id, &local_id, &record) != OS_SUCCESS)
   {
      task_id = 0;
   }

   return(task_id);
} /* end OS_TaskGetId */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskGetIdByName (uint32 *task_id, const char *task_name)
{
   int32 return_code;

   if (task_id == NULL || task_name == NULL)
   {
       return OS_INVALID_POINTER;
   }

   return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, task_name, task_id);

   return return_code;

} /* end OS_TaskGetIdByName */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskGetInfo (uint32 task_id, OS_task_prop_t *task_prop)
{
   OS_common_record_t *record;
   int32             return_code;
   uint32            local_id;

   /* Check parameters */
   if (task_prop == NULL)
   {
      return OS_INVALID_POINTER;
   }

   memset(task_prop,0,sizeof(OS_task_prop_t));

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL,LOCAL_OBJID_TYPE, task_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      if (record->name_entry != NULL)
      {
          strncpy(task_prop->name, record->name_entry, sizeof(task_prop->name)-1);
          task_prop->name[sizeof(task_prop->name)-1] = 0;
      }
      task_prop->creator =    record->creator;
      task_prop->stack_size = OS_task_table[local_id].stack_size;
      task_prop->priority =   OS_task_table[local_id].priority;

      return_code = OS_TaskGetInfo_Impl(local_id, task_prop);

      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   return return_code;

} /* end OS_TaskGetInfo */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TaskInstallDeleteHandler
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer)
{
   OS_common_record_t *record;
   int32 return_code;
   uint32 local_id;
   uint32 task_id;

   task_id = OS_TaskGetId_Impl();
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, task_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      /*
      ** Install the pointer
      */
      OS_task_table[local_id].delete_hook_pointer = function_pointer;

      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   return return_code;
} /* end OS_TaskInstallDeleteHandler */
