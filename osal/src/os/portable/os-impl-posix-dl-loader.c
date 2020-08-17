/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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


