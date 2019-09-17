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
 * \file   osapi-printf.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *
 *      Contains the abstraction for the OS_printf() call.
 *
 *      This top level contains only the master on/off switch for OS_printf(),
 *      that is the OS_printf_enable() and OS_printf_disable() API calls.
 *
 *      If enabled, this OS_printf() uses the C library "vsnprintf()" call
 *      to format the actual string for output.  As this is a C99 function
 *      it should be present on all compliant machines.  In the event that
 *      the machine's C library does not provide this function, the user
 *      would have to provide a compatible substitute to link to.
 *
 *      Once the string is formatted, it is passed to the lower level
 *      implementation to do the actual output.  This would typically write
 *      to a console device but may alternatively write to any other
 *      implementation-defined output interface, such as a system log or
 *      serial port.
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

/*
 * The OS_BUFFER_SIZE and OS_BUFFER_MSG_DEPTH
 * directives pertain to the printf() buffer.
 *
 * In conjuction with a "utility task" this decouples
 * the OS_printf() call from the actual console output,
 * intended for systems where the console is a slow
 * serial link that can block.
 *
 * These are both typically defined in osconfig.h,
 * but if not, then supply default values here.
 */
#ifndef OS_BUFFER_SIZE
#define OS_BUFFER_SIZE          172
#endif

#ifndef OS_BUFFER_MSG_DEPTH
/* When using the utility task, buffer up to 100 messages by default */
#define OS_BUFFER_MSG_DEPTH     100
#endif


/*
 * The OS_PRINTF_CONSOLE_NAME, if defined, will be pre-pended to the content of
 * every call to OS_printf().  This can be a useful feature to provide
 * some visual differentiation between strings produced from OS_printf()
 * versus other debug prints.
 *
 * If desired, it can be specified in osconfig.h.  Otherwise the empty
 * string will be used (no added tag).
 */
#ifndef OS_PRINTF_CONSOLE_NAME
#define OS_PRINTF_CONSOLE_NAME ""
#endif

/* reserve buffer memory for the printf console device */
static char OS_printf_buffer_mem[(sizeof(OS_PRINTF_CONSOLE_NAME) + OS_BUFFER_SIZE) * OS_BUFFER_MSG_DEPTH];

/* The global console state table */
OS_console_internal_record_t        OS_console_table[OS_MAX_CONSOLES];

/*
 *********************************************************************************
 *          INITIALIZATION
 *********************************************************************************
 */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_ConsoleAPI_Init(void)
{
    OS_console_internal_record_t *console;
    int32 return_code;
    uint32 local_id;
    OS_common_record_t *record;

    memset(&OS_console_table, 0, sizeof(OS_console_table));


    /*
     * Configure a console device to be used for OS_printf() calls.
     */
    return_code = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_CONSOLE, OS_PRINTF_CONSOLE_NAME, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
        console = &OS_console_table[local_id];

        record->name_entry = console->device_name;
        strcpy(console->device_name, OS_PRINTF_CONSOLE_NAME);

        /*
         * Initialize the ring buffer pointers
         */
        console->BufBase = OS_printf_buffer_mem;
        console->BufSize = sizeof(OS_printf_buffer_mem);

        return_code = OS_ConsoleCreate_Impl(local_id);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, record, &OS_SharedGlobalVars.PrintfConsoleId);

        /*
         * Printf can be enabled by default now that the buffer is configured.
         */
        OS_SharedGlobalVars.PrintfEnabled = true;
    }

    return OS_SUCCESS;
} /* end OS_ConsoleAPI_Init */

/*
 *********************************************************************************
 *          LOCAL HELPER FUNCTIONS
 *********************************************************************************
 */

/*----------------------------------------------------------------
 *
 * Function: OS_Console_CopyOut
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *    Write into the console ring buffer
 *
 *    The NextWritePos is an input-output and contains the position
 *    in the ring buffer to start writing into.  This may or may not
 *    be the same as the value in the global.  It is only updated
 *    if the string is written in its entirety.
 *
 *    The intent is to avoid truncating a string if it does not fit.
 *    Either the entire string should be written, or none of it.
 *
 *-----------------------------------------------------------------*/
static int32 OS_Console_CopyOut(OS_console_internal_record_t *console, const char *Str, uint32 *NextWritePos)
{
    const char *pmsg;
    uint32 WriteOffset;
    int32 return_code;

    return_code = OS_ERROR;
    pmsg = Str;
    WriteOffset = *NextWritePos;
    while (true)
    {
        if (*pmsg == 0)
        {
            /* String is complete */
            *NextWritePos = WriteOffset;
            return_code = OS_SUCCESS;
            break;
        }
        console->BufBase[WriteOffset] = *pmsg;
        WriteOffset = WriteOffset + 1;
        if (WriteOffset >= console->BufSize)
        {
            WriteOffset = 0;
        }

        if (WriteOffset == console->ReadPos)
        {
            /* out of space */
            return_code = OS_QUEUE_FULL;
            break;
        }

        ++pmsg;
    }

    return return_code;
} /* end OS_Console_CopyOut */

/*
 *********************************************************************************
 *          PUBLIC API (application-callable functions)
 *********************************************************************************
 */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleWrite
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Write into the console ring buffer
 *
 *-----------------------------------------------------------------*/
int32 OS_ConsoleWrite(uint32 console_id, const char *Str)
{
    int32 return_code;
    OS_common_record_t *record;
    uint32 local_id;
    OS_console_internal_record_t *console;
    uint32 PendingWritePos;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_CONSOLE, console_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
        console = &OS_console_table[local_id];

        /*
         * The entire string should be put to the ring buffer,
         * or none of it.  Therefore the WritePos in the table
         * is not updated until complete success.
         */
        PendingWritePos = console->WritePos;

        if (console->device_name[0] != 0)
        {
            return_code = OS_Console_CopyOut(console, console->device_name, &PendingWritePos);
        }
        if (return_code == OS_SUCCESS)
        {
            return_code = OS_Console_CopyOut(console, Str, &PendingWritePos);
        }

        if (return_code == OS_SUCCESS)
        {
            /* the entire message was successfully written */
            console->WritePos = PendingWritePos;
        }
        else
        {
            /* the message did not fit */
            ++console->OverflowEvents;
        }

        /*
         * Notify the underlying console implementation of new data.
         * This will forward the data to the actual console device.
         *
         * This is done while still locked, so it can support
         * either a synchronous or asynchronous implementation.
         */
        OS_ConsoleWakeup_Impl(local_id);

        OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_CONSOLE);
    }


    return return_code;
} /* end OS_ConsoleWrite */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_printf
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_printf( const char *String, ...)
{
    va_list va;
    char msg_buffer [OS_BUFFER_SIZE];
    int actualsz;

    if ( !OS_SharedGlobalVars.Initialized )
    {
        /*
         * Catch some historical mis-use of the OS_printf() call.
         *
         * Typically OS_printf() should NOT be called before OS_API_Init().
         *
         * This was never guaranteed to work, particularly on a VxWorks
         * deployment where the utility task was enabled.
         *
         * However, some PSPs do this, particularly those that used POSIX
         * where it happened to work (because OS_printf just called printf).
         *
         * As a workaround, use the OS_DEBUG facility to dump the message,
         * along with a clue that this API is being used inappropriately.
         *
         * If debugging is not enabled, then this message will be silently
         * discarded.
         */
        OS_DEBUG("BUG: OS_printf() called before init: %s", String);
    }
    else if ( OS_SharedGlobalVars.PrintfEnabled )
    {
        /*
         * Call vsnprintf() to determine the actual size of the
         * string we are going to write to the buffer after formatting.
         */
        va_start(va,String);
        actualsz = vsnprintf(msg_buffer, sizeof(msg_buffer), String, va);
        va_end(va);

        if (actualsz < 0)
        {
            /* unlikely: vsnprintf failed */
            actualsz = 0;
        }
        else if (actualsz >= OS_BUFFER_SIZE)
        {
            /* truncate */
            actualsz = OS_BUFFER_SIZE-1;
        }

        msg_buffer[actualsz] = 0;

        OS_ConsoleWrite(OS_SharedGlobalVars.PrintfConsoleId, msg_buffer);
    }

} /* end OS_printf */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_printf_disable
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_printf_disable(void)
{
   OS_SharedGlobalVars.PrintfEnabled = false;
} /* end OS_printf_disable */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_printf_enable
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_printf_enable(void)
{
   OS_SharedGlobalVars.PrintfEnabled = true;
} /* end OS_printf_enable */

