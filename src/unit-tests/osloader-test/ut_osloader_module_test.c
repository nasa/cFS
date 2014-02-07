/*================================================================================*
** File:  ut_osloader_module_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osloader_module_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#ifndef OS_INCLUDE_MODULE_LOADER
#define OS_MAX_MODULES  10
#endif  /* !OS_INCLUDE_MODULE_LOADER */

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern UT_OsLogInfo_t g_logInfo;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/* Test code template for testing a single OSAL API with multiple test cases */

#if 0
void UT_os_sample_test()
{
    /* Must declare these variables for each function. They can be renamed.
     * They're referenced in the macros used to track test cases and their results. */
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc = NULL;

    /* Call this once at the beginning of the function to initialize the test variables. */
    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*
     * For each test case,
     *   1. Assign testDesc a brief description of the test
     *   2. Setup the test environment, if necessary
     *   3. Run the test
     *   4. Log result by calling UT_OS_SET_TEST_RESULT_MACRO
     *   4. Reset the test environment, if neccessary
     *
     * NOTE: "Not implemented" is always checked first but not
     *       being included as a test case.
     *       "Nominal" test case is always the last test case.
     *-----------------------------------------------------*/

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    /* TODO: Setup the test environment, if necessary */

    if (OS_xxx() == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_sample_test_exit_tag;
    }

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(NULL,...) == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(aVeryLoooooongName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(...) != OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)

    /* TODO: Reset the test environment here, if necessary */

UT_os_sample_test_exit_tag:
    /* Call these macros at the very end of the function to close out the test variables
     * and get it added to the global list being tracked. */
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_xxx", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}
#endif

/*--------------------------------------------------------------------------------*
** Syntax: OS_ModuleTableInit
** Purpose: Initialize the tables that the OS API uses to keep track of information
**          about objects
** Parameters: To-be-filled-in
** Returns: OS_ERROR on an unsuccessful inits
**          OS_SUCCESS on a successful inits
**--------------------------------------------------------------------------------*/
void UT_os_module_table_init_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "#1 Init-not-call-first - Manual inspection required";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_MIR)

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_ModuleTableInit();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        testDesc = "API Not implemented";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
    }
    else if (res == OS_SUCCESS)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    }
    else
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_ModuleTableInit", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

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
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  test_setup_invalid = 0;
    uint32  module_id;
    uint32  module_id2;
    char    module_name[OS_MAX_API_NAME];
    char    module_file_name[OS_MAX_API_NAME];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_ModuleLoad(0, "TestModule","/cf/module.so");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_module_load_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_ModuleLoad(0, "TestModule", "/cf/module.so");
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_ModuleLoad(&module_id, 0, "/cf/module.so");
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Null-pointer-arg-3";

    res = OS_ModuleLoad(&module_id, "TestModule", 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 No-free-IDs";

    /* Setup */
    for ( i = 0; i< OS_MAX_MODULES; i++ )
    {
        memset(module_name, '\0', sizeof(module_name));
        UT_os_sprintf(module_name, "MODULE%d",i);
        memset(module_file_name, '\0', sizeof(module_file_name));
        UT_os_sprintf(module_file_name, "/cf/MODULE%d.so",i);
        res = OS_ModuleLoad(&module_id, module_name, module_file_name);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#4 No-free-IDs - Module Load failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
            test_setup_invalid = 1;
            break;
        }
    }

    if ( test_setup_invalid == 0 )
    {
        res = OS_ModuleLoad(&module_id, "OneTooMany", "/cf/MODULE1.so");
        if (res == OS_ERR_NO_FREE_IDS)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    }
    /* Reset test environment */
    for ( i = 0; i< OS_MAX_MODULES; i++ )
    {
        res = OS_ModuleUnload(i); /* Ignore errors, does not matter here */
    }

    /*-----------------------------------------------------*/
    testDesc = "#5 Duplicate-name";

    /* Setup */
    res = OS_ModuleLoad(&module_id2, "DUPLICATE", "/cf/MODULE1.so");
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Duplicate-name - Module Load failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_ModuleLoad(&module_id, "DUPLICATE", "/cf/MODULE1.so");
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Reset test environment */
        res = OS_ModuleUnload(module_id2);
    }

    /*-----------------------------------------------------*/
    testDesc = "#6 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#7 Nominal";

    res = OS_ModuleLoad(&module_id, "Good", "/cf/MODULE1.so");
    if ( res == OS_SUCCESS )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    res = OS_ModuleUnload(module_id);

UT_os_module_load_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_ModuleLoad", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
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
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  module_id = 0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_ModuleUnload(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_module_unload_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_ModuleUnload(99999);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_ModuleLoad(&module_id, "Good", "/cf/MODULE1.so");
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Module Load failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_ModuleUnload(module_id);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

UT_os_module_unload_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_ModuleUnload", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
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
    UT_OsApiInfo_t     apiInfo;
    int32              res = 0, idx = 0;
    const char*        testDesc = NULL;
    uint32             module_id;
    OS_module_record_t module_info;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_ModuleInfo(0, &module_info);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_module_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg";

    res = OS_ModuleInfo(0, NULL);
    if ( res == OS_INVALID_POINTER )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-ID-arg";

    res = OS_ModuleInfo(99999, &module_info);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_ModuleLoad(&module_id, "Good", "/cf/MODULE1.so");
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Module Load failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_ModuleInfo(module_id, &module_info);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_ModuleUnload(module_id);
    }

UT_os_module_info_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_ModuleInfo", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*================================================================================*
** End of File: ut_osloader_module_test.c
**================================================================================*/
