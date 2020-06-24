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

