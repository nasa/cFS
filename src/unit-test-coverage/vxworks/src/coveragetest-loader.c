/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     coveragetest-loader.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-loader.h"

#include "os-shared-module.h"

#include <OCS_string.h>
#include <OCS_fcntl.h>
#include <OCS_unistd.h>
#include <OCS_errnoLib.h>
#include <OCS_moduleLib.h>
#include <OCS_loadLib.h>
#include <OCS_unldLib.h>

void Test_OS_VxWorks_ModuleAPI_Impl_Init(void)
{
    /* Test Case For:
     * int32 OS_VxWorks_ModuleAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_ModuleAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_ModuleLoad_Impl(void)
{
    /* Test Case For:
     * int32 OS_ModuleLoad_Impl ( uint32 module_id, char *translated_path )
     */
    OSAPI_TEST_FUNCTION_RC(OS_ModuleLoad_Impl(0,"local"), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_ModuleLoad_Impl(0,"local"), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_open));
    UT_SetForceFail(UT_KEY(OCS_loadModule), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ModuleLoad_Impl(0,"local"), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_loadModule));
}

void Test_OS_ModuleUnload_Impl(void)
{
    /* Test Case For:
     * int32 OS_ModuleUnload_Impl ( uint32 module_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_ModuleUnload_Impl(0), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_unldByModuleId), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ModuleUnload_Impl(0), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_unldByModuleId));
}

void Test_OS_ModuleGetInfo_Impl(void)
{
    /* Test Case For:
     * int32 OS_ModuleGetInfo_Impl ( uint32 module_id, OS_module_prop_t *module_prop )
     */
    OS_module_prop_t module_prop;

    memset(&module_prop, 0, sizeof(module_prop));
    OSAPI_TEST_FUNCTION_RC(OS_ModuleGetInfo_Impl(0,&module_prop), OS_SUCCESS);
    UtAssert_True(module_prop.addr.valid, "addresses in output valid");

    /*
     * Note this still returns SUCCESS if the underlying call fails,
     * but the boolean in the output struct should be false.
     */
    memset(&module_prop, 0, sizeof(module_prop));
    UT_SetForceFail(UT_KEY(OCS_moduleInfoGet), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ModuleGetInfo_Impl(0,&module_prop), OS_SUCCESS);
    UT_ClearForceFail(UT_KEY(OCS_moduleInfoGet));
    UtAssert_True(!module_prop.addr.valid, "addresses in output not valid");
}


/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);

    memset(OS_module_table, 0, sizeof(OS_module_table));
    memset(OS_global_module_table, 0, sizeof(OS_common_record_t) * OS_MAX_MODULES);

    memset(UT_Ref_OS_impl_module_table, 0, UT_Ref_OS_impl_module_table_SIZE);
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void)
{

}


/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_VxWorks_ModuleAPI_Impl_Init);
    ADD_TEST(OS_ModuleLoad_Impl);
    ADD_TEST(OS_ModuleUnload_Impl);
    ADD_TEST(OS_ModuleGetInfo_Impl);
}


