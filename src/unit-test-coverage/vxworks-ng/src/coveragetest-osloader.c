/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osloader-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-vxworks-coveragetest.h"
#include "ut-osloader.h"


void Test_OS_VxWorks_ModuleAPI_Impl_Init(void)
{
    /* Test Case For:
     * int32 OS_VxWorks_ModuleAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_ModuleAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_SymbolLookup_Impl(void)
{
    /* Test Case For:
     * int32 OS_SymbolLookup_Impl( cpuaddr *SymbolAddress, const char *SymbolName )
     */
    cpuaddr SymAddr;
    OSAPI_TEST_FUNCTION_RC(OS_SymbolLookup_Impl(&SymAddr, "symname"), OS_SUCCESS);
}

void Test_OS_SymTableIterator_Impl(void)
{
    /* Test Case For:
     * BOOL OS_SymTableIterator_Impl(char *name, SYM_VALUE val, SYM_TYPE type, int arg, SYM_GROUP group)
     */
    uint32 Data = 0;

    OSAPI_TEST_FUNCTION_RC(Osapi_Internal_CallIteratorFunc("ut",&Data,100,1000), true);
    OSAPI_TEST_FUNCTION_RC(Osapi_Internal_CallIteratorFunc("ut",&Data,100,101), false);
}

void Test_OS_ModuleLoad_Impl(void)
{
    /* Test Case For:
     * int32 OS_ModuleLoad_Impl ( uint32 module_id, char *translated_path )
     */
    OSAPI_TEST_FUNCTION_RC(OS_ModuleLoad_Impl(0,"local"), OS_SUCCESS);
}

void Test_OS_ModuleUnload_Impl(void)
{
    /* Test Case For:
     * int32 OS_ModuleUnload_Impl ( uint32 module_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_ModuleUnload_Impl(0), OS_SUCCESS);
}

void Test_OS_ModuleGetInfo_Impl(void)
{
    /* Test Case For:
     * int32 OS_ModuleGetInfo_Impl ( uint32 module_id, OS_module_prop_t *module_prop )
     */
    OS_module_prop_t module_prop;
    OSAPI_TEST_FUNCTION_RC(OS_ModuleGetInfo_Impl(0,&module_prop), OS_SUCCESS);
}

void Test_OS_SymbolTableDump_Impl(void)
{
    /* Test Case For:
     * int32 OS_SymbolTableDump_Impl ( const char *filename, uint32 SizeLimit )
     */
    OSAPI_TEST_FUNCTION_RC(OS_SymbolTableDump_Impl("file",10000), OS_SUCCESS);
}


/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Osapi_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_TearDown(void)
{

}


/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_VxWorks_ModuleAPI_Impl_Init);
    ADD_TEST(OS_SymTableIterator_Impl);
    ADD_TEST(OS_SymbolLookup_Impl);
    ADD_TEST(OS_ModuleLoad_Impl);
    ADD_TEST(OS_ModuleUnload_Impl);
    ADD_TEST(OS_ModuleGetInfo_Impl);
    ADD_TEST(OS_SymbolTableDump_Impl);
}


