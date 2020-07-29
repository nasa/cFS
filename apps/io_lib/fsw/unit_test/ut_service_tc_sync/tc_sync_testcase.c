/*
 * Filename: tc_sync_testcase.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains a unit test cases for the TC_SYNC Service
 *
 * Modification History:
 *   12/09/2015 Guy de Carufel, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "cfe.h"
#include "utassert.h"
#include "uttest.h"
#include "utlist.h"
#include "ut_cfe_time_stubs.h"
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

#include "tc_sync.h"

/* 2-byte Start, 8-byte tail, 10x BCH Codeblock (7-bytes + 1 parity byte) */
uint8 cltuBuffer[90];
/* 70 Byte TF (10x BCH Code Block -> 8x7bytes) */
uint8 tfBuffer[70];

uint8 tailSeq[8] = {0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0x79};

/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  Test DeRandomizeFrame 
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TC_SYNC_DeRandomizeFrame(void)
{
    uint8 expSeq[5] = {0xff, 0x39, 0x9e, 0x5a, 0x68};

    /* Execute Test */
    TC_SYNC_LibInit();
    TC_SYNC_DeRandomizeFrame(&tfBuffer[0], 70);

    /* Verify Outputs */
    UtAssert_True(tfBuffer[0] == expSeq[0], "Pseudo-Random Seq byte 0");
    UtAssert_True(tfBuffer[1] == expSeq[1], "Pseudo-Random Seq byte 1");
    UtAssert_True(tfBuffer[2] == expSeq[2], "Pseudo-Random Seq byte 2");
    UtAssert_True(tfBuffer[3] == expSeq[3], "Pseudo-Random Seq byte 3");
    UtAssert_True(tfBuffer[4] == expSeq[4], "Pseudo-Random Seq byte 4");
    UtAssert_True(tfBuffer[32] == 0xfe, "Test Pseudo-Random 1x period lapse");
    UtAssert_True(tfBuffer[64] == 0xfc, "Test Pseudo-Random 2x period lapse");

    /* Execute Test */
    TC_SYNC_DeRandomizeFrame(&tfBuffer[0], 70);

    /* Verify Outputs */
    UtAssert_True(tfBuffer[43] == 0x00, "Re-randomize check.");
}


/*******************************************************************************
**
**  Test GetTransferFrame
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TC_SYNC_GetTransferFrame_NullPtr(void)
{
    /* Execute Test */
    int32 expected = TC_SYNC_INVALID_POINTER;
    int32 actual = TC_SYNC_GetTransferFrame(NULL, cltuBuffer, 70, 90, FALSE);
    UtAssert_True(actual == expected, "NULL TF Buffer");

    actual = TC_SYNC_GetTransferFrame(tfBuffer, NULL, 70, 70, FALSE);
    UtAssert_True(actual == expected, "NULL CLTU Buffer");
}


void Test_TC_SYNC_GetTransferFrame_BadCltu(void)
{
    /* Execute Test */
    cltuBuffer[0] = 0x00;
    cltuBuffer[1] = 0x90; 
    int32 expected = TC_SYNC_INVALID_CLTU;
    int32 actual = TC_SYNC_GetTransferFrame(tfBuffer, cltuBuffer, 70, 90, FALSE);
    UtAssert_True(actual == expected, "Bad CLTU Start Sequence");

    cltuBuffer[0] = 0xeb;
    cltuBuffer[1] = 0x00;
    actual = TC_SYNC_GetTransferFrame(tfBuffer, cltuBuffer, 70, 90, FALSE);
    UtAssert_True(actual == expected, "Bad CLTU Start Sequence");
    
    cltuBuffer[0] = 0xeb;
    cltuBuffer[1] = 0x90; 
    actual = TC_SYNC_GetTransferFrame(tfBuffer, cltuBuffer, 70, 80, FALSE);
    UtAssert_True(actual == expected, "No CLTU Tail Sequence");
}


void Test_TC_SYNC_GetTransferFrame_BadLength(void)
{
    /* Execute Test */
    int32 expected = TC_SYNC_INVALID_LENGTH;
    int32 actual = TC_SYNC_GetTransferFrame(tfBuffer, cltuBuffer, 60, 90, FALSE);
    UtAssert_True(actual == expected, "TF Buffer size to small");
}
 

void Test_TC_SYNC_GetTransferFrame(void)
{
    /* Setup */
    cltuBuffer[10] = 0xac;

    /* Execute Test */
    int32 expected = 70;
    int32 actual = TC_SYNC_GetTransferFrame(tfBuffer, cltuBuffer, 70, 90, FALSE);
    UtAssert_True(actual == expected, "TF Buffer Extracted");
    UtAssert_True(tfBuffer[7] == 0xac, "TF byte check");
}

void Test_TC_SYNC_GetTransferFrame_DeRandom(void)
{
    /* Setup */
    cltuBuffer[0] = 0xeb;
    cltuBuffer[1] = 0x90;
    CFE_PSP_MemCpy(&cltuBuffer[82], tailSeq, 8);

    /* Execute Test */
    int32 expected = 70;
    int32 actual = TC_SYNC_GetTransferFrame(tfBuffer, cltuBuffer, 70, 90, TRUE);
    UtAssert_True(actual == expected, "TF Buffer Extracted");
    UtAssert_True(tfBuffer[32] == 0xfe, "TF Byte check");
}

/*******************************************************************************
**
**  Test CheckStartSeq
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TC_SYNC_CheckStartSeq_NullPtr(void)
{
    uint16 cltuOffset = 0;
    
    /* Execute Test */
    int32 expected = TC_SYNC_INVALID_POINTER;
    int32 actual = TC_SYNC_CheckStartSeq(NULL, &cltuOffset); 
    UtAssert_True(actual == expected, "NULL Seq Ptr");

    actual = TC_SYNC_CheckStartSeq(cltuBuffer, NULL); 
    UtAssert_True(actual == expected, "NULL cltuOffset Ptr");
}


/*******************************************************************************
**
**  Test GetGetCodeBlockData
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TC_SYNC_GetCodeBlockData_NullPtr(void)
{
    uint16 cltuOffset = 0;
    uint16 tfOffset = 0;
    uint8 *pCodeBlock = cltuBuffer;
    
    /* Execute Test */
    int32 expected = TC_SYNC_INVALID_POINTER;
    int32 actual = TC_SYNC_GetCodeBlockData(NULL, pCodeBlock, &tfOffset,
                                          &cltuOffset, 70, 90);
    UtAssert_True(actual == expected, "NULL TF Buffer");

    actual = TC_SYNC_GetCodeBlockData(tfBuffer, NULL, &tfOffset,
                                          &cltuOffset, 70, 90);
    UtAssert_True(actual == expected, "NULL Code Block Ptr");
    
    actual = TC_SYNC_GetCodeBlockData(tfBuffer, pCodeBlock, NULL,
                                          &cltuOffset, 70, 90);
    UtAssert_True(actual == expected, "NULL tfOffset Ptr");
    
    actual = TC_SYNC_GetCodeBlockData(tfBuffer, pCodeBlock, &tfOffset,
                                          NULL, 70, 90);
    UtAssert_True(actual == expected, "NULL cltuOffset Ptr");
}


/* ------------------- End of test cases --------------------------------------*/


/*
 * TC_SYNC_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void TC_SYNC_Setup(void)
{
    memset(cltuBuffer, 0x0, 90);    
    memset(tfBuffer, 0x0, 70);

    cltuBuffer[0] = 0xeb;
    cltuBuffer[1] = 0x90;
    memcpy(&cltuBuffer[82], tailSeq, 8);
}

/*
 * TC_SYNC_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void TC_SYNC_TearDown(void)
{

}


/*
 * TC_SYNC_Setup_ProcessFrame
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test of TC_SYNC_ProcessFrame
 *
 * Notes:
 */
void TC_SYNC_Setup_ProcessFrame(void)
{
    TC_SYNC_Setup();
}




#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* TC_SYNC_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void TC_SYNC_AddTestCase(void)
{
    /* TC_SYNC_DeRandomizeFrame */
    ADD_TEST(Test_TC_SYNC_DeRandomizeFrame,         TC_SYNC_Setup, TC_SYNC_TearDown);
    ADD_TEST(Test_TC_SYNC_GetTransferFrame_NullPtr, TC_SYNC_Setup, TC_SYNC_TearDown);
    ADD_TEST(Test_TC_SYNC_GetTransferFrame_BadCltu, TC_SYNC_Setup, TC_SYNC_TearDown);
    ADD_TEST(Test_TC_SYNC_GetTransferFrame_BadLength,   TC_SYNC_Setup, TC_SYNC_TearDown);
    ADD_TEST(Test_TC_SYNC_GetTransferFrame,             TC_SYNC_Setup, TC_SYNC_TearDown);
    ADD_TEST(Test_TC_SYNC_GetTransferFrame_DeRandom,    TC_SYNC_Setup, TC_SYNC_TearDown);
    ADD_TEST(Test_TC_SYNC_CheckStartSeq_NullPtr,      TC_SYNC_Setup, TC_SYNC_TearDown);
    ADD_TEST(Test_TC_SYNC_GetCodeBlockData_NullPtr,   TC_SYNC_Setup, TC_SYNC_TearDown);
}
