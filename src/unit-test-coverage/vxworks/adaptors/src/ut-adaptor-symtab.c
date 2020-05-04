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
 * \file     ut-adaptor-symtab.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-loader.h"

#include <os-vxworks.h>
#include <os-impl-symtab.h>

/*
 * A UT-specific wrapper function to invoke the Symbol Table Iterator.
 * This is normally static so it needs this wrapper to call it.
 */
int32 UT_SymTabTest_CallIteratorFunc(const char *name, void* val, uint32 TestSize, uint32 SizeLimit)
{
    OS_VxWorks_SymbolDumpState.Sizelimit = SizeLimit;
    OS_VxWorks_SymbolDumpState.CurrSize = TestSize;
    /*
     * note the internal function is takes a name declared as "char*" to be
     * consistent with the VxWorks API, however the implementation does not
     * modify this argument.
     */
    return OS_SymTableIterator_Impl((char*)name,(OCS_SYM_VALUE)val,0,0,0);
}

