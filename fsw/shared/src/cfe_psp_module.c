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
 * \file cfe_psp_module.c
 *
 *  Created on: Jul 25, 2014
 *      Author: jphickey
 */

#include <stdio.h>
#include <string.h>
#include <osapi.h>

#include "cfe_psp_configdata.h"
#include "cfe_psp_module.h"

/*
 * When using an OSAL that also supports "opaque object ids", choose values here
 * that will fit in with the OSAL object ID values and not overlap anything.
 */
#ifdef OS_OBJECT_TYPE_USER
#define CFE_PSP_MODULE_BASE         ((OS_OBJECT_TYPE_USER + 0x100) << OS_OBJECT_TYPE_SHIFT)
#define CFE_PSP_MODULE_INDEX_MASK   OS_OBJECT_INDEX_MASK
#else
#define CFE_PSP_MODULE_BASE         0x01100000
#define CFE_PSP_MODULE_INDEX_MASK   0xFFFF
#endif

static uint32 CFE_PSP_ModuleCount = 0;

/***************************************************
 * Function Name: CFE_PSP_ModuleInit
 *
 * See prototype for full description
 */
void CFE_PSP_ModuleInit(void)
{
    CFE_StaticModuleLoadEntry_t *Entry;
    CFE_PSP_ModuleApi_t *ApiPtr;

    /*
     * Call the init function for all statically linked modules
     */
    Entry = GLOBAL_CONFIGDATA.PspModuleList;
    if (Entry != NULL)
    {
        while(Entry->Name != NULL)
        {
            ApiPtr = (CFE_PSP_ModuleApi_t *)Entry->Api;
            if ((uint32)ApiPtr->ModuleType > CFE_PSP_MODULE_TYPE_VALID_RANGE &&
                    (uint32)ApiPtr->ModuleType < CFE_PSP_MODULE_TYPE_MAX &&
                    ApiPtr->Init != NULL)
            {
                (*ApiPtr->Init)(CFE_PSP_MODULE_BASE | CFE_PSP_ModuleCount);
            }
            ++Entry;
            ++CFE_PSP_ModuleCount;
        }
    }
}


/***************************************************
 * Function Name: CFE_PSP_Module_GetAPIEntry
 *
 * See prototype for full description
 */
int32 CFE_PSP_Module_GetAPIEntry(uint32 PspModuleId, CFE_PSP_ModuleApi_t **API)
{
    int32 Result;
    uint32 LocalId;

    Result = CFE_PSP_INVALID_MODULE_ID;
    if ((PspModuleId & ~CFE_PSP_MODULE_INDEX_MASK) == CFE_PSP_MODULE_BASE)
    {
        LocalId = PspModuleId & CFE_PSP_MODULE_INDEX_MASK;
        if (LocalId < CFE_PSP_ModuleCount)
        {
            *API = (CFE_PSP_ModuleApi_t *)GLOBAL_CONFIGDATA.PspModuleList[LocalId].Api;
            Result = CFE_PSP_SUCCESS;
        }
    }

    return Result;
}


/***************************************************
 * Function Name: CFE_PSP_Module_FindByName
 *
 * See prototype for full description
 */
int32 CFE_PSP_Module_FindByName(const char *ModuleName, uint32 *PspModuleId)
{
    uint32 i;
    int32 Result;
    CFE_StaticModuleLoadEntry_t *Entry;

    Entry = GLOBAL_CONFIGDATA.PspModuleList;
    Result = CFE_PSP_INVALID_MODULE_NAME;
    i = 0;
    while (i < CFE_PSP_ModuleCount)
    {
        if (strcmp(Entry->Name, ModuleName) == 0)
        {
            *PspModuleId = CFE_PSP_MODULE_BASE | (i & CFE_PSP_MODULE_INDEX_MASK);
            Result = CFE_PSP_SUCCESS;
            break;
        }
        ++Entry;
        ++i;
    }

    return Result;
}
