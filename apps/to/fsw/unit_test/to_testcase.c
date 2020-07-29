/*
 * Filename: to_testcase.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains unit test cases for the ci application
 * 
 */

/*
 * Includes
 */
#include "cfe.h"
#include "cfe_tbl_msg.h"

#include "to_stubs.h"
#include "to_app.h"
#include "to_grpids.h"

#include "utassert.h"
#include "uttest.h"
#include "utlist.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include <errno.h>

extern TO_AppData_t  g_TO_AppData;


TO_ConfigTable_t to_ConfigTable =
{
   {
       {TO_HK_TLM_MID,                {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {TO_OUT_DATA_MID,              {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {TO_DATA_TYPE_MID,             {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0}
    }
};


extern void TO_SendDataTypePktCmd(CFE_SB_MsgPtr_t pMsg);
extern int32 Ut_OS_CountSemGetInfoHook(uint32 sem_id, OS_count_sem_prop_t *count_prop);

/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  TO_InitEvent Tests
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_InitEvent_RegisterFail(void)
{
    /* Setup Inputs */
    int32 expected = TO_ERROR;
    int32 actual = 0;
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, 
                            CFE_EVS_UNKNOWN_FILTER, 1);

    /* Execute Test */
    actual = TO_AppInit();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitEvent - Event Register Fail");
}

void Test_TO_InitEvent(void)
{
    /* Setup Inputs */
    int32 expected = CFE_SUCCESS;
    int32 actual = 0;

    /* Execute Test */
    actual = TO_InitEvent();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitEvent - Nominal");
}


/*******************************************************************************
**
**  TO_InitData Tests
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_InitData(void)
{
    /* Setup Inputs */
    int32 expected = TO_SUCCESS;
    int32 actual = 0;

    /* Execute Test */
    actual = TO_InitData();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitData - Nominal");
    UtAssert_True(g_TO_AppData.uiWakeupTimeout == TO_WAKEUP_TIMEOUT,
                  "Side effect test.");
}



/*******************************************************************************
**
**  TO_InitCustom Tests
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_CustomInit_Fail(void)
{
    /* Setup Inputs */
    int32 expected = TO_ERROR;
    int32 actual = 0;

    Ut_TO_SetReturnCode(UT_TO_CUSTOMINIT_INDEX, 
                        TO_ERROR, 1);

    /* Execute Test */
    actual = TO_CustomInit();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "CustomInit - Fail");
}


/*******************************************************************************
**
**  TO_InitTable Tests
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_InitTable_RegisterFail(void)
{
    /* Setup Inputs */
    int32 expected = -1; 
    int32 actual = 0;
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, 
                             -1, 1);

    /* Execute Test */
    actual = TO_AppInit();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitTable - RegisterFail");
}


void Test_TO_InitTable_LoadFail(void)
{
    /* Setup Inputs */
    int32 expected = -1; 
    int32 actual = 0;

    g_TO_AppData.tableHandle = -1;
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, 
                             -1, 1);

    /* Execute Test */
    actual = TO_InitTable();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitTable - LoadFail");
    UtAssert_True(g_TO_AppData.tableHandle == 0, "Side effect test.");
}


void Test_TO_InitTable_ManageFail(void)
{
    /* Setup Inputs */
    int32 expected = -2; 
    int32 actual = 0;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, 
                             expected, 1);

    /* Execute Test */
    actual = TO_InitTable();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitTable - ManageFail");
}


void Test_TO_InitTable_GetAddressFail(void)
{
    /* Setup Inputs */
    int32 expected = -3; 
    int32 actual = 0;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, 
                             expected, 1);

    /* Execute Test */
    actual = TO_InitTable();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitTable - GetAddressFail");
}


void Test_TO_InitTable_NotifyByMessageFail(void)
{
    /* Setup Inputs */
    int32 expected = -4; 
    int32 actual = 0;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_NOTIFYBYMESSAGE_INDEX, 
                             expected, 1);

    /* Execute Test */
    actual = TO_InitTable();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitTable - NotifyByMessageFail");
}


void Test_TO_InitTable(void)
{
    /* Setup Inputs */
    int32 expected = TO_SUCCESS; 
    int32 actual = 0;


    /* Execute Test */
    actual = TO_InitTable();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitTable - Nominal");
}

/*******************************************************************************
**
**  TO_InitPipe Tests
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_InitPipe_CreatePipeFail(void)
{
    /* Setup Inputs */
    int32 expected = TO_ERROR;
    int32 actual = 0;
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, 
                            CFE_SB_BAD_ARGUMENT, 1);

    /* Execute Test */
    actual = TO_AppInit();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitPipe - SCH CreatePipe Fail");

    expected = CFE_SB_BAD_ARGUMENT;
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, 
                            CFE_SB_BAD_ARGUMENT, 2);
    
    /* Execute Test */
    actual = TO_InitPipe();
    
    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitPipe - CMD CreatePipe Fail");
    UtAssert_True(g_TO_AppData.usCmdPipeDepth == TO_CMD_PIPE_DEPTH, 
                  "Side effect test.");
    
    expected = CFE_SB_BAD_ARGUMENT;
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, 
                            CFE_SB_BAD_ARGUMENT, 3);
    
    /* Execute Test */
    actual = TO_InitPipe();
    
    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitPipe - Route 0 TlmPipe CreatePipe Fail");
}

void Test_TO_InitPipe_CfChnlIdxFail(void)
{
    /* Setup Inputs */
    int32 expected = TO_ERROR;
    int32 actual = 0;

    g_TO_AppData.routes[0].sCfChnlIdx = TO_NUM_CF_CHANNELS;

    /* Execute Test */
    actual = TO_InitPipe();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitPipe - sCFChnlIdx Invalid");
}


void Test_TO_InitPipe_CfCountSemCreateFail(void)
{
    /* Setup Inputs */
    int32 expected = OS_ERROR;
    int32 actual = 0;

    g_TO_AppData.routes[0].sCfChnlIdx = 0;
    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMCREATE_INDEX, 
                           OS_ERROR, 1);

    /* Execute Test */
    actual = TO_InitPipe();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitPipe - CF CountSemCreate Fail");
}
    

void Test_TO_InitPipe_SubscribeMsgFail(void)
{
    /* Setup Inputs */
    int32 expected = -1;
    int32 actual = 0;

    g_TO_AppData.pConfigTable = NULL;
    
    /* Execute Test */
    actual = TO_InitPipe();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "InitPipe - Tlm message Subscribe Fail");
}


void Test_TO_InitPipe(void)
{
    /* Setup Inputs */
    int32 expected = CFE_SUCCESS; 
    int32 actual = 0;
    
    /* Prepare table first */
    TO_InitTable();

    /* Execute Test */
    actual = TO_InitPipe();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Init Pipe - Nominal");
}

/*******************************************************************************
**
**  TO_AppInit Tests
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_AppInit(void)
{
    /* Setup Inputs */
    int32 expected = TO_SUCCESS;
    int32 actual = 0;

    /* Execute Test */
    actual = TO_AppInit();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "AppInit - Nominal");
}


/*******************************************************************************
**
**  TO_ValidateTable Tests
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_ValidateTable_Gap(void)
{
    /* Setup Inputs */
    int32 expected = TO_ERROR; 
    int32 actual = 0;

    to_ConfigTable.entries[0].usMsgId = TO_UNUSED_ENTRY; 
    
    /* Execute Test */
    actual = TO_ValidateTable(&to_ConfigTable);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "ValidateTable - Gap Error");
}


void Test_TO_ValidateTable_Duplicate(void)
{
    /* Setup Inputs */
    int32 expected = TO_ERROR; 
    int32 actual = 0;

    to_ConfigTable.entries[0].usMsgId = TO_DATA_TYPE_MID;
    
    /* Execute Test */
    actual = TO_ValidateTable(&to_ConfigTable);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "ValidateTable - Duplicate Error");
}


void Test_TO_ValidateTable_CriticalMid(void)
{
    /* Setup Inputs */
    int32 expected = TO_ERROR; 
    int32 actual = 0;

    to_ConfigTable.entries[0].usMsgId = TO_HK_TLM_MID;
    g_TO_AppData.criticalMid[1] = CFE_EVS_EVENT_MSG_MID;
    
    /* Execute Test */
    actual = TO_ValidateTable(&to_ConfigTable);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "ValidateTable - CriticalMid Error");
}

void Test_TO_ValidateTable(void)
{
    /* Setup Inputs */
    int32 expected = TO_SUCCESS;
    int32 actual = 0;

    g_TO_AppData.criticalMid[1] = 0;
    
    /* Execute Test */
    actual = TO_ValidateTable(&to_ConfigTable);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "ValidateTable - Nominal");
}

/*******************************************************************************
**
**  TO_AppMain Tests
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_AppMain_RegisterFail(void)
{
    g_TO_AppData.uiWakeupTimeout = 0;
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, 
                            -1, 1);
    
    /* Execute Test */
    TO_AppMain();

    UtAssert_True(g_TO_AppData.uiWakeupTimeout == 0,
                  "AppMain - RegisterApp Fail");
}


void Test_TO_AppMain_InitFail(void)
{
    Ut_TO_SetReturnCode(UT_TO_CUSTOMINIT_INDEX, 
                        TO_ERROR, 1);

    TO_AppMain();
    
    UtAssert_True(g_TO_AppData.uiWakeupTimeout == TO_WAKEUP_TIMEOUT,
                  "AppMain - AppInit Fail");
    
}

void Test_TO_AppMain_RcvMsgFail(void)
{
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, 
                            CFE_SB_BAD_ARGUMENT, 1);
    
    TO_AppMain();
    
    UtAssert_True(g_TO_AppData.uiRunStatus == CFE_ES_APP_ERROR,
                  "AppMain - RcvMsg Fail");

    /* For code coverage */
    TO_CleanupCallback();                  
}


/*******************************************************************************
**
** TO_RcvMsg Test
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_RcvMsg_NoMsgError(void)
{
    int32 actual;
    int32 expected = CFE_SB_NO_MESSAGE;

    /* Initialize the Command pipe and subscribe to messages */
    TO_InitPipe();

    actual = TO_RcvMsg(CFE_SB_POLL);
    UtAssert_True(actual == expected, "RcvMsg - NoMsgError");
}

void Test_TO_RcvMsg_BadMsg(void)
{
    int32 actual;
    int32 expected = CFE_SUCCESS;
    CFE_SB_Msg_t msg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &msg;

    /* Initialize the Command pipe and subscribe to messages */
    TO_InitPipe();

    CFE_SB_SetMsgId(pMsg, 0);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(msg));         
    Ut_CFE_SB_AddMsgToPipe(pMsg, g_TO_AppData.SchPipeId);

    actual = TO_RcvMsg(CFE_SB_PEND_FOREVER);
    UtAssert_True(actual == expected, "RcvMsg - Bad MID");
}

void Test_TO_RcvMsg_Wakeup(void)
{
    int32 actual;
    int32 expected = CFE_SUCCESS;
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;

    /* Initialize the Command pipe and subscribe to messages */
    TO_InitPipe();

    CFE_SB_SetMsgId(pMsg, TO_WAKEUP_MID);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(cmdMsg));         
    Ut_CFE_SB_AddMsgToPipe(pMsg, g_TO_AppData.SchPipeId);

    actual = TO_RcvMsg(CFE_SB_PEND_FOREVER);
    UtAssert_True(actual == expected, "RcvMsg - Wakeup MID");
}

void Test_TO_RcvMsg_Timeout(void)
{
    int32 actual;
    int32 expected = CFE_SB_TIME_OUT;
    
    /* Initialize the Command pipe and subscribe to messages */
    TO_InitPipe();

    actual = TO_RcvMsg(10);
    UtAssert_True(actual == expected, "RcvMsg - timeout");
}
    

/*******************************************************************************
**
** TO_ProcessTlmPipes Test 
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_ProcessTlmPipes(void)
{
    TO_AppInit();
    
    g_TO_AppData.routes[0].usWakePeriod = 3;

    TO_ProcessTlmPipes();
    UtAssert_True(g_TO_AppData.usWakeupCount == 1, "ProcessTlmPipes - Nominal");

    TO_ProcessTlmPipes();
    TO_ProcessTlmPipes();
    UtAssert_True(g_TO_AppData.usWakeupCount == 0, "ProcessTlmPipes - Rollover");
}


/*******************************************************************************
**
** TO_ProcessNewData Test
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_ProcessNewData_FrameError(void)
{
    TO_AppInit();
    g_TO_AppData.usOutputEnabled = 1;
    g_TO_AppData.usOutputActive = 1;
    g_TO_AppData.routes[0].usIsEnabled = 1;

    Ut_TO_SetReturnCode(UT_TO_CUSTOMFRAMESTART_INDEX, 
                        TO_ERROR, 1);

    TO_ProcessNewData(&g_TO_AppData.tlmPipes[0], 0);

    UtAssert_True(g_TO_AppData.HkTlm.usFrameErrCnt == 1, 
                  "ProcessNewData - Framing Fail");
}


void Test_TO_ProcessNewData_BadMsg(void)
{
    TO_OutData_t    msg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &msg;
    
    TO_AppInit();
    g_TO_AppData.usOutputEnabled = 1;
    g_TO_AppData.usOutputActive = 1;
    g_TO_AppData.routes[0].usIsEnabled = 1;

    /* Send a Bad Message */
    CFE_SB_SetMsgId(pMsg, 0);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(msg));
    Ut_CFE_SB_AddMsgToPipe(pMsg, g_TO_AppData.tlmPipes[0].cfePipeId);

    TO_ProcessNewData(&g_TO_AppData.tlmPipes[0], 0);

    UtAssert_True(g_TO_AppData.HkTlm.usPktCnt == 0, "ProcessNewData - Bad Msg");
}


void Test_TO_ProcessNewData_CustomProcessError(void)
{
    TO_OutData_t    msg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &msg;
    
    TO_AppInit();
    g_TO_AppData.usOutputEnabled = 1;
    g_TO_AppData.usOutputActive = 1;
    g_TO_AppData.routes[0].usIsEnabled = 1;

    /* Send a Bad Message */
    CFE_SB_SetMsgId(pMsg, TO_OUT_DATA_MID);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(msg));
    Ut_CFE_SB_AddMsgToPipe(pMsg, g_TO_AppData.tlmPipes[0].cfePipeId);

    Ut_TO_SetReturnCode(UT_TO_CUSTOMPROCESSDATA_INDEX, 
                        TO_ERROR, 1);

    TO_ProcessNewData(&g_TO_AppData.tlmPipes[0], 0);

    UtAssert_True(g_TO_AppData.HkTlm.usPktErrCnt == 1, 
                  "ProcessNewData - CustomProcessData Fail");
}


void Test_TO_ProcessNewData(void)
{
    TO_OutData_t    msg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &msg;
    
    TO_AppInit();
    g_TO_AppData.usOutputEnabled = 1;
    g_TO_AppData.usOutputActive = 1;
    g_TO_AppData.routes[0].usIsEnabled = 1;

    /* Send a good Message */
    CFE_SB_SetMsgId(pMsg, TO_OUT_DATA_MID);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(msg));
    Ut_CFE_SB_AddMsgToPipe(pMsg, g_TO_AppData.tlmPipes[0].cfePipeId);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETINFO_INDEX,
                             Ut_OS_CountSemGetInfoHook); 

    TO_ProcessNewData(&g_TO_AppData.tlmPipes[0], 0);

    UtAssert_True(g_TO_AppData.HkTlm.usPktCnt == 1, 
                  "ProcessNewData - Nominal");
}

/*******************************************************************************
**
** TO_ProcessNewCmds Test 
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_ProcessNewCmds_BadMsg(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;

    /* Initialize the Command pipe and subscribe to messages */
    TO_InitPipe();

    /* Send a Bad Command */
    CFE_SB_SetMsgId(pMsg, 0);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));         
    Ut_CFE_SB_AddMsgToPipe(pMsg, g_TO_AppData.CmdPipeId);

    TO_ProcessNewCmds();
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ProcessNewCmds - Bad Msg");
}


void Test_TO_ProcessNewCmds_AppCmd(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;

    /* Initialize the Command pipe and subscribe to messages */
    TO_InitPipe();

    /* Send Noop Cmd Command */
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_NOOP_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));                      

    Ut_CFE_SB_AddMsgToPipe(pMsg, g_TO_AppData.CmdPipeId);

    TO_ProcessNewCmds();
    UtAssert_True(g_TO_AppData.HkTlm.usCmdCnt == 1,
                  "ProcessNewCmds - AppCmd Msg");
}
    

void Test_TO_ProcessNewCmds_SendHk(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;

    TO_InitData();
    TO_InitPipe();

    CFE_SB_SetMsgId(pMsg, TO_SEND_HK_MID);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));                      

    Ut_CFE_SB_AddMsgToPipe(pMsg, g_TO_AppData.CmdPipeId);

    TO_ProcessNewCmds();
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 0,
                  "ProcessNewCmds - SendHk Msg");
}




/*******************************************************************************
**
** TO_ProcessNewAppCmds Test 
**
*******************************************************************************/

/*----------------------------------------------------------------------------*/
void Test_TO_ProcessNewAppCmds_Noop(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_NOOP_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ProcessNewAppCmds - NOOP_CC - Invalid Len.");
    
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.HkTlm.usCmdCnt == 1,
                  "ProcessNewAppCmds - NOOP_CC");
}


void Test_TO_ProcessNewAppCmds_Reset(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_RESET_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ProcessNewAppCmds - RESET_CC - Invalid Len.");
    
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 0,
                  "ProcessNewAppCmds - RESET_CC");
}


void Test_TO_ProcessNewAppCmds_Custom(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;

    /* Send Noop Cmd Command */
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, 40);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));                      
    
    Ut_TO_SetReturnCode(UT_TO_CUSTOMAPPCMDS_INDEX,
                        TO_ERROR, 1);

    TO_ProcessNewAppCmds(pMsg);
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ProcessNewCmds - Bad Custom Command");

    TO_ProcessNewAppCmds(pMsg);
    UtAssert_True(g_TO_AppData.HkTlm.usCmdCnt == 1,
                  "ProcessNewCmds - Custom Command");

}


/*******************************************************************************
**
** TO_EnableOutputCmd Test
**
*******************************************************************************/
void Test_TO_EnableOutputCmd_MsgLength(void)
{
    TO_EnableOutputCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ENABLE_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ENABLE_OUTPUT_CC - Invalid Len.");
}


void Test_TO_EnableOutputCmd(void)
{
    TO_EnableOutputCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    int32 expected = 1;   
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ENABLE_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_EnableOutputCmd_t));

    /* Setup */
    Ut_TO_SetReturnCode(UT_TO_CUSTOMENABLEOUTPUTCMD_INDEX, 
                        TO_ERROR, 1);
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                              "EnableOutputCmd - Custom fail.");
    
    /* Setup */
    Ut_TO_SetReturnCode(UT_TO_CUSTOMENABLEOUTPUTCMD_INDEX, 
                        0xffffff, 1);
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "EnableOutputCmd - RouteMask exceeds TO_MAX_ROUTE_MASK.");
    

    /* Setup */
    Ut_TO_SetReturnCode(UT_TO_CUSTOMENABLEOUTPUTCMD_INDEX, 
                        0xffff, 1);
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
    
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "EnableOutputCmd - RouteMask includes un-configured routes");
    
    /* Setup */
    g_TO_AppData.routes[0].usExists = 1;
    TO_SetRouteAsConfigured(0);
    Ut_TO_SetReturnCode(UT_TO_CUSTOMENABLEOUTPUTCMD_INDEX, 
                        0x0001, 1);
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.usOutputEnabled == 1 && 
                  g_TO_AppData.routes[0].usIsEnabled == 1,
                  "EnableOutputCmd - Nominal.");
}


/*******************************************************************************
**
** TO_DisableOutputCmd Test 
**
*******************************************************************************/
void Test_TO_DisableOutputCmd_MsgLength(void)
{
    TO_DisableOutputCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "DISABLE_OUTPUT_CC - Invalid Len.");
}



void Test_TO_DisableOutputCmd(void)
{
    int32 expected = 1;
    TO_DisableOutputCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_DisableOutputCmd_t));
    
    /* Setup */
    Ut_TO_SetReturnCode(UT_TO_CUSTOMDISABLEOUTPUTCMD_INDEX, 
                        TO_ERROR, 1);
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                              "DisableOutputCmd - Custom fail.");
    
    /* Setup */
    Ut_TO_SetReturnCode(UT_TO_CUSTOMDISABLEOUTPUTCMD_INDEX, 
                        0xffffff, 1);
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
    
    
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "DisableOutputCmd - RouteMask exceeds TO_MAX_ROUTE_MASK.");
    
   
    /* Setup */
    g_TO_AppData.usOutputEnabled = 1;
    g_TO_AppData.routes[0].usIsEnabled = 1;
    cmdMsg.usRouteMask = 0x0000;
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
    
    UtAssert_True(g_TO_AppData.usOutputEnabled == 0 &&
                  g_TO_AppData.routes[0].usIsEnabled == 1,
                  "DisableOutputCmd - Nominal no routeMask.");
    
    /* Setup */
    g_TO_AppData.routes[0].usExists = 1;
    TO_SetRouteAsConfigured(0);
    g_TO_AppData.routes[0].usIsEnabled = 1;
    g_TO_AppData.usOutputEnabled = 1;
    cmdMsg.usRouteMask = 0x0001;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.usOutputEnabled == 0 && 
                  g_TO_AppData.routes[0].usIsEnabled == 0,
                  "DisableOutputCmd - Nominal with routeMask.");
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.usOutputEnabled == 0 && 
                  g_TO_AppData.routes[0].usIsEnabled == 0,
                  "DisableOutputCmd - Second call ignored.");
}

/*******************************************************************************
**
** TO_ActivateRoutesCmd Test 
**
*******************************************************************************/
void Test_TO_ActivateRoutesCmd_MsgLength(void)
{
    TO_RouteMaskArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ACTIVATE_ROUTES_CC - Invalid Len.");
}


void Test_TO_ActivateRoutesCmd(void)
{
    TO_RouteMaskArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ACTIVATE_ROUTES_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_RouteMaskArgCmd_t));

    cmdMsg.usRouteMask = 0x0001;
    
    TO_SetRouteAsUnconfigured(0);
    g_TO_AppData.routes[0].usIsEnabled = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ACTIVATE_ROUTES_CC - Non-configured routeMask");

    
    g_TO_AppData.routes[0].usExists = 1;
    TO_SetRouteAsConfigured(0);
    g_TO_AppData.routes[0].usIsEnabled = 1;
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.routes[0].usIsEnabled == 1,
                  "ACTIVATE_ROUTES_CC - Route Already Enabled");

    cmdMsg.usRouteMask = 0x0000;
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.HkTlm.usEnabledRoutes == 0x0001,
                  "ACTIVATE_ROUTES_CC - Empty routeMask check");
}


/*******************************************************************************
**
** TO_DeactivateRoutesCmd Test 
**
*******************************************************************************/
void Test_TO_DeactivateRoutesCmd_MsgLength(void)
{
    TO_RouteMaskArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DEACTIVATE_ROUTES_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "DEACTIVATE_ROUTES_CC - Invalid Len.");
}


void Test_TO_DeactivateRoutesCmd(void)
{
    TO_RouteMaskArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DEACTIVATE_ROUTES_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_RouteMaskArgCmd_t));

    g_TO_AppData.routes[0].usIsEnabled = 1;
    g_TO_AppData.routes[1].usIsEnabled = 1;
    g_TO_AppData.HkTlm.usEnabledRoutes = 0x0003;
    cmdMsg.usRouteMask = 0x0001;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.routes[0].usIsEnabled == 0 &&
                  g_TO_AppData.routes[1].usIsEnabled == 1 &&
                  g_TO_AppData.HkTlm.usEnabledRoutes == 0x0002,
                  "DEACTIVATE_ROUTES_CC - Nominal");
}


/*******************************************************************************
**
** TO_PauseOutputCmd Test 
**
*******************************************************************************/
void Test_TO_PauseOutputCmd_MsgLength(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_PAUSE_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "PAUSE_OUTPUT_CC - Invalid Len.");
}


void Test_TO_PauseOutputCmd(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_PAUSE_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));

    g_TO_AppData.usOutputEnabled = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "PAUSE_OUTPUT_CC - Output Disabled, can't pause.");
    
    g_TO_AppData.usOutputEnabled = 1;
    g_TO_AppData.usOutputActive = 1;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.usOutputActive == 0,
                  "PAUSE_OUTPUT_CC - Nominal");
}


/*******************************************************************************
**
** TO_ResumeOutputCmd Test 
**
*******************************************************************************/
void Test_TO_ResumeOutputCmd_MsgLength(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_RESUME_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "RESUME_OUTPUT_CC - Invalid Len.");
}


void Test_TO_ResumeOutputCmd(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_RESUME_OUTPUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));

    g_TO_AppData.usOutputEnabled = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "RESUME_OUTPUT_CC - Output Disabled, can't resume.");
    
    g_TO_AppData.usOutputEnabled = 1;
    g_TO_AppData.usOutputActive = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.usOutputActive == 1,
                  "RESUME_OUTPUT_CC - Nominal");
}


/*******************************************************************************
**
** TO_AddTblEntryCmd Test 
**
*******************************************************************************/
void Test_TO_AddTblEntryCmd_MsgLength(void)
{
    TO_AddTblEntryCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ADD_TBL_ENTRY_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ADD_TBL_ENTRY_CC - Invalid Len.");
}


void Test_TO_AddTblEntryCmd(void)
{
    int32 expected = 1;
    TO_AddTblEntryCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ADD_TBL_ENTRY_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_AddTblEntryCmd_t));

    cmdMsg.usMsgId = TO_UNUSED_ENTRY;

    /* Initialize Table */
    TO_InitTable();

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "ADD_TBL_ENTRY_CC - Bad MID Arg.");
    
    cmdMsg.usMsgId = CFE_EVS_EVENT_MSG_MID;

    g_TO_AppData.pConfigTable->entries[3].usMsgId = TO_HK_TLM_MID;
    g_TO_AppData.pConfigTable->entries[4].usMsgId = TO_HK_TLM_MID;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "ADD_TBL_ENTRY_CC - Table Full");
    
    cmdMsg.usMsgId = TO_HK_TLM_MID;
    
    g_TO_AppData.pConfigTable->entries[3].usMsgId = TO_UNUSED_ENTRY;
    g_TO_AppData.pConfigTable->entries[4].usMsgId = TO_UNUSED_ENTRY;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "ADD_TBL_ENTRY_CC - Duplicate MID entries");
    
    g_TO_AppData.routes[0].usExists = 1;
    cmdMsg.usMsgId = CFE_EVS_EVENT_MSG_MID;
    cmdMsg.usRouteMask = 0x0001;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, 
                            -1, 1);

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                             "ADD_TBL_ENTRY_CC - Subscription Failed");
    

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[3].usMsgId == CFE_EVS_EVENT_MSG_MID,
                  "ADD_TBL_ENTRY_CC - Nominal");
}


/*******************************************************************************
**
** TO_RemoveTblEntryCmd - Test
**
*******************************************************************************/
void Test_TO_RemoveTblEntryCmd_MsgLength(void)
{
    TO_MidArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_REMOVE_TBL_ENTRY_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "REMOVE_TBL_ENTRY_CC - Invalid Len.");
}


void Test_TO_RemoveTblEntryCmd(void)
{
    int32 expected = 1;
    TO_MidArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_REMOVE_TBL_ENTRY_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_MidArgCmd_t));

    cmdMsg.usMsgId = TO_UNUSED_ENTRY;

    /* Initialize Table */
    TO_InitTable();

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "REMOVE_TBL_ENTRY_CC - Bad MID Arg.");
    
    cmdMsg.usMsgId = CFE_EVS_EVENT_MSG_MID;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "REMOVE_TBL_ENTRY_CC - MID not found.");
    
    cmdMsg.usMsgId = TO_HK_TLM_MID;
    g_TO_AppData.routes[0].usExists = 1;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, 
                            -1, 1);

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                             "REMOVE_TBL_ENTRY_CC - Unsubscription Failed");
    

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usMsgId == TO_REMOVED_ENTRY,
                  "REMOVE_TBL_ENTRY_CC - Nominal");
}


/*******************************************************************************
**
** TO_EnableTblEntryCmd - Test
**
*******************************************************************************/
void Test_TO_EnableTblEntryCmd_MsgLength(void)
{
    TO_MidArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ENABLE_TBL_ENTRY_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ENABLE_TBL_ENTRY_CC - Invalid Len.");
}


void Test_TO_EnableTblEntryCmd(void)
{
    int32 expected = 1;
    TO_MidArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ENABLE_TBL_ENTRY_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_MidArgCmd_t));

    cmdMsg.usMsgId = TO_UNUSED_ENTRY;

    /* Initialize Table */
    TO_InitTable();

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "ENABLE_TBL_ENTRY_CC - Bad MID Arg.");
    
    cmdMsg.usMsgId = CFE_EVS_EVENT_MSG_MID;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "ENABLE_TBL_ENTRY_CC - MID not found.");
    
    cmdMsg.usMsgId = TO_HK_TLM_MID;
    g_TO_AppData.pConfigTable->entries[0].usState = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 1,
                             "ENABLE_TBL_ENTRY_CC - Nominal");
    

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 1,
                  "ENABLE_TBL_ENTRY_CC - Second call ignored");
}


/*******************************************************************************
**
** TO_DisableTblEntryCmd - Test
**
*******************************************************************************/
void Test_TO_DisableTblEntryCmd_MsgLength(void)
{
    TO_MidArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_TBL_ENTRY_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "DISABLE_TBL_ENTRY_CC - Invalid Len.");
}


void Test_TO_DisableTblEntryCmd(void)
{
    int32 expected = 1;
    TO_MidArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_TBL_ENTRY_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_MidArgCmd_t));

    /* Initialize Table */
    TO_InitTable();

    cmdMsg.usMsgId = TO_UNUSED_ENTRY;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "DISABLE_TBL_ENTRY_CC - Bad MID Arg.");
    
    cmdMsg.usMsgId = CFE_EVS_EVENT_MSG_MID;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "DISABLE_TBL_ENTRY_CC - MID not found.");
    
    cmdMsg.usMsgId = TO_HK_TLM_MID;
    g_TO_AppData.pConfigTable->entries[0].usState = 1;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 0,
                             "DISABLE_TBL_ENTRY_CC - Nominal");
    

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 0,
                  "DISABLE_TBL_ENTRY_CC - Second call ignored");
}

/*******************************************************************************
**
** TO_EnableGroupCmd - Test
**
*******************************************************************************/
void Test_TO_EnableGroupCmd_MsgLength(void)
{
    TO_GroupArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ENABLE_GROUP_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ENABLE_GROUP_CC - Invalid Len.");
}

void Test_TO_EnableGroupCmd(void)
{
    int32 expected = 1;
    TO_GroupArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ENABLE_GROUP_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_GroupArgCmd_t));

    /* Initialize Table */
    TO_InitTable();

    cmdMsg.uiGroupData = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "ENABLE_GROUP_CC - Bad Group Arg.");
    
    cmdMsg.uiGroupData = TO_MGROUP_TWO;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "ENABLE_GROUP_CC - Group not found.");
    
    cmdMsg.uiGroupData = TO_MGROUP_ONE;

    g_TO_AppData.pConfigTable->entries[0].usState = 0;
    g_TO_AppData.pConfigTable->entries[1].usState = 0;
    g_TO_AppData.pConfigTable->entries[2].usState = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 1,
                             "ENABLE_GROUP_CC - Nominal");
    

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 1,
                  "ENABLE_GROUP_CC - Second call ignored");
}


/*******************************************************************************
**
** TO_DisableGroupCmd - Test
**
*******************************************************************************/
void Test_TO_DisableGroupCmd_MsgLength(void)
{
    TO_GroupArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_GROUP_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "DISABLE_GROUP_CC - Invalid Len.");
}

void Test_TO_DisableGroupCmd(void)
{
    int32 expected = 1;
    TO_GroupArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_GROUP_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_GroupArgCmd_t));

    /* Initialize Table */
    TO_InitTable();

    cmdMsg.uiGroupData = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "DISABLE_GROUP_CC - Bad Group Arg.");
    
    cmdMsg.uiGroupData = TO_MGROUP_TWO;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "DISABLE_GROUP_CC - Group not found.");
    
    cmdMsg.uiGroupData = TO_MGROUP_ONE;

    g_TO_AppData.pConfigTable->entries[0].usState = 1;
    g_TO_AppData.pConfigTable->entries[1].usState = 1;
    g_TO_AppData.pConfigTable->entries[2].usState = 1;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 0,
                             "DISABLE_GROUP_CC - Nominal");
    

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 0,
                  "DISABLE_GROUP_CC - Second call ignored");
}

/*******************************************************************************
**
** TO_EnableAllCmd - Test
**
*******************************************************************************/
void Test_TO_EnableAllCmd_MsgLength(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ENABLE_ALL_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "ENABLE_ALL_CC - Invalid Len.");
}

void Test_TO_EnableAllCmd(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_ENABLE_ALL_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));

    /* Initialize Table */
    TO_InitTable();

    g_TO_AppData.pConfigTable->entries[0].usState = 0;
    g_TO_AppData.pConfigTable->entries[1].usState = 0;
    g_TO_AppData.pConfigTable->entries[2].usState = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 1,
                             "ENABLE_ALL_CC - Nominal");
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 1,
                             "ENABLE_ALL_CC - Second call ignored.");
}

/*******************************************************************************
**
** TO_DisableAllCmd - Test
**
*******************************************************************************/
void Test_TO_DisableAllCmd_MsgLength(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_ALL_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "DISABLE_ALL_CC - Invalid Len.");
}

void Test_TO_DisableAllCmd(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_DISABLE_ALL_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));

    /* Initialize Table */
    TO_InitTable();

    g_TO_AppData.pConfigTable->entries[0].usState = 1;
    g_TO_AppData.pConfigTable->entries[1].usState = 1;
    g_TO_AppData.pConfigTable->entries[2].usState = 1;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 0,
                             "DISABLE_ALL_CC - Nominal");
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usState == 0,
                             "DISABLE_ALL_CC - Second call ignored.");
}

/*******************************************************************************
**
** TO_SetRouteByMidCmd - Test
**
*******************************************************************************/
void Test_TO_SetRouteByMidCmd_MsgLength(void)
{
    TO_SetRouteByMidCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SET_ROUTE_BY_MID_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "SET_ROUTE_BY_MID_CC - Invalid Len.");
}

void Test_TO_SetRouteByMidCmd(void)
{
    int32 expected = 1;
    TO_SetRouteByMidCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SET_ROUTE_BY_MID_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_SetRouteByMidCmd_t));

    /* Initialize Table */
    TO_InitTable();

    cmdMsg.usMsgId = TO_UNUSED_ENTRY;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "SET_ROUTE_BY_MID_CC - Bad MID Arg.");

    
    cmdMsg.usMsgId = CFE_EVS_EVENT_MSG_MID;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "SET_ROUTE_BY_MID_CC - MID not found.");
    
    cmdMsg.usMsgId = TO_HK_TLM_MID;
    cmdMsg.usRouteMask = 0x0003; 
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[0].usRouteMask == 0x0003,
                             "SET_ROUTE_BY_MID_CC - Nominal");
}

/*******************************************************************************
**
** TO_SetRouteByGroupCmd - Test
**
*******************************************************************************/
void Test_TO_SetRouteByGroupCmd_MsgLength(void)
{
    TO_SetRouteByGroupCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SET_ROUTE_BY_GROUP_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "SET_ROUTE_BY_GROUP_CC - Invalid Len.");
}

void Test_TO_SetRouteByGroupCmd(void)
{
    int32 expected = 1;
    TO_SetRouteByGroupCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SET_ROUTE_BY_GROUP_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_SetRouteByGroupCmd_t));

    /* Initialize Table */
    TO_InitTable();

    cmdMsg.uiGroupData = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "SET_ROUTE_BY_GROUP_CC - Bad Group Arg.");

    
    cmdMsg.uiGroupData = TO_MGROUP_TWO;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "SET_ROUTE_BY_GROUP_CC - Group not found.");
    
    
    cmdMsg.uiGroupData = TO_MGROUP_ONE;
    cmdMsg.usRouteMask = 0x0003; 
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[1].usRouteMask == 0x0003,
                             "SET_ROUTE_BY_GROUP_CC - Nominal");
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.pConfigTable->entries[1].usRouteMask == 0x0003,
                             "SET_ROUTE_BY_GROUP_CC - Second call ignored");
}


/*******************************************************************************
**
** TO_ManageTableCmd Test
**
*******************************************************************************/
void Test_TO_ManageTableCmd_MsgLength(void)
{
    CFE_TBL_NotifyCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_MANAGE_TABLE_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "MANAGE_TABLE_CC - Invalid Len.");
}


void Test_TO_ManageTableCmd(void)
{
    int32 expected = 1;
    CFE_TBL_NotifyCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_MANAGE_TABLE_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(cmdMsg));

    TO_InitTable();
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_RELEASEADDRESS_INDEX, -1, 1);

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "MANAGE_TABLE_CC - ReleaseAddress fail.");

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, -1, 1);

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "MANAGE_TABLE_CC - Manage fail.");
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, -1, 1);

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "MANAGE_TABLE_CC - GetAddress fail.");
    
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, -1, 1);
    CFE_TBL_Load(g_TO_AppData.tableHandle, CFE_TBL_SRC_ADDRESS, TO_CONFIG_FILENAME); 

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "MANAGE_TABLE_CC - Unsubscribe fail.");

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, -1, 1);
    CFE_TBL_Load(g_TO_AppData.tableHandle, CFE_TBL_SRC_ADDRESS, TO_CONFIG_FILENAME); 

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "MANAGE_TABLE_CC - Subscribe fail.");
    
    CFE_TBL_Load(g_TO_AppData.tableHandle, CFE_TBL_SRC_ADDRESS, TO_CONFIG_FILENAME); 
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usTblUpdateCnt == 1,
                  "MANAGE_TABLE_CC - Nominal.");
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usTblUpdateCnt == 1,
                  "MANAGE_TABLE_CC - Second call ingored.");
}

/*******************************************************************************
**
** TO_SetRoutePeriodCmd - Test
**
*******************************************************************************/
void Test_TO_SetRoutePeriodCmd_MsgLength(void)
{
    TO_SetRoutePeriodCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SET_ROUTE_PERIOD_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "SET_ROUTE_PERIOD_CC - Invalid Len.");
}

void Test_TO_SetRoutePeriodCmd(void)
{
    int32 expected = 1;
    TO_SetRoutePeriodCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SET_ROUTE_PERIOD_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_SetRoutePeriodCmd_t));

    g_TO_AppData.routes[0].usExists = 0;
    cmdMsg.usRouteMask = 0x0001;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "SET_ROUTE_PERIOD_CC - Route does not exist.");

    g_TO_AppData.routes[0].usExists = 1;
    cmdMsg.usWakePeriod = TO_MAX_WAKEUP_COUNT + 1; 

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "SET_ROUTE_PERIOD_CC - WakePeriod to large.");
    
    cmdMsg.usWakePeriod = TO_MAX_WAKEUP_COUNT - 1; 

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "SET_ROUTE_PERIOD_CC - WakePeriod invalid.");
    
    cmdMsg.usWakePeriod = TO_MAX_WAKEUP_COUNT; 
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.routes[0].usWakePeriod = TO_MAX_WAKEUP_COUNT,
                             "SET_ROUTE_PERIOD_CC - Nominal");
}


/*******************************************************************************
**
** TO_SetWakeupTimeoutCmd - Test
**
*******************************************************************************/
void Test_TO_SetWakeupTimeoutCmd_MsgLength(void)
{
    TO_SetWakeupTimeoutCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SET_WAKEUP_TIMEOUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "SET_WAKEUP_TIMEOUT_CC - Invalid Len.");
}

void Test_TO_SetWakeupTimeoutCmd(void)
{
    int32 expected = 1;
    TO_SetWakeupTimeoutCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SET_WAKEUP_TIMEOUT_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_SetWakeupTimeoutCmd_t));

    cmdMsg.uiWakeupTimeout = 0;

    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    expected += UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == expected,
                  "SET_WAKEUP_TIMEOUT_CC - WakeupTimeout too small.");

    cmdMsg.uiWakeupTimeout = CFE_SB_PEND_FOREVER;
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.uiWakeupTimeout == CFE_SB_PEND_FOREVER,
                             "SET_WAKEUP_TIMEOUT_CC - Nominal (Pend forever)");
    
    cmdMsg.uiWakeupTimeout = TO_MIN_WAKEUP_TIMEOUT;
    
    /* Execute test */
    TO_ProcessNewAppCmds(pMsg);

    UtAssert_True(g_TO_AppData.uiWakeupTimeout == TO_MIN_WAKEUP_TIMEOUT,
                             "SET_WAKEUP_TIMEOUT_CC - Nominal");
}

/*******************************************************************************
**
** TO_SendDataTypePktCmd - Test
**
*******************************************************************************/
void Test_TO_SendDataTypePktCmd_MsgLength(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SEND_DATA_TYPE_CC);
    CFE_SB_SetTotalMsgLength(pMsg, 20);                      

    /* Execute test */
    TO_SendDataTypePktCmd(pMsg);
   
    UtAssert_True(g_TO_AppData.HkTlm.usCmdErrCnt == 1,
                  "SEND_DATA_TYPE_CC - Invalid Len.");
}

void Test_TO_SendDataTypePktCmd(void)
{
    TO_NoArgCmd_t cmdMsg;
    CFE_SB_MsgPtr_t pMsg = (CFE_SB_MsgPtr_t) &cmdMsg;
    
    CFE_SB_SetMsgId(pMsg, TO_APP_CMD_MID);
    CFE_SB_SetCmdCode(pMsg, TO_SEND_DATA_TYPE_CC);
    CFE_SB_SetTotalMsgLength(pMsg, sizeof(TO_NoArgCmd_t));

    /* Execute test */
    TO_SendDataTypePktCmd(pMsg);

    UtAssert_True(g_TO_AppData.uiWakeupTimeout == TO_MIN_WAKEUP_TIMEOUT,
                             "SEND_DATA_TYPE_CC - Nominal");
}


/*******************************************************************************
**
** Utilities - Tests
**
*******************************************************************************/
void Test_TO_DisableRoute(void)
{
    g_TO_AppData.routes[0].usIsEnabled = 1;

    /* Execute test */
    TO_DisableRoute(0);

    UtAssert_True(g_TO_AppData.routes[0].usIsEnabled == 0,
                  "TO_DisableRoute - Nominal");
}


void Test_TO_GetRouteMask(void)
{
    uint16 routeMask;

    TO_InitTable();

    /* Execute test */
    routeMask = TO_GetRouteMask(TO_MAX_TBL_ENTRIES);

    UtAssert_True(routeMask == 0x0000,
                  "TO_DisableRoute - Large table index");


    g_TO_AppData.pConfigTable->entries[0].usRouteMask = 0x0005;
    routeMask = TO_GetRouteMask(0);

    UtAssert_True(routeMask == 0x0005,
                  "TO_DisableRoute - Nominal");
}


void Test_TO_GetMessageID(void)
{
    CFE_SB_MsgId_t mid;

    TO_InitTable();

    /* Execute test */
    mid = TO_GetMessageID(TO_MAX_TBL_ENTRIES);

    UtAssert_True(mid == 0,
                  "TO_GetMessageID - Large table index");


    mid = TO_GetMessageID(0);

    UtAssert_True(mid == TO_HK_TLM_MID,
                  "TO_GetMessageID - Nominal");
}
    

void Test_TO_SubscribeAllMsgs(void)
{
    int32 actual;
    
    TO_InitTable();
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX,
                            -1, 1);

    /* Execute test */
    actual = TO_SubscribeAllMsgs(); 

    UtAssert_True(actual == TO_ERROR,
                  "TO_SubscribeAllMsgs - Subscribe Error");
}


void Test_TO_SubscribeMsg(void)
{
    /* Execute test */
    int32 actual = TO_SubscribeMsg(NULL); 

    UtAssert_True(actual == TO_BAD_ARG_ERR,
                  "TO_SubscribeMsg - NULL input arg.");
}


void Test_TO_UnsubscribeAllMsgs(void)
{
    int32 actual;

    /* Execute test */
    actual = TO_UnsubscribeAllMsgs(NULL); 
    
    UtAssert_True(actual == TO_BAD_ARG_ERR,
                  "TO_UnsubscribeAllMsgs - NULL input arg.");
    
    TO_InitTable();

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX,
                            -1, 1);
    
    /* Execute test */
    actual = TO_UnsubscribeAllMsgs(g_TO_AppData.pConfigTable); 

    UtAssert_True(actual == TO_ERROR,
                  "TO_UnsubscribeAllMsgs - Unsubscribe fail.");

    
    /* Execute test */
    actual = TO_UnsubscribeAllMsgs(g_TO_AppData.pConfigTable); 

    UtAssert_True(actual == TO_SUCCESS,
                  "TO_UnsubscribeAllMsgs - Nominal.");
}


void Test_TO_UnsubscribeMsg(void)
{
    /* Execute test */
    int32 actual = TO_UnsubscribeMsg(NULL); 

    UtAssert_True(actual == TO_BAD_ARG_ERR,
                  "TO_UnsubscribeMsg - NULL input arg.");
}

/* ------------------- End of test cases --------------------------------------*/



/*
 * TO_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void TO_Setup(void)
{  
    Ut_OSAPI_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_CFE_EVS_Reset();
    Ut_CFE_TBL_Reset();

    Ut_CFE_TBL_AddTable(TO_CONFIG_FILENAME, (void *) &to_ConfigTable);
}

/*
 * TO_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void TO_TearDown(void)
{
    CFE_PSP_MemSet((void*)&g_TO_AppData.HkTlm, 0x00, 
                   sizeof(g_TO_AppData.HkTlm));
    Ut_CFE_SB_ClearPipes();
}


/* TO_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void TO_AddTestCase(void)
{
    /* TO_AppInit Tests */
    UtTest_Add(Test_TO_InitEvent_RegisterFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitEvent_RegisterFail");
    UtTest_Add(Test_TO_InitEvent,  TO_Setup, TO_TearDown,
              "Test_TO_InitEvent");
    UtTest_Add(Test_TO_InitData,  TO_Setup, TO_TearDown,
              "Test_TO_InitData");
    UtTest_Add(Test_TO_CustomInit_Fail,  TO_Setup, TO_TearDown,
              "Test_TO_CustomInit_Fail");
    UtTest_Add(Test_TO_InitTable_RegisterFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitTable_RegisterFail");
    UtTest_Add(Test_TO_InitTable_LoadFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitTable_LoadFail");
    UtTest_Add(Test_TO_InitTable_ManageFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitTable_ManageFail");
    UtTest_Add(Test_TO_InitTable_GetAddressFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitTable_GetAddressFail");
    UtTest_Add(Test_TO_InitTable_NotifyByMessageFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitTable_NotifyByMessageFail");
    UtTest_Add(Test_TO_InitTable,  TO_Setup, TO_TearDown,
              "Test_TO_InitTable");
    UtTest_Add(Test_TO_InitPipe_CreatePipeFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitPipe_CreatePipeFail");
    UtTest_Add(Test_TO_InitPipe_CfChnlIdxFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitPipe_CfChnlIdxFail");
    UtTest_Add(Test_TO_InitPipe_CfCountSemCreateFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitPipe_CfCountSemCreateFail");
    UtTest_Add(Test_TO_InitPipe_SubscribeMsgFail,  TO_Setup, TO_TearDown,
              "Test_TO_InitPipe_SubscribeMsgFail");
    UtTest_Add(Test_TO_InitPipe,  TO_Setup, TO_TearDown,
              "Test_TO_InitPipe");
    UtTest_Add(Test_TO_AppInit,  TO_Setup, TO_TearDown,
              "Test_TO_AppInit");
    
    /* TO_ValidateTable */
    UtTest_Add(Test_TO_ValidateTable_Gap,  TO_Setup, TO_TearDown,
              "Test_TO_ValidateTable_Gap");
    UtTest_Add(Test_TO_ValidateTable_Duplicate,  TO_Setup, TO_TearDown,
              "Test_TO_ValidateTable_Duplicate");
    UtTest_Add(Test_TO_ValidateTable_CriticalMid,  TO_Setup, TO_TearDown,
              "Test_TO_ValidateTable_CriticalMid");
    UtTest_Add(Test_TO_ValidateTable,  TO_Setup, TO_TearDown,
              "Test_TO_ValidateTable");

    /* TO_AppMain */
    UtTest_Add(Test_TO_AppMain_RegisterFail, TO_Setup, TO_TearDown,
               "Test_TO_AppMain_Registerfail");
    UtTest_Add(Test_TO_AppMain_InitFail, TO_Setup, TO_TearDown,
               "Test_TO_AppMain_Initfail");
    UtTest_Add(Test_TO_AppMain_RcvMsgFail, TO_Setup, TO_TearDown,
               "Test_TO_AppMain_RcvMsgfail");

    /* TO_RcvMsg */
    UtTest_Add(Test_TO_RcvMsg_NoMsgError,  TO_Setup, TO_TearDown,
              "Test_TO_RcvMsg_NoMsgError");
    UtTest_Add(Test_TO_RcvMsg_BadMsg,  TO_Setup, TO_TearDown,
              "Test_TO_RcvMsg_BadMsg");
    UtTest_Add(Test_TO_RcvMsg_Wakeup,  TO_Setup, TO_TearDown,
              "Test_TO_RcvMsg_Wakeup");
    UtTest_Add(Test_TO_RcvMsg_Timeout,  TO_Setup, TO_TearDown,
              "Test_TO_RcvMsg_Timeout");

    /* TO_ProcessTlmPipes */
    UtTest_Add(Test_TO_ProcessTlmPipes,  TO_Setup, TO_TearDown,
              "Test_TO_ProcessTlmPipes");
    
    /* TO_ProcessNewData */
    UtTest_Add(Test_TO_ProcessNewData_FrameError,  TO_Setup, TO_TearDown,
              "Test_TO_ProcessNewData_FrameError");
    UtTest_Add(Test_TO_ProcessNewData_BadMsg,  TO_Setup, TO_TearDown,
              "Test_TO_ProcessNewData_BadMsg");
    UtTest_Add(Test_TO_ProcessNewData_CustomProcessError,  TO_Setup, TO_TearDown,
              "Test_TO_ProcessNewData_CustomProcessError");
    UtTest_Add(Test_TO_ProcessNewData,  TO_Setup, TO_TearDown,
              "Test_TO_ProcessNewData");

    /* TO_ProcessNewCmds */
    UtTest_Add(Test_TO_ProcessNewCmds_BadMsg,  TO_Setup, TO_TearDown,
               "Test_TO_ProcessNewCmds_BadMsg");
    UtTest_Add(Test_TO_ProcessNewCmds_AppCmd,  TO_Setup, TO_TearDown,
               "Test_TO_ProcessNewCmds_AppCmd");
    UtTest_Add(Test_TO_ProcessNewCmds_SendHk,  TO_Setup, TO_TearDown,
               "Test_TO_ProcessNewCmds_SendHk");

    /* TO_ProcessNewAppCmds */
    UtTest_Add(Test_TO_ProcessNewAppCmds_Noop,  TO_Setup, TO_TearDown,
               "Test_TO_ProcessNewAppCmds_Noop");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Reset,  TO_Setup, TO_TearDown,
               "Test_TO_ProcessNewAppCmds_Reset");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Custom,  TO_Setup, TO_TearDown,
              "Test_TO_ProcessNewAppCmds_Custom");
   
    /* Commands */
    UtTest_Add(Test_TO_EnableOutputCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_EnableOutputCmd_MsgLength");
    UtTest_Add(Test_TO_EnableOutputCmd,  TO_Setup, TO_TearDown,
              "Test_TO_EnableOutputCmd");
    
    UtTest_Add(Test_TO_DisableOutputCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_DisableOutputCmd_MsgLength");
    UtTest_Add(Test_TO_DisableOutputCmd,  TO_Setup, TO_TearDown,
              "Test_TO_DisableOutputCmd");
    
    UtTest_Add(Test_TO_ActivateRoutesCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_ActivateRoutesCmd_MsgLength");
    UtTest_Add(Test_TO_ActivateRoutesCmd,  TO_Setup, TO_TearDown,
              "Test_TO_ActivateRoutesCmd");
    
    UtTest_Add(Test_TO_DeactivateRoutesCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_DeactivateRoutesCmd_MsgLength");
    UtTest_Add(Test_TO_DeactivateRoutesCmd,  TO_Setup, TO_TearDown,
              "Test_TO_DeactivateRoutesCmd");
    
    UtTest_Add(Test_TO_PauseOutputCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_PauseOutputCmd_MsgLength");
    UtTest_Add(Test_TO_PauseOutputCmd,  TO_Setup, TO_TearDown,
              "Test_TO_PauseOutputCmd");
    
    UtTest_Add(Test_TO_ResumeOutputCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_ResumeOutputCmd_MsgLength");
    UtTest_Add(Test_TO_ResumeOutputCmd,  TO_Setup, TO_TearDown,
              "Test_TO_ResumeOutputCmd");
    
    UtTest_Add(Test_TO_AddTblEntryCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_AddTblEntryCmd_MsgLength");
    UtTest_Add(Test_TO_AddTblEntryCmd,  TO_Setup, TO_TearDown,
              "Test_TO_AddTblEntryCmd");
    
    UtTest_Add(Test_TO_RemoveTblEntryCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_RemoveTblEntryCmd_MsgLength");
    UtTest_Add(Test_TO_RemoveTblEntryCmd,  TO_Setup, TO_TearDown,
              "Test_TO_RemoveTblEntryCmd");

    UtTest_Add(Test_TO_EnableTblEntryCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_EnableTblEntryCmd_MsgLength");
    UtTest_Add(Test_TO_EnableTblEntryCmd,  TO_Setup, TO_TearDown,
              "Test_TO_EnableTblEntryCmd");

    UtTest_Add(Test_TO_DisableTblEntryCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_DisableTblEntryCmd_MsgLength");
    UtTest_Add(Test_TO_DisableTblEntryCmd,  TO_Setup, TO_TearDown,
              "Test_TO_DisableTblEntryCmd");

    UtTest_Add(Test_TO_EnableGroupCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_EnableGroupCmd_MsgLength");
    UtTest_Add(Test_TO_EnableGroupCmd,  TO_Setup, TO_TearDown,
              "Test_TO_EnableGroupCmd");
    
    UtTest_Add(Test_TO_DisableGroupCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_DisableGroupCmd_MsgLength");
    UtTest_Add(Test_TO_DisableGroupCmd,  TO_Setup, TO_TearDown,
              "Test_TO_DisableGroupCmd");
    
    UtTest_Add(Test_TO_EnableAllCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_EnableAllCmd_MsgLength");
    UtTest_Add(Test_TO_EnableAllCmd,  TO_Setup, TO_TearDown,
              "Test_TO_EnableAllCmd");
    
    UtTest_Add(Test_TO_DisableAllCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_DisableAllCmd_MsgLength");
    UtTest_Add(Test_TO_DisableAllCmd,  TO_Setup, TO_TearDown,
              "Test_TO_DisableAllCmd");
    
    UtTest_Add(Test_TO_SetRouteByMidCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_SetRouteByMidCmd_MsgLength");
    UtTest_Add(Test_TO_SetRouteByMidCmd,  TO_Setup, TO_TearDown,
              "Test_TO_SetRouteByMidCmd");
    
    UtTest_Add(Test_TO_SetRouteByGroupCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_SetRouteByGroupCmd_MsgLength");
    UtTest_Add(Test_TO_SetRouteByGroupCmd,  TO_Setup, TO_TearDown,
              "Test_TO_SetRouteByGroupCmd");
    
    UtTest_Add(Test_TO_ManageTableCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_ManageTableCmd_MsgLength");
    UtTest_Add(Test_TO_ManageTableCmd,  TO_Setup, TO_TearDown,
              "Test_TO_ManageTableCmd");
    
    UtTest_Add(Test_TO_SetRoutePeriodCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_SetRoutePeriodCmd_MsgLength");
    UtTest_Add(Test_TO_SetRoutePeriodCmd,  TO_Setup, TO_TearDown,
              "Test_TO_SetRoutePeriodCmd");
    
    UtTest_Add(Test_TO_SetWakeupTimeoutCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_SetWakeupTimeoutCmd_MsgLength");
    UtTest_Add(Test_TO_SetWakeupTimeoutCmd,  TO_Setup, TO_TearDown,
              "Test_TO_SetWakeupTimeoutCmd");
    
    UtTest_Add(Test_TO_SendDataTypePktCmd_MsgLength,  TO_Setup, TO_TearDown,
              "Test_TO_SendDataTypePktCmd_MsgLength");
    UtTest_Add(Test_TO_SendDataTypePktCmd,  TO_Setup, TO_TearDown,
              "Test_TO_SendDataTypePktCmd");


    /* Utilities */
    UtTest_Add(Test_TO_DisableRoute,  TO_Setup, TO_TearDown,
              "Test_TO_DisableRoute");
    UtTest_Add(Test_TO_GetRouteMask,  TO_Setup, TO_TearDown,
              "Test_TO_GetRouteMask");
    UtTest_Add(Test_TO_GetMessageID,  TO_Setup, TO_TearDown,
              "Test_TO_GetMessageID");
    UtTest_Add(Test_TO_SubscribeAllMsgs,  TO_Setup, TO_TearDown,
              "Test_TO_SubscribeAllMsgs");
    UtTest_Add(Test_TO_SubscribeMsg,  TO_Setup, TO_TearDown,
              "Test_TO_SubscribeMsg");
    UtTest_Add(Test_TO_UnsubscribeAllMsgs,  TO_Setup, TO_TearDown,
              "Test_TO_UnsubscribeAllMsgs");
    UtTest_Add(Test_TO_UnsubscribeMsg,  TO_Setup, TO_TearDown,
              "Test_TO_UnsubscribeMsg");
}
