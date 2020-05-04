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
 * \file     osapi-debug.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *      Contains the implementation for OS_DEBUG().
 *
 *      This is only compiled in when OSAL_CONFIG_DEBUG_PRINTF is enabled.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


/*
 * User defined include files
 */
#include "os-shared-globaldefs.h"
#include "os-shared-common.h"

#define OS_DEBUG_OUTPUT_STREAM      stdout



/*----------------------------------------------------------------
 *
 * Function: OS_DebugPrintf
 *
 *  Purpose: Outputs a single debug statement to the console
 *
 *-----------------------------------------------------------------*/
void OS_DebugPrintf(uint32 Level, const char *Func, uint32 Line, const char *Format, ...)
{
    va_list va;

    if (OS_SharedGlobalVars.DebugLevel >= Level)
    {
        va_start(va, Format);
        fprintf(OS_DEBUG_OUTPUT_STREAM, "%s():%lu:",Func,(unsigned long)Line);
        vfprintf(OS_DEBUG_OUTPUT_STREAM, Format, va);
        va_end(va);
    }

} /* end OS_DebugPrintf */

