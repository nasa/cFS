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
 * \file     bsp-console-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "OCS_bsp-impl.h"

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
void OCS_OS_BSP_ConsoleOutput_Impl(const char *Str, uint32_t DataLen)
{
    int32_t retcode = UT_DEFAULT_IMPL(OCS_OS_BSP_ConsoleOutput_Impl);

    if (retcode == 0)
    {
        UT_Stub_CopyFromLocal(UT_KEY(OCS_OS_BSP_ConsoleOutput_Impl), Str, DataLen);
    }
}

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
void OCS_OS_BSP_ConsoleSetMode_Impl(uint32_t ModeBits)
{
    int32_t retcode = UT_DEFAULT_IMPL(OCS_OS_BSP_ConsoleSetMode_Impl);

    if (retcode == 0)
    {
        UT_Stub_CopyFromLocal(UT_KEY(OCS_OS_BSP_ConsoleSetMode_Impl), &ModeBits, sizeof(ModeBits));
    }
}

