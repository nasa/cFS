/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 * 
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file     os-impl-no-symtab.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains a symbol table implementation for systems
 * that do not use dynamic symbol lookups. It returns OS_ERR_NOT_IMPLEMENTED
 * for all calls.
 */

#include <osapi.h>
#include "os-shared-module.h"


/*----------------------------------------------------------------
 *
 * Function: OS_SymbolLookup_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolLookup_Impl( cpuaddr *SymbolAddress, const char *SymbolName )
{
    return OS_ERR_NOT_IMPLEMENTED;

} /* end OS_SymbolLookup_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl ( const char *filename, uint32 SizeLimit )
{
   return(OS_ERR_NOT_IMPLEMENTED);

} /* end OS_SymbolTableDump_Impl */





