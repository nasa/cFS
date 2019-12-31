/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-osapi-module.h"

/*
 * The UT test case provides its own static symbol table
 */
#define OS_STATIC_SYMTABLE_SOURCE   OS_UT_STATIC_SYMBOL_TABLE

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi-module.c"


int32 Osapi_Call_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName)
{
    return OS_SymbolLookup_Static(SymbolAddress, SymbolName);
}

int32 Osapi_Call_ModuleLoad_Static(const char *ModuleName)
{
    return OS_ModuleLoad_Static(ModuleName);
}
