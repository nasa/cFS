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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "bsp-impl.h"

#include "os-impl-tasks.h"
#include "os-impl-queues.h"
#include "os-impl-binsem.h"
#include "os-impl-countsem.h"
#include "os-impl-mutex.h"

#include "os-shared-common.h"
#include "os-shared-idmap.h"
#include "os-shared-errors.h"


POSIX_GlobalVars_t   POSIX_GlobalVars = { 0 };


/*---------------------------------------------------------------------------------------
   Name: OS_API_Impl_Init

   Purpose: Initialize the tables that the OS API uses to keep track of information
            about objects

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_API_Impl_Init(uint32 idtype)
{
    int32               return_code;

    return_code = OS_Posix_TableMutex_Init(idtype);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
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
void OS_IdleLoop_Impl(void)
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
} /* end OS_IdleLoop_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_ApplicationShutdown_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_ApplicationShutdown_Impl(void)
{
   /*
    * Raise a signal that is unblocked in OS_IdleLoop(),
    * which should break it out of the sigsuspend() call.
    */
   kill(getpid(), SIGHUP);
} /* end OS_ApplicationShutdown_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_CompAbsDelayTime
 *
 * Purpose:  Local helper function
 *
 * This function accept time interval, msecs, as an input and
 * computes the absolute time at which this time interval will expire.
 * The absolute time is programmed into a struct.
 *
 *-----------------------------------------------------------------*/
void  OS_Posix_CompAbsDelayTime( uint32 msecs, struct timespec * tm)
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
} /* end OS_CompAbsDelayTime */


