/*================================================================================*
** File:  ut_oscore_exception_test.c
** Owner: Tam Ngo
** Date:  June 2015
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_exception_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_FPUExcSetMask(uint32 mask)
** Purpose: Sets the FPU exception mask
** Parameters: mask - new mask value
** Returns: OS_ERR_NOT_IMPLEMENTED if function is not implemented
**          OS_SUCCESS if succeeded
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Nominal condition
**   1) Call this routine with valid argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_fpuexc_setmask_test()
{
    uint32         oldMask=0x00, newMask=0x01, curMask=0x00;
    int32          res = 0;
    const char*    testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    OS_FPUExcGetMask(&oldMask);
    res = OS_FPUExcSetMask(newMask);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_fpuexc_setmask_exit_tag;
    }

    /* Reset test environment */
    OS_FPUExcSetMask(oldMask);

    /*-----------------------------------------------------*/
    testDesc = "#1 Nominal";

    OS_FPUExcGetMask(&oldMask);
    res = OS_FPUExcSetMask(newMask);
    if (res == OS_SUCCESS)
    {
    	res = OS_FPUExcGetMask(&curMask);
    	if ((res == OS_SUCCESS) && (curMask == newMask))
    		UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    	else
    	    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }
    else
    {
    	UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    /* Reset test environment */
    OS_FPUExcSetMask(oldMask);

UT_os_fpuexc_setmask_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_FPUExcGetMask(uint32* mask)
** Purpose: Gets the FPU exception mask
** Parameters: mask - out pointer to mask value
** Returns: OS_ERR_NOT_IMPLEMENTED if function is not implemented
**          OS_INVALID_POINTER if passing in null pointer
**          OS_SUCCESS if succeeded
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null-pointer argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine with valid argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_fpuexc_getmask_test()
{
    uint32         curMask=0x00;
    int32          res = 0;
    const char*    testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_FPUExcGetMask(&curMask);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_fpuexc_getmask_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_FPUExcGetMask(NULL);
    if (res == OS_INVALID_POINTER)
    	UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
    	UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    res = OS_FPUExcGetMask(&curMask);
    if (res == OS_SUCCESS)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
	UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

UT_os_fpuexc_getmask_exit_tag:
    return;
    
}

/*================================================================================*
** End of File: ut_oscore_exception_test.c
**================================================================================*/
