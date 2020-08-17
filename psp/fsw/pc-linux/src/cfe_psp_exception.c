/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/******************************************************************************
S
** File:  cfe_psp_exception.c
**
**      POSIX ( Mac OS X, Linux, Cygwin ) version
**
** Purpose:
**   cFE PSP Exception handling functions
**
** History:
**   2007/05/29  A. Cudmore      | POSIX Version
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <string.h>
#include <pthread.h>

/*
** cFE includes
*/
#include "common_types.h"
#include "osapi.h"
#include "cfe_psp.h"
#include "cfe_psp_config.h"
#include "cfe_psp_exceptionstorage_types.h"
#include "cfe_psp_exceptionstorage_api.h"

#include <execinfo.h>
#include <signal.h>

/*
 * A set of asynchronous signals which will be masked during other signal processing
 */
sigset_t    CFE_PSP_AsyncMask;




/***************************************************************************
 **                        FUNCTIONS DEFINITIONS
 ***************************************************************************/


/*
** Name: CFE_PSP_ExceptionSigHandler
**
** Installed as a signal handler to log exception events.
**
*/
void CFE_PSP_ExceptionSigHandler (int signo, siginfo_t *si, void *ctxt)
{
    CFE_PSP_Exception_LogData_t* Buffer;
    int NumAddrs;

    /*
     * Note that the time between CFE_PSP_Exception_GetNextContextBuffer()
     * and CFE_PSP_Exception_WriteComplete() is sensitive in that it is
     * accessing a global.
     *
     * Cannot use a conventional lock because this is a signal handler, the
     * solution would need to involve a signal-safe spinlock and/or C11
     * atomic ops.
     *
     * This means if another exception occurs on another task during this
     * time window, it may use the same buffer.
     *
     * However, exceptions should be rare enough events that this is highly
     * unlikely to occur, so leaving this unhandled for now.
     */
    Buffer = CFE_PSP_Exception_GetNextContextBuffer();
    if (Buffer != NULL)
    {
        /*
         * read the clock as a timestamp - note "clock_gettime" is signal safe per POSIX,
         *
         * _not_ going through OSAL to read this as it may do something signal-unsafe...
         * (current implementation would be safe, but it is not guaranteed to always be).
         */
        clock_gettime(CLOCK_MONOTONIC, &Buffer->context_info.event_time);
        memcpy(&Buffer->context_info.si, si, sizeof(Buffer->context_info.si));
        NumAddrs = backtrace(Buffer->context_info.bt_addrs, CFE_PSP_MAX_EXCEPTION_BACKTRACE_SIZE);
        Buffer->context_size = offsetof(CFE_PSP_Exception_ContextDataEntry_t, bt_addrs[NumAddrs]);
        /* pthread_self() is signal-safe per POSIX.1-2013 */
        Buffer->sys_task_id = pthread_self();
        CFE_PSP_Exception_WriteComplete();
    }

    /*
     * notify the main (idle) thread that an interesting event occurred.
     * Note on this platform this cannot _directly_ invoke CFE from a signal handler.
     */
    pthread_kill(CFE_PSP_IdleTaskState.ThreadID, CFE_PSP_EXCEPTION_EVENT_SIGNAL);
}

/*
** Name: CFE_PSP_ExceptionSigHandlerSuspend
**
** An extension of CFE_PSP_ExceptionSigHandler that also
** suspends the calling task and prevents returning to the
** application.
**
** This is required for handling events like Floating Point exceptions,
** where returning to the application would resume at the same instruction
** and re-trigger the exception, resulting in a loop.
**
*/
void CFE_PSP_ExceptionSigHandlerSuspend (int signo, siginfo_t *si, void *ctxt)
{
    /*
     * Perform normal exception logging
     */
    CFE_PSP_ExceptionSigHandler(signo, si, ctxt);

    /*
     * calling "sigsuspend" with an empty mask should
     * block this thread indefinitely.  This is intended
     * to replicate the behavior of vxworks which suspends
     * the task after an exception.
     *
     * This stops execution of the thread in anticipation that it
     * will be deleted by the CFE/OSAL.
     */
    sigsuspend(&CFE_PSP_AsyncMask);

} /* end function */

/*
 * Helper function to call sigaction() to attach a signal handler
 */
void CFE_PSP_AttachSigHandler (int signo)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_mask = CFE_PSP_AsyncMask;

    if(!sigismember(&CFE_PSP_AsyncMask, signo))
    {
        /*
         * In the event that the handler is being installed for one of the
         * synchronous events, use the CFE_PSP_ExceptionSigHandlerSuspend variant.
         *
         * This suspends the caller and prevents returning to the application.
         */
        sa.sa_sigaction = CFE_PSP_ExceptionSigHandlerSuspend;

        /*
         * add it back to the mask set.
         * This is supposed to be default unless SA_NODEFER flag is set,
         * but also setting it here to be sure.
         */
        sigaddset(&sa.sa_mask, signo);
    }
    else
    {
        /*
         * Use default handler which will return to the application
         * after logging the event
         */
        sa.sa_sigaction = CFE_PSP_ExceptionSigHandler;
    }
    sa.sa_flags = SA_SIGINFO;

    sigaction(signo, &sa, NULL);
}



/*
**   Name: CFE_PSP_AttachExceptions
**
**   This is called from the CFE Main task, before any other threads
**   are started.  Use this opportunity to install the handler for
**   CTRL+C events, which will now be treated as an exception.
**
**   Not only does this clean up the code by NOT requiring a specific
**   handler for CTRL+C, it also provides a way to exercise and test
**   the exception handling in general, which tends to be infrequently
**   invoked because otherwise it only happens with off nominal behavior.
**
**   This has yet another benefit that SIGINT events will make their
**   way into the exception and reset log, so it is visible why the
**   CFE shut down.
*/

void CFE_PSP_AttachExceptions(void)
{
    void *Addr[1];

    /*
     * preemptively call "backtrace" -
     * The manpage notes that backtrace is implemented in libgcc
     * which may be dynamically linked with lazy binding. So
     * by calling it once we ensure that it is loaded and therefore
     * it is safe to use in a signal handler.
     */
    backtrace(Addr, 1);

    OS_printf("CFE_PSP: CFE_PSP_AttachExceptions Called\n");

    /*
     * Block most other signals during handler execution.
     * Exceptions are for synchronous errors SIGFPE/SIGSEGV/SIGILL/SIGBUS
     */
    sigfillset(&CFE_PSP_AsyncMask);
    sigdelset(&CFE_PSP_AsyncMask, SIGILL);
    sigdelset(&CFE_PSP_AsyncMask, SIGFPE);
    sigdelset(&CFE_PSP_AsyncMask, SIGBUS);
    sigdelset(&CFE_PSP_AsyncMask, SIGSEGV);

    /*
     * Install sigint_handler as the signal handler for SIGINT.
     *
     * In the event that the user presses CTRL+C at the console
     * this will be recorded as an exception and use the general
     * purpose exception processing logic to shut down CFE.
     *
     * Also include SIGTERM so it will invoke a graceful shutdown
     */
    CFE_PSP_AttachSigHandler(SIGINT);
    CFE_PSP_AttachSigHandler(SIGTERM);

    /*
     * Clear any pending exceptions.
     *
     * This is just in case this is a PROCESSOR reset and there
     * was something still in the queue from the last lifetime.
     *
     * It should have been logged already, but if not, then
     * don't action on it now.
     */
    CFE_PSP_Exception_Reset();
}

/*
**
**   Name: CFE_PSP_SetDefaultExceptionEnvironment
**
**   Purpose: This function sets a default exception environment that can be used
**
**   Notes: The exception environment is local to each task Therefore this must be
**          called for each task that that wants to do floating point and catch exceptions
**          Currently, this is automaticall called from OS_TaskRegister for every task
*/

void CFE_PSP_SetDefaultExceptionEnvironment(void)
{
    /*
     * This additionally sets a handler for SIGFPE which will catch arithmetic errors
     * such as divide by zero.  Other possibilities are SIGILL/SIGBUS/SIGSEGV.
     *
     * This is primarily used as a proof-of-concept on this platform to demonstrate
     * how the exception handling feature works.
     *
     * As the PC-Linux platform is often used for debugging, it is better to
     * maintain the default signal handler for the SIGILL/SIGBUS/SIGSEGV which will
     * abort the program and generate a core file.
     */
    CFE_PSP_AttachSigHandler(SIGFPE);
}

int32 CFE_PSP_ExceptionGetSummary_Impl(const CFE_PSP_Exception_LogData_t* Buffer, char *ReasonBuf, uint32 ReasonSize)
{
    const char *ComputedReason = "unknown";

    /* check the "code" within the siginfo structure, which reveals more info about the FP exception */
    if (Buffer->context_info.si.si_signo == SIGFPE)
    {
        switch(Buffer->context_info.si.si_code)
        {
        case FPE_INTDIV:
            ComputedReason = "Integer divide by zero";
            break;
        case FPE_INTOVF:
            ComputedReason = "Integer overflow";
            break;
        case FPE_FLTDIV:
            ComputedReason = "Floating-point divide by zero";
            break;
        case FPE_FLTOVF:
            ComputedReason = "Floating-point overflow";
            break;
        case FPE_FLTUND:
            ComputedReason = "Floating-point underflow";
            break;
        case FPE_FLTRES:
            ComputedReason = "Floating-point inexact result";
            break;
        case FPE_FLTINV:
            ComputedReason = "Invalid floating-point operation";
            break;
        case FPE_FLTSUB:
            ComputedReason = "Subscript out of range";
            break;
        default:
            ComputedReason = "Unknown SIGFPE";
        }
        (void)snprintf(ReasonBuf, ReasonSize, "%s at ip 0x%lx", ComputedReason,
                (unsigned long)Buffer->context_info.si.si_addr);

    }
    else if (Buffer->context_info.si.si_signo == SIGINT)
    {
        /* interrupt e.g. CTRL+C */
        (void)snprintf(ReasonBuf, ReasonSize, "Caught SIGINT");
    }
    else
    {
        /*
         * other signal....
         * POSIX 2008 does provide a strsignal() function to get the name, but this
         * is a newer spec than what is targeted by CFE, so just print the number.
         */
        (void)snprintf(ReasonBuf, ReasonSize, "Caught Signal %d",Buffer->context_info.si.si_signo);
    }

    return CFE_PSP_SUCCESS;
}


