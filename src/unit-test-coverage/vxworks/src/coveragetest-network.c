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
 * \file     coveragetest-network.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"

#include "os-shared-network.h"

#include <OCS_unistd.h>
#include <OCS_hostLib.h>

/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetHostName_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void Test_OS_NetworkGetHostName_Impl(void)

{
    /*
     * Test Case For:
     * int32 OS_NetworkGetHostName_Impl       (char *host_name, uint32 name_len)
     */
    char buffer[16];

    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName_Impl(buffer, sizeof(buffer)), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_gethostname), -1);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName_Impl(buffer, sizeof(buffer)), OS_ERROR);

} /* end OS_NetworkGetHostName_Impl */


void Test_OS_NetworkGetID_Impl(void)

{
    /*
     * Test Case For:
     * int32 OS_NetworkGetID_Impl             (int32 *IdBuf)
     */
    int32 IdBuf;

    UT_SetForceFail(UT_KEY(OCS_hostGetByName), 1234);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetID_Impl(&IdBuf), OS_SUCCESS);
    UtAssert_True(IdBuf == 1234, "IdBuf (%ld) == 1234", (long)IdBuf);
    UT_SetForceFail(UT_KEY(OCS_hostGetByName), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetID_Impl(&IdBuf), OS_ERROR);
} /* end OS_NetworkGetID_Impl */


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
    ADD_TEST(OS_NetworkGetID_Impl);
    ADD_TEST(OS_NetworkGetHostName_Impl);
}

