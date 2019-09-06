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
 * \file   osapi.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 *
 *      NOTE: This uses only the "Classic" RTEMS API.  It is intended to
 *      work on RTEMS targets that do not provide the POSIX API, i.e.
 *      when "--disable-posix" is given during the configuration stage.
 *
 *      If the RTEMS POSIX API is enabled, then it may be possible to
 *      use the POSIX OSAL which is more full featured.
 */
/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"

#include <rtems/libcsupport.h>

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define OSAL_CONSOLE_STREAM         stdout
#define RTEMS_INT_LEVEL_ENABLE_ALL  0
#define RTEMS_INT_LEVEL_DISABLE_ALL 7
#define MAX_SEM_VALUE               0x7FFFFFFF

/*
 * Define all of the RTEMS semaphore attributes
 * 1. The TABLE_MUTEX attributes are for the internal OSAL tables.
 *    In RTEMS, a MUTEX is defined as a binary semaphore
 *    It allows nested locks, priority wait order, and supports priority inheritance
 *
 * 2. OSAL Mutex attributes -- same as 1
 *
 * 3. OSAL Binary Semaphore attributes
 *    This is a simple binary semaphore used for synchronization. It does not
 *    allow nested calls ( nor should it ) It should not be used for mutual exclusion.
 *
 * 4. OSAL Counting Semaphore attributes
 *     This is a counting semaphore with priority wait order.
 *
 */

#define OSAL_RTEMS_INHERIT_PRIO            RTEMS_INHERIT_PRIORITY

#define OSAL_TABLE_MUTEX_ATTRIBS (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | OSAL_RTEMS_INHERIT_PRIO)

#define OSAL_MUTEX_ATTRIBS (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | OSAL_RTEMS_INHERIT_PRIO)

#define OSAL_BINARY_SEM_ATTRIBS (RTEMS_SIMPLE_BINARY_SEMAPHORE | RTEMS_PRIORITY)

#define OSAL_COUNT_SEM_ATTRIBS (RTEMS_PRIORITY)

/*
 * By default use the stdout stream for the console (OS_printf)
 */
#define OSAL_CONSOLE_FILENO     STDOUT_FILENO

/*
 * By default the console output is always asynchronous
 * (equivalent to "OS_UTILITY_TASK_ON" being set)
 *
 * This option was removed from osconfig.h and now is
 * assumed to always be on.
 */
#define OS_CONSOLE_ASYNC                true
#define OS_CONSOLE_TASK_PRIORITY        OS_UTILITYTASK_PRIORITY
#define OS_CONSOLE_TASK_STACKSIZE       OS_UTILITYTASK_STACK_SIZE


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/
/*  tables for the properties of objects */

/*tasks */
typedef struct
{
    rtems_id id;
} OS_impl_internal_record_t;

/* Console device */
typedef struct
{
    bool            is_async;
    rtems_id        data_sem;
    int             out_fd;
} OS_impl_console_internal_record_t;


/* Tables where the OS object information is stored */
OS_impl_internal_record_t    OS_impl_task_table          [OS_MAX_TASKS];
OS_impl_internal_record_t    OS_impl_queue_table         [OS_MAX_QUEUES];
OS_impl_internal_record_t    OS_impl_bin_sem_table       [OS_MAX_BIN_SEMAPHORES];
OS_impl_internal_record_t    OS_impl_count_sem_table     [OS_MAX_COUNT_SEMAPHORES];
OS_impl_internal_record_t    OS_impl_mut_sem_table       [OS_MAX_MUTEXES];
OS_impl_console_internal_record_t   OS_impl_console_table       [OS_MAX_CONSOLES];

rtems_id            OS_task_table_sem;
rtems_id            OS_queue_table_sem;
rtems_id            OS_bin_sem_table_sem;
rtems_id            OS_mut_sem_table_sem;
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
            [OS_OBJECT_TYPE_OS_MUTEX] = &OS_mut_sem_table_sem,
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

const OS_ErrorTable_Entry_t OS_IMPL_ERROR_NAME_TABLE[] = { { 0, NULL } };

RTEMS_GlobalVars_t RTEMS_GlobalVars = { 0 };


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
}

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
}



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
   int32               return_code = OS_SUCCESS;
   rtems_status_code   rtems_sc;

   do
   {
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
              break;
          }
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
   }
   while (0);


   return(return_code);
}

/*---------------------------------------------------------------------------------------
   Name: OS_IdleLoop

   Purpose: Wait for external events.

   returns: no value
---------------------------------------------------------------------------------------*/
void OS_IdleLoop_Impl()
{
    RTEMS_GlobalVars.IdleTaskId = rtems_task_self();
    rtems_task_suspend(RTEMS_SELF);
}

/*---------------------------------------------------------------------------------------
   Name: OS_ApplicationShutdown_Impl

   Purpose: Ensures that the thread waiting in OS_IdleLoop_Impl is waken up

   returns: no value

   NOTE: Might be called from an ISR/signal handler
---------------------------------------------------------------------------------------*/
void OS_ApplicationShutdown_Impl()
{
   /* Note that setting the IdleTaskId and suspending
    * the idle task is not an atomic operation, so there
    * is a remote chance that this could attempt to
    * resume a task that is not yet suspended. */
   rtems_task_resume(RTEMS_GlobalVars.IdleTaskId);
}



/*---------------------------------------------------------------------------------------
   Name: OS_RtemsEntry

   Purpose: A Simple RTEMS-compatible entry point that calls the common task entry function

   NOTES: This wrapper function is only used locally by OS_TaskCreate below

---------------------------------------------------------------------------------------*/
static rtems_task OS_RtemsEntry(rtems_task_argument arg)
{
    OS_TaskEntryPoint((uint32)arg);
}



/****************************************************************************************
                                    TASK API
 ***************************************************************************************/

int32 OS_Rtems_TaskAPI_Impl_Init(void)
{
    memset(OS_impl_task_table, 0, sizeof(OS_impl_task_table));
    return (OS_SUCCESS);
}

/*---------------------------------------------------------------------------------------
   Name: OS_TaskCreate

   Purpose: Creates a task and starts running it.

   returns: OS_INVALID_POINTER if any of the necessary pointers are NULL
            OS_ERR_NAME_TOO_LONG if the name of the task is too long to be copied
            OS_ERR_INVALID_PRIORITY if the priority is bad
            OS_ERR_NO_FREE_IDS if there can be no more tasks created
            OS_ERR_NAME_TAKEN if the name specified is already used by a task
            OS_ERROR if the operating system calls fail
            OS_SUCCESS if success

    NOTES: task_id is passed back to the user as the ID. stack_pointer is usually null.


---------------------------------------------------------------------------------------*/

int32 OS_TaskCreate_Impl (uint32 task_id, uint32 flags)
{
    rtems_status_code  status;
    rtems_name         r_name;
    rtems_mode         r_mode;
    rtems_attribute    r_attributes;

    /*
    ** RTEMS task names are 4 byte integers.
    ** It is convenient to use the OSAL task ID in here, as we know it is already unique
    ** and trying to use the real task name would be less than useful (only 4 chars)
    */
    r_name = OS_global_task_table[task_id].active_id;
    r_mode = RTEMS_PREEMPT | RTEMS_NO_ASR | RTEMS_NO_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0);

    /*
    ** see if the user wants floating point enabled. If
    ** so, then set the correct option.
    */
    r_attributes = RTEMS_LOCAL;
    if (flags & OS_FP_ENABLED)
    {
        r_attributes |= RTEMS_FLOATING_POINT;
    }

    status = rtems_task_create(
                 r_name,
                 OS_task_table[task_id].priority,
                 OS_task_table[task_id].stack_size,
                 r_mode,
                 r_attributes,
                 &OS_impl_task_table[task_id].id);

    /* check if task_create failed */
    if (status != RTEMS_SUCCESSFUL )
    {
        /* Provide some freedback as to why this failed */
        OS_printf("rtems_task_create failed: %s\n", rtems_status_text(status));
        return OS_ERROR;
    }

    /* will place the task in 'ready for scheduling' state */
    status = rtems_task_start (OS_impl_task_table[task_id].id, /*rtems task id*/
			     (rtems_task_entry) OS_RtemsEntry, /* task entry point */
			     (rtems_task_argument) OS_global_task_table[task_id].active_id );  /* passed argument  */

    if (status != RTEMS_SUCCESSFUL )
    {
        OS_printf("rtems_task_start failed: %s\n", rtems_status_text(status));
        rtems_task_delete(OS_impl_task_table[task_id].id);
        return OS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_TaskCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_TaskDelete

    Purpose: Deletes the specified Task and removes it from the OS_task_table.

    returns: OS_ERR_INVALID_ID if the ID given to it is invalid
             OS_ERROR if the OS delete call fails
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_TaskDelete_Impl (uint32 task_id)
{
    /*
    ** Try to delete the task
    ** If this fails, not much recourse - the only potential cause of failure
    ** to cancel here is that the thread ID is invalid because it already exited itself,
    ** and if that is true there is nothing wrong - everything is OK to continue normally.
    */

    rtems_task_delete(OS_impl_task_table[task_id].id);
    return OS_SUCCESS;
}/* end OS_TaskDelete */

/*--------------------------------------------------------------------------------------
     Name:    OS_TaskExit

     Purpose: Exits the calling task and removes it from the OS_task_table.

     returns: Nothing
---------------------------------------------------------------------------------------*/
void OS_TaskExit_Impl()
{
    rtems_task_delete(RTEMS_SELF);

}/*end OS_TaskExit */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskDelay

   Purpose: Delay a task for specified amount of milliseconds

   returns: OS_ERROR if sleep fails
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 OS_TaskDelay_Impl (uint32 milli_second)
{
    rtems_interval     ticks;

    ticks = OS_Milli2Ticks(milli_second);

    rtems_task_wake_after(ticks);
    /*
    ** Always successful ( from RTEMS docs )
    */
    return (OS_SUCCESS);

}/* end OS_TaskDelay */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskSetPriority

   Purpose: Sets the given task to a new priority

    returns: OS_ERR_INVALID_ID if the ID passed to it is invalid
             OS_ERR_INVALID_PRIORITY if the priority is greater than the max
             allowed
             OS_ERROR if the OS call to change the priority fails
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 OS_TaskSetPriority_Impl (uint32 task_id, uint32 new_priority)
{
    rtems_task_priority old_pri;
    rtems_status_code status;

    /* Set RTEMS Task Priority */
    status = rtems_task_set_priority(OS_impl_task_table[task_id].id, new_priority, &old_pri);
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled task_set_priority error: %s\n",rtems_status_text(status));
        return OS_ERROR;
    }

    return OS_SUCCESS;

}/* end OS_TaskSetPriority */

/*--------------------------------------------------------------------------------------
     Name: OS_TaskMatch

    Purpose: Determines if the caller matches the given task_id

    returns: OS_ERROR if not a match
             OS_SUCCESS if match
---------------------------------------------------------------------------------------*/
int32 OS_TaskMatch_Impl(uint32 task_id)
{
    /*
    ** Get RTEMS Task Id
    */
    if ( rtems_task_self() != OS_impl_task_table[task_id].id )
    {
       return(OS_ERROR);
    }


   return OS_SUCCESS;
}

/*---------------------------------------------------------------------------------------
   Name: OS_TaskRegister

   Purpose: Registers the calling task id with the task by adding the var to the tcb

   Returns: OS_ERR_INVALID_ID if there the specified ID could not be found
            OS_ERROR if the OS call fails
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_TaskRegister_Impl (uint32 global_task_id)
{
    /*
     * This is supposed to maintain the "reverse lookup" information used
     * to map an RTEMS task ID back into an OSAL ID.
     *
     * Originally this used "task variables" which got deprecated.
     * So this changed to "task notes" which are also now deprecated in 4.11.
     *
     * So there is now no documented per-task thread local storage facility in RTEMS
     * with these two options gone.  RTEMS does seem to have TLS, but there is just
     * no published (non-deprecated) API to access it.
     *
     * Right now this does nothing and the OS_TaskGetId() must brute-force it.
     *
     * An alternative for performance improvements might be to use a locally maintained
     * hash table here.
     */
    return OS_SUCCESS;

}/* end OS_TaskRegister */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskGetId

   Purpose: This function returns the OSAL task id of the calling task

   Notes: The OS_task_key is initialized by the task switch if AND ONLY IF the
          OS_task_key has been registered via OS_TaskRegister(..).  If this is not
          called prior to this call, the value will be old and wrong.
---------------------------------------------------------------------------------------*/
uint32 OS_TaskGetId_Impl (void)
{
    uint32            global_task_id;
    rtems_id          task_self;
    rtems_name        self_name;
    rtems_status_code status;

    task_self = rtems_task_self();
    /* When the task was created the OSAL ID was used as the "classic name",
     * which gives us an easy way to map it back again */
    status = rtems_object_get_classic_name(task_self, &self_name);
    if (status == RTEMS_SUCCESSFUL)
    {
        global_task_id = self_name;
    }
    else
    {
        global_task_id = 0;
    }

    return global_task_id;

}/* end OS_TaskGetId */

/*---------------------------------------------------------------------------------------
    Name: OS_TaskGetInfo

    Purpose: This function will pass back a pointer to structure that contains
             all of the relevant info (creator, stack size, priority, name) about the
             specified task.

    Returns: OS_ERR_INVALID_ID if the ID passed to it is invalid
             OS_INVALID_POINTER if the task_prop pointer is NULL
             OS_SUCCESS if it copied all of the relevant info over

---------------------------------------------------------------------------------------*/
int32 OS_TaskGetInfo_Impl (uint32 task_id, OS_task_prop_t *task_prop)
{
    task_prop->OStask_id =  (uint32) OS_impl_task_table[task_id].id;
    return OS_SUCCESS;

} /* end OS_TaskGetInfo */


/****************************************************************************************
                                MESSAGE QUEUE API
 ***************************************************************************************/

int32 OS_Rtems_QueueAPI_Impl_Init(void)
{
    memset(OS_impl_queue_table, 0, sizeof(OS_impl_queue_table));
    return (OS_SUCCESS);
}

/*---------------------------------------------------------------------------------------
   Name: OS_QueueCreate

   Purpose: Create a message queue which can be refered to by name or ID

   Returns: OS_INVALID_POINTER if a pointer passed in is NULL
            OS_ERR_NAME_TOO_LONG if the name passed in is too long
            OS_ERR_NO_FREE_IDS if there are already the max queues created
            OS_ERR_NAME_TAKEN if the name is already being used on another queue
            OS_ERROR if the OS create call fails
            OS_SUCCESS if success

   Notes: the flahs parameter is unused.
---------------------------------------------------------------------------------------*/

int32 OS_QueueCreate_Impl (uint32 queue_id, uint32 flags)
{
    rtems_status_code  status;
    rtems_name         r_name;


    /*
    ** RTEMS task names are 4 byte integers.
    ** It is convenient to use the OSAL queue ID in here, as we know it is already unique
    ** and trying to use the real queue name would be less than useful (only 4 chars)
    */
    r_name = OS_global_queue_table[queue_id].active_id;

    /*
    ** Create the message queue.
    ** The queue attributes are set to default values; the waiting order
    ** (RTEMS_FIFO or RTEMS_PRIORITY) is irrelevant since only one task waits
    ** on each queue.
    */
    status = rtems_message_queue_create(
            r_name,                        /* 32-bit RTEMS object name; not used */
            OS_queue_table[queue_id].max_depth,                   /* maximum number of messages in queue (queue depth) */
            OS_queue_table[queue_id].max_size,                    /* maximum size in bytes of a message */
            RTEMS_FIFO|RTEMS_LOCAL,        /* attributes (default) */
            &(OS_impl_queue_table[queue_id].id)  /* object ID returned for queue */
    );

    /*
    ** If the operation failed, report the error
    */
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled queue_create error: %s\n",rtems_status_text(status));
       return OS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_QueueCreate */

/*--------------------------------------------------------------------------------------
    Name: OS_QueueDelete

    Purpose: Deletes the specified message queue.

    Returns: OS_ERR_INVALID_ID if the id passed in does not exist
             OS_ERROR if the OS call to delete the queue fails
             OS_SUCCESS if success

    Notes: If There are messages on the queue, they will be lost and any subsequent
           calls to QueueGet or QueuePut to this queue will result in errors
---------------------------------------------------------------------------------------*/
int32 OS_QueueDelete_Impl (uint32 queue_id)
{
    rtems_status_code status;

    /* Try to delete the queue */
    status = rtems_message_queue_delete(OS_impl_queue_table[queue_id].id);
    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled queue_delete error: %s\n",rtems_status_text(status));
        return OS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_QueueDelete */


/*---------------------------------------------------------------------------------------
   Name: OS_QueueGet

   Purpose: Receive a message on a message queue.  Will pend or timeout on the receive.
   Returns: OS_ERR_INVALID_ID if the given ID does not exist
            OS_INVALID_POINTER if a pointer passed in is NULL
            OS_QUEUE_EMPTY if the Queue has no messages on it to be recieved
            OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
            OS_QUEUE_INVALID_SIZE if the size passed in may be too small for the message
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_QueueGet_Impl (uint32 queue_id, void *data, uint32 size, uint32 *size_copied,
                    int32 timeout)
{
    int32              return_code;
    rtems_status_code  status;
    rtems_interval     ticks;
    rtems_option       option_set;
    size_t             rtems_size;
    rtems_id           rtems_queue_id;

    rtems_queue_id = OS_impl_queue_table[queue_id].id;

    /* Get Message From Message Queue */
    if (timeout == OS_PEND)
    {
        option_set = RTEMS_WAIT;
        ticks = RTEMS_NO_TIMEOUT;
    }
    else if (timeout == OS_CHECK)
    {
        option_set = RTEMS_NO_WAIT;
        ticks = RTEMS_NO_TIMEOUT;
    }
    else
    {
        option_set = RTEMS_WAIT;
        /* msecs rounded to the closest system tick count */
        ticks = OS_Milli2Ticks(timeout);
    }

    /*
     ** Pend until a message arrives.
     */
    status = rtems_message_queue_receive(
            rtems_queue_id,            /* message queue descriptor */
            data,                      /* pointer to message buffer */
            &rtems_size,               /* returned size of message */
            option_set,                /* wait option */
            ticks                      /* timeout */
    );

    if (status == RTEMS_SUCCESSFUL)
    {
        return_code = OS_SUCCESS;
    }
    else if (status == RTEMS_TIMEOUT)
    {
        return_code = OS_QUEUE_TIMEOUT;
    }
    else if (status == RTEMS_UNSATISFIED)
    {
        return_code = OS_QUEUE_EMPTY;
    }
    else
	{
	    /* Something else went wrong */
	    return_code = OS_ERROR;
        OS_DEBUG("Unhandled queue_receive error: %s\n",rtems_status_text(status));
    }

    /*
    ** Check the size of the message.  If a valid message was
    ** obtained, indicate success.
    */
    if (status == RTEMS_SUCCESSFUL)
    {
        *size_copied = rtems_size;
        if (rtems_size != size)
        {
            /* Success, but the size was wrong */
            return_code = OS_QUEUE_INVALID_SIZE;
        }
    }
    else
    {
        *size_copied = 0;
    }

    return return_code;
}/* end OS_QueueGet */

/*---------------------------------------------------------------------------------------
   Name: OS_QueuePut

   Purpose: Put a message on a message queue.

   Returns: OS_ERR_INVALID_ID if the queue id passed in is not a valid queue
            OS_INVALID_POINTER if the data pointer is NULL
            OS_QUEUE_FULL if the queue cannot accept another message
            OS_ERROR if the OS call returns an error
            OS_SUCCESS if SUCCESS

   Notes: The flags parameter is not used.  The message put is always configured to
            immediately return an error if the receiving message queue is full.
---------------------------------------------------------------------------------------*/

int32 OS_QueuePut_Impl (uint32 queue_id, const void *data, uint32 size, uint32 flags)
{
    rtems_status_code  status;
    rtems_id           rtems_queue_id;

    rtems_queue_id = OS_impl_queue_table[queue_id].id;

    /* Write the buffer pointer to the queue.  If an error occurred, report it
    ** with the corresponding SB status code.
    */
    status = rtems_message_queue_send(
            rtems_queue_id,                   /* message queue descriptor */
            data,                             /* pointer to message */
            size                              /* length of message */
    );

    if (status == RTEMS_TOO_MANY)
    {
        /*
        ** Queue is full.
        */
        return OS_QUEUE_FULL;
    }

    if (status != RTEMS_SUCCESSFUL)
    {
        /*
        ** Unexpected error while writing to queue.
        */
        OS_DEBUG("Unhandled queue_send error: %s\n",rtems_status_text(status));
        return OS_ERROR;
    }

    return OS_SUCCESS;

}/* end OS_QueuePut */

/*---------------------------------------------------------------------------------------
    Name: OS_QueueGetInfo

    Purpose: This function will pass back a pointer to structure that contains
             all of the relevant info (name and creator) about the specified queue.

    Returns: OS_INVALID_POINTER if queue_prop is NULL
             OS_ERR_INVALID_ID if the ID given is not  a valid queue
             OS_SUCCESS if the info was copied over correctly
---------------------------------------------------------------------------------------*/

int32 OS_QueueGetInfo_Impl (uint32 queue_id, OS_queue_prop_t *queue_prop)
{
    /* No extra info for queues in the OS implementation */
    return OS_SUCCESS;

} /* end OS_QueueGetInfo */
/****************************************************************************************
                                  SEMAPHORE API
 ***************************************************************************************/

int32 OS_Rtems_BinSemAPI_Impl_Init(void)
{
    memset(OS_impl_bin_sem_table, 0, sizeof(OS_impl_bin_sem_table));
    return (OS_SUCCESS);
}

/*---------------------------------------------------------------------------------------
   Name: OS_BinSemCreate

   Purpose: Creates a binary semaphore with initial value specified by
            sem_initial_value and name specified by sem_name. sem_id will be
            returned to the caller

   Returns: OS_INVALID_POINTER if sen name or sem_id are NULL
            OS_ERR_NAME_TOO_LONG if the name given is too long
            OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
            OS_ERR_NAME_TAKEN if this is already the name of a binary semaphore
            OS_SEM_FAILURE if the OS call failed
            OS_SUCCESS if success


   Notes: options is an unused parameter
---------------------------------------------------------------------------------------*/

int32 OS_BinSemCreate_Impl (uint32 sem_id, uint32 sem_initial_value, uint32 options)
{
    rtems_status_code status;
    rtems_name        r_name;

    /*
    ** RTEMS task names are 4 byte integers.
    ** It is convenient to use the OSAL ID in here, as we know it is already unique
    ** and trying to use the real name would be less than useful (only 4 chars)
    */
    r_name = OS_global_bin_sem_table[sem_id].active_id;

    /* Check to make sure the sem value is going to be either 0 or 1 */
    if (sem_initial_value > 1)
    {
        sem_initial_value = 1;
    }

    /* Create RTEMS Semaphore */
    status = rtems_semaphore_create( r_name, sem_initial_value,
                                     OSAL_BINARY_SEM_ATTRIBS,
                                     0,
                                     &(OS_impl_bin_sem_table[sem_id].id));

    /* check if Create failed */
    if ( status != RTEMS_SUCCESSFUL )
    {
        OS_DEBUG("Unhandled semaphore_create error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_BinSemCreate */


/*--------------------------------------------------------------------------------------
     Name: OS_BinSemDelete

    Purpose: Deletes the specified Binary Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore
             OS_SEM_FAILURE the OS call failed
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_BinSemDelete_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_delete(OS_impl_bin_sem_table[sem_id].id);
    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_delete error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_BinSemDelete */

/*---------------------------------------------------------------------------------------
    Name: OS_BinSemGive

    Purpose: The function  unlocks the semaphore referenced by sem_id by performing
             a semaphore unlock operation on that semaphore.If the semaphore value
             resulting from this operation is positive, then no threads were blocked
             waiting for the semaphore to become unlocked; the semaphore value is
             simply incremented for this semaphore.


    Returns: OS_SEM_FAILURE the semaphore was not previously  initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_BinSemGive_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_release(OS_impl_bin_sem_table[sem_id].id);
    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_release error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return  OS_SUCCESS;
}/* end OS_BinSemGive */

/*---------------------------------------------------------------------------------------
    Name: OS_BinSemFlush

    Purpose: The function  releases all the tasks pending on this semaphore. Note
             that the state of the semaphore is not changed by this operation.

    Returns: OS_SEM_FAILURE the semaphore was not previously  initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_BinSemFlush_Impl (uint32 sem_id)
{
    rtems_status_code status;

    /* Give Semaphore */
    status = rtems_semaphore_flush(OS_impl_bin_sem_table[sem_id].id);
    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_flush error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

	return  OS_SUCCESS;

}/* end OS_BinSemFlush */

/*---------------------------------------------------------------------------------------
    Name:    OS_BinSemTake

    Purpose: The locks the semaphore referenced by sem_id by performing a
             semaphore lock operation on that semaphore.If the semaphore value
             is currently zero, then the calling thread shall not return from
             the call until it either locks the semaphore or the call is
             interrupted by a signal.

    Return:  OS_ERR_INVALID_ID : the semaphore was not previously initialized
             or is not in the array of semaphores defined by the system
             OS_SEM_FAILURE if the OS call failed and the semaphore is not obtained
             OS_SUCCESS if success

----------------------------------------------------------------------------------------*/

int32 OS_BinSemTake_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_obtain(OS_impl_bin_sem_table[sem_id].id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    /*
    ** If the semaphore is flushed, this function will return
    ** RTEMS_UNSATISFIED. If this happens, the OSAL does not want to return
    ** an error, it would be inconsistent with the other ports
    **
    ** I currently do not know of any other reasons this call would return
    **  RTEMS_UNSATISFIED, so I think it is OK.
    */
    if ( status != RTEMS_SUCCESSFUL && status != RTEMS_UNSATISFIED )
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_BinSemTake */
/*---------------------------------------------------------------------------------------
    Name: OS_BinSemTimedWait

    Purpose: The function locks the semaphore referenced by sem_id . However,
             if the semaphore cannot be locked without waiting for another process
             or thread to unlock the semaphore , this wait shall be terminated when
             the specified timeout ,msecs, expires.

    Returns: OS_SEM_TIMEOUT if semaphore was not relinquished in time
             OS_SUCCESS if success
             OS_SEM_FAILURE the semaphore was not previously initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
----------------------------------------------------------------------------------------*/

int32 OS_BinSemTimedWait_Impl (uint32 sem_id, uint32 msecs)
{
    rtems_status_code status;
    uint32            TimeInTicks;

    TimeInTicks = OS_Milli2Ticks(msecs);

    status  = 	rtems_semaphore_obtain(OS_impl_bin_sem_table[sem_id].id, RTEMS_WAIT, TimeInTicks) ;

    if ( status == RTEMS_TIMEOUT )
    {
        return OS_SEM_TIMEOUT;
    }

    /* See BinSemWait regarding UNSATISFIED */
    if ( status != RTEMS_SUCCESSFUL && status != RTEMS_UNSATISFIED )
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_BinSemTimedWait */

/*---------------------------------------------------------------------------------------
    Name: OS_BinSemGetInfo

    Purpose: This function will pass back a pointer to structure that contains
             all of the relevant info( name and creator) about the specified binary
             semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
             OS_INVALID_POINTER if the bin_prop pointer is null
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_BinSemGetInfo_Impl (uint32 sem_id, OS_bin_sem_prop_t *bin_prop)
{
    /* RTEMS has no API for obtaining the current value of a semaphore */
    return OS_SUCCESS;
} /* end OS_BinSemGetInfo */


int32 OS_Rtems_CountSemAPI_Impl_Init(void)
{
    memset(OS_impl_count_sem_table, 0, sizeof(OS_impl_count_sem_table));
    return (OS_SUCCESS);
}


/*---------------------------------------------------------------------------------------
   Name: OS_CountSemCreate

   Purpose: Creates a countary semaphore with initial value specified by
            sem_initial_value and name specified by sem_name. sem_id will be
            returned to the caller

   Returns: OS_INVALID_POINTER if sen name or sem_id are NULL
            OS_ERR_NAME_TOO_LONG if the name given is too long
            OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
            OS_ERR_NAME_TAKEN if this is already the name of a countary semaphore
            OS_SEM_FAILURE if the OS call failed
            OS_SUCCESS if success

   Notes: options is an unused parameter
---------------------------------------------------------------------------------------*/

int32 OS_CountSemCreate_Impl (uint32 sem_id, uint32 sem_initial_value, uint32 options)
{
    rtems_status_code status;
    rtems_name        r_name;

    /*
    ** Verify that the semaphore maximum value is not too high
    */
    if ( sem_initial_value > MAX_SEM_VALUE )
    {
        return OS_INVALID_SEM_VALUE;
    }

    /*
    ** RTEMS task names are 4 byte integers.
    ** It is convenient to use the OSAL ID in here, as we know it is already unique
    ** and trying to use the real name would be less than useful (only 4 chars)
    */
    r_name = OS_global_count_sem_table[sem_id].active_id;
    status = rtems_semaphore_create( r_name, sem_initial_value,
                                     OSAL_COUNT_SEM_ATTRIBS,
                                     0,
                                     &(OS_impl_count_sem_table[sem_id].id));

    /* check if Create failed */
    if ( status != RTEMS_SUCCESSFUL )
    {
        OS_DEBUG("Unhandled semaphore_create error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_CountSemCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_CountSemDelete

    Purpose: Deletes the specified Counting Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid countary semaphore
             OS_SEM_FAILURE the OS call failed
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_CountSemDelete_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_delete( OS_impl_count_sem_table[sem_id].id);
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_delete error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_CountSemDelete */

/*---------------------------------------------------------------------------------------
    Name: OS_CountSemGive

    Purpose: The function  unlocks the semaphore referenced by sem_id by performing
             a semaphore unlock operation on that semaphore.If the semaphore value
             resulting from this operation is positive, then no threads were blocked
             waiting for the semaphore to become unlocked; the semaphore value is
             simply incremented for this semaphore.


    Returns: OS_SEM_FAILURE the semaphore was not previously  initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the id passed in is not a countary semaphore
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_CountSemGive_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_release(OS_impl_count_sem_table[sem_id].id);
    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_release error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return(OS_SUCCESS);

}/* end OS_CountSemGive */

/*---------------------------------------------------------------------------------------
    Name:    OS_CountSemTake

    Purpose: The locks the semaphore referenced by sem_id by performing a
             semaphore lock operation on that semaphore.If the semaphore value
             is currently zero, then the calling thread shall not return from
             the call until it either locks the semaphore or the call is
             interrupted by a signal.

    Return:  OS_SEM_FAILURE : the semaphore was not previously initialized
             or is not in the array of semaphores defined by the system
             OS_ERR_INVALID_ID the Id passed in is not a valid countar semaphore
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success

----------------------------------------------------------------------------------------*/

int32 OS_CountSemTake_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_obtain(OS_impl_count_sem_table[sem_id].id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_CountSemTake */


/*---------------------------------------------------------------------------------------
    Name: OS_CountSemTimedWait

    Purpose: The function locks the semaphore referenced by sem_id . However,
             if the semaphore cannot be locked without waiting for another process
             or thread to unlock the semaphore , this wait shall be terminated when
             the specified timeout ,msecs, expires.

    Returns: OS_SEM_TIMEOUT if semaphore was not relinquished in time
             OS_SUCCESS if success
             OS_SEM_FAILURE the semaphore was not previously initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
----------------------------------------------------------------------------------------*/

int32 OS_CountSemTimedWait_Impl (uint32 sem_id, uint32 msecs)
{
    rtems_status_code status;
    uint32            TimeInTicks;

    TimeInTicks = OS_Milli2Ticks(msecs);

    status = rtems_semaphore_obtain(OS_impl_count_sem_table[sem_id].id, RTEMS_WAIT, TimeInTicks);
    if (status == RTEMS_TIMEOUT)
    {
        return OS_SEM_TIMEOUT;
    }

    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_CountSemTimedWait */

/*---------------------------------------------------------------------------------------
    Name: OS_CountSemGetInfo

    Purpose: This function will pass back a pointer to structure that contains
             all of the relevant info( name and creator) about the specified countary
             semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
             OS_INVALID_POINTER if the count_prop pointer is null
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_CountSemGetInfo_Impl (uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    /* RTEMS does not provide an API to get the value */
    return OS_SUCCESS;

} /* end OS_CountSemGetInfo */

/****************************************************************************************
                                  MUTEX API
 ***************************************************************************************/


int32 OS_Rtems_MutexAPI_Impl_Init(void)
{
    memset(OS_impl_mut_sem_table, 0, sizeof(OS_impl_mut_sem_table));
    return (OS_SUCCESS);
}

/*---------------------------------------------------------------------------------------
    Name: OS_MutSemCreate

    Purpose: Creates a mutex semaphore initially full.

    Returns: OS_INVALID_POINTER if sem_id or sem_name are NULL
             OS_ERR_NAME_TOO_LONG if the sem_name is too long to be stored
             OS_ERR_NO_FREE_IDS if there are no more free mutex Ids
             OS_ERR_NAME_TAKEN if there is already a mutex with the same name
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success

    Notes: the options parameter is not used in this implementation

---------------------------------------------------------------------------------------*/

int32 OS_MutSemCreate_Impl (uint32 sem_id, uint32 options)
{
    rtems_status_code   status;
    rtems_name          r_name;

    /*
    ** Try to create the mutex
    */
    r_name = OS_global_mutex_table[sem_id].active_id;
    status = rtems_semaphore_create ( r_name, 1,
                                      OSAL_MUTEX_ATTRIBS ,
                                      0,
                                      &OS_impl_mut_sem_table[sem_id].id );

    if ( status != RTEMS_SUCCESSFUL )
    {
        OS_DEBUG("Unhandled semaphore_create error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_MutSemCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_MutSemDelete

    Purpose: Deletes the specified Mutex Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_MutSemDelete_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_delete( OS_impl_mut_sem_table[sem_id].id);
    if (status != RTEMS_SUCCESSFUL)
    {
        /* clean up? */
        OS_DEBUG("Unhandled semaphore_delete error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_MutSemDelete */


/*---------------------------------------------------------------------------------------
    Name: OS_MutSemGive

    Purpose: The function releases the mutex object referenced by sem_id.The
             manner in which a mutex is released is dependent upon the mutex's type
             attribute.  If there are threads blocked on the mutex object referenced by
             mutex when this function is called, resulting in the mutex becoming
             available, the scheduling policy shall determine which thread shall
             acquire the mutex.

    Returns: OS_SUCCESS if success
             OS_SEM_FAILURE if the semaphore was not previously  initialized
             OS_ERR_INVALID_ID if the id passed in is not a valid mutex

---------------------------------------------------------------------------------------*/

int32 OS_MutSemGive_Impl (uint32 sem_id)
{
    rtems_status_code status;

    /* Give the mutex */
    status = rtems_semaphore_release(OS_impl_mut_sem_table[sem_id].id);

    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_release error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return  OS_SUCCESS;

}/* end OS_MutSemGive */

/*---------------------------------------------------------------------------------------
    Name: OS_MutSemTake

    Purpose: The mutex object referenced by sem_id shall be locked by calling this
             function. If the mutex is already locked, the calling thread shall
             block until the mutex becomes available. This operation shall return
             with the mutex object referenced by mutex in the locked state with the
             calling thread as its owner.

    Returns: OS_SUCCESS if success
             OS_SEM_FAILURE if the semaphore was not previously initialized or is
             not in the array of semaphores defined by the system
             OS_ERR_INVALID_ID the id passed in is not a valid mutex
---------------------------------------------------------------------------------------*/
int32 OS_MutSemTake_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_obtain(OS_impl_mut_sem_table[sem_id].id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_MutSemGive */

/*---------------------------------------------------------------------------------------
    Name: OS_MutSemGetInfo

    Purpose: This function will pass back a pointer to structure that contains
             all of the relevant info( name and creator) about the specified mutex
             semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
             OS_INVALID_POINTER if the mut_prop pointer is null
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_MutSemGetInfo_Impl (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
{
    /* RTEMS provides no additional info */
    return OS_SUCCESS;

} /* end OS_MutSemGetInfo */

/****************************************************************************************
                                    TICK API
 ***************************************************************************************/

/****************************************************************************************
                                 INT API
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_IntAttachHandler

   Purpose: The call associates a specified C routine to a specified interrupt
            number.Upon occurring of the InterruptNumber the InerruptHandler
            routine will be called and passed the parameter.

   Parameters:
        InterruptNumber : The Interrupt Number that will cause the start of the ISR
        InerruptHandler : The ISR associatd with this interrupt
        parameter :The parameter that is passed to the ISR

---------------------------------------------------------------------------------------*/
int32 OS_IntAttachHandler_Impl  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
#if (CPU_SIMPLE_VECTORED_INTERRUPTS == false)
  return OS_ERR_NOT_IMPLEMENTED;
#else
   rtems_status_code ret_status;
   uint32 status ;
   rtems_isr_entry old_handler;

   ret_status = rtems_interrupt_catch(
		(rtems_isr_entry)InterruptHandler,
		(rtems_vector_number)InterruptNumber,
		&old_handler);

   switch (ret_status)
   {
       case RTEMS_SUCCESSFUL :
          status = OS_SUCCESS;
	  break ;

       case RTEMS_INVALID_NUMBER :
          status = OS_INVALID_INT_NUM;
          break ;

       case RTEMS_INVALID_ADDRESS :
          status = OS_INVALID_POINTER;
          break ;

    default :
          status = OS_ERROR;
          break ;
    }
    return(status) ;
#endif
}/* end OS_IntAttachHandler */
/*---------------------------------------------------------------------------------------
   Name: OS_IntUnlock

   Purpose: Enable previous state of interrupts

   Parameters:
        IntLevel : The Interrupt Level to be reinstated
---------------------------------------------------------------------------------------*/

int32 OS_IntUnlock_Impl (int32 IntLevel)
{
    rtems_interrupt_enable ( (rtems_interrupt_level) IntLevel);
    return (OS_SUCCESS);

}/* end OS_IntUnlock */

/*---------------------------------------------------------------------------------------
   Name: OS_IntLock

   Purpose: Disable interrupts.

   Parameters:

   Returns: Interrupt level
---------------------------------------------------------------------------------------*/

int32 OS_IntLock_Impl (void)
{
   rtems_interrupt_level rtems_int_level;

   rtems_interrupt_disable(rtems_int_level) ;
   return ( (int32) rtems_int_level) ;

}/* end OS_IntLock */


/*---------------------------------------------------------------------------------------
   Name: OS_IntEnable

   Purpose: Enable previous state of interrupts

   Parameters:
        IntLevel : The Interrupt Level to be reinstated
---------------------------------------------------------------------------------------*/

int32 OS_IntEnable_Impl (int32 Level)
{
    rtems_interrupt_enable ( (rtems_interrupt_level) Level);
    return(OS_SUCCESS);
}/* end OS_IntEnable */

/*---------------------------------------------------------------------------------------
   Name: OS_IntDisable

   Purpose: Disable the corresponding interrupt number.

   Parameters:

   Returns: Interrupt level before OS_IntDisable Call
---------------------------------------------------------------------------------------*/

int32 OS_IntDisable_Impl (int32 Level)
{
   rtems_interrupt_level rtems_int_level;

   rtems_interrupt_disable(rtems_int_level) ;
   return ( (int32) rtems_int_level) ;
}/* end OS_IntDisable */

/*---------------------------------------------------------------------------------------
   Name: OS_HeapGetInfo

   Purpose: Return current info on the heap

   Parameters:

---------------------------------------------------------------------------------------*/
int32 OS_HeapGetInfo_Impl       (OS_heap_prop_t *heap_prop)
{
    region_information_block info;
    int                      status;

    status = malloc_info( &info );

    if ( status != 0 )
    {
       return(OS_ERROR);
    }

    heap_prop->free_bytes         = (uint32) info.Free.total;
    heap_prop->free_blocks        = (uint32) info.Free.number;
    heap_prop->largest_free_block = (uint32) info.Free.largest;

    return (OS_SUCCESS);
}

/*---------------------------------------------------------------------------------------
 * Name: OS_SetMask
 * Purpose:
 *      Set the masking register to mask and unmask interrupts
 *
 * Assumptions and Notes:
 *      HW interrupt control is not supported from a user task
 *
 * Parameters:
 *      MaskSetting :the value to be written into the mask register
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *
---------------------------------------------------------------------------------------*/
int32 OS_IntSetMask_Impl ( uint32 MaskSetting )
{
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*--------------------------------------------------------------------------------------
 * Name: OS_GetMask
 * Purpose:
 *      Read and report the setting of the cpu mask register.
 *
 * Assumptions and Notes:
 *      HW interrupt control is not supported from a user task
 *
 * Parameters:
 *      MaskSettingPtr : pointer to a location where the function store the
 *                               reading of the cpu mask register.
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *
---------------------------------------------------------------------------------------*/
int32 OS_IntGetMask_Impl ( uint32 * MaskSettingPtr )
{
    *MaskSettingPtr = 0;
    return(OS_ERR_NOT_IMPLEMENTED);
}

int32 OS_FPUExcAttachHandler_Impl(uint32 ExceptionNumber, void * ExceptionHandler,
                                 int32 parameter)
{
    /*
    ** Not implemented in RTEMS.
    */
    return(OS_ERR_NOT_IMPLEMENTED);
}

int32 OS_FPUExcEnable_Impl(int32 ExceptionNumber)
{
    /*
    ** Not implemented in RTEMS.
    */
    return(OS_SUCCESS);
}

int32 OS_FPUExcDisable_Impl(int32 ExceptionNumber)
{
    /*
    ** Not implemented in RTEMS.
    */
    return(OS_SUCCESS);
}

/*
 *
 *   Name: OS_FPUExcSetMask
 *
 *   Purpose: This function sets the FPU exception mask
 *
 *   Notes: The exception environment is local to each task Therefore this must be
 *          called for each task that that wants to do floating point and catch exceptions.
 */
int32 OS_FPUExcSetMask_Impl(uint32 mask)
{
    /*
    ** Not implemented in RTEMS.
    */
    return(OS_SUCCESS);
}

/*
 *
 *   Name: OS_FPUExcGetMask
 *
 *   Purpose: This function gets the FPU exception mask
 *
 *   Notes: The exception environment is local to each task Therefore this must be
 *          called for each task that that wants to do floating point and catch exceptions.
 */
int32 OS_FPUExcGetMask_Impl(uint32 *mask)
{
    /*
    ** Not implemented in RTEMS.
    */
    return(OS_SUCCESS);
}

/********************************************************************/
/*                 CONSOLE OUTPUT                                   */
/********************************************************************/

/* use the portable version of OS_ConsoleWrite_Impl() */
#include "../portable/os-impl-console-directwrite.c"


void  OS_ConsoleWakeup_Impl(uint32 local_id)
{
    OS_impl_console_internal_record_t *local = &OS_impl_console_table[local_id];

    if (local->is_async)
    {
        /* post the sem for the utility task to run */
        rtems_semaphore_release(local->data_sem);
    }
    else
    {
        /* output directly */
        OS_ConsoleOutput_Impl(local_id);
    }
}

static void OS_ConsoleTask_Entry(rtems_task_argument arg)
{
    uint32 local_id = arg;
    OS_impl_console_internal_record_t *local;

    local = &OS_impl_console_table[local_id];
    while (true)
    {
        OS_ConsoleOutput_Impl(local_id);
        rtems_semaphore_obtain(local->data_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    }
}

int32 OS_ConsoleCreate_Impl(uint32 local_id)
{
    OS_impl_console_internal_record_t *local = &OS_impl_console_table[local_id];
    int32 return_code;
    rtems_name r_name;
    rtems_id r_task_id;
    rtems_status_code status;

    if (local_id == 0)
    {
        return_code = OS_SUCCESS;
        local->is_async = OS_CONSOLE_ASYNC;
        local->out_fd = OSAL_CONSOLE_FILENO;

        if (local->is_async)
        {
            OS_DEBUG("%s(): Starting Async Console Handler\n", __func__);
            /*
            ** RTEMS task names are 4 byte integers.
            ** It is convenient to use the OSAL ID in here, as we know it is already unique
            ** and trying to use the real name would be less than useful (only 4 chars)
            */
            r_name = OS_global_console_table[local_id].active_id;
            status = rtems_semaphore_create( r_name, 0,
                                             OSAL_COUNT_SEM_ATTRIBS,
                                             0,
                                             &local->data_sem);
            if (status != RTEMS_SUCCESSFUL)
            {
                return_code = OS_SEM_FAILURE;
            }
            else
            {
                status = rtems_task_create(
                             r_name,
                             OS_CONSOLE_TASK_PRIORITY,
                             OS_CONSOLE_TASK_STACKSIZE,
                             RTEMS_PREEMPT | RTEMS_NO_ASR | RTEMS_NO_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0),
                             RTEMS_LOCAL,
                             &r_task_id);

                /* check if task_create failed */
                if (status != RTEMS_SUCCESSFUL )
                {
                    /* Provide some freedback as to why this failed */
                    OS_DEBUG("rtems_task_create failed: %s\n", rtems_status_text(status));
                    rtems_semaphore_delete(local->data_sem);
                    return_code = OS_ERROR;
                }
                else
                {
                    /* will place the task in 'ready for scheduling' state */
                    status = rtems_task_start (r_task_id, /*rtems task id*/
                            OS_ConsoleTask_Entry, /* task entry point */
                            (rtems_task_argument)local_id );  /* passed argument  */

                    if (status != RTEMS_SUCCESSFUL )
                    {
                        OS_printf("rtems_task_start failed: %s\n", rtems_status_text(status));
                        rtems_task_delete(r_task_id);
                        rtems_semaphore_delete(local->data_sem);
                        return_code = OS_ERROR;
                    }
                }
            }
        }
    }
    else
    {
        /* only one physical console device is implemented */
        return_code = OS_ERR_NOT_IMPLEMENTED;
    }

    return return_code;
}


