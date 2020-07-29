 /*************************************************************************
 ** File:
 **   $Id: sch_api_test.c 1.2 2017/06/21 15:28:56EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains unit test cases for the functions contained in the file sch_api.c
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 ** Notes:
 **
 *************************************************************************/

/*
 * Includes
 */

#include "sch_api_test.h"
#include "sch_apipriv.h"
#include "sch_app.h"
#include "sch_msg.h"
#include "sch_msgdefs.h"
#include "sch_events.h"
#include "sch_version.h"
#include "sch_test_utils.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
// FIXME: these need to be implemented/updated in UT-Assert (by copying from sch_test_utils.c/.h):
//#include "ut_cfe_evs_stubs.h"
//#include "ut_cfe_time_stubs.h"
//#include "ut_ostimer_stubs.h"
//#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_watchdog_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Function Definitions
 */

void SCH_LibInit_Test(void)
{
    int32   Result;

    /* Execute the function being tested */
    Result = SCH_LibInit();
    
    /* Verify results */
    UtAssert_True (SCH_LibData.ProcessingDisabledCtr == SCH_LIB_DIS_CTR, "SCH_LibData.ProcessingDisabledCtr == SCH_LIB_DIS_CTR");
    UtAssert_True (Result == OS_SUCCESS, "Result == OS_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_LibInit_Test */

void SCH_EnableProcessing_Test(void)
{
    SCH_LibData.ProcessingDisabledCtr = 1;

    /* Execute the function being tested */
    SCH_EnableProcessing();
    
    /* Verify results */
    UtAssert_True (SCH_LibData.ProcessingDisabledCtr == 0, "SCH_LibData.ProcessingDisabledCtr == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_EnableProcessing_Test */

void SCH_DisableProcessing_Test(void)
{
    /* Execute the function being tested */
    SCH_DisableProcessing();
    
    /* Verify results */
    UtAssert_True (SCH_LibData.ProcessingDisabledCtr == 1, "SCH_LibData.ProcessingDisabledCtr == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_DisableProcessing_Test */

void SCH_GetProcessingState_Test_True(void)
{
    boolean   Result;

    SCH_LibData.ProcessingDisabledCtr = 0;

    /* Execute the function being tested */
    Result = SCH_GetProcessingState();
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_GetProcessingState_Test_True */

void SCH_GetProcessingState_Test_False(void)
{
    boolean   Result;

    SCH_LibData.ProcessingDisabledCtr = 1;

    /* Execute the function being tested */
    Result = SCH_GetProcessingState();
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_GetProcessingState_Test_False */

void SCH_API_Test_AddTestCases(void)
{
    UtTest_Add(SCH_LibInit_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_LibInit_Test");

    UtTest_Add(SCH_EnableProcessing_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableProcessing_Test");

    UtTest_Add(SCH_DisableProcessing_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableProcessing_Test");

    UtTest_Add(SCH_GetProcessingState_Test_True, SCH_Test_Setup, SCH_Test_TearDown, "SCH_GetProcessingState_Test_True");
    UtTest_Add(SCH_GetProcessingState_Test_False, SCH_Test_Setup, SCH_Test_TearDown, "SCH_GetProcessingState_Test_False");

} /* end SCH_API_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
