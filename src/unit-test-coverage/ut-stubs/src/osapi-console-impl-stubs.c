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
 * \file     osapi-console-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-printf.h"

/*
** Console output API (printf)
*/
void  OS_ConsoleWakeup_Impl(uint32 local_id)
{
    UT_DEFAULT_IMPL(OS_ConsoleWakeup_Impl);
}
int32 OS_ConsoleCreate_Impl(uint32 local_id)
{
    return UT_DEFAULT_IMPL(OS_ConsoleCreate_Impl);
}

