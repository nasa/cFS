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
 * Purpose: This file contains some of the OS APIs abstraction layer
 *    implementation for POSIX, specifically for Linux with the 2.6 kernel ( > 2.6.18 )
 *    with the glibc library.  uClibc or other embedded C libraries are not yet tested.
 *
 *    This implementation works with the "shared" OSAL to complete the functionality.
 *    This contains only the POSIX-specific parts.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include <sched.h>

/*
 * Defines
 */
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 8092
#endif

/*
 * Added SEM_VALUE_MAX Define
 */
#ifndef SEM_VALUE_MAX
#define SEM_VALUE_MAX       (1 << 31)
#endif

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

/*
 * Global data for the API
 */

/*
 * Tables for the properties of objects
 */

/*tasks */
typedef struct
{
    pthread_t id;
} OS_impl_task_internal_record_t;

/* queues */
typedef struct
{
    mqd_t id;
} OS_impl_queue_internal_record_t;

/* Counting & Binary Semaphores */
typedef struct
{
    pthread_mutex_t id;
    pthread_cond_t  cv;
    volatile sig_atomic_t    flush_request;
    volatile sig_atomic_t    current_value;
}OS_impl_binsem_internal_record_t;

typedef struct
{
    sem_t           id;
}OS_impl_countsem_internal_record_t;

/* Mutexes */
typedef struct
{
    pthread_mutex_t id;
}OS_impl_mut_sem_internal_record_t;

/* Console device */
typedef struct
{
    bool            is_async;
    sem_t           data_sem;
    int             out_fd;
}OS_impl_console_internal_record_t;


/* Tables where the OS object information is stored */
OS_impl_task_internal_record_t      OS_impl_task_table          [OS_MAX_TASKS];
OS_impl_queue_internal_record_t     OS_impl_queue_table         [OS_MAX_QUEUES];
OS_impl_binsem_internal_record_t    OS_impl_bin_sem_table       [OS_MAX_BIN_SEMAPHORES];
OS_impl_countsem_internal_record_t  OS_impl_count_sem_table     [OS_MAX_COUNT_SEMAPHORES];
OS_impl_mut_sem_internal_record_t   OS_impl_mut_sem_table       [OS_MAX_MUTEXES];
OS_impl_console_internal_record_t   OS_impl_console_table       [OS_MAX_CONSOLES];

typedef struct
{
   pthread_mutex_t mutex;
   sigset_t sigmask;
} POSIX_GlobalLock_t;

static POSIX_GlobalLock_t OS_global_task_table_mut;
static POSIX_GlobalLock_t OS_queue_table_mut;
static POSIX_GlobalLock_t OS_bin_sem_table_mut;
static POSIX_GlobalLock_t OS_mut_sem_table_mut;
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
            [OS_OBJECT_TYPE_OS_MUTEX] = &OS_mut_sem_table_mut,
            [OS_OBJECT_TYPE_OS_STREAM] = &OS_stream_table_mut,
            [OS_OBJECT_TYPE_OS_DIR] = &OS_dir_table_mut,
            [OS_OBJECT_TYPE_OS_TIMEBASE] = &OS_timebase_table_mut,
            [OS_OBJECT_TYPE_OS_MODULE] = &OS_module_table_mut,
            [OS_OBJECT_TYPE_OS_FILESYS] = &OS_filesys_table_mut,
            [OS_OBJECT_TYPE_OS_CONSOLE] = &OS_console_mut,
      };


POSIX_GlobalVars_t   POSIX_GlobalVars = { 0 };

enum
{
   MUTEX_TABLE_SIZE = (sizeof(MUTEX_TABLE) / sizeof(MUTEX_TABLE[0]))
};

const OS_ErrorTable_Entry_t OS_IMPL_ERROR_NAME_TABLE[] = { { 0, NULL } };

/*
 * Local Function Prototypes
 */
static void  OS_CompAbsDelayTime( uint32 milli_second , struct timespec * tm);
static int   OS_PriorityRemap(uint32 InputPri);


/*---------------------------------------------------------------------------------------
   Name: OS_NoopSigHandler

   Purpose: A POSIX signal handler that does nothing
---------------------------------------------------------------------------------------*/
static void  OS_NoopSigHandler (int signal)
{
}


/*---------------------------------------------------------------------------------------
   Name: OS_Lock_Global_Impl

   Purpose: Locks the global table identified by "idtype"

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
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
}

/*---------------------------------------------------------------------------------------
   Name: OS_Unlock_Global_Impl

   Purpose: Unlocks the global table identified by "idtype"

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
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
}


/*---------------------------------------------------------------------------------------
   Name: OS_API_Init

   Purpose: Initialize the tables that the OS API uses to keep track of information
            about objects

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_API_Impl_Init(uint32 idtype)
{
   int                 ret;
   int32               return_code = OS_SUCCESS;
   pthread_mutexattr_t mutex_attr;

   do
   {
      /* Initialize the table mutex for the given idtype */
      if (idtype < MUTEX_TABLE_SIZE && MUTEX_TABLE[idtype] != NULL)
      {
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

      switch(idtype)
      {
      case OS_OBJECT_TYPE_OS_TASK:
         return_code = OS_Posix_TaskAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_QUEUE:
         return_code = OS_Posix_QueueAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_BINSEM:
         return_code = OS_Posix_BinSemAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_COUNTSEM:
         return_code = OS_Posix_CountSemAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_MUTEX:
         return_code = OS_Posix_MutexAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_MODULE:
         return_code = OS_Posix_ModuleAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_TIMEBASE:
         return_code = OS_Posix_TimeBaseAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_STREAM:
         return_code = OS_Posix_StreamAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_DIR:
         return_code = OS_Posix_DirAPI_Impl_Init();
         break;
      case OS_OBJECT_TYPE_OS_FILESYS:
         return_code = OS_Posix_FileSysAPI_Impl_Init();
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

   Purpose: Should be called after all initialization is done
            This is used to wait for and handle timer expiration events

   returns: no value
---------------------------------------------------------------------------------------*/
void OS_IdleLoop_Impl()
{
   /*
    * Unblock signals and wait for something to occur
    *
    * Note - "NormalSigMask" was calculated during task init to be the original signal mask
    * of the process PLUS all "RT" signals.  The RT signals are used by timers, so we want
    * to keep them masked here (this is different than the original POSIX impl).  The
    * timebase objects have a dedicated thread that will be doing "sigwait" on those.
    */
   sigsuspend(&POSIX_GlobalVars.NormalSigMask);
}

/*---------------------------------------------------------------------------------------
   Name: OS_ApplicationShutdown_Impl

   Purpose: Ensures that the thread waiting in OS_IdleLoop_Impl is waken up

   returns: no value

   NOTE: Might be called from an ISR/signal handler
---------------------------------------------------------------------------------------*/
void OS_ApplicationShutdown_Impl()
{
   /*
    * Raise a signal that is unblocked in OS_IdleLoop(),
    * which should break it out of the sigsuspend() call.
    */
   kill(getpid(), SIGHUP);
}


/*---------------------------------------------------------------------------------------
   Name: OS_PthreadEntry

   Purpose: A Simple pthread-compatible entry point that calls the real task function

   returns: NULL

    NOTES: This wrapper function is only used locally by OS_TaskCreate below

---------------------------------------------------------------------------------------*/
static void *OS_PthreadTaskEntry(void *arg)
{
   OS_U32ValueWrapper_t local_arg;

   local_arg.opaque_arg = arg;
   OS_TaskEntryPoint(local_arg.value); /* Never returns */

   return NULL;
}

/*---------------------------------------------------------------------------------------
   Name: OS_Posix_GetSchedulerParams

   Purpose: Helper function to get the details of the given OS scheduling policy.
            Determines if the policy is usable by OSAL - namely, that it provides
            enough priority levels to be useful.

   returns: true if policy is suitable for use by OSAL

    NOTES: Only used locally by task API initialization

---------------------------------------------------------------------------------------*/
static bool OS_Posix_GetSchedulerParams(int sched_policy, POSIX_PriorityLimits_t *PriLim)
{
    int ret;

    /*
     * Set up the local Min/Max priority levels (varies by OS and scheduler policy)
     *
     * Per POSIX:
     *  - The sched_get_priority_min/max() returns a number >= 0 on success.
     *    (-1 indicates an error)
     *  - Numerically higher values are scheduled before numerically lower values
     *  - A compliant OS will have a spread of at least 32 between min and max
     */
    ret = sched_get_priority_max(sched_policy);
    if (ret < 0)
    {
        OS_DEBUG("Policy %d: Unable to obtain maximum scheduling priority: %s\n", sched_policy, strerror(errno));
        return false;
    }

    PriLim->PriorityMax = ret;

    ret = sched_get_priority_min(sched_policy);
    if (ret < 0)
    {
        OS_DEBUG("Policy %d: Unable to obtain minimum scheduling priority: %s\n", sched_policy, strerror(errno));
        return false;
    }

    PriLim->PriorityMin = ret;

    /*
     * For OSAL, the absolute minimum spread between min and max must be 4.
     *
     * Although POSIX stipulates 32, we don't necessarily need that many, but we
     * also want to confirm that there is an acceptable spread.
     *
     * - Highest is reserved for the root task
     * - Next highest is reserved for OSAL priority=0 task(s)
     * - Lowest is reserved for OSAL priority=255 tasks(s)
     * - Need at least 1 for everything else.
     */
    if ((PriLim->PriorityMax - PriLim->PriorityMin) < 4)
    {
        OS_DEBUG("Policy %d: Insufficient spread between priority min-max: %d-%d\n",
                sched_policy, (int)PriLim->PriorityMin, (int)PriLim->PriorityMax);
        return false;
    }

    /* If we get here, then the sched_policy is potentially valid */
    OS_DEBUG("Policy %d: available, min-max: %d-%d\n", sched_policy,
            (int)PriLim->PriorityMin, (int)PriLim->PriorityMax);
    return true;
}

/*
 *********************************************************************************
 *          TASK API
 *********************************************************************************
 */

/*---------------------------------------------------------------------------------------
   Name: OS_Posix_TaskAPI_Impl_Init

   Purpose: Initialize the Posix Task data structures

 ----------------------------------------------------------------------------------------*/
int32 OS_Posix_TaskAPI_Impl_Init(void)
{
   int                 ret;
   int                 sig;
   struct sched_param  sched_param;
   int                 sched_policy;
   POSIX_PriorityLimits_t sched_fifo_limits;
   bool                   sched_fifo_valid;
   POSIX_PriorityLimits_t sched_rr_limits;
   bool                   sched_rr_valid;

   /* Initialize Local Tables */
   memset(OS_impl_task_table, 0, sizeof(OS_impl_task_table));

   /* Clear the "limits" structs otherwise the compiler may warn
    * about possibly being used uninitialized (false warning)
    */
   memset(&sched_fifo_limits, 0, sizeof(sched_fifo_limits));
   memset(&sched_rr_limits, 0, sizeof(sched_rr_limits));

   /*
    * Create the key used to store OSAL task IDs
    */
   ret = pthread_key_create(&POSIX_GlobalVars.ThreadKey, NULL );
   if ( ret != 0 )
   {
      OS_DEBUG("Error creating thread key: %s (%d)\n",strerror(ret),ret);
      return OS_ERROR;
   }

   /*
   ** Disable Signals to parent thread and therefore all
   ** child threads create will block all signals
   ** Note: Timers will not work in the application unless
   **       threads are spawned in OS_Application_Startup.
   */
   sigfillset(&POSIX_GlobalVars.MaximumSigMask);

   /*
    * Keep these signals unblocked so the process can be interrupted
    */
   sigdelset(&POSIX_GlobalVars.MaximumSigMask, SIGINT);  /* CTRL+C */
   sigdelset(&POSIX_GlobalVars.MaximumSigMask, SIGABRT); /* Abort */

   /*
    * One should not typically block ANY of the synchronous error
    * signals, i.e. SIGSEGV, SIGFPE, SIGILL, SIGBUS
    *
    * The kernel generates these signals in response to hardware events
    * and they get routed to the _specific thread_ that was executing when
    * the problem occurred.
    *
    * While it is technically possible to block these signals, the result is
    * undefined, and it makes debugging _REALLY_ hard.  If the kernel ever does
    * send one it means there really is a major problem, best to listen to it,
    * and not ignore it.
    */
   sigdelset(&POSIX_GlobalVars.MaximumSigMask, SIGSEGV); /* Segfault */
   sigdelset(&POSIX_GlobalVars.MaximumSigMask, SIGILL);  /* Illegal instruction */
   sigdelset(&POSIX_GlobalVars.MaximumSigMask, SIGBUS);  /* Bus Error */
   sigdelset(&POSIX_GlobalVars.MaximumSigMask, SIGFPE);  /* Floating Point Exception */

   /*
    * Set the mask and store the original (default) mask in the POSIX_GlobalVars.NormalSigMask
    */
   sigprocmask(SIG_SETMASK, &POSIX_GlobalVars.MaximumSigMask, &POSIX_GlobalVars.NormalSigMask);

   /*
    * Add all "RT" signals into the POSIX_GlobalVars.NormalSigMask
    * This will be used for the signal mask of the main thread
    * (This way it will end up as the default/original signal mask plus all RT sigs)
    */
   for (sig = SIGRTMIN; sig <= SIGRTMAX; ++sig)
   {
      sigaddset(&POSIX_GlobalVars.NormalSigMask, sig);
   }

   /*
    * SIGHUP is used to wake up the main thread when necessary,
    * so make sure it is NOT in the set.
    */
   sigdelset(&POSIX_GlobalVars.NormalSigMask, SIGHUP);

   /*
   ** Install noop as the signal handler for SIGUP.
   */
   signal(SIGHUP, OS_NoopSigHandler);

   /*
   ** Raise the priority of the current (main) thread so that subsequent
   ** application initialization will complete.  This had previously been
   ** done by the BSP and but it is moved here.
   **
   ** This will only work if the user owning this process has permission
   ** to create real time threads.  Otherwise, the default priority will
   ** be retained.  Typically this is only the root user, but finer grained
   ** permission controls are out there.  So if it works, great, but if
   ** a permission denied error is generated, that is OK too - this allows
   ** easily debugging code as a normal user.
   */
   ret = pthread_getschedparam(pthread_self(), &sched_policy, &sched_param);
   if (ret == 0)
   {
       POSIX_GlobalVars.SelectedRtScheduler = sched_policy; /* Fallback/default */
       do
       {
           sched_fifo_valid = OS_Posix_GetSchedulerParams(SCHED_FIFO, &sched_fifo_limits);
           sched_rr_valid = OS_Posix_GetSchedulerParams(SCHED_RR, &sched_rr_limits);

           /*
            * If both policies are valid, choose the best. In general, FIFO is preferred
            * since it is simpler.
            *
            * But, RR is preferred if mapping several OSAL priority levels into the
            * same local priority level. For instance, if 2 OSAL tasks are created at priorities
            * "2" and "1", both may get mapped to local priority 98, and if using FIFO then the
            * task at priority "2" could run indefinitely, never letting priority "1" execute.
            *
            * This violates the original intent, which would be to have priority "1" preempt
            * priority "2" tasks.  RR is less bad since it at least guarantees both tasks some
            * CPU time,
            */
           if (sched_fifo_valid && sched_rr_valid)
           {
               /*
                * If the spread from min->max is greater than what OSAL actually needs,
                * then FIFO is the preferred scheduler.  Must take into account one extra level
                * for the root task.
                */
               if ((sched_fifo_limits.PriorityMax - sched_fifo_limits.PriorityMin) > OS_MAX_TASK_PRIORITY)
               {
                   sched_policy = SCHED_FIFO;
                   POSIX_GlobalVars.PriLimits = sched_fifo_limits;
               }
               else
               {
                   sched_policy = SCHED_RR;
                   POSIX_GlobalVars.PriLimits = sched_rr_limits;
               }

           }
           else if (sched_fifo_valid)
           {
               /* only FIFO is available */
               sched_policy = SCHED_FIFO;
               POSIX_GlobalVars.PriLimits = sched_fifo_limits;
           }
           else if (sched_rr_valid)
           {
               /* only RR is available */
               sched_policy = SCHED_RR;
               POSIX_GlobalVars.PriLimits = sched_rr_limits;
           }
           else
           {
               /* Nothing is valid, use default */
               break;
           }

           /*
            * This OSAL POSIX implementation will reserve the absolute highest priority
            * for the root thread, which ultimately will just pend in sigsuspend() so
            * it will not actually DO anything, except if sent a signal.  This way,
            * that thread will still be able to preempt a high-priority user thread that
            * has gone awry (i.e. using 100% cpu in FIFO mode).
            */
           sched_param.sched_priority = POSIX_GlobalVars.PriLimits.PriorityMax;
           --POSIX_GlobalVars.PriLimits.PriorityMax;

           OS_DEBUG("Selected policy %d for RT tasks, root task = %d\n", sched_policy, (int)sched_param.sched_priority);

           /*
            * If the spread from min->max is greater than what OSAL actually needs,
            * then truncate it at the number of OSAL priorities.  This will end up mapping 1:1.
            * and leaving the highest priority numbers unused.
            */
           if ((POSIX_GlobalVars.PriLimits.PriorityMax - POSIX_GlobalVars.PriLimits.PriorityMin) > OS_MAX_TASK_PRIORITY)
           {
               POSIX_GlobalVars.PriLimits.PriorityMax = POSIX_GlobalVars.PriLimits.PriorityMin + OS_MAX_TASK_PRIORITY;
           }

#ifndef OSAL_DEBUG_DISABLE_TASK_PRIORITIES
           ret = pthread_setschedparam(pthread_self(), sched_policy, &sched_param);
           if (ret != 0)
           {
               OS_DEBUG("Could not setschedparam in main thread: %s (%d)\n",strerror(ret),ret);
               break;
           }

           /*
            * Set the boolean to indicate that "setschedparam" worked --
            * This means that it is also expected to work for future calls.
            */
           POSIX_GlobalVars.SelectedRtScheduler = sched_policy;
           POSIX_GlobalVars.EnableTaskPriorities = true;
#endif
       }
       while (0);
   }
   else
   {
      OS_DEBUG("Could not getschedparam in main thread: %s (%d)\n",strerror(ret),ret);
   }

#if !defined(OSAL_DEBUG_PERMISSIVE_MODE) && !defined(OSAL_DEBUG_DISABLE_TASK_PRIORITIES)
   /*
    * In strict (non-permissive) mode, if the task priority setting did not work, fail with an error.
    * This would be used on a real target where it needs to be ensured that priorities are active
    * and the "silent fallback" of debug mode operation is not desired.
    */
   if (!POSIX_GlobalVars.EnableTaskPriorities)
   {
      return OS_ERROR;
   }
#endif

   return OS_SUCCESS;
}

int32 OS_Posix_InternalTaskCreate_Impl(pthread_t *pthr, uint32 priority, size_t stacksz, PthreadFuncPtr_t entry, void *entry_arg)
{
    int                return_code = 0;
    pthread_attr_t     custom_attr;
    struct sched_param priority_holder;


    /*
     ** Initialize the pthread_attr structure.
     ** The structure is used to set the stack and priority
     */
    memset(&custom_attr, 0, sizeof(custom_attr));
    return_code = pthread_attr_init(&custom_attr);
    if(return_code != 0)
    {
       OS_DEBUG("pthread_attr_init error in OS_TaskCreate: %s\n",strerror(return_code));
       return(OS_ERROR);
    }


     /*
     ** Test to see if the original main task scheduling priority worked.
     ** If so, then also set the attributes for this task.  Otherwise attributes
     ** are left at default.
     */
     if (POSIX_GlobalVars.EnableTaskPriorities)
     {
        /*
        ** Set the scheduling inherit attribute to EXPLICIT
        */
        return_code = pthread_attr_setinheritsched(&custom_attr, PTHREAD_EXPLICIT_SCHED);
        if ( return_code != 0 )
        {
            OS_DEBUG("pthread_attr_setinheritsched error in OS_TaskCreate, errno = %s\n",strerror(return_code));
            return(OS_ERROR);
        }

       /*
       ** Set the Stack Size
       */
       if (stacksz > 0)
       {
           if (stacksz < PTHREAD_STACK_MIN)
           {
              stacksz = PTHREAD_STACK_MIN;
           }

           return_code = pthread_attr_setstacksize(&custom_attr, stacksz);
           if (return_code != 0)
           {
              OS_DEBUG("pthread_attr_setstacksize error in OS_TaskCreate: %s\n",strerror(return_code));
              return(OS_ERROR);
           }
       }

       /*
       ** Set the scheduling policy
       ** The best policy is determined during initialization
       */
       return_code = pthread_attr_setschedpolicy(&custom_attr, POSIX_GlobalVars.SelectedRtScheduler);
       if (return_code != 0)
       {
          OS_DEBUG("pthread_attr_setschedpolity error in OS_TaskCreate: %s\n",strerror(return_code));
          return(OS_ERROR);
       }

       /*
       ** Set priority
       */
       return_code = pthread_attr_getschedparam(&custom_attr, &priority_holder);
       if (return_code != 0)
       {
          OS_DEBUG("pthread_attr_getschedparam error in OS_TaskCreate: %s\n",strerror(return_code));
          return(OS_ERROR);
       }

       priority_holder.sched_priority = OS_PriorityRemap(priority);
       return_code = pthread_attr_setschedparam(&custom_attr,&priority_holder);
       if(return_code != 0)
       {
          OS_DEBUG("pthread_attr_setschedparam error in OS_TaskCreate: %s\n",strerror(return_code));
          return(OS_ERROR);
       }

     } /* End if user is root */

    /*
     ** Create thread
     */
    return_code = pthread_create(pthr, &custom_attr, entry, entry_arg);
    if (return_code != 0)
    {
       OS_DEBUG("pthread_create error in OS_TaskCreate: %s\n",strerror(return_code));
       return(OS_ERROR);
    }

    /*
     ** Free the resources that are no longer needed
     ** Since the task is now running - pthread_create() was successful -
     ** Do not treat anything bad that happens after this point as fatal.
     ** The task is running, after all - better to leave well enough alone.
     */
    return_code = pthread_detach(*pthr);
    if (return_code != 0)
    {
       OS_DEBUG("pthread_detach error in OS_TaskCreate: %s\n",strerror(return_code));
    }

    return_code = pthread_attr_destroy(&custom_attr);
    if (return_code != 0)
    {
       OS_DEBUG("pthread_attr_destroy error in OS_TaskCreate: %s\n",strerror(return_code));
    }

    return OS_SUCCESS;
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

    NOTES: the flags parameter is unused.

---------------------------------------------------------------------------------------*/
int32 OS_TaskCreate_Impl (uint32 task_id, uint32 flags)
{
    OS_U32ValueWrapper_t arg;
    int32 return_code;

    arg.opaque_arg = NULL;
    arg.value = OS_global_task_table[task_id].active_id;

    return_code = OS_Posix_InternalTaskCreate_Impl(
           &OS_impl_task_table[task_id].id,
           OS_task_table[task_id].priority,
           OS_task_table[task_id].stack_size,
           OS_PthreadTaskEntry,
           arg.opaque_arg);

   return return_code;
}/* end OS_TaskCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_TaskMatch

    Purpose: Determines if the caller matches the given global_task_id

    returns: OS_ERROR if not a match
             OS_SUCCESS if match
---------------------------------------------------------------------------------------*/
int32 OS_TaskMatch_Impl(uint32 task_id)
{
   if (pthread_equal(pthread_self(), OS_impl_task_table[task_id].id) == 0)
   {
      return OS_ERROR;
   }

   return OS_SUCCESS;
}

/*--------------------------------------------------------------------------------------
     Name: OS_TaskDelete

    Purpose: Deletes the specified Task and removes it from the OS_global_task_table.

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
    pthread_cancel(OS_impl_task_table[task_id].id);
    return OS_SUCCESS;

}/* end OS_TaskDelete */

/*--------------------------------------------------------------------------------------
     Name: OS_TaskExit

    Purpose: Exits the calling task and removes it from the OS_global_task_table.

    returns: Nothing
---------------------------------------------------------------------------------------*/
void OS_TaskExit_Impl()
{
    pthread_exit(NULL);

}/*end OS_TaskExit */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskDelay

   Purpose: Delay a task for specified amount of milliseconds

   returns: OS_ERROR if sleep fails or millisecond = 0
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 OS_TaskDelay_Impl(uint32 millisecond)
{
   struct timespec sleep_end;
   int status;

   clock_gettime(CLOCK_MONOTONIC, &sleep_end);
   sleep_end.tv_sec += millisecond / 1000;
   sleep_end.tv_nsec += 1000000 * (millisecond % 1000);

   if (sleep_end.tv_nsec >= 1000000000)
   {
      sleep_end.tv_nsec -= 1000000000;
      ++sleep_end.tv_sec;
   }

   do
   {
      status = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &sleep_end, NULL);
   }
   while (status == EINTR);

   if (status != 0)
   {
     return OS_ERROR;
   }
   else
   {
     return OS_SUCCESS;
   }
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
    int                os_priority;
    int                ret;

    if (POSIX_GlobalVars.EnableTaskPriorities)
    {
       /* Change OSAL priority into a priority that will work for this OS */
       os_priority = OS_PriorityRemap(new_priority);

       /*
       ** Set priority
       */
       ret = pthread_setschedprio(OS_impl_task_table[task_id].id, os_priority);
       if( ret != 0 )
       {
          OS_DEBUG("pthread_setschedprio: Task ID = %u, prio = %d, err = %s\n",
                    (unsigned int)task_id,os_priority,strerror(ret));
          return(OS_ERROR);
       }
    }

   return OS_SUCCESS;
} /* end OS_TaskSetPriority */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskRegister

   Purpose: Registers the calling task id with the task by adding the thread-specific variable
            This is now _always_ called by the task entry function and no longer up to the
            user to do so.

---------------------------------------------------------------------------------------*/
int32 OS_TaskRegister_Impl(uint32 global_task_id)
{
   int32 return_code;
   OS_U32ValueWrapper_t arg;

   arg.opaque_arg = 0;
   arg.value = global_task_id;

   return_code = pthread_setspecific(POSIX_GlobalVars.ThreadKey, arg.opaque_arg);
   if (return_code == 0)
   {
      return_code = OS_SUCCESS;
   }
   else
   {
      OS_DEBUG("OS_TaskRegister_Impl failed during pthread_setspecific() error=%s\n",strerror(return_code));
      return_code = OS_ERROR;
   }

   return return_code;
}

/*---------------------------------------------------------------------------------------
   Name: OS_TaskGetId

   Purpose: This function returns the #defined task id of the calling task

   Notes: The OS_task_key is initialized by the task switch if AND ONLY IF the
          OS_task_key has been registered via OS_TaskRegister(..).  If this is not
          called prior to this call, the value will be old and wrong.
---------------------------------------------------------------------------------------*/
uint32 OS_TaskGetId_Impl (void)
{
   OS_U32ValueWrapper_t self_record;

   self_record.opaque_arg = pthread_getspecific(POSIX_GlobalVars.ThreadKey);

   return(self_record.value);
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
   size_t copy_sz;

   /*
    * NOTE - this is not really valid, as you can't officially
    * cast a pthread_t to an integer
    * (in fact this fails on cygwin where pthread_t is NOT an integral type)
    *
    * This is just a hack to fill the value with something.
    * To be portable, the application should _NOT_ be using OStask_id for anything.
    */
   task_prop->OStask_id = 0;

   if (sizeof(pthread_t) < sizeof(task_prop->OStask_id))
   {
      copy_sz = sizeof(pthread_t);
   }
   else
   {
      copy_sz = sizeof(task_prop->OStask_id);
   }

   memcpy(&task_prop->OStask_id, &OS_impl_task_table[task_id].id, copy_sz);

   return OS_SUCCESS;
} /* end OS_TaskGetInfo */

/****************************************************************************************
                                MESSAGE QUEUE API
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_Posix_QueueAPI_Impl_Init

   Purpose: Initialize the Queue data structures

 ----------------------------------------------------------------------------------------*/
int32 OS_Posix_QueueAPI_Impl_Init(void)
{
   memset(OS_impl_queue_table, 0, sizeof(OS_impl_queue_table));

   /*
    * Initialize this to zero to indicate no limit
    * (would have expected osconfig.h to specify an upper limit, but it does not)
    */
   POSIX_GlobalVars.TruncateQueueDepth = 0;

   /*
    * Automatic truncation is dependent on the OSAL_DEBUG_PERMISSIVE_MODE compile-time define - so
    * creating a too-large message queue on a target without OSAL_DEBUG_PERMISSIVE_MODE will fail
    * with an OS error as intended.
    */
#ifdef OSAL_DEBUG_PERMISSIVE_MODE
   {
      FILE *fp;
      char buffer[32];

      /*
       * If running on Linux, /proc/sys/fs/mqueue/msg_max represents the max depth of a posix message queue for a user.
       *
       * In order to facilitate running in simulation mode without any need for root access --
       * this will allow the OSAL to successfully create message queues by truncating anything larger than this size.
       *
       * No need to check _LINUX_OS_ here; if the file fails to open, i.e. if not on Linux and the file does not exist,
       * then leave well enough alone and don't do anything.
       */

      fp = fopen("/proc/sys/fs/mqueue/msg_max","r");
      if (fp)
      {
         if (fgets(buffer,sizeof(buffer),fp) != NULL)
         {
            POSIX_GlobalVars.TruncateQueueDepth = strtoul(buffer, NULL, 10);
            OS_DEBUG("Maximum user msg queue depth = %u\n", (unsigned int)POSIX_GlobalVars.TruncateQueueDepth);
         }
         fclose(fp);
      }
   }
#endif

   return OS_SUCCESS;
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
   int                     return_code;
   mqd_t                   queueDesc;
   struct mq_attr          queueAttr;
   char                    name[OS_MAX_API_NAME * 2];

   /* set queue attributes */
   memset(&queueAttr, 0, sizeof(queueAttr));
   queueAttr.mq_maxmsg  = OS_queue_table[queue_id].max_depth;
   queueAttr.mq_msgsize = OS_queue_table[queue_id].max_size;

   /*
    * The "TruncateQueueDepth" indicates a soft limit to the size of a queue.
    * If nonzero, anything larger than this will be silently truncated
    * (Supports running applications as non-root)
    */
   if (POSIX_GlobalVars.TruncateQueueDepth > 0 &&
         POSIX_GlobalVars.TruncateQueueDepth < queueAttr.mq_maxmsg)
   {
      queueAttr.mq_maxmsg = POSIX_GlobalVars.TruncateQueueDepth;
   }

    /*
    ** Construct the queue name:
    ** The name will consist of "/<process_id>.queue_name"
    */
   snprintf(name, sizeof(name), "/%d.%s", (int)getpid(), OS_global_queue_table[queue_id].name_entry);

   /*
    ** create message queue
    */
   queueDesc = mq_open(name, O_CREAT | O_RDWR, 0666, &queueAttr);
   if ( queueDesc == (mqd_t)(-1) )
   {
      OS_DEBUG("OS_QueueCreate Error. errno = %d (%s)\n",errno,strerror(errno));
      if( errno == EINVAL )
      {
         OS_DEBUG("Your queue depth may be too large for the\n");
         OS_DEBUG("OS to handle. Please check the msg_max\n");
         OS_DEBUG("parameter located in /proc/sys/fs/mqueue/msg_max\n");
         OS_DEBUG("on your Linux file system and raise it if you\n");
         OS_DEBUG(" need to or run as root\n");
      }
      return_code = OS_ERROR;
   }
   else
   {
      OS_impl_queue_table[queue_id].id = queueDesc;
      return_code = OS_SUCCESS;

      /*
       * Unlink the queue right now --
       * queues have kernel persistence and if we do a lot of restarts (i.e. during debugging)
       * a lot of stale message queues will collect in the system.  It is OK to unlink right now
       * as this only affects the ability of another process to open the same queue, but we do
       * not need that to happen anyway.
       */
      if(mq_unlink(name) != 0)
      {
         OS_DEBUG("OS_QueueDelete Error during mq_unlink(). errno = %d (%s)\n",errno,strerror(errno));
         /* Note - since the queue is already closed, we cannot really handle this failure gracefully,
          * The queue is no longer usable so we can't return an error and go back to the way things were.
          * In this case we need to return OS_SUCCESS so the rest of the entry will be cleaned up.
          */
      }
   }

   return return_code;
}/* end OS_QueueCreate */

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
   int32     return_code;

   /* Try to delete and unlink the queue */
   if (mq_close(OS_impl_queue_table[queue_id].id) != 0)
   {
      OS_DEBUG("OS_QueueDelete Error during mq_close(). errno = %d (%s)\n",errno,strerror(errno));
      return_code = OS_ERROR;
   }
   else
   {
      return_code = OS_SUCCESS;
   }

    return return_code;
} /* end OS_QueueDelete */

/*---------------------------------------------------------------------------------------
 Name: OS_QueueGet

 Purpose: Receive a message on a message queue.  Will pend or timeout on the receive.
 Returns: OS_ERR_INVALID_ID if the given ID does not exist
 OS_ERR_INVALID_POINTER if a pointer passed in is NULL
 OS_QUEUE_EMPTY if the Queue has no messages on it to be recieved
 OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
 OS_QUEUE_INVALID_SIZE if the size of the buffer passed in is not big enough for the
                                  maximum size message
 OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_QueueGet_Impl (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
{
   int32 return_code;
   ssize_t sizeCopied;
   struct timespec ts;

   /*
    ** Read the message queue for data
    */
   sizeCopied = -1;
   if (timeout == OS_PEND)
   {
      /*
       ** A signal can interrupt the mq_receive call, so the call has to be done with
       ** a loop
       */
      do
      {
         sizeCopied = mq_receive(OS_impl_queue_table[queue_id].id, data, size, NULL);
      }
      while ( sizeCopied < 0 && errno == EINTR );

   }
   else
   {
      /*
       * NOTE - a prior implementation of OS_CHECK would check the mq_attr for a nonzero depth
       * and then call mq_receive().  This is insufficient since another thread might do the same
       * thing at the same time in which case one thread will read and the other will block.
       *
       * Calling mq_timedreceive with a zero timeout effectively does the same thing in the typical
       * case, but for the case where two threads do a simultaneous read, one will get the message
       * while the other will NOT block (as expected).
       */
      if (timeout == OS_CHECK)
      {
         memset(&ts, 0, sizeof(ts));
      }
      else
      {
         OS_CompAbsDelayTime( timeout, &ts);
      }

      /*
       ** If the mq_timedreceive call is interrupted by a system call or signal,
       ** call it again.
       */
      do
      {
         sizeCopied = mq_timedreceive(OS_impl_queue_table[queue_id].id, data, size, NULL, &ts);
      }
      while ( timeout != OS_CHECK && sizeCopied < 0 && errno == EINTR );

   } /* END timeout */

   /* Figure out the return code */
   if(sizeCopied == -1)
   {
      *size_copied = 0;

      /* Map the system errno to the most appropriate OSAL return code */
      if (errno == EMSGSIZE)
      {
         return_code = OS_QUEUE_INVALID_SIZE;
      }
      else if (timeout == OS_PEND || errno != ETIMEDOUT)
      {
         /* OS_PEND was supposed to pend forever until a message arrived
          * so something else is wrong.  Otherwise, at this point the only
          * "acceptable" errno is TIMEDOUT for the other cases.
          */
         return_code = OS_ERROR;
      }
      else if (timeout == OS_CHECK)
      {
         return_code = OS_QUEUE_EMPTY;
      }
      else
      {
         return_code = OS_QUEUE_TIMEOUT;
      }
   }
   else
   {
      *size_copied = sizeCopied;
      return_code = OS_SUCCESS;
   }

   return return_code;
} /* end OS_QueueGet */

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
   int32 return_code;
   int result;
   struct timespec ts;

   /*
    * NOTE - using a zero timeout here for the same reason that QueueGet does ---
    * checking the attributes and doing the actual send is non-atomic, and if
    * two threads call QueuePut() at the same time on a nearly-full queue,
    * one could block.
    */
   memset(&ts, 0, sizeof(ts));

   /* send message */
   do
   {
      result = mq_timedsend(OS_impl_queue_table[queue_id].id, data, size, 1, &ts);
   }
   while ( result == -1 && errno == EINTR );

   if(result == 0)
   {
      return_code = OS_SUCCESS;
   }
   else if (errno == ETIMEDOUT)
   {
      return_code = OS_QUEUE_FULL;
   }
   else
   {
      /* Something else went wrong */
      return_code = OS_ERROR;
   }

   return return_code;

} /* end OS_QueuePut */



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

        memset(sem, 0, sizeof (*sem));
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

}/* end OS_BinSemCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_BinSemDelete

    Purpose: Deletes the specified Binary Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore
             OS_SEM_FAILURE the OS call failed
             OS_SUCCESS if success

    Notes: Since we can't delete a semaphore which is currently locked by some task
           (as it may ber crucial to completing the task), the semaphore must be full to
           allow deletion.
---------------------------------------------------------------------------------------*/
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
}/* end OS_BinSemGive */

/*---------------------------------------------------------------------------------------
    Name: OS_BinSemFlush

    Purpose: The function unblocks all tasks pending on the specified semaphore. However,
             this function does not change the state of the semaphore.


    Returns: OS_SEM_FAILURE the semaphore was not previously  initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/
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
}/* end OS_BinSemFlush */

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
}

/*---------------------------------------------------------------------------------------
    Name:    OS_BinSemTake

    Purpose: The locks the semaphore referenced by sem_id by performing a
             semaphore lock operation on that semaphore.If the semaphore value
             is currently zero, then the calling thread shall not return from
             the call until it either locks the semaphore or the call is
             interrupted by a signal.

    Return:  OS_ERR_INVALID_ID the Id passed in is not a valid binary semaphore
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success

----------------------------------------------------------------------------------------*/
int32 OS_BinSemTake_Impl ( uint32 sem_id )
{
   return (OS_GenericBinSemTake_Impl (&OS_impl_bin_sem_table[sem_id], NULL));
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
int32 OS_BinSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
{
   struct timespec ts;

   /*
    ** Compute an absolute time for the delay
    */
   OS_CompAbsDelayTime(msecs, &ts);

   return (OS_GenericBinSemTake_Impl (&OS_impl_bin_sem_table[sem_id], &ts));
}

/*---------------------------------------------------------------------------------------
    Name: OS_BinSemGetInfo

    Purpose: This function will pass back a pointer to structure that contains
             all of the relevant info( name and creator) about the specified counting
             semaphore.

    Returns: OS_SUCCESS
---------------------------------------------------------------------------------------*/

int32 OS_BinSemGetInfo_Impl (uint32 sem_id, OS_bin_sem_prop_t *sem_prop)
{
    /* put the info into the stucture */
    sem_prop -> value = OS_impl_bin_sem_table[sem_id].current_value;
    return OS_SUCCESS;
} /* end OS_CountSemGetInfo */


/****************************************************************************************
                               COUNTING SEMAPHORE API
 ***************************************************************************************/

/*
 * Unlike binary semaphores, counting semaphores can use the standard POSIX semaphore facility.
 * This has the advantage of more correct behavior on "give" operations:
 *  - give may be done from a signal / ISR context
 *  - give should not cause an unexpected task switch nor should it ever block
 */

/*---------------------------------------------------------------------------------------
   Name: OS_Posix_CountSemAPI_Impl_Init

   Purpose: Initialize the Counting Semaphore data structures

---------------------------------------------------------------------------------------*/
int32 OS_Posix_CountSemAPI_Impl_Init(void)
{
   memset(OS_impl_count_sem_table, 0, sizeof(OS_impl_count_sem_table));
   return OS_SUCCESS;
}

/*---------------------------------------------------------------------------------------
   Name: OS_CountSemCreate

   Purpose: Creates a counting semaphore with initial value specified by
            sem_initial_value and name specified by sem_name. sem_id will be
            returned to the caller

   Returns: OS_INVALID_POINTER if sen name or sem_id are NULL
            OS_ERR_NAME_TOO_LONG if the name given is too long
            OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
            OS_ERR_NAME_TAKEN if this is already the name of a counting semaphore
            OS_SEM_FAILURE if the OS call failed
            OS_INVALID_SEM_VALUE if the semaphore value is too high
            OS_SUCCESS if success


   Notes: options is an unused parameter
---------------------------------------------------------------------------------------*/
int32 OS_CountSemCreate_Impl (uint32 sem_id, uint32 sem_initial_value, uint32 options)
{
    if (sem_initial_value > SEM_VALUE_MAX)
    {
        return OS_INVALID_SEM_VALUE;
    }

    if (sem_init(&OS_impl_count_sem_table[sem_id].id, 0, sem_initial_value) < 0)
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_CountSemCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_CountSemDelete

    Purpose: Deletes the specified Counting Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore
             OS_SEM_FAILURE the OS call failed
             OS_SUCCESS if success

    Notes: Since we can't delete a semaphore which is currently locked by some task
           (as it may ber crucial to completing the task), the semaphore must be full to
           allow deletion.
---------------------------------------------------------------------------------------*/
int32 OS_CountSemDelete_Impl (uint32 sem_id)
{
    if (sem_destroy(&OS_impl_count_sem_table[sem_id].id) < 0)
    {
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
             OS_ERR_INVALID_ID if the id passed in is not a counting semaphore
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/
int32 OS_CountSemGive_Impl ( uint32 sem_id )
{
    if (sem_post(&OS_impl_count_sem_table[sem_id].id) < 0)
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

}/* end OS_CountSemGive */

/*---------------------------------------------------------------------------------------
    Name:    OS_CountSemTake

    Purpose: The locks the semaphore referenced by sem_id by performing a
             semaphore lock operation on that semaphore.If the semaphore value
             is currently zero, then the calling thread shall not return from
             the call until it either locks the semaphore or the call is
             interrupted by a signal.

    Return:  OS_ERR_INVALID_ID the Id passed in is not a valid counting semaphore
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success

----------------------------------------------------------------------------------------*/
int32 OS_CountSemTake_Impl ( uint32 sem_id )
{
    if (sem_wait(&OS_impl_count_sem_table[sem_id].id) < 0)
    {
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
int32 OS_CountSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
{
   struct timespec ts;
   int result;

   /*
    ** Compute an absolute time for the delay
    */
   OS_CompAbsDelayTime(msecs, &ts);

   if (sem_timedwait(&OS_impl_count_sem_table[sem_id].id, &ts) == 0)
   {
       result = OS_SUCCESS;
   }
   else if (errno == ETIMEDOUT)
   {
       result = OS_SEM_TIMEOUT;
   }
   else
   {
       /* unspecified failure */
       result = OS_SEM_FAILURE;
   }

   return result;
}

/*---------------------------------------------------------------------------------------
    Name: OS_CountSemGetInfo

    Purpose: This function will pass back a pointer to structure that contains
             all of the relevant info( name and creator) about the specified counting
             semaphore.

    Returns: OS_SUCCESS
---------------------------------------------------------------------------------------*/

int32 OS_CountSemGetInfo_Impl (uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    int sval;

    if (sem_getvalue(&OS_impl_count_sem_table[sem_id].id, &sval) < 0)
    {
        return OS_SEM_FAILURE;
    }

    /* put the info into the stucture */
    count_prop -> value = sval;
    return OS_SUCCESS;
} /* end OS_CountSemGetInfo */

/****************************************************************************************
                                  MUTEX API
 ***************************************************************************************/

int32 OS_Posix_MutexAPI_Impl_Init(void)
{
   memset(OS_impl_mut_sem_table, 0, sizeof(OS_impl_mut_sem_table));
   return OS_SUCCESS;
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
    return_code = pthread_mutex_init(&OS_impl_mut_sem_table[sem_id].id,&mutex_attr);
    if ( return_code != 0 )
    {
       OS_DEBUG("Error: Mutex could not be created. ID = %u: %s\n",
             (unsigned int)sem_id,strerror(return_code));
       return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}/* end OS_MutexSemCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_MutSemDelete

    Purpose: Deletes the specified Mutex Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success

    Notes: The mutex must be full to take it, so we have to check for fullness

---------------------------------------------------------------------------------------*/

int32 OS_MutSemDelete_Impl (uint32 sem_id)
{
    int status;

    status = pthread_mutex_destroy( &(OS_impl_mut_sem_table[sem_id].id)); /* 0 = success */

    if (status != 0)
    {
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

int32 OS_MutSemGive_Impl ( uint32 sem_id )
{
   int status;

   /*
    ** Unlock the mutex
    */
   status = pthread_mutex_unlock(&(OS_impl_mut_sem_table[sem_id].id));
   if(status != 0)
   {
      return OS_SEM_FAILURE;
   }

   return OS_SUCCESS;
} /* end OS_MutSemGive */

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
int32 OS_MutSemTake_Impl ( uint32 sem_id )
{
    int status;

    /*
    ** Lock the mutex
    */
    status = pthread_mutex_lock(&(OS_impl_mut_sem_table[sem_id].id));
    if( status != 0 )
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

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
    return OS_SUCCESS;

} /* end OS_MutSemGetInfo */


/****************************************************************************************
                                    INT API
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
 * Name: OS_IntAttachHandler
 *
 * Purpose:
 *      The call associates a specified C routine to a specified interrupt
 *      number.Upon occurring of the InterruptNumber , the InerruptHandler
 *      routine will be called and passed the parameter.
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *      InterruptNumber : The Interrupt Number that will cause the start of
 *                        the ISR
 *      InerruptHandler : The ISR associatd with this interrupt
 *      parameter :The parameter that is passed to the ISR
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *     OS_SUCCESS on success
 *     OS_INVALID_POINTER if the Interrupt handler pointer is NULL
---------------------------------------------------------------------------------------*/
int32 OS_IntAttachHandler_Impl  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*---------------------------------------------------------------------------------------
 * Name: OS_IntUnlock
 * Purpose:
 *      Enable the interrupts.
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntUnlock_Impl (int32 IntLevel)
{
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*---------------------------------------------------------------------------------------
 * Name: OS_Intlock
 * Purpose:
 *      Disable the interrupts.
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntLock_Impl ( void )
{
    return(OS_ERR_NOT_IMPLEMENTED);
}
/*---------------------------------------------------------------------------------------
 * Name: OS_IntEnable
 * Purpose:
 *      Enables interrupts through Level
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *              Level - the interrupts to enable
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntEnable_Impl(int32 Level)
{
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*---------------------------------------------------------------------------------------
 * Name: OS_IntDisable
 * Purpose:
 *      Disables interrupts through Level
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *              Level - the interrupts to disable
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *      OS_SUCCESS
---------------------------------------------------------------------------------------*/

int32 OS_IntDisable_Impl(int32 Level)
{
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*---------------------------------------------------------------------------------------
   Name: OS_HeapGetInfo

   Purpose: Return current info on the heap

   Parameters:

---------------------------------------------------------------------------------------*/
int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop)
{
    /*
    ** Not implemented yet
    */
    return (OS_ERR_NOT_IMPLEMENTED);
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

/*---------------------------------------------------------------------------------------
 * Name: OS_CompAbsDelayTime
 *
 * Purpose:
 * This function accept time interval, msecs, as an input and
 * computes the absolute time at which this time interval will expire.
 * The absolute time is programmed into a struct.
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values: OS_SUCCESS,
---------------------------------------------------------------------------------------*/
void  OS_CompAbsDelayTime( uint32 msecs, struct timespec * tm)
{
    clock_gettime( CLOCK_REALTIME,  tm );

    /* add the delay to the current time */
    tm->tv_sec  += (time_t) (msecs / 1000) ;
    /* convert residue ( msecs )  to nanoseconds */
    tm->tv_nsec +=  (msecs % 1000) * 1000000L ;

    if(tm->tv_nsec  >= 1000000000L )
    {
        tm->tv_nsec -= 1000000000L ;
        tm->tv_sec ++ ;
    }
}

/*----------------------------------------------------------------------------
 * Name: OS_PriorityRemap
 *
 * Purpose: Remaps the OSAL priority into one that is viable for this OS
 *
 * Note: This implementation assumes that InputPri has already been verified
 * to be within the range of [0,OS_MAX_TASK_PRIORITY]
 *
----------------------------------------------------------------------------*/

static int OS_PriorityRemap(uint32 InputPri)
{
    int OutputPri;

    if (InputPri == 0)
    {
        /* use the "MAX" local priority only for OSAL tasks with priority=0 */
        OutputPri = POSIX_GlobalVars.PriLimits.PriorityMax;
    }
    else if (InputPri >= OS_MAX_TASK_PRIORITY)
    {
        /* use the "MIN" local priority only for OSAL tasks with priority=255 */
        OutputPri = POSIX_GlobalVars.PriLimits.PriorityMin;
    }
    else
    {
        /*
         * Spread the remainder of OSAL priorities over the remainder of local priorities
         *
         * Note OSAL priorities use the VxWorks style with zero being the
         * highest and OS_MAX_TASK_PRIORITY being the lowest, this inverts it
         */
        OutputPri = (OS_MAX_TASK_PRIORITY - 1) - (int)InputPri;

        OutputPri *= (POSIX_GlobalVars.PriLimits.PriorityMax - POSIX_GlobalVars.PriLimits.PriorityMin) - 2;
        OutputPri += OS_MAX_TASK_PRIORITY / 2;
        OutputPri /= (OS_MAX_TASK_PRIORITY - 2);
        OutputPri += POSIX_GlobalVars.PriLimits.PriorityMin + 1;
    }

    return OutputPri;
}/*end OS_PriortyRemap*/


int32 OS_FPUExcAttachHandler_Impl(uint32 ExceptionNumber, void * ExceptionHandler,
                                 int32 parameter)
{
    /*
    ** Not implemented in linux.
    */
    return(OS_ERR_NOT_IMPLEMENTED);
}

int32 OS_FPUExcEnable_Impl(int32 ExceptionNumber)
{
    /*
    ** Not implemented in linux.
    */
    return(OS_SUCCESS);
}

int32 OS_FPUExcDisable_Impl(int32 ExceptionNumber)
{
    /*
    ** Not implemented in linux.
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
    ** Not implemented in linux.
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
    ** Not implemented in linux.
    */
    *mask = 0;
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
        sem_post(&local->data_sem);
    }
    else
    {
        /* output directly */
        OS_ConsoleOutput_Impl(local_id);
    }
}

static void*  OS_ConsoleTask_Entry(void* arg)
{
    OS_U32ValueWrapper_t local_arg;
    OS_impl_console_internal_record_t *local;

    local_arg.opaque_arg = arg;
    local = &OS_impl_console_table[local_arg.value];
    while (true)
    {
        OS_ConsoleOutput_Impl(local_arg.value);
        sem_wait(&local->data_sem);
    }
    return NULL;
}

int32 OS_ConsoleCreate_Impl(uint32 local_id)
{
    OS_impl_console_internal_record_t *local = &OS_impl_console_table[local_id];
    pthread_t consoletask;
    int32 return_code;
    OS_U32ValueWrapper_t local_arg = { 0 };

    if (local_id == 0)
    {
        return_code = OS_SUCCESS;
        local->is_async = OS_CONSOLE_ASYNC;
        local->out_fd = OSAL_CONSOLE_FILENO;

        if (local->is_async)
        {
            if (sem_init(&OS_impl_console_table[local_id].data_sem, 0, 0) < 0)
            {
                return_code = OS_SEM_FAILURE;
            }
            else
            {
                local_arg.value = local_id;
                return_code = OS_Posix_InternalTaskCreate_Impl(&consoletask, OS_CONSOLE_TASK_PRIORITY, 0,
                    OS_ConsoleTask_Entry, local_arg.opaque_arg);

                if (return_code != OS_SUCCESS)
                {
                    sem_destroy(&OS_impl_console_table[local_id].data_sem);
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

