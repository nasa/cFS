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
 * \file     OCS_bsp-impl.h
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
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

#ifndef INCLUDE_OCS_BSP_IMPL_H_
#define INCLUDE_OCS_BSP_IMPL_H_


#include <OCS_basetypes.h>

#define OCS_OS_BSP_CONSOLEMODE_NORMAL    0x2101
#define OCS_OS_BSP_CONSOLEMODE_RED       0x2102
#define OCS_OS_BSP_CONSOLEMODE_GREEN     0x2104
#define OCS_OS_BSP_CONSOLEMODE_BLUE      0x2108
#define OCS_OS_BSP_CONSOLEMODE_HIGHLIGHT 0x2110

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
extern void OCS_OS_BSP_ConsoleOutput_Impl(const char *Str, uint32_t DataLen);

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
extern void OCS_OS_BSP_ConsoleSetMode_Impl(uint32_t ModeBits);


#endif  /* INCLUDE_OCS_BSP_IMPL_H_ */

