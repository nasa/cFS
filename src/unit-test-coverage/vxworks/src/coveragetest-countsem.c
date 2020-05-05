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
 * \file     coveragetest-countsem.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-countsem.h"

#include "os-shared-countsem.h"

void Test_OS_VxWorks_CountSemAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_CountSemAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_CountSemAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_CountSemCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemCreate_Impl (uint32 sem_id, uint32 sem_initial_value, uint32 options)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemCreate_Impl(0,0,0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_semCInitialize), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CountSemCreate_Impl(0,0,0), OS_SEM_FAILURE);
}

void Test_OS_CountSemDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemDelete_Impl (uint32 sem_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemDelete_Impl(0), OS_SUCCESS);
}

void Test_OS_CountSemGive_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGive_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemGive_Impl(0), OS_SUCCESS);
}

void Test_OS_CountSemTake_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTake_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemTake_Impl(0), OS_SUCCESS);
}

void Test_OS_CountSemTimedWait_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemTimedWait_Impl(0, 100), OS_SUCCESS);
}

void Test_OS_CountSemGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGetInfo_Impl (uint32 sem_id, OS_count_sem_prop_t *count_prop)
     */
    OS_count_sem_prop_t count_prop;
    memset(&count_prop, 0xEE, sizeof(count_prop));
    OSAPI_TEST_FUNCTION_RC(OS_CountSemGetInfo_Impl(0, &count_prop), OS_SUCCESS);
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

    memset(OS_count_sem_table, 0, sizeof(OS_count_sem_table));
    memset(OS_global_count_sem_table, 0, sizeof(OS_common_record_t) * OS_MAX_BIN_SEMAPHORES);

    memset(UT_Ref_OS_impl_count_sem_table, 0, UT_Ref_OS_impl_count_sem_table_SIZE);
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
    ADD_TEST(OS_VxWorks_CountSemAPI_Impl_Init);
    ADD_TEST(OS_CountSemCreate_Impl);
    ADD_TEST(OS_CountSemDelete_Impl);
    ADD_TEST(OS_CountSemGive_Impl);
    ADD_TEST(OS_CountSemTake_Impl);
    ADD_TEST(OS_CountSemTimedWait_Impl);
    ADD_TEST(OS_CountSemGetInfo_Impl);
}


