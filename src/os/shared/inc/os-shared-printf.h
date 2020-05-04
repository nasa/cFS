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
 * \file     os-shared-printf.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_PRINTF_H_
#define INCLUDE_OS_SHARED_PRINTF_H_

#include <os-shared-globaldefs.h>

/*
 * Variables related to the console buffer.
 * This is a simple ring buffer that decouples
 * the OS_printf() call from actual console output.
 *
 * The implementation layer may optionally spawn a
 * "utility task" or equivalent to forward data, or
 * it may process data immediately.
 */

typedef struct
{
    char device_name[OS_MAX_API_NAME];

    char *BufBase;                    /**< Start of the buffer memory */
    uint32 BufSize;                   /**< Total size of the buffer */
    volatile uint32 ReadPos;          /**< Offset of next byte to read */
    volatile uint32 WritePos;         /**< Offset of next byte to write */
    uint32 OverflowEvents;            /**< Number of lines dropped due to overflow */

} OS_console_internal_record_t;


extern OS_console_internal_record_t        OS_console_table[OS_MAX_CONSOLES];



/****************************************************************************************
                 CONSOLE / DEBUG API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_ConsoleAPI_Init

   Purpose: Initialize the OS-independent layer for console service

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_ConsoleAPI_Init             (void);



/*----------------------------------------------------------------
   Function: OS_ConsoleCreate_Impl

    Purpose: Prepare a console device for use
             For Async devices, this sets up the background writer task
 ------------------------------------------------------------------*/
int32 OS_ConsoleCreate_Impl(uint32 local_id);

/*----------------------------------------------------------------
   Function: OS_ConsoleOutput_Impl

    Purpose: Basic Console output implementation

   This function forwards the data from the console
   ring buffer into the actual output device/descriptor

   The data is already formatted, this just writes the characters.
 ------------------------------------------------------------------*/
void  OS_ConsoleOutput_Impl(uint32 local_id);

/*----------------------------------------------------------------
   Function: OS_ConsoleOutput_Impl

    Purpose: Console output data notification

   This is a notification API that is invoked whenever there
   is new data available in the console output buffer.

   For a synchronous console service, this may call
   OS_ConsoleWrite_Impl() directly.  For an async console
   service, this should wakeup the actual console servicing
   thread.
 ------------------------------------------------------------------*/
void  OS_ConsoleWakeup_Impl(uint32 local_id);


#endif  /* INCLUDE_OS_SHARED_PRINTF_H_ */

