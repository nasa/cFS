/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/**
 * \file cfe_psp_module.h
 *
 *  Created on: Jul 17, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Placeholder for file content description
 */

#ifndef CFE_PSP_MODULE_H_
#define CFE_PSP_MODULE_H_

#include <cfe_psp.h>
#include <target_config.h>

typedef enum
{
    CFE_PSP_MODULE_TYPE_INVALID = 0,
    CFE_PSP_MODULE_TYPE_VALID_RANGE  = 1000,
    CFE_PSP_MODULE_TYPE_SIMPLE,
    /* May be extended in the future */
    CFE_PSP_MODULE_TYPE_MAX
} CFE_PSP_ModuleType_t;

/**
 * Protoype for a PSP module initialization function
 */
typedef void (*CFE_PSP_ModuleInitFunc_t)(uint32 PspModuleId);

/**
 * Concrete version of the abstract API definition structure
 */
typedef const struct
{
    CFE_PSP_ModuleType_t ModuleType;
    uint32 OperationFlags;
    CFE_PSP_ModuleInitFunc_t Init;
    /* More API calls may be added for other module types */
} CFE_PSP_ModuleApi_t;

/**
 * Macro to simplify declaration of the IO Driver API structure
 * according to the required naming convention.
 *
 * The "name" argument should match the name of the module object file
 */
#define CFE_PSP_MODULE_DECLARE_SIMPLE(name)                 \
    static void name##_Init(uint32 PspModuleId);            \
    CFE_PSP_ModuleApi_t CFE_PSP_##name##_API =              \
    {                                                       \
        .ModuleType = CFE_PSP_MODULE_TYPE_SIMPLE,           \
        .OperationFlags = 0,                                \
        .Init = name##_Init,                                \
    }

/**
 * Initialize the included PSP modules.
 *
 * This is an optional part of the PSP and some PSPs may not use it.
 *
 * This function should only be called during PSP initialization before the
 * system is operational.  It is not intended to be called from application code after
 * CFE has started.  The function is not necessarily be thread-safe and should be called
 * before any child threads are created.
 *
 * Note that this does _not_ return any status --
 * If a failure occurs during initialization that would make normal operation impossible,
 * then the module itself will call CFE_PSP_Panic() and this will not return.  Otherwise,
 * benign/recoverable failures are expected to be just that, and the calling code will not
 * need to take any special action either way.
 *
 * In short, if this function returns, then it means the system is good enough to continue.
 */
void CFE_PSP_ModuleInit(void);

/**
 * Obtain the ID for a named module.
 *
 * Although this is currently prototyped as a function scoped to the PSP,
 * this prototype could be moved to the public area so the CFS could use this (TBD)
 *
 * \param ModuleName    Name of the module to look up
 * \param PspModuleId   Will be set to the ID of the module if successful
 * \returns CFE_PSP_SUCCESS if lookup succeeded
 */
int32 CFE_PSP_Module_FindByName(const char *ModuleName, uint32 *PspModuleId);

/**
 * Obtain the API for a specific module.
 *
 * \param PspModuleId   The ID of the module (configuration-dependent)
 * \param API           Will be set to the API structure if successful
 * \returns CFE_PSP_SUCCESS if lookup succeeded
 */
int32 CFE_PSP_Module_GetAPIEntry(uint32 PspModuleId, CFE_PSP_ModuleApi_t **API);


#endif /* CFE_PSP_MODULE_H_ */
