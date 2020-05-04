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
 * \file   bsp-impl.h
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: Contains functions prototype definitions and variables declarations
 *          for the OSAL BSP layer
 *
 * This is used to provide an abstract notion of certain platform-specific features:
 *
 *  - Startup arguments/options.  These are optional strings containing any
 *    passed-in parameters from a bootloader/shell/etc.  Not all platforms
 *    support this notion, but this provides a consistent access method for
 *    those platforms that do pass startup options.
 *
 *  - Exit code.  This is the "status" passed back to the operating system
 *    if/when the application terminates.  This is a numeric value with
 *    platform-defined meaning.
 *
 *  - Low-level console output.  Provide direct, synchronous access to the
 *    BSP-provided console or debug terminal device.
 */

#ifndef _osapi_bsp_impl_
#define _osapi_bsp_impl_

#include "osapi.h"

/*
 * A set of simplified console control options
 *
 * These constants are used with the OS_BSP_ConsoleSetMode_Impl() API
 * to allow debug console text to be output with additional attributes
 * on platforms that support it.
 *
 * This is NOT intended as a full terminal control abstraction,
 * just enough to draw visual attention to certain messages on
 * platforms that support it.
 */
#define OS_BSP_CONSOLEMODE_NORMAL    0x0 /**< Default output mode - clears any previously-set attributes */
#define OS_BSP_CONSOLEMODE_RED       0x1 /**< Red text, if terminal supports color */
#define OS_BSP_CONSOLEMODE_GREEN     0x2 /**< Green text, if terminal supports color */
#define OS_BSP_CONSOLEMODE_BLUE      0x4 /**< Blue text, if terminal supports color */
#define OS_BSP_CONSOLEMODE_HIGHLIGHT 0x8 /**< Highlighted/Emphasis text, if terminal supports it */

#define OS_BSP_CONSOLEMODE_TO_ANSICOLOR(x)  ((x) & 0x07)

/*
 * Macro for BSP debug messages, similar to OS_DEBUG in OSAL code.
 *
 * This is also controlled by the OS_DEBUG_PRINTF compile-time option,
 * except it does not reference the OSAL global variables like OS_DEBUG does.
 *
 * (This global is likely not initialized at the time the BSP code executes,
 * and are also generally not accessible from BSP code)
 */
#if defined(OSAL_CONFIG_DEBUG_PRINTF)
#define BSP_DEBUG(...)             \
    do                             \
    {                              \
        printf("%s():", __func__); \
        printf(__VA_ARGS__);       \
    } while (0)
#else
/* Debug printfs are not compiled in at all */
#define BSP_DEBUG(...)
#endif

/*
** Common/Abstract BSP state data
*/
typedef struct
{
    uint32 ArgC;      /* number of boot/startup parameters in ArgV */
    char **ArgV;      /* strings for boot/startup parameters */
    int32  AppStatus; /* value which can be returned to the OS (0=nominal) */
    uint32 MaxQueueDepth;   /* Queue depth limit supported by BSP (0=no limit) */
} OS_BSP_GlobalData_t;

/*
 * Common/Abstracted BSP state data
 */
extern OS_BSP_GlobalData_t OS_BSP_Global;

/*
 * Volume Table declaration (supplied by BSP; typically defined in bsp_voltab.c)
 */
extern OS_VolumeInfo_t OS_VolumeTable[NUM_TABLE_ENTRIES];

/********************************************************************/
/* INTERNAL BSP IMPLEMENTATION FUNCTIONS                            */
/********************************************************************/

/*----------------------------------------------------------------
   Function: OS_BSP_ConsoleOutput_Impl

    Purpose: Low level raw console data output.  Writes a sequence of
             characters directly to the BSP debug terminal or console device.

             The string is not required to be null terminated, and
             any control characters will be passed through.  Any
             non-printable ASCII codes will have platform-defined
             interpretation.

       Note: This should write the string as-is without buffering.
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleOutput_Impl(const char *Str, uint32 DataLen);

/*----------------------------------------------------------------
   Function: OS_BSP_ConsoleSetMode_Impl

    Purpose: Set the console output mode, if supported by the BSP.

             Causes any future text written to the debug console to
             be colored/highlighted accordingly.  Intended for use
             with test applications where certain messages may need
             visual distinction (e.g. failures).

             See the OS_BSP_CONSOLEMODE constants for possible values.
             Values may be bitwise OR'ed together.

             This call is ignored if the BSP does not support console
             control codes.
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleSetMode_Impl(uint32 ModeBits);

/*********************
   END bsp-impl.h
 *********************/
#endif /* _osapi_bsp_impl_ */
