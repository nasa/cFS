/*
** File   :	osapi.c
**
**      Copyright (c) 2004-2015, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved. This software was created at NASA Glenn
**      Research Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Joe Hickey based on original VxWorks implementation by Ezra Yeheskeli
**
** Purpose:
**      This file contains some of the OS APIs abstraction layer for VxWorks
**      This has been tested against the current VxWorks 4.11 release branch
**      
**      NOTE: This uses only the "Classic" VxWorks API.  It is intended to
**      work on VxWorks targets that do not provide the POSIX API, i.e.
**      when "--disable-posix" is given during the configuration stage.
**
**      If the VxWorks POSIX API is enabled, then it should be possible to
**      use the POSIX OSAL which is more full featured.
*/
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include <errnoLib.h>
#include <memPartLib.h>
#include <semLib.h>
#include <taskLib.h>
#include <memPartLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <msgQLib.h>
#include <intLib.h>
#include <iv.h>

#include <unistd.h>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/*
 * macros for stack size manipulation.
 * These are normally provided by vxWorks.h if relevant for the platform.
 * If they are not defined, use a reasonable default/substitute.
 */
#if defined(_STACK_ALIGN_SIZE)
#define VX_IMPL_STACK_ALIGN_SIZE    _STACK_ALIGN_SIZE
#else
#define VX_IMPL_STACK_ALIGN_SIZE    16
#endif

#if defined(STACK_ROUND_DOWN)
#define VX_IMPL_STACK_ROUND_DOWN(x) STACK_ROUND_DOWN(x)
#else
#define VX_IMPL_STACK_ROUND_DOWN(x) ((x) & ~(VX_IMPL_STACK_ALIGN_SIZE-1))
#endif

#if defined(STACK_ROUND_UP)
#define VX_IMPL_STACK_ROUND_UP(x)   STACK_ROUND_UP(x)
#else
#define VX_IMPL_STACK_ROUND_UP(x)   (((x) + (VX_IMPL_STACK_ALIGN_SIZE-1)) & ~(VX_IMPL_STACK_ALIGN_SIZE-1))
#endif

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
****************************************************************************************/
/*  tables for the properties of objects */
/*tasks */
typedef struct
{
    WIND_TCB tcb;           /* Must be first */
    TASK_ID vxid;
    void *heap_block;    /* set non-null if the stack was obtained with malloc() */
    long heap_block_size;
} OS_impl_task_internal_record_t;

/* queues */
typedef struct
{
    MSG_Q_ID vxid;
} OS_impl_queue_internal_record_t;

/* Counting & Binary Semaphores */
typedef struct
{
    VX_BINARY_SEMAPHORE(bmem);
    SEM_ID vxid;
} OS_impl_binsem_internal_record_t;

/* Counting & Binary Semaphores */
typedef struct
{
    VX_COUNTING_SEMAPHORE(cmem);
    SEM_ID vxid;
} OS_impl_countsem_internal_record_t;

/* Counting & Binary Semaphores */
typedef struct
{
    VX_MUTEX_SEMAPHORE(mmem);
    SEM_ID vxid;
} OS_impl_mutsem_internal_record_t;

/* Console device */
typedef struct
{
    VX_COUNTING_SEMAPHORE(cmem);
    bool            is_async;
    SEM_ID          datasem;
    TASK_ID         taskid;
} OS_impl_console_internal_record_t;


/* Tables where the OS object information is stored */
OS_impl_task_internal_record_t  OS_impl_task_table      [OS_MAX_TASKS];
OS_impl_queue_internal_record_t OS_impl_queue_table     [OS_MAX_QUEUES];
OS_impl_binsem_internal_record_t   OS_impl_bin_sem_table   [OS_MAX_BIN_SEMAPHORES];
OS_impl_countsem_internal_record_t OS_impl_count_sem_table [OS_MAX_COUNT_SEMAPHORES];
OS_impl_mutsem_internal_record_t   OS_impl_mut_sem_table   [OS_MAX_MUTEXES];
OS_impl_console_internal_record_t   OS_impl_console_table       [OS_MAX_CONSOLES];

static TASK_ID     OS_idle_task_id;

VX_MUTEX_SEMAPHORE(OS_task_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_queue_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_bin_sem_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_mut_sem_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_count_sem_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_stream_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_dir_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_timebase_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_module_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_filesys_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_console_mut_mem);

typedef struct
{
    char * const mem;
    SEM_ID vxid;
} VxWorks_GlobalMutex_t;

static VxWorks_GlobalMutex_t VX_MUTEX_TABLE[] =
      {
            [OS_OBJECT_TYPE_UNDEFINED] = { NULL },
            [OS_OBJECT_TYPE_OS_TASK] = { .mem = OS_task_table_mut_mem },
            [OS_OBJECT_TYPE_OS_QUEUE] = { .mem = OS_queue_table_mut_mem },
            [OS_OBJECT_TYPE_OS_COUNTSEM] = { .mem = OS_count_sem_table_mut_mem },
            [OS_OBJECT_TYPE_OS_BINSEM] = { .mem = OS_bin_sem_table_mut_mem },
            [OS_OBJECT_TYPE_OS_MUTEX] = { .mem = OS_mut_sem_table_mut_mem },
            [OS_OBJECT_TYPE_OS_STREAM] = { .mem = OS_stream_table_mut_mem },
            [OS_OBJECT_TYPE_OS_DIR] = { .mem = OS_dir_table_mut_mem },
            [OS_OBJECT_TYPE_OS_TIMEBASE] = { .mem = OS_timebase_table_mut_mem },
            [OS_OBJECT_TYPE_OS_MODULE] = { .mem = OS_module_table_mut_mem },
            [OS_OBJECT_TYPE_OS_FILESYS] = { .mem = OS_filesys_table_mut_mem },
            [OS_OBJECT_TYPE_OS_CONSOLE] = { .mem = OS_console_mut_mem },
      };

enum
{
    VX_MUTEX_TABLE_SIZE = (sizeof(VX_MUTEX_TABLE) / sizeof(VX_MUTEX_TABLE[0]))
};

const OS_ErrorTable_Entry_t OS_IMPL_ERROR_NAME_TABLE[] = { { 0, NULL } };

int32 OS_Lock_Global_Impl(uint32 idtype)
{
    VxWorks_GlobalMutex_t *mut;

    if (idtype >= VX_MUTEX_TABLE_SIZE)
    {
        return OS_ERROR;
    }

    mut = &VX_MUTEX_TABLE[idtype];
    if (mut->vxid == (SEM_ID)0)
    {
        return OS_ERROR;
    }

    if (semTake(mut->vxid, WAIT_FOREVER) != OK)
    {
        OS_DEBUG("semTake() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

int32 OS_Unlock_Global_Impl(uint32 idtype)
{
    VxWorks_GlobalMutex_t *mut;

    if (idtype >= VX_MUTEX_TABLE_SIZE)
    {
        return OS_ERROR;
    }

    mut = &VX_MUTEX_TABLE[idtype];
    if (mut->vxid == (SEM_ID)0)
    {
        return OS_ERROR;
    }

    if (semGive(mut->vxid) != OK)
    {
        OS_DEBUG("semGive() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    return OS_SUCCESS;
}



/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_API_Init

   Purpose: Initialize the tables that the OS API uses to keep track of information
            about objects

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_API_Impl_Init(uint32 idtype)
{
   int32    return_code = OS_SUCCESS;
   SEM_ID   semid;

   do
   {
      /* Initialize the table mutex for the given idtype */
      if (idtype < VX_MUTEX_TABLE_SIZE && VX_MUTEX_TABLE[idtype].mem != NULL)
      {
          semid = semMInitialize (VX_MUTEX_TABLE[idtype].mem, SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

          if ( semid == (SEM_ID)0 )
          {
              OS_DEBUG("Error: semMInitialize() failed - vxWorks errno %d\n",errno);
              return_code = OS_ERROR;
              break;
          }

          VX_MUTEX_TABLE[idtype].vxid = semid;
      }

      switch(idtype)
      {
      case OS_OBJECT_TYPE_OS_TASK:
         return_code = OS_VxWorks_TaskAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_QUEUE:
         return_code = OS_VxWorks_QueueAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_BINSEM:
         return_code = OS_VxWorks_BinSemAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_COUNTSEM:
         return_code = OS_VxWorks_CountSemAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_MUTEX:
         return_code = OS_VxWorks_MutexAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_MODULE:
         return_code = OS_VxWorks_ModuleAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_TIMEBASE:
         return_code = OS_VxWorks_TimeBaseAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_STREAM:
         return_code = OS_VxWorks_StreamAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_DIR:
         return_code = OS_VxWorks_DirAPI_Impl_Init();
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
    TASK_ID tid = taskIdSelf();
    OS_idle_task_id = tid;
    taskSuspend(tid);
}

/*---------------------------------------------------------------------------------------
   Name: OS_ApplicationShutdown_Impl

   Purpose: Ensures that the thread waiting in OS_IdleLoop_Impl is waken up

   returns: no value

   NOTE: Might be called from an ISR/signal handler
---------------------------------------------------------------------------------------*/
void OS_ApplicationShutdown_Impl()
{
    taskResume(OS_idle_task_id);
}



/*---------------------------------------------------------------------------------------
   Name: OS_VxWorksEntry

   Purpose: A Simple VxWorks-compatible entry point that calls the common task entry function

   NOTES: This wrapper function is only used locally by OS_TaskCreate below

---------------------------------------------------------------------------------------*/
static int OS_VxWorksEntry(int arg)
{
    OS_TaskEntryPoint((uint32)arg);
    return 0;
}



/****************************************************************************************
                                    TASK API
****************************************************************************************/

int32 OS_VxWorks_TaskAPI_Impl_Init(void)
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
    STATUS status;
    int vxflags;
    int vxpri;
    long actualsz;
    long userstackbase;
    long actualstackbase;
    OS_impl_task_internal_record_t *lrec;

    lrec = &OS_impl_task_table[task_id];

    /* Create VxWorks Task */

    /* see if the user wants floating point enabled. If
     * so, then se the correct option.
     */
    vxflags = 0;
    if (flags & OS_FP_ENABLED)
    {
        vxflags |= VX_FP_TASK;
    }


    /*
     * Get priority/stack specs from main struct
     * priority should be a direct passthru
     */
    vxpri = OS_task_table[task_id].priority;
    actualsz = OS_task_table[task_id].stack_size;
    userstackbase = (long)OS_task_table[task_id].stack_pointer;

    /*
     * NOTE: Using taskInit() here rather than taskSpawn() allows us
     * to specify a specific statically-allocated WIND_TCB instance.
     *
     * This is very important as it allows for efficient reverse-lookup;
     * a call to taskTcb() will get the WIND_TCB pointer back, which
     * in turn provides an index into OSAL local data structures.  With
     * this we can have the equivalent of a taskVar that works on both
     * UMP and SMP deployments.
     *
     * The difficulty with taskInit() is that we must also manually
     * allocate the stack as well (there is no API that allows
     * a specific WIND_TCB but automatically allocates the stack).
     * Furthermore, VxWorks uses this pointer directly as the CPU
     * stack pointer register, so we need to manually adjust it for
     * downward-growing stacks.
     *
     * NOTE: Allocation of the stack requires a malloc() of some form.
     * This is what taskSpawn() effectively does internally to create
     * stack.  If the system malloc() is unacceptable here then this
     * could be replaced with a statically-allocated OSAL stack buffer.
     *
     * ALSO NOTE: The stack-rounding macros are normally supplied from
     * vxWorks.h on relevant platforms.  If not provided then it is
     * assumed that no specific alignment is needed on this platform.
     */

    if (userstackbase == 0)
    {
        /* add a little extra in case the base address needs alignment too.
         * this helps ensure that the final aligned stack is not less
         * than what was originally requested (but might be a bit more)  */
        actualsz += VX_IMPL_STACK_ALIGN_SIZE;
        actualsz = VX_IMPL_STACK_ROUND_UP(actualsz);

        /*
         * VxWorks does not provide a way to deallocate
         * a taskInit-provided stack when a task exits.
         *
         * So in this case we will find the leftover heap
         * buffer when OSAL reuses this local record block.
         *
         * If that leftover heap buffer is big enough it
         * can be used directly.  Otherwise it needs to be
         * re-created.
         */
        if (lrec->heap_block_size < actualsz)
        {
            if (lrec->heap_block != NULL)
            {
                /* release the old block */
                free(lrec->heap_block);
                lrec->heap_block_size = 0;
            }

            /* allocate a new heap block to use for a stack */
            lrec->heap_block = malloc(actualsz);

            if (lrec->heap_block != NULL)
            {
                lrec->heap_block_size = actualsz;
            }

        }

        userstackbase = (long)lrec->heap_block;
    }

    if (userstackbase == 0)
    {
        /* no stack - cannot create task */
        return OS_ERROR;
    }

    actualstackbase = userstackbase;

    /* also round the base address */
    actualstackbase = VX_IMPL_STACK_ROUND_UP(actualstackbase);
    actualsz -= (actualstackbase - userstackbase);
    actualsz = VX_IMPL_STACK_ROUND_DOWN(actualsz);

    /*
     * On most CPUs the stack grows downward, so assume that to be
     * the case in the event that _STACK_DIR is not defined/known
     */
#if !defined(_STACK_DIR) || (_STACK_DIR != _STACK_GROWS_UP)
    actualstackbase  += actualsz;         /* move to last byte of stack block */
#endif

    status = taskInit(
            &lrec->tcb,                 /* address of new task's TCB */
            (char*)OS_global_task_table[task_id].name_entry,
            vxpri,                      /* priority of new task */
            vxflags,                    /* task option word */
            (char *)actualstackbase,    /* base of new task's stack */
            actualsz,                   /* size (bytes) of stack needed */
            (FUNCPTR)OS_VxWorksEntry,   /* entry point of new task */
            OS_global_task_table[task_id].active_id, /* 1st arg is ID */
            0,0,0,0,0,0,0,0,0);

    if (status != OK)
    {
        return OS_ERROR;
    }

    lrec->vxid = (TASK_ID)&lrec->tcb;

    taskActivate(lrec->vxid);

    return OS_SUCCESS;

} /* end OS_TaskCreate_Impl */

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
    if (taskDelete(OS_impl_task_table[task_id].vxid) != OK)
    {
        OS_DEBUG("taskDelete() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    OS_impl_task_table[task_id].vxid = 0;
    return OS_SUCCESS;

}/* end OS_TaskDelete_Impl */

/*--------------------------------------------------------------------------------------
     Name:    OS_TaskExit

     Purpose: Exits the calling task and removes it from the OS_task_table.

     returns: Nothing
---------------------------------------------------------------------------------------*/
void OS_TaskExit_Impl()
{
    taskExit(0);
}/*end OS_TaskExit_Impl */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskDelay

   Purpose: Delay a task for specified amount of milliseconds

   returns: OS_ERROR if sleep fails
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 OS_TaskDelay_Impl (uint32 milli_second)
{
    /* msecs rounded to the closest system tick count */
    int sys_ticks;

    sys_ticks = OS_Milli2Ticks(milli_second);

    /* if successful, the execution of task will pend here until delay finishes */
    if(taskDelay(sys_ticks) != OK)
    {
        return OS_ERROR;
    }
    return OS_SUCCESS;

}/* end OS_TaskDelay_Impl */

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
    /* Set VxWorks Task Priority */
    if(taskPrioritySet(OS_impl_task_table[task_id].vxid, new_priority) != OK)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;

}/* end OS_TaskSetPriority_Impl */

/*--------------------------------------------------------------------------------------
     Name: OS_TaskMatch

    Purpose: Determines if the caller matches the given task_id

    returns: OS_ERROR if not a match
             OS_SUCCESS if match
---------------------------------------------------------------------------------------*/
int32 OS_TaskMatch_Impl(uint32 task_id)
{
    /*
    ** Get VxWorks Task Id
    */
    if ( taskIdSelf() != OS_impl_task_table[task_id].vxid )
    {
       return(OS_ERROR);
    }


   return OS_SUCCESS;
} /* end OS_TaskMatch_Impl */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskRegister

   Purpose: Registers the calling task id with the task by adding the var to the tcb

   Returns: OS_ERR_INVALID_ID if there the specified ID could not be found
            OS_ERROR if the OS call fails
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_TaskRegister_Impl (uint32 global_task_id)
{
    return OS_SUCCESS;
}/* end OS_TaskRegister_Impl */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskGetId

   Purpose: This function returns the OSAL task id of the calling task

   Notes: The OS_task_key is initialized by the task switch if AND ONLY IF the
          OS_task_key has been registered via OS_TaskRegister(..).  If this is not
          called prior to this call, the value will be old and wrong.
---------------------------------------------------------------------------------------*/
uint32 OS_TaskGetId_Impl (void)
{
    OS_impl_task_internal_record_t *lrec;
    size_t index;
    uint32 id;

    id = 0;
    lrec = (OS_impl_task_internal_record_t *)taskTcb(taskIdSelf());

    if (lrec != NULL)
    {
        index = lrec - &OS_impl_task_table[0];
        if (index < OS_MAX_TASKS)
        {
            id = OS_global_task_table[index].active_id;
        }
    }

    return id;

}/* end OS_TaskGetId_Impl */

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
    union
    {
        TASK_ID vxid;
        uint32 value;
    } u;

    /*
     * The "OStask_id" is a broken concept and only included for backward compatibility.
     * On 32 bit platforms this should produce a backward-compatible result.
     * But on 64 bit platforms this value should never be used.....
     * using a union defeats a (valid) warning on 64-bit.
     */
    u.vxid = OS_impl_task_table[task_id].vxid;
    task_prop->OStask_id = u.value;

    return OS_SUCCESS;

} /* end OS_TaskGetInfo_Impl */


/****************************************************************************************
                                MESSAGE QUEUE API
****************************************************************************************/

int32 OS_VxWorks_QueueAPI_Impl_Init(void)
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

   Notes: the flags parameter is unused.
---------------------------------------------------------------------------------------*/

int32 OS_QueueCreate_Impl (uint32 queue_id, uint32 flags)
{
    MSG_Q_ID tmp_msgq_id;
    int queue_depth = OS_queue_table[queue_id].max_depth; /* maximum number of messages in queue (queue depth) */
    int data_size = OS_queue_table[queue_id].max_size;    /* maximum size in bytes of a message */

    /* Create VxWorks Message Queue */
    tmp_msgq_id = msgQCreate(queue_depth, data_size, MSG_Q_FIFO);

    /* check if message Q create failed */
    if(tmp_msgq_id == 0)
    {
        OS_DEBUG("msgQCreate() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    OS_impl_queue_table[queue_id].vxid = tmp_msgq_id;
    return OS_SUCCESS;

} /* end OS_QueueCreate_Impl */

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
    /* Try to delete the queue */
    if (msgQDelete(OS_impl_queue_table[queue_id].vxid) != OK)
    {
        OS_DEBUG("msgQDelete() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    OS_impl_queue_table[queue_id].vxid = 0;
    return OS_SUCCESS;

} /* end OS_QueueDelete_Impl */


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
    STATUS             status;
    int                ticks;

    /* Get Message From Message Queue */
    if (timeout == OS_PEND)
    {
        ticks = WAIT_FOREVER;
    }
    else if (timeout == OS_CHECK)
    {
        ticks = NO_WAIT;
    }
    else
    {
        /* msecs rounded to the closest system tick count */
        ticks = OS_Milli2Ticks(timeout);
    }

    status = msgQReceive(OS_impl_queue_table[queue_id].vxid, data, size, ticks);

    if(status == ERROR)
    {
        *size_copied = 0;
        if (errno == S_objLib_OBJ_TIMEOUT)
        {
            return_code = OS_QUEUE_TIMEOUT;
        }
        else if (errno == S_objLib_OBJ_UNAVAILABLE)
        {
            return_code = OS_QUEUE_EMPTY;
        }
        else
        {
            OS_DEBUG("msgQReceive() - vxWorks errno %d\n",errno);
            return_code = OS_ERROR;
        }
    }
    else
    {
        *size_copied = status;
        return_code = OS_SUCCESS;
    }

    return return_code;
}/* end OS_QueueGet_Impl */

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
    int32              return_code;

    if(msgQSend(OS_impl_queue_table[queue_id].vxid, (void*)data, size, NO_WAIT, MSG_PRI_NORMAL) == OK)
    {
        return_code = OS_SUCCESS;
    }
    else if(errno == S_objLib_OBJ_UNAVAILABLE)
    {
        return_code = OS_QUEUE_FULL;
    }
    else
    {
        OS_DEBUG("msgQSend() - vxWorks errno %d\n",errno);
        return_code = OS_ERROR;
    }

    return return_code;

}/* end OS_QueuePut_Impl */

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

} /* end OS_QueueGetInfo_Impl */


/****************************************************************************************
                              GENERIC SEMAPHORE API
****************************************************************************************/

/*
 * ----------------------------------
 * generic semaphore give/take -
 * VxWorks uses the sem semTake()/semGive() API for all types of semaphores.
 * Only the initialization is different between them.
 * Therefore all semaphore actions can just invoke these generic actions
 * -----------------------------------
 */
static int32 OS_VxWorks_GenericSemGive(SEM_ID vxid)
{
    /* Give VxWorks Semaphore */
    if(semGive(vxid) != OK)
    {
        OS_DEBUG("semGive() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }
    return OS_SUCCESS;
}

static int32 OS_VxWorks_GenericSemTake(SEM_ID vxid, int sys_ticks)
{
    int vx_status;

    /* Take VxWorks Semaphore */
    vx_status = semTake(vxid, sys_ticks);
    if (vx_status != OK)
    {
        /*
         * check for the timeout condition,
         * which has a different return code and
         * not necessarily an error of concern.
         */
        if (errno == S_objLib_OBJ_TIMEOUT)
        {
           return OS_SEM_TIMEOUT;
        }

        OS_DEBUG("semTake() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

/****************************************************************************************
                             BINARY SEMAPHORE API
****************************************************************************************/
int32 OS_VxWorks_BinSemAPI_Impl_Init(void)
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
    SEM_ID tmp_sem_id;

    /* Initialize VxWorks Semaphore.
     * The memory for this sem is statically allocated. */
    tmp_sem_id = semBInitialize(OS_impl_bin_sem_table[sem_id].bmem, SEM_Q_PRIORITY, sem_initial_value);

    /* check if semBInitialize failed */
    if(tmp_sem_id == (SEM_ID)0)
    {
        OS_DEBUG("semBInitialize() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    OS_impl_bin_sem_table[sem_id].vxid = tmp_sem_id;
    return OS_SUCCESS;

}/* end OS_BinSemCreate_Impl */


/*--------------------------------------------------------------------------------------
     Name: OS_BinSemDelete

    Purpose: Deletes the specified Binary Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore
             OS_SEM_FAILURE the OS call failed
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_BinSemDelete_Impl (uint32 sem_id)
{
    /*
     * As the memory for the sem is statically allocated, delete is a no-op.
     */
    OS_impl_bin_sem_table[sem_id].vxid = 0;
    return OS_SUCCESS;

}/* end OS_BinSemDelete_Impl */

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
    /* Use common routine */
    return OS_VxWorks_GenericSemGive(OS_impl_bin_sem_table[sem_id].vxid);
}/* end OS_BinSemGive_Impl */

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
    /* Flush VxWorks Semaphore */
    if(semFlush(OS_impl_bin_sem_table[sem_id].vxid) != OK)
    {
        OS_DEBUG("semFlush() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}/* end OS_BinSemFlush_Impl */

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
    /* Use common routine */
    return OS_VxWorks_GenericSemTake(OS_impl_bin_sem_table[sem_id].vxid, WAIT_FOREVER);

}/* end OS_BinSemTake_Impl */

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
    return OS_VxWorks_GenericSemTake(OS_impl_bin_sem_table[sem_id].vxid, OS_Milli2Ticks(msecs));
}/* end OS_BinSemTimedWait_Impl */

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
    /* VxWorks has no API for obtaining the current value of a semaphore */
    return OS_SUCCESS;
} /* end OS_BinSemGetInfo_Impl */


/****************************************************************************************
                             COUNTING SEMAPHORE API
****************************************************************************************/

int32 OS_VxWorks_CountSemAPI_Impl_Init(void)
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
    SEM_ID tmp_sem_id;

    /* Initialize VxWorks Semaphore.
     * The memory for this sem is statically allocated. */
    tmp_sem_id = semCInitialize(OS_impl_count_sem_table[sem_id].cmem, SEM_Q_PRIORITY, sem_initial_value);

    /* check if semCInitialize failed */
    if(tmp_sem_id == (SEM_ID)0)
    {
        OS_DEBUG("semCInitialize() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    OS_impl_count_sem_table[sem_id].vxid = tmp_sem_id;
    return OS_SUCCESS;

}/* end OS_CountSemCreate_Impl */

/*--------------------------------------------------------------------------------------
     Name: OS_CountSemDelete

    Purpose: Deletes the specified Counting Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid countary semaphore
             OS_SEM_FAILURE the OS call failed
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_CountSemDelete_Impl (uint32 sem_id)
{
    /*
     * As the memory for the sem is statically allocated, delete is a no-op.
     */
    OS_impl_count_sem_table[sem_id].vxid = 0;
    return OS_SUCCESS;

}/* end OS_CountSemDelete_Impl */

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
    /* Give VxWorks Semaphore */
    return OS_VxWorks_GenericSemGive(OS_impl_count_sem_table[sem_id].vxid);
}/* end OS_CountSemGive_Impl */

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
    return OS_VxWorks_GenericSemTake(OS_impl_count_sem_table[sem_id].vxid, WAIT_FOREVER);
}/* end OS_CountSemTake_Impl */


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
    return OS_VxWorks_GenericSemTake(OS_impl_count_sem_table[sem_id].vxid,
            OS_Milli2Ticks(msecs));
}/* end OS_CountSemTimedWait_Impl */

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
    /* VxWorks does not provide an API to get the value */
    return OS_SUCCESS;

} /* end OS_CountSemGetInfo */

/****************************************************************************************
                                  MUTEX API
****************************************************************************************/


int32 OS_VxWorks_MutexAPI_Impl_Init(void)
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
    SEM_ID tmp_sem_id;

    /* Initialize VxWorks Semaphore.
     * The memory for this sem is statically allocated. */
    tmp_sem_id = semMInitialize(OS_impl_mut_sem_table[sem_id].mmem, SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

    if(tmp_sem_id == (SEM_ID)0)
    {
        OS_DEBUG("semMInitalize() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    OS_impl_mut_sem_table[sem_id].vxid = tmp_sem_id;
    return OS_SUCCESS;
}/* end OS_MutSemCreate_Impl */

/*--------------------------------------------------------------------------------------
     Name: OS_MutSemDelete

    Purpose: Deletes the specified Mutex Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_MutSemDelete_Impl (uint32 sem_id)
{
    /*
     * As the memory for the sem is statically allocated, delete is a no-op.
     */
    OS_impl_mut_sem_table[sem_id].vxid = 0;
    return OS_SUCCESS;

}/* end OS_MutSemDelete_Impl */


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
    /* Give VxWorks Semaphore */
    return OS_VxWorks_GenericSemGive(OS_impl_mut_sem_table[sem_id].vxid);
}/* end OS_MutSemGive_Impl */

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
    /* Take VxWorks Semaphore */
    return OS_VxWorks_GenericSemTake(OS_impl_mut_sem_table[sem_id].vxid, WAIT_FOREVER);
}/* end OS_MutSemTake_Impl */

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
    /* VxWorks provides no additional info */
    return OS_SUCCESS;

} /* end OS_MutSemGetInfo_Impl */

/****************************************************************************************
                                    TICK API
****************************************************************************************/

/****************************************************************************************
                                 INT API
****************************************************************************************/

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
    /* The Xenomai-VxWorks emulation layer does not support interrupt control */
    if(intConnect(INUM_TO_IVEC(InterruptNumber),
      (VOIDFUNCPTR)InterruptHandler, parameter) != OK)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}/* end OS_IntAttachHandler_Impl */
/*---------------------------------------------------------------------------------------
   Name: OS_IntUnlock

   Purpose: Enable previous state of interrupts

   Parameters:
        IntLevel : The Interrupt Level to be reinstated
---------------------------------------------------------------------------------------*/

int32 OS_IntUnlock_Impl (int32 IntLevel)
{
    intUnlock(IntLevel);
    return(OS_SUCCESS);
}/* end OS_IntUnlock_Impl */

/*---------------------------------------------------------------------------------------
   Name: OS_IntLock

   Purpose: Disable interrupts.

   Parameters:

   Returns: Interrupt level
---------------------------------------------------------------------------------------*/

int32 OS_IntLock_Impl (void)
{
    return (int32)intLock();
}/* end OS_IntLock_Impl */


/*---------------------------------------------------------------------------------------
   Name: OS_IntEnable

   Purpose: Enable previous state of interrupts

   Parameters:
        IntLevel : The Interrupt Level to be reinstated
---------------------------------------------------------------------------------------*/

int32 OS_IntEnable_Impl (int32 Level)
{
    int32 RetCode;
    int Status;

    Status = intEnable(Level);

    if(Status == OK)
    {
        RetCode = OS_SUCCESS;
    }
    else
    {
        RetCode = OS_ERROR;
    }

    return RetCode;
}/* end OS_IntEnable_Impl */

/*---------------------------------------------------------------------------------------
   Name: OS_IntDisable

   Purpose: Disable the corresponding interrupt number.

   Parameters:

   Returns: Interrupt level before OS_IntDisable Call
---------------------------------------------------------------------------------------*/

int32 OS_IntDisable_Impl (int32 Level)
{
    int32 RetCode;
    int Status;

    Status = intDisable(Level);

    if(Status == OK)
    {
        RetCode = OS_SUCCESS;
    }
    else
    {
        RetCode = OS_ERROR;
    }

    return RetCode;
}/* end OS_IntDisable_Impl */

/*---------------------------------------------------------------------------------------
   Name: OS_HeapGetInfo

   Purpose: Return current info on the heap

   Parameters:

---------------------------------------------------------------------------------------*/
int32 OS_HeapGetInfo_Impl       (OS_heap_prop_t *heap_prop)
{
    MEM_PART_STATS stats;
    STATUS status;

    status = memPartInfoGet(memSysPartId, &stats);

    if (status != OK)
    {
        return OS_ERROR;
    }

    heap_prop->free_bytes         = stats.numBytesFree;
    heap_prop->free_blocks        = stats.numBlocksFree;
    heap_prop->largest_free_block = stats.maxBlockSizeFree;

    return (OS_SUCCESS);
} /* end OS_HeapGetInfo_Impl */

/*---------------------------------------------------------------------------------------
** Name: OS_SetMask
** Purpose:
**      Set the masking register to mask and unmask interrupts
**
** Assumptions and Notes:
**      HW interrupt control is not supported from a user task
**
** Parameters:
**      MaskSetting :the value to be written into the mask register
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**
---------------------------------------------------------------------------------------*/
int32 OS_IntSetMask_Impl ( uint32 MaskSetting )
{
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntSetMask_Impl */

/*--------------------------------------------------------------------------------------
** Name: OS_GetMask
** Purpose:
**      Read and report the setting of the cpu mask register.
**
** Assumptions and Notes:
**      HW interrupt control is not supported from a user task
**
** Parameters:
**      MaskSettingPtr : pointer to a location where the function store the
**                               reading of the cpu mask register.
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**
---------------------------------------------------------------------------------------*/
int32 OS_IntGetMask_Impl ( uint32 * MaskSettingPtr )
{
    *MaskSettingPtr = 0;
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntGetMask_Impl */

int32 OS_FPUExcAttachHandler_Impl(uint32 ExceptionNumber, void * ExceptionHandler,
                                 int32 parameter)
{
    /*
    ** Not implemented in VxWorks.
    */
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_FPUExcAttachHandler_Impl */

int32 OS_FPUExcEnable_Impl(int32 ExceptionNumber)
{
    /*
    ** Not implemented in VxWorks.
    */
    return(OS_SUCCESS);
} /* end OS_FPUExcEnable_Impl */

int32 OS_FPUExcDisable_Impl(int32 ExceptionNumber)
{
    /*
    ** Not implemented in VxWorks.
    */
    return(OS_SUCCESS);
} /* end OS_FPUExcDisable_Impl */

/*
**
**   Name: OS_FPUExcSetMask
**
**   Purpose: This function sets the FPU exception mask
**
**   Notes: The exception environment is local to each task Therefore this must be
**          called for each task that that wants to do floating point and catch exceptions.
*/
int32 OS_FPUExcSetMask_Impl(uint32 mask)
{
    int32 Status;

    /*
     * only implemented on ppc except ppc440
     * carry-over from previous vxworks osal
     */
#if defined(_PPC_) && CPU != PPC440
    vxFpscrSet( mask);
    Status = OS_SUCCESS;
#else
    Status = OS_ERR_NOT_IMPLEMENTED;
#endif

    return Status;
} /* end OS_FPUExcSetMask_Impl */

/*
**
**   Name: OS_FPUExcGetMask
**
**   Purpose: This function gets the FPU exception mask
**
**   Notes: The exception environment is local to each task Therefore this must be
**          called for each task that that wants to do floating point and catch exceptions.
*/
int32 OS_FPUExcGetMask_Impl(uint32 *mask)
{
    int32 Status;

#if defined(_PPC_) && CPU != PPC440
    *mask = vxFpscrGet();
    Status = OS_SUCCESS;
#else
    Status = OS_ERR_NOT_IMPLEMENTED;
#endif
    return Status;
} /* end OS_FPUExcGetMask_Impl */

/********************************************************************/
/*                 CONSOLE OUTPUT                                   */
/********************************************************************/


/*
 *   Name: OS_ConsoleOutput_Impl
 *
 *   Purpose: Transfer output data to the real console.
 *
 *   The data is already formatted, this just writes the characters.
 */
void  OS_ConsoleOutput_Impl(uint32 local_id)
{
    uint32 StartPos;
    uint32 EndPos;
    OS_console_internal_record_t *console;

    console = &OS_console_table[local_id];
    StartPos = console->ReadPos;
    EndPos = console->WritePos;
    while (StartPos != EndPos)
    {
        putchar(console->BufBase[StartPos]);
        ++StartPos;
        if (StartPos >= console->BufSize)
        {
            /* handle wrap */
            StartPos = 0;
        }
    }

    /* Update the global with the new read location */
    console->ReadPos = StartPos;
} /* end OS_ConsoleOutput_Impl */

void  OS_ConsoleWakeup_Impl(uint32 local_id)
{
    OS_impl_console_internal_record_t *local = &OS_impl_console_table[local_id];

    if (local->is_async)
    {
        /* post the sem for the utility task to run */
        if(semGive(local->datasem) == ERROR)
        {
            OS_DEBUG("semGive() - vxWorks errno %d\n",errno);
        }
    }
    else
    {
        /* output directly */
        OS_ConsoleOutput_Impl(local_id);
    }
} /* end OS_ConsoleWakeup_Impl */

static void OS_ConsoleTask_Entry(int arg)
{
    uint32 local_id = arg;
    OS_impl_console_internal_record_t *local;

    local = &OS_impl_console_table[local_id];
    while (true)
    {
        OS_ConsoleOutput_Impl(local_id);
        if(semTake(local->datasem, WAIT_FOREVER) == ERROR)
        {
            OS_DEBUG("semTake() - vxWorks errno %d\n",errno);
            break;
        }
    }
} /* end OS_ConsoleTask_Entry */

int32 OS_ConsoleCreate_Impl(uint32 local_id)
{
    OS_impl_console_internal_record_t *local = &OS_impl_console_table[local_id];
    int32 return_code;

    if (local_id == 0)
    {
        return_code = OS_SUCCESS;
        local->is_async = OS_CONSOLE_ASYNC;

        if (local->is_async)
        {
            OS_DEBUG("%s(): Starting Async Console Handler\n", __func__);

            /* Initialize VxWorks Semaphore.
             * The memory for this sem is statically allocated. */
            local->datasem = semCInitialize(local->cmem, SEM_Q_PRIORITY, 0);

            /* check if semCInitialize failed */
            if(local->datasem == (SEM_ID)0)
            {
                OS_DEBUG("semCInitialize() - vxWorks errno %d\n",errno);
                return OS_SEM_FAILURE;
            }

            /* spawn the async output helper task */
            local->taskid = taskSpawn(OS_console_table[local_id].device_name,
                    OS_CONSOLE_TASK_PRIORITY,
                    0,
                    OS_CONSOLE_TASK_STACKSIZE ,
                    (FUNCPTR)OS_ConsoleTask_Entry,
                    local_id,0,0,0,0,0,0,0,0,0);

            if (local->taskid == (TASK_ID)0)
            {
                OS_DEBUG("taskSpawn() - vxWorks errno %d\n",errno);
                return_code = OS_ERROR;
            }
        }
    }
    else
    {
        /* only one physical console device is implemented */
        return_code = OS_ERR_NOT_IMPLEMENTED;
    }

    return return_code;
} /* end OS_ConsoleCreate_Impl */



