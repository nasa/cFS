/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osfileapi-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-vxworks-coveragetest.h"

#include <OCS_stdlib.h>
#include <OCS_taskLib.h>
#include <OCS_shellLib.h>
#include <OCS_dirent.h>
#include <OCS_unistd.h>
#include <OCS_stat.h>

#include "ut-osfileapi.h"

void Test_OS_ShellOutputToFile_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_ShellOutputToFile_Impl(uint32 file_id, const char *Cmd)
     */
    int32 expected = OS_SUCCESS;
    int32 actual;

    /*
     * The ShellOutputToFile will loop until the
     * taskNameToId() function returns ERROR, so this
     * must be set to avoid getting into an endless loop.
     */
    UT_SetDeferredRetcode(UT_KEY(OCS_taskNameToId), 2, -1);

    actual = OS_ShellOutputToFile_Impl(0, "TestCmd");

    UtAssert_True(actual == expected, "OS_ShellOutputToFile_Impl() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_shellGenericInit)) == 1, "shellGenericInit() called");

    /* failure to open the output file */
    UT_SetForceFail(UT_KEY(OS_creat), OS_ERROR);
    expected = OS_ERROR;
    actual = OS_ShellOutputToFile_Impl(0, "TestCmd");
    UtAssert_True(actual == expected, "OS_ShellOutputToFile_Impl() (%ld) == OS_ERROR", (long)actual);
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

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_ShellOutputToFile_Impl);
}


