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
 * \file     ut-adaptor-console.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-console.h"

#include <os-vxworks.h>
#include <os-impl-console.h>

void*  const UT_Ref_OS_impl_console_table = OS_impl_console_table;
size_t const UT_Ref_OS_impl_console_table_SIZE = sizeof(OS_impl_console_table);

void UT_ConsoleTest_TaskEntry(int arg)
{
    OS_VxWorks_ConsoleTask_Entry(arg);
}

void UT_ConsoleTest_SetConsoleAsync(uint32 local_id, bool is_async)
{
    OS_impl_console_table[local_id].is_async = is_async;
}
