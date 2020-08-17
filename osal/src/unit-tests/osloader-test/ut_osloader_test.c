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

/*================================================================================*
** File:  ut_osloader_test.c
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osloader_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Main
**--------------------------------------------------------------------------------*/

void UtTest_Setup(void)
{
    uint32 fs_id;

    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * This test needs to load the modules from the filesystem, so
     * there must be a virtual path corresponding to the path where
     * the module files reside.  This UT-specific mapping should be
     * independent of the volume tables provided by the BSP.
     */
    if (OS_FileSysAddFixedMap(&fs_id, "./utmod", "/utmod") != OS_SUCCESS)
    {
        UtAssert_Abort("OS_FileSysAddFixedMap() failed");
    }

    UtTest_Add(UT_os_module_load_test, NULL, NULL, "OS_ModuleLoad");
    UtTest_Add(UT_os_module_unload_test, NULL, NULL, "OS_ModuleUnload");
    UtTest_Add(UT_os_module_info_test, NULL, NULL, "OS_ModuleInfo");

    UtTest_Add(UT_os_symbol_lookup_test, NULL, NULL, "OS_SymbolLookup");
    UtTest_Add(UT_os_symbol_table_dump_test, NULL, NULL, "OS_SymbolTableDump");
}

/*================================================================================*
** End of File: ut_osloader_test.c
**================================================================================*/
