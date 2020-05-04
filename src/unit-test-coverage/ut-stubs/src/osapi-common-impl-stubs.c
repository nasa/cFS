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
 * \file     osapi-common-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-common.h"

UT_DEFAULT_STUB(OS_API_Impl_Init, (uint32 idtype))

void  OS_IdleLoop_Impl               (void)
{
    UT_DEFAULT_IMPL(OS_IdleLoop_Impl);
}

void  OS_ApplicationShutdown_Impl    (void)
{
    UT_DEFAULT_IMPL(OS_ApplicationShutdown_Impl);
}

