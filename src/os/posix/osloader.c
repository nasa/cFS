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
 * \file   osloader.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains the module loader and symbol lookup functions for the OSAL.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "os-impl.h"

/*
 * The posix OSAL (by definition) always uses the posix dlfcn.h loading functions
 * Other non-posix OS's (i.e. classic RTEMS) share posix-style dl functions which is
 * why it is put into a separate code blob.
 */
#include "../portable/os-impl-posix-dl.c"

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl ( uint32 module_id, OS_module_prop_t *module_prop )
{
   /*
    * Note that the "dlinfo()" function might return some interesting
    * information for this API, but this is actually a non-posix GNU extension.
    * However this is not done for now, limiting to only posix APIs here
    */
    return(OS_SUCCESS);

} /* end OS_ModuleGetInfo_Impl */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl ( const char *filename, uint32 SizeLimit )
{

   return(OS_ERR_NOT_IMPLEMENTED);

} /* end OS_SymbolTableDump_Impl */

