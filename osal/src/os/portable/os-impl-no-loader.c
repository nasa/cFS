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
 * \file     os-impl-no-loader.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains a module loader implementation for systems
 * that do not use dynamic modules. It returns OS_ERR_NOT_IMPLEMENTED
 * for all calls.
 */


#include <osapi.h>
#include "os-shared-module.h"

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
    return OS_ERR_NOT_IMPLEMENTED;

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
    return OS_ERR_NOT_IMPLEMENTED;

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
    return OS_ERR_NOT_IMPLEMENTED;

} /* end OS_ModuleGetInfo_Impl */






