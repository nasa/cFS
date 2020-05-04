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
 * \file     ut-adaptor-module.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "osconfig.h"
#include "ut-adaptor-module.h"
#include "os-shared-module.h"


int32 Osapi_Call_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName)
{
    return OS_SymbolLookup_Static(SymbolAddress, SymbolName);
}

int32 Osapi_Call_ModuleLoad_Static(const char *ModuleName)
{
    return OS_ModuleLoad_Static(ModuleName);
}

