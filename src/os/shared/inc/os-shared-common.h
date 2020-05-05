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
 * \file     os-shared-common.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_COMMON_H_
#define INCLUDE_OS_SHARED_COMMON_H_

#include <os-shared-globaldefs.h>

/*
 * A "magic number" that when written to the "ShutdownFlag" member
 * of the global state structure indicates an active shutdown request.
 */
#define OS_SHUTDOWN_MAGIC_NUMBER    0xABADC0DE



/* Global variables that are common between implementations */
struct OS_shared_global_vars
{
   bool              Initialized;

   /*
    * The console device ID used for OS_printf() calls
    */
   uint32            PrintfConsoleId;

   /*
    * PrintfEnabled and ShutdownFlag are marked "volatile"
    * because they are updated and read by different threads
    */
   volatile bool     PrintfEnabled;
   volatile uint32   ShutdownFlag;
   int32             MicroSecPerTick;
   int32             TicksPerSecond;

#ifdef OSAL_CONFIG_DEBUG_PRINTF
   uint8             DebugLevel;
#endif

};


/*
 * Shared data structure for global values
 */
extern OS_SharedGlobalVars_t OS_SharedGlobalVars;

/*---------------------------------------------------------------------------------------
   Name: OS_API_Impl_Init

   Purpose: Initialize the OS-specific layer for the given object type

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_API_Impl_Init               (uint32 idtype);



/*
 * This functions implement a the OS-specific portion
 * of various OSAL functions.  They are defined in
 * OS-specific source files.
 */


/*----------------------------------------------------------------

   Function: OS_IdleLoop_Impl

   Purpose: Block the "idle" thread until woken up

   The "IdleLoop_Impl" is called by the main thread once
   everything is running and there is no more work to do.

   It should suspend the calling thread until a wakeup
   event happens.

 ------------------------------------------------------------------*/
void  OS_IdleLoop_Impl               (void);

/*----------------------------------------------------------------

   Function: OS_ApplicationShutdown_Impl

    Purpose: Wake up the idle task

   The "ApplicationShutdown_Impl" should wake up whatever
   task is currently suspended in "IdleLoop_Impl" and cause
   that thread to resume and return to its caller.

   NOTE: This should not block but it may cause the current
   thread to be preempted by the thread that was woken up,
   depending on priority levels.

 ------------------------------------------------------------------*/
void  OS_ApplicationShutdown_Impl    (void);

#endif  /* INCLUDE_OS_SHARED_COMMON_H_ */

