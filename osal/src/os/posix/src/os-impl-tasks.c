/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     os-impl-tasks.c
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

#include "os-impl-tasks.h"

#include "os-shared-task.h"
#include "os-shared-idmap.h"

/*
 * Defines
 */
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN   (8*1024)
#endif

/* Tables where the OS object information is stored */
OS_impl_task_internal_record_t      OS_impl_task_table          [OS_MAX_TASKS];

/*
 * Local Function Prototypes
 */

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
} /* end OS_PriorityRemap */



/*----------------------------------------------------------------
 *
 * Function: OS_NoopSigHandler
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           A POSIX signal handler that does nothing
 *
 *-----------------------------------------------------------------*/
static void  OS_NoopSigHandler (int signal)
{
} /* end OS_NoopSigHandler */



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
} /* end OS_Posix_GetSchedulerParams */

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
       }
       while (0);
   }
   else
   {
      OS_DEBUG("Could not getschedparam in main thread: %s (%d)\n",strerror(ret),ret);
   }

#if !defined(OSAL_CONFIG_DEBUG_PERMISSIVE_MODE)
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

   POSIX_GlobalVars.PageSize = sysconf(_SC_PAGESIZE);

   return OS_SUCCESS;
} /* end OS_Posix_TaskAPI_Impl_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_Posix_InternalTaskCreate_Impl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
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
     * Adjust the stack size parameter.
     *
     * POSIX has additional restrictions/limitations on the stack size of tasks that
     * other RTOS environments may not have.  Specifically POSIX says that the stack
     * size must be at least PTHREAD_STACK_MIN and may also need to be a multiple of the
     * system page size.
     *
     * Rounding up means the user might get a bigger stack than they requested, but
     * that should not break anything aside from consuming extra memory.
     */
    if (stacksz < PTHREAD_STACK_MIN)
    {
        stacksz = PTHREAD_STACK_MIN;
    }

    stacksz += POSIX_GlobalVars.PageSize - 1;
    stacksz -= stacksz % POSIX_GlobalVars.PageSize;

    /*
    ** Set the Stack Size
    */
    return_code = pthread_attr_setstacksize(&custom_attr, stacksz);
    if (return_code != 0)
    {
        OS_DEBUG("pthread_attr_setstacksize error in OS_TaskCreate: %s\n",strerror(return_code));
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
} /* end OS_Posix_InternalTaskCreate_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
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
} /* end OS_TaskCreate_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskMatch_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskMatch_Impl(uint32 task_id)
{
   if (pthread_equal(pthread_self(), OS_impl_task_table[task_id].id) == 0)
   {
      return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_TaskMatch_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
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

} /* end OS_TaskDelete_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskExit_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_TaskExit_Impl()
{
    pthread_exit(NULL);

} /* end OS_TaskExit_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskDelay_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
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
} /* end OS_TaskDelay_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskSetPriority_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
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
} /* end OS_TaskSetPriority_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskRegister_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
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
} /* end OS_TaskRegister_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetId_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
uint32 OS_TaskGetId_Impl (void)
{
   OS_U32ValueWrapper_t self_record;

   self_record.opaque_arg = pthread_getspecific(POSIX_GlobalVars.ThreadKey);

   return(self_record.value);
} /* end OS_TaskGetId_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskGetInfo_Impl (uint32 task_id, OS_task_prop_t *task_prop)
{
#ifndef OSAL_OMIT_DEPRECATED
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
#endif

   return OS_SUCCESS;
} /* end OS_TaskGetInfo_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskIdMatchSystemData_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
bool OS_TaskIdMatchSystemData_Impl(void *ref, uint32 local_id, const OS_common_record_t *obj)
{
    const pthread_t *target = (const pthread_t *)ref;

    return (pthread_equal(*target, OS_impl_task_table[local_id].id) != 0);
}

/*----------------------------------------------------------------
 *
 * Function: OS_TaskValidateSystemData_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskValidateSystemData_Impl(const void *sysdata, uint32 sysdata_size)
{
    if (sysdata == NULL || sysdata_size != sizeof(pthread_t))
    {
        return OS_INVALID_POINTER;
    }
    return OS_SUCCESS;
}



