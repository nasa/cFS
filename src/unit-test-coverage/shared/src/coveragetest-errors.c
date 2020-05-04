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
 * \file     coveragetest-errors.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-errors.h"

void Test_OS_GetErrorName(void)
{
    /*
     * Test Case For:
     * int32 OS_GetErrorName(int32 error_num, os_err_name_t* err_name);
     */
    os_err_name_t err_name;
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_GetErrorName(OS_ERROR, &err_name);

    UtAssert_True(actual == expected, "OS_GetErrorName(%s) (%ld) == OS_SUCCESS", "OS_ERROR", (long)actual);
    UtAssert_True(strcmp(err_name,"OS_ERROR") == 0, "string(%s) == OS_ERROR", err_name);

    expected = OS_ERROR;
    actual   = OS_GetErrorName(-555555, &err_name);
    UtAssert_True(actual == expected, "OS_GetErrorName(%s) (%ld) == OS_SUCCESS", err_name, (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_GetErrorName(-555555, NULL);
    UtAssert_True(actual == expected, "OS_GetErrorName(NULL) (%ld) == OS_SUCCESS", (long)actual);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
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

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_GetErrorName);
}




