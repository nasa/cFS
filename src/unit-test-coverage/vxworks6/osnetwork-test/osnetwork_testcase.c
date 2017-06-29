/*
 * Filename: osnetwork_testcase.c
 *
 * Purpose: This file contains unit test cases
 *
 * Modification History:
 *   04/28/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include <fcntl.h>  /* for O_WRONLY etc. */

#include "osapi.h"              /* cfe.h not available from within osal. */

#include "utassert.h"
#include "uttest.h"
#include "utlist.h"

#include "osnetwork_stubs.h"


#define MAX_BUF 100

/* Prototypes for non-exported functions */


/* -------------------- Special Test Case Variables ------------------------- */


/* Utility functions */
/* -------------------------------------------------------------------------- */



/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_NetworkGetID Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_NetworkGetID_Nominal(void)
{
    /* Setup Inputs */
    Osnetwork_SetReturnCode(OSNETWORK_GETHOSTNAME_INDEX, 0, 1);
    Osnetwork_SetReturnCode(OSNETWORK_HOSTGETBYNAME_INDEX, 99, 1);

    /* Execute Test */
    int32 retval = OS_NetworkGetID();

#ifdef OS_INCLUDE_NETWORK
    /* Verify Outputs - WITH NETWORK */
    UtAssert_True(retval == 99, "retval == 99");
#else
    /* Verify Outputs - NO NETWORK */
    UtAssert_True(retval == OS_ERR_NOT_IMPLEMENTED, "retval == OS_ERR_NOT_IMPLEMENTED");
#endif

}

void Test_OS_NetworkGetID_Failgethostname(void)
{
    /* Setup Inputs */
    Osnetwork_SetReturnCode(OSNETWORK_GETHOSTNAME_INDEX, -1, 1);
    Osnetwork_SetReturnCode(OSNETWORK_HOSTGETBYNAME_INDEX, 99, 1);

    /* Execute Test */
    int32 retval = OS_NetworkGetID();

    /* Verify Outputs - WITH NETWORK */
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");
}

void Test_OS_NetworkGetID_FailhostGetByName(void)
{
    /* Setup Inputs */
    Osnetwork_SetReturnCode(OSNETWORK_GETHOSTNAME_INDEX, 0, 1);
    Osnetwork_SetReturnCode(OSNETWORK_HOSTGETBYNAME_INDEX, -1, 1);

    /* Execute Test */
    int32 retval = OS_NetworkGetID();

    /* Verify Outputs - WITH NETWORK */
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");
}

/*******************************************************************************
**
**  OS_NetworkGetHostName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_NetworkGetHostName_Nominal(void)
{
    /* Setup Inputs */
    char hostname[255] = "UNSET";
    uint32 name_len = 13;

    /* Execute Test */
    int32 retval = OS_NetworkGetHostName(hostname, name_len);

#ifdef OS_INCLUDE_NETWORK
    /* Verify Outputs - WITH NETWORK */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_StrCmp(hostname, "testname", "hostname == testname");
#else
    /* Verify Outputs - NO NETWORK */
    UtAssert_True(retval == OS_ERR_NOT_IMPLEMENTED, "retval == OS_ERR_NOT_IMPLEMENTED");
    UtAssert_StrCmp(hostname, "UNSET", "hostname == UNSET");
#endif
}

/*----------------------------------------------------------------------------*/
void Test_OS_NetworkGetHostName_NullArg1(void)
{
    /* Setup Inputs */
    uint32 name_len = 13;

    /* Execute Test */
    int32 retval = OS_NetworkGetHostName(NULL, name_len);

    /* Verify Outputs - WITH OR WITHOUT NETWORK */
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_NetworkGetHostName_ZeroLenArg2(void)
{
    /* Setup Inputs */
    char hostname[255] = "UNSET";
    uint32 name_len = 0;

    /* Execute Test */
    int32 retval = OS_NetworkGetHostName(hostname, name_len);

    /* Verify Outputs - WITH OR WITHOUT NETWORK */
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");
    UtAssert_StrCmp(hostname, "UNSET", "hostname == UNSET");
}

/*----------------------------------------------------------------------------*/
void Test_OS_NetworkGetHostName_gethostnameFail(void)
{
    /* Setup Inputs */
    char hostname[255] = "UNSET";
    uint32 name_len = 13;
    Osnetwork_SetReturnCode(OSNETWORK_GETHOSTNAME_INDEX, -1, 1);

    /* Execute Test */
    int32 retval = OS_NetworkGetHostName(hostname, name_len);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");
    UtAssert_StrCmp(hostname, "UNSET", "hostname == UNSET");
}

/* ------------------- End of test cases --------------------------------------*/





/*
 * Osnetwork_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osnetwork_Setup(void)
{
    Osnetwork_Reset();
}


/*
 * Osnetwork_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osnetwork_TearDown(void)
{

}


#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* Osnetwork_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    /* OS_NetworkGetID */
    ADD_TEST(Test_OS_NetworkGetID_Nominal,                 Osnetwork_Setup, Osnetwork_TearDown);
#ifdef OS_INCLUDE_NETWORK
    ADD_TEST(Test_OS_NetworkGetID_Failgethostname,         Osnetwork_Setup, Osnetwork_TearDown);
    ADD_TEST(Test_OS_NetworkGetID_FailhostGetByName,       Osnetwork_Setup, Osnetwork_TearDown);
#endif

    /* OS_NetworkGetHostName Tests */
    ADD_TEST(Test_OS_NetworkGetHostName_Nominal,           Osnetwork_Setup, Osnetwork_TearDown);
    ADD_TEST(Test_OS_NetworkGetHostName_NullArg1,          Osnetwork_Setup, Osnetwork_TearDown);
    ADD_TEST(Test_OS_NetworkGetHostName_ZeroLenArg2,       Osnetwork_Setup, Osnetwork_TearDown);
#ifdef OS_INCLUDE_NETWORK
    ADD_TEST(Test_OS_NetworkGetHostName_gethostnameFail,   Osnetwork_Setup, Osnetwork_TearDown);
#endif
}




