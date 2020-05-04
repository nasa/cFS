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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"
#include "os-shared-heap.h"

#include <OCS_memPartLib.h>


void Test_OS_HeapGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop)
     */
    OS_heap_prop_t heap_prop;

    memset(&heap_prop, 0xEE, sizeof(heap_prop));
    OSAPI_TEST_FUNCTION_RC(OS_HeapGetInfo_Impl(&heap_prop), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_memPartInfoGet), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_HeapGetInfo_Impl(&heap_prop), OS_ERROR);
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
    ADD_TEST(OS_HeapGetInfo_Impl);
}


