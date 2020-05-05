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
 * \file   os-impl-posix-dl-symtab.c
 * \author joseph.p.hickey@nasa.gov
 *
 * This file contains a module loader implementation for systems
 * that implement a POSIX-style dynamic module loader.  This includes
 * RTEMS even if built without its native POSIX API.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * typically this must include dlfcn.h and whatever else is required
 * to get the prototypes of these functions:
 *
 *   dlsym()
 *   dlerror()
 *
 * In POSIX these functions are grouped with the loader (dl) library
 */
#include <string.h>
#include <stdlib.h>

#include "os-impl-loader.h"
#include "os-shared-module.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/*
 * Determine what to pass in for the first parameter of dlsym()
 *
 * If the "os-impl-loader.h" header already defined this, then use that.
 *
 * Otherwise, check if the C library provides an "RTLD_DEFAULT" symbol -
 * This symbol is not POSIX standard but many implementations do provide it.
 *
 * Lastly, if nothing else works, use NULL.  This is technically undefined
 * behavior per POSIX, but most implementations do seem to interpret this
 * as referring to the complete process (base executable + all loaded modules).
 */
#ifndef OSAL_DLSYM_DEFAULT_HANDLE
#ifdef RTLD_DEFAULT
#define OSAL_DLSYM_DEFAULT_HANDLE   RTLD_DEFAULT
#else
#define OSAL_DLSYM_DEFAULT_HANDLE   NULL
#endif
#endif



/****************************************************************************************
                                    Symbol table API
 ***************************************************************************************/

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
    int32        status = OS_ERROR;
    const char   *dlError;           /*  Pointer to error string   */
    void         *Function;

    /*
     * call dlerror() to clear any prior error that might have occurred.
     */
    dlerror();
    Function = dlsym(OSAL_DLSYM_DEFAULT_HANDLE, SymbolName);
    dlError = dlerror();

    /*
     * For the POSIX DL implementation, if the symbol does not exist
     * then dlerror() is supposed to return non-null.  This is intended
     * to differentiate between valid symbols which are actually 0/NULL
     * and invalid symbols that do not exist.
     *
     * Some implementations do _not_ implement this detail, and dlerror()
     * still returns NULL after looking up an invalid symbol name.
     *
     * In practice, this is expected to be used for looking up functions
     * and as such all valid symbols should be non-NULL, so NULL is considered
     * an error even if the C library doesn't consider this an error.
     */
    if( dlError == NULL && Function != NULL )
    {
        *SymbolAddress = (cpuaddr)Function;
        status = OS_SUCCESS;
    }

    return status;

} /* end OS_SymbolLookup_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *  POSIX DL does not provide
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl ( const char *filename, uint32 SizeLimit )
{
    /*
     * Limiting strictly to POSIX-defined API means there is no defined
     * method to get iterate over the symbol table.
     *
     * Some C libraries do offer an extension to provide this function, so
     * if this becomes a requirement on those platforms, this function
     * might need to move.
     *
     * Currently this is not a widely used/needed feature so it will report
     * unimplemented here.
     */

   return(OS_ERR_NOT_IMPLEMENTED);

} /* end OS_SymbolTableDump_Impl */


