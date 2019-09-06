/*
 *  Copyright (c) 2004-2018, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
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
 * Stub function for OS_printf()
 *
 *****************************************************************************/
void OS_printf(const char *string, ...)
{
    int32   status;
    int32   length = strlen(string);
    va_list va;

    va_start(va,string);

    UT_Stub_RegisterContext(UT_KEY(OS_printf), string);
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

