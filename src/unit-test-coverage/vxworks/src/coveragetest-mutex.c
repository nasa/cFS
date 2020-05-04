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
 * \file     coveragetest-mutex.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-mutex.h"
#include "os-shared-mutex.h"

void Test_OS_VxWorks_MutexAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_MutexAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_MutexAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_MutSemCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemCreate_Impl (uint32 sem_id, uint32 options)
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate_Impl(0,0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_semMInitialize), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate_Impl(0,0), OS_SEM_FAILURE);
}

void Test_OS_MutSemDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemDelete_Impl (uint32 sem_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemDelete_Impl(0), OS_SUCCESS);
}

void Test_OS_MutSemGive_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGive_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGive_Impl(0), OS_SUCCESS);
}

void Test_OS_MutSemTake_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemTake_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemTake_Impl(0), OS_SUCCESS);
}

void Test_OS_MutSemGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetInfo_Impl (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
     */
    OS_mut_sem_prop_t mut_prop;
    memset(&mut_prop, 0xEE, sizeof(mut_prop));
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetInfo_Impl(0,&mut_prop), OS_SUCCESS);
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
    ADD_TEST(OS_VxWorks_MutexAPI_Impl_Init);
    ADD_TEST(OS_MutSemCreate_Impl);
    ADD_TEST(OS_MutSemDelete_Impl);
    ADD_TEST(OS_MutSemGive_Impl);
    ADD_TEST(OS_MutSemTake_Impl);
    ADD_TEST(OS_MutSemGetInfo_Impl);
}


