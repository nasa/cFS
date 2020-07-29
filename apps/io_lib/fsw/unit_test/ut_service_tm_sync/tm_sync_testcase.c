/*
 * Filename: tm_sync_testcase.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains a unit test cases for the TM_SYNC Service
 *
 * Modification History:
 *   11/02/2015 Guy de Carufel, Odyssey Space Research, LLC
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

#include "tm_sync.h"


/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  Test PseudoRandomize 
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TM_SYNC_PseudoRandomize(void)
{
    uint8 expSeq[5] = {0xff, 0x48, 0x0e, 0xc0, 0x9A};
    uint8 data[100];

    /* Clear buffer */
    memset(data, 0x0, 100);

    /* Execute Test */
    TM_SYNC_LibInit();
    TM_SYNC_PseudoRandomize(&data[0], 100);

    /* Verify Outputs */
    UtAssert_True(data[0] == expSeq[0], "Pseudo-Random Seq byte 0");
    UtAssert_True(data[1] == expSeq[1], "Pseudo-Random Seq byte 1");
    UtAssert_True(data[2] == expSeq[2], "Pseudo-Random Seq byte 2");
    UtAssert_True(data[3] == expSeq[3], "Pseudo-Random Seq byte 3");
    UtAssert_True(data[4] == expSeq[4], "Pseudo-Random Seq byte 4");
    UtAssert_True(data[32] == 0xfe, "Test Pseudo-Random 1x period lapse");
    UtAssert_True(data[64] == 0xfd, "Test Pseudo-Random 2x period lapse");

    /* Execute Test */
    TM_SYNC_PseudoRandomize(&data[0], 100);

    /* Verify Outputs */
    UtAssert_True(data[43] == 0x00, "De-randomize check.");
}


/*******************************************************************************
**
**  Test Synchronize
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TM_SYNC_Synchronize_NullPtr(void)
{
    uint8 buffer[100];
    
    /* Execute Test */
    int32 expected = TM_SYNC_INVALID_POINTER;
    int32 actual = TM_SYNC_Synchronize(NULL, TM_SYNC_ASM_STR, TM_SYNC_ASM_SIZE, 96, TRUE);
    UtAssert_True(actual == expected, "NULL Pointer");

    actual = TM_SYNC_Synchronize(buffer, NULL, TM_SYNC_ASM_SIZE, 96, TRUE);
    UtAssert_True(actual == expected, "NULL ASM String");
}


void Test_TM_SYNC_Synchronize_BadAsm(void)
{
    uint8 buffer[100];
    
    /* Execute Test */
    int32 expected = TM_SYNC_INVALID_ASM_SIZE;
    int32 actual = TM_SYNC_Synchronize(buffer, TM_SYNC_ASM_STR, 3, 96, TRUE);
    UtAssert_True(actual == expected, "Invalid ASM Size");

    actual = TM_SYNC_Synchronize(buffer, TM_SYNC_ASM_STR, 5, 96, TRUE);
    UtAssert_True(actual == expected, "Invalid ASM Size");
}


void Test_TM_SYNC_Synchronize(void)
{
    uint8 expAsm[4] = {0x1a, 0xcf, 0xfc, 0x1d};
    uint8 expSeq[5] = {0xff, 0x48, 0x0e, 0xc0, 0x9A};
    uint8 buffer[100];

    /* Clear buffer */
    memset(buffer, 0x0, 100);

    /* Execute Test */
    TM_SYNC_LibInit();
    TM_SYNC_Synchronize(buffer, TM_SYNC_ASM_STR, TM_SYNC_ASM_SIZE, 96, TRUE);

    /* Verify Outputs */
    UtAssert_True(buffer[0] == expAsm[0], "ASM Header byte 0");
    UtAssert_True(buffer[1] == expAsm[1], "ASM Header byte 1");
    UtAssert_True(buffer[2] == expAsm[2], "ASM Header byte 2");
    UtAssert_True(buffer[3] == expAsm[3], "ASM Header byte 3");
    UtAssert_True(buffer[5] == expSeq[1], "Pseudo-Randomize check");

    /* Execute Test */
    TM_SYNC_PseudoRandomize(&buffer[4], 96);

    /* Verify Outputs */
    UtAssert_True(buffer[5] == 0x00, "De-randomize check.");
}

/* ------------------- End of test cases --------------------------------------*/

/*******************************************************************************
**
**  Test Synchronize no random.
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TM_SYNC_Synchronize_NoRandom(void)
{
    uint8 expAsm[4] = {0x1a, 0xcf, 0xfc, 0x1d};
    uint8 buffer[100];

    /* Clear buffer */
    memset(buffer, 0x0, 100);

    /* Execute Test */
    TM_SYNC_LibInit();
    TM_SYNC_Synchronize(buffer, TM_SYNC_ASM_STR, TM_SYNC_ASM_SIZE, 96, FALSE);

    /* Verify Outputs */
    UtAssert_True(buffer[0] == expAsm[0], "ASM Header byte 0");
    UtAssert_True(buffer[1] == expAsm[1], "ASM Header byte 1");
    UtAssert_True(buffer[2] == expAsm[2], "ASM Header byte 2");
    UtAssert_True(buffer[3] == expAsm[3], "ASM Header byte 3");
    UtAssert_True(buffer[5] == 0x00, "No pseudo-Randomize check");
}

/* ------------------- End of test cases --------------------------------------*/


/*
 * TM_SYNC_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void TM_SYNC_Setup(void)
{

}

/*
 * TM_SYNC_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void TM_SYNC_TearDown(void)
{

}


/*
 * TM_SYNC_Setup_ProcessFrame
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test of TM_SYNC_ProcessFrame
 *
 * Notes:
 */
void TM_SYNC_Setup_ProcessFrame(void)
{
    TM_SYNC_Setup();
}




#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* TM_SYNC_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void TM_SYNC_AddTestCase(void)
{
    /* TM_SYNC_PseudoRandomize */
    ADD_TEST(Test_TM_SYNC_PseudoRandomize,       TM_SYNC_Setup, TM_SYNC_TearDown);
    ADD_TEST(Test_TM_SYNC_Synchronize_NullPtr,   TM_SYNC_Setup, TM_SYNC_TearDown);
    ADD_TEST(Test_TM_SYNC_Synchronize_BadAsm,    TM_SYNC_Setup, TM_SYNC_TearDown);
    ADD_TEST(Test_TM_SYNC_Synchronize,           TM_SYNC_Setup, TM_SYNC_TearDown);
    ADD_TEST(Test_TM_SYNC_Synchronize_NoRandom,  TM_SYNC_Setup, TM_SYNC_TearDown);
}
