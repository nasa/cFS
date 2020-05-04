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
 * \file     os-impl-posix-dl-loader.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains a module loader implementation for systems
 * that implement a POSIX-style dynamic module loader.  This includes
 * RTEMS even if built without its native POSIX API.
 *
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
 *   dlopen()
 *   dlclose()
 *   dlsym()
 *   dlerror()
 */
#include <string.h>
#include <stdlib.h>

#include "os-impl-loader.h"
#include "os-shared-module.h"




/****************************************************************************************
                                    Module Loader API
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl ( uint32 module_id, const char *translated_path )
{
    int32 status = OS_ERROR;

   dlerror();
   OS_impl_module_table[module_id].dl_handle = dlopen(translated_path, RTLD_NOW | RTLD_GLOBAL);
   if (OS_impl_module_table[module_id].dl_handle != NULL)
   {
       status = OS_SUCCESS;
   }
   else
   {
       OS_DEBUG("Error loading shared library: %s\n", dlerror());
   }

   return status;

} /* end OS_ModuleLoad_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_ModuleUnload_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl ( uint32 module_id )
{
    int32 status = OS_ERROR;

    /*
    ** Attempt to close/unload the module
    */
    dlerror();
    if (dlclose(OS_impl_module_table[module_id].dl_handle) == 0)
    {
        OS_impl_module_table[module_id].dl_handle = NULL;
        status = OS_SUCCESS;
    }
    else
    {
        OS_DEBUG("Error unloading shared library: %s\n", dlerror());
    }

    return status;

} /* end OS_ModuleUnload_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_ModuleGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl ( uint32 module_id, OS_module_prop_t *module_prop )
{
   /*
    * Limiting strictly to POSIX-defined API means there is no defined
    * method to get information about a module contents.
    *
    * The "dlinfo()" function might return some interesting information
    * but this is actually a non-posix extension / platform-defined API.
    *
    * This returns success - although there is no information to add here,
    * the parent/shared layer information is still valid.
    */
    return(OS_SUCCESS);

} /* end OS_ModuleGetInfo_Impl */


