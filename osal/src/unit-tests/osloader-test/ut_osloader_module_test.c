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
** File:  ut_osloader_module_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osloader_module_test.h"
#include "ut_osloader_test_platforms.h"

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
** Syntax: OS_ModuleLoad
** Purpose: Loads the new ELF object module into the RTOS
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NO_FREE_IDS if the module table is full
**          OS_ERR_NAME_TAKEN if the module name has already been used
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_module_load_test()
{
    int i;
    int32 res = 0;
    const char* testDesc;
    uint32  test_setup_invalid = 0;
    uint32  module_id;
    uint32  module_id2;
    char    module_name[UT_OS_NAME_BUFF_SIZE];
    char    module_file_name[UT_OS_PATH_BUFF_SIZE];

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_ModuleLoad(0, "TestModule", UT_OS_GENERIC_MODULE_NAME1);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_module_load_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_ModuleLoad(0, "TestModule", UT_OS_GENERIC_MODULE_NAME1);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_ModuleLoad(&module_id, 0, UT_OS_GENERIC_MODULE_NAME1);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Null-pointer-arg-3";

    res = OS_ModuleLoad(&module_id, "TestModule", 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 No-free-IDs";

    /* Setup */
    for ( i = 0; i< OS_MAX_MODULES; i++ )
    {
        snprintf(module_name, sizeof(module_name), UT_OS_GENERIC_MODULE_NAME_TEMPLATE, i);
        snprintf(module_file_name, sizeof(module_file_name), UT_OS_GENERIC_MODULE_FILE_TEMPLATE, i);
        res = OS_ModuleLoad(&module_id, module_name, module_file_name);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#4 No-free-IDs - Module Load failed";
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
            test_setup_invalid = 1;
            break;
        }
    }

    if ( test_setup_invalid == 0 )
    {
        res = OS_ModuleLoad(&module_id, "OneTooMany", UT_OS_GENERIC_MODULE_NAME2);
        if (res == OS_ERR_NO_FREE_IDS)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    }
    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    testDesc = "#5 Duplicate-name";

    /* Setup */
    res = OS_ModuleLoad(&module_id2, "DUPLICATE", UT_OS_GENERIC_MODULE_NAME2);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Duplicate-name - Module Load failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_ModuleLoad(&module_id, "DUPLICATE", UT_OS_GENERIC_MODULE_NAME2);
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        res = OS_ModuleUnload(module_id2);
    }

    /*-----------------------------------------------------*/
    testDesc = "#6 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#7 Nominal";

    res = OS_ModuleLoad(&module_id, "Good", UT_OS_GENERIC_MODULE_NAME2);
    if ( res == OS_SUCCESS )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    res = OS_ModuleUnload(module_id);

UT_os_module_load_test_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_ModuleUnload
** Purpose: Unloads the module from the running RTOS
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid module id
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_module_unload_test()
{
    int32 res = 0;
    const char* testDesc;
    uint32  module_id = 0;

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_ModuleUnload(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_module_unload_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_ModuleUnload(99999);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_ModuleLoad(&module_id, "Good", UT_OS_GENERIC_MODULE_NAME2);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Module Load failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_ModuleUnload(module_id);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

UT_os_module_unload_test_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_ModuleInfo
** Purpose: Returns module information about a given module id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid module id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_module_info_test()
{
    int32              res = 0;
    const char*        testDesc;
    uint32             module_id;
    OS_module_prop_t   module_info;

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_ModuleInfo(0, &module_info);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_module_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg";

    res = OS_ModuleInfo(0, NULL);
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-ID-arg";

    res = OS_ModuleInfo(99999, &module_info);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_ModuleLoad(&module_id, "Good", UT_OS_GENERIC_MODULE_NAME2);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Module Load failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_ModuleInfo(module_id, &module_info);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_ModuleUnload(module_id);
    }

UT_os_module_info_test_exit_tag:
    return;
    
}

/*================================================================================*
** End of File: ut_osloader_module_test.c
**================================================================================*/
