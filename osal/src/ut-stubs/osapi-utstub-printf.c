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
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"


int32 OS_ConsoleAPI_Init(void)
{
    return UT_DEFAULT_IMPL(OS_ConsoleAPI_Init);
}

/*****************************************************************************
 *
 * Stub function for OS_ConsoleWrite()
 *
 *****************************************************************************/
int32 OS_ConsoleWrite(uint32 console_id, const char *Str)
{
    UT_Stub_RegisterContext(UT_KEY(OS_ConsoleWrite), Str);
    return UT_DEFAULT_IMPL(OS_ConsoleWrite);
}

/*****************************************************************************
 *
 * Stub function for OS_printf()
 *
 *****************************************************************************/
void OS_printf(const char *string, ...)
{
    UT_Stub_RegisterContext(UT_KEY(OS_printf), string);

    int32   status;
    int32   length = strlen(string);
    va_list va;

    va_start(va,string);

    status = UT_DefaultStubImplWithArgs(__func__, UT_KEY(OS_printf), 0, va);

    if (status >= 0)
    {
        /*
         * Special treatment of a format string which is only "%s" ...
         * This is merely a way to avoid having to do full-blown printf processing
         * inside the UT stub (which would make it the full version, not a stub)
         */
        if (strcmp(string, "%s") == 0 ||
                strcmp(string, "%s\n") == 0)
        {
            string = va_arg(va,const char *);
        }
        length = strlen(string);
        if (length > 0)
        {
            /*
             * prune any supplied newlines -
             * one will be explicitly added
             *
             * (this is to ensure a consistent separator in the output buffer)
             */
            while (length > 0 && string[length-1] == '\n')
            {
                --length;
            }
            UT_Stub_CopyFromLocal(UT_KEY(OS_printf), string, length);
            UT_Stub_CopyFromLocal(UT_KEY(OS_printf), "\n", 1);
        }
    }

    va_end(va);
}

/*****************************************************************************
 *
 * Stub function for OS_printf_disable()
 *
 *****************************************************************************/
void OS_printf_disable(void)
{
    UT_DEFAULT_IMPL(OS_printf_disable);
}

/*****************************************************************************
 *
 * Stub function for OS_printf_enable()
 *
 *****************************************************************************/
void OS_printf_enable(void)
{
    UT_DEFAULT_IMPL(OS_printf_enable);
}

