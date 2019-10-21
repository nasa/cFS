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

#include <utassert.h>
#include <uttest.h>
#include <utstubs.h>

#include <osapi.h>
#include <os-impl.h>

#include <overrides/stdlib.h>
#include <overrides/unistd.h>
#include <overrides/fcntl.h>
#include <overrides/errno.h>

#include "ut-osfileapi.h"

#define OSAPI_TEST_FUNCTION_RC(func,args,exp)   \
{                                               \
    int32 rcexp = exp;                          \
    int32 rcact = func args;                    \
    UtAssert_True(rcact == rcexp, "%s%s (%ld) == %s (%ld)", \
        #func, #args, (long)rcact, #exp, (long)rcexp);      \
}



void Test_OS_GenericClose_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_GenericClose_Impl(uint32 local_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_GenericClose_Impl,(0), OS_SUCCESS);

    /*
     * Test path where underlying close() fails.
     * Should still return success.
     */
    UT_SetForceFail(UT_KEY(OCS_close), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GenericClose_Impl,(0), OS_SUCCESS);
}

void Test_OS_GenericSeek_Impl (void)
{
    /*
     * Test Case For:
     * int32 OS_GenericSeek_Impl (uint32 local_id, int32 offset, uint32 whence)
     */

    /* note on success this wrapper returns the result of lseek(), not OS_SUCCESS */
    UT_SetForceFail(UT_KEY(OCS_lseek), 111);
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl,(0,0,OS_SEEK_CUR), 111);
    UT_SetForceFail(UT_KEY(OCS_lseek), 222);
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl,(0,0,OS_SEEK_SET), 222);
    UT_SetForceFail(UT_KEY(OCS_lseek), 333);
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl,(0,0,OS_SEEK_END), 333);

    /* bad whence */
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl,(0,0,-1234), OS_FS_ERROR);

    /* generic failure of lseek() */
    UT_SetForceFail(UT_KEY(OCS_lseek),-1);
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl,(0,0,OS_SEEK_END), OS_FS_ERROR);

    /* The seek implementation also checks for this specific pipe errno */
    OCS_errno = OCS_ESPIPE;
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl,(0,0,OS_SEEK_END), OS_FS_UNIMPLEMENTED);
}

void Test_OS_GenericRead_Impl (void)
{
    /*
     * Test Case For:
     * int32 OS_GenericRead_Impl (uint32 local_id, void *buffer, uint32 nbytes, int32 timeout)
     */
    char SrcData[] = "ABCDEFGHIJK";
    char DestData[sizeof(SrcData)] = { 0 };

    UT_SetDataBuffer(UT_KEY(OCS_read),SrcData,sizeof(SrcData),false);
    Osapi_Internal_Set_Selectable(0, false);
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl,(0,DestData,sizeof(DestData),0), sizeof(DestData));
    UtAssert_MemCmp(SrcData, DestData, sizeof(SrcData),"read() data Valid");

    /* test invocation of select() in nonblocking mode */
    UT_ResetState(UT_KEY(OCS_read));
    UT_SetDataBuffer(UT_KEY(OCS_read),SrcData,sizeof(SrcData),false);
    Osapi_Internal_Set_Selectable(0, true);
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl,(0,DestData,sizeof(DestData),0), sizeof(DestData));
    UtAssert_True(UT_GetStubCount(UT_KEY(OS_SelectSingle_Impl)) == 1, "OS_SelectSingle() called");

    /* read() failure */
    UT_SetForceFail(UT_KEY(OCS_read), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl,(0,DestData,sizeof(DestData),0), OS_ERROR);
}

void Test_OS_GenericWrite_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_GenericWrite_Impl(uint32 local_id, const void *buffer, uint32 nbytes, int32 timeout)
     */
    char SrcData[] = "ABCDEFGHIJKL";
    char DestData[sizeof(SrcData)] = { 0 };

    UT_SetDataBuffer(UT_KEY(OCS_write),DestData,sizeof(DestData),false);
    Osapi_Internal_Set_Selectable(0, false);
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl,(0,SrcData,sizeof(SrcData),0), sizeof(SrcData));
    UtAssert_MemCmp(SrcData, DestData, sizeof(SrcData), "write() data valid");


    /* test invocation of select() in nonblocking mode */
    UT_ResetState(UT_KEY(OCS_write));
    UT_SetDataBuffer(UT_KEY(OCS_write),DestData,sizeof(DestData),false);
    Osapi_Internal_Set_Selectable(0, true);
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl,(0,SrcData,sizeof(SrcData),0), sizeof(SrcData));
    UtAssert_True(UT_GetStubCount(UT_KEY(OS_SelectSingle_Impl)) == 1, "OS_SelectSingle() called");

    /* write() failure */
    UT_SetForceFail(UT_KEY(OCS_write), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl,(0,DestData,sizeof(DestData),0), OS_ERROR);
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


#define ADD_TEST(test) UtTest_Add((Test_ ## test), Osapi_Task_Setup, Osapi_TearDown, #test)

/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_GenericClose_Impl);
    ADD_TEST(OS_GenericSeek_Impl);
    ADD_TEST(OS_GenericRead_Impl);
    ADD_TEST(OS_GenericWrite_Impl);
}


