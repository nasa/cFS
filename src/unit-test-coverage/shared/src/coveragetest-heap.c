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
 * \file     coveragetest-heap.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-heap.h"


void Test_OS_HeapGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop)
     */
    OS_heap_prop_t heap_prop;
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_HeapGetInfo(&heap_prop);

    UtAssert_True(actual == expected, "OS_HeapGetInfo() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_HeapGetInfo(NULL);

    UtAssert_True(actual == expected, "OS_HeapGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

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
    ADD_TEST(OS_HeapGetInfo);
}




