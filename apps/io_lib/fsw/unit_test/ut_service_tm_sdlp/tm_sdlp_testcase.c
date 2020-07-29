/*
 * Filename: tm_sync_testcase.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains a unit test cases for the TM_SDLP Service
 *
 * Modification History:
 *   11/02/2015 Guy de Carufel, Odyssey Space Research, LLC
 *    * Created
 *
 */

/* This test intensionally exercices the tmtf library (not stubbed) */ 

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

#include "tm_sdlp.h"
#include "io_lib_utils.h"

/* Allocate memory */
uint8 idleBuff[100];
uint8 ocfBuff[4];
uint8 tframe[100];
uint8 overflowBuff[200];
uint8 idlePattern[32];
uint8 dataPacket[200];

TM_SDLP_FrameInfo_t frameInfo;
TM_SDLP_GlobalConfig_t globConfig;
TM_SDLP_ChannelConfig_t chnlConfig;
CFE_SB_Msg_t *idlePacket = (CFE_SB_Msg_t *) idleBuff;
uint8 mcFrameCnt;


void Test_Reset(void)
{
    memset(&frameInfo, 0x0, sizeof(TM_SDLP_FrameInfo_t));
    memset(&globConfig, 0x0, sizeof(TM_SDLP_GlobalConfig_t));
    memset(&chnlConfig, 0x0, sizeof(TM_SDLP_ChannelConfig_t));

    mcFrameCnt = 0;
}



/* ---------------------  Begin test cases  --------------------------------- */


/*******************************************************************************
**
**  TM_SDLP_InitIdlePacket 
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TM_SDLP_InitIdlePacket_Error(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual   = 99;

    /* Execute test */
    actual = TM_SDLP_InitIdlePacket(NULL, NULL, 100, 10);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Execute test */
    actual = TM_SDLP_InitIdlePacket((CFE_SB_Msg_t *) idleBuff, NULL, 100, 255);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Execute test */
    actual = TM_SDLP_InitIdlePacket((CFE_SB_Msg_t *) idleBuff, idlePattern, 100, 0);
    
    /* Verify results */
    expected = TM_SDLP_INVALID_LENGTH;
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TM_SDLP_InitIdlePacket(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    // For this example, use tm_sync pseudo-random pattern as the idle data
    // pattern.
    IO_LIB_UTIL_GenPseudoRandomSeq(idlePattern, 0xa9, 0xff); 

    /* Execute test */
    actual = TM_SDLP_InitIdlePacket(idlePacket, idlePattern, 100, 255);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    /* Expected pattern based on tm_sync pseudo-random pattern. */
    UtAssert_True(idleBuff[8]  == 0x0e, "IdlePacket data byte 3 == expected");
    UtAssert_True(idleBuff[70] == 0xfd, "IdlePacket data byte 64 == expected");
}


/*******************************************************************************
**
**  TM_SDLP_InitChannel
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TM_SDLP_InitChannel_NullPtr(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual   = 99;

    /* Execute test */
    actual = TM_SDLP_InitChannel(NULL, NULL, NULL, NULL, NULL);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Execute test */
    actual = TM_SDLP_InitChannel(NULL, tframe, overflowBuff, &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Execute test */
    actual = TM_SDLP_InitChannel(&frameInfo, NULL, overflowBuff, &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Execute test */
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, NULL, &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Execute test */
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, NULL, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Execute test */
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, &globConfig, NULL);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_InitChannel_SecHdr_BadLen(void)
{
    int32 expected = TM_SDLP_INVALID_LENGTH;
    int32 actual;
    
    /* Sub-test 1 */
    chnlConfig.fshFlag = 0;
    chnlConfig.secHdrLength = 1;

    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Sub-test 2 */
    chnlConfig.fshFlag = 1;
    chnlConfig.secHdrLength = 0;

    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Sub-test 3 */
    chnlConfig.fshFlag = 1;
    chnlConfig.secHdrLength = 64;

    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_InitChannel_Frame_BadLen(void)
{
    int32 expected = TM_SDLP_INVALID_LENGTH;
    int32 actual;
    
    /* Sub-test 1 */
    globConfig.frameLength = 5;
    chnlConfig.fshFlag = 0;
    chnlConfig.secHdrLength = 0;

    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");

    /* Sub-test 2 */
    globConfig.frameLength = 6;
    chnlConfig.fshFlag = 1;
    chnlConfig.secHdrLength = 1;

    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Sub-test 3 */
    globConfig.frameLength = 9;
    chnlConfig.fshFlag = 0;
    chnlConfig.secHdrLength = 0;
    chnlConfig.ocfFlag = 1;
    
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Sub-test 4 */
    globConfig.frameLength = 7;
    globConfig.hasErrCtrl = 1;
    chnlConfig.fshFlag = 0;
    chnlConfig.secHdrLength = 0;
    chnlConfig.ocfFlag = 0;
    
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Sub-test 5 */
    globConfig.frameLength = 11;
    globConfig.hasErrCtrl = 1;
    chnlConfig.fshFlag = 0;
    chnlConfig.secHdrLength = 0;
    chnlConfig.ocfFlag = 1;
    
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
}



void Test_TM_SDLP_InitChannel(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;
    
    /* Sub-test 1 */
    globConfig.scId = 1;
    globConfig.frameLength = 100;
    
    chnlConfig.vcId = 2;
    chnlConfig.dataType = 1;
    chnlConfig.fshFlag = 0;
    chnlConfig.ocfFlag = 0;
    chnlConfig.secHdrLength = 0;
    chnlConfig.isMaster = 0;
    chnlConfig.overflowSize = 100;

    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(TMTF_GetMcId(frameInfo.frame) == 0x01, "McId == expected");
    UtAssert_True(TMTF_GetGlobalVcId(frameInfo.frame) == 0x14 , "GVcId == expected");
    UtAssert_True((frameInfo.frame->DataFieldStatus[0] & 0xf8) == 0x40, "DataFieldStatus == expected");
    UtAssert_True(frameInfo.freeOctets == 94, "freeOctets == expected");
    UtAssert_True(frameInfo.dataFieldOffset == 6, "dataFieldOffset == expected");
    UtAssert_True(frameInfo.ocfOffset == 100, "ocfOffset == expected");
    UtAssert_True(frameInfo.errCtrlOffset == 100, "ocfOffset == expected");

    /* Sub-test 2 */
    globConfig.hasErrCtrl = 1;
    chnlConfig.dataType = 0;
    chnlConfig.fshFlag = 1;
    chnlConfig.ocfFlag = 1;
    chnlConfig.secHdrLength = 19;

    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True((frameInfo.frame->DataFieldStatus[0] & 0xf8) == 0x98, "DataFieldStatus == expected");
    UtAssert_True(frameInfo.freeOctets == 68, "freeOctets == expected");
    UtAssert_True(frameInfo.dataFieldOffset == 26, "dataFieldOffset == expected");
    UtAssert_True(frameInfo.ocfOffset == 94, "ocfOffset == expected");
    UtAssert_True(frameInfo.errCtrlOffset == 98, "ocfOffset == expected");

    /* Sub-test 3 */
    chnlConfig.ocfFlag = 0;

    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    UtAssert_True(frameInfo.ocfOffset == 98, "ocfOffset == expected");
    UtAssert_True(frameInfo.errCtrlOffset == 98, "ocfOffset == expected");
}


/*******************************************************************************
**
**  TM_SDLP_FrameHasData
**
*******************************************************************************/
void Test_TM_SDLP_FrameHasData_NullPtr(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual   = 99;

    actual = TM_SDLP_FrameHasData(NULL);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_FrameHasData(void)
{
    int32 expected = 0;
    int32 actual   = 99;
    
    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 200;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    actual = TM_SDLP_FrameHasData(&frameInfo);
    UtAssert_True(actual == expected, "return value == expected");
    
    /* Force value of free octets to 0 */
    frameInfo.freeOctets = 0;
    expected = 1;
    actual = TM_SDLP_FrameHasData(&frameInfo);
    UtAssert_True(actual == expected, "return value == expected");
}


/*******************************************************************************
**
**  TM_SDLP_AddPacket
**
*******************************************************************************/
void Test_TM_SDLP_AddPacket_NullPtr(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual   = 99;

    actual = TM_SDLP_AddPacket(NULL, NULL);
    UtAssert_True(actual == expected, "return value == expected");

    actual = TM_SDLP_AddPacket(&frameInfo, NULL);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_AddPacket_NotInit(void)
{
    int32 expected = TM_SDLP_FRAME_NOT_INIT;
    int32 actual   = 99;

    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 55, TRUE);

    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_AddPacket_NotReady(void)
{
    int32 expected = TM_SDLP_FRAME_NOT_READY;
    int32 actual   = 99;

    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 200;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 55, TRUE);

    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_AddPacket(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual   = 99;
    
    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 200;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    /* Force as ready. Normally done in StartFrame */
    frameInfo.isReady = TRUE;
    UtAssert_True(actual == expected, "properly initialized");

    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 55, TRUE);

    expected = 94 - 55;
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    UtAssert_True(actual == expected, "One Packet Added");

    expected = 0;
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    UtAssert_True(actual == expected, "Packet to overflow");
    UtAssert_True(frameInfo.overflowInfo.partialOctets == 55 - 39, "Overflow partialOctets check");
    
    expected = TM_SDLP_OVERFLOW_FULL;
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    UtAssert_True(actual == expected, "Overflow Full");
}


/*******************************************************************************
**
**  TM_SDLP_AddIdlePacket
**
*******************************************************************************/
void Test_TM_SDLP_AddIdlePacket_NullPtr(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual   = 99;

    actual = TM_SDLP_AddIdlePacket(NULL, NULL);
    UtAssert_True(actual == expected, "return value == expected");

    actual = TM_SDLP_AddIdlePacket(&frameInfo, NULL);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_AddIdlePacket_NotInit(void)
{
    int32 expected = TM_SDLP_FRAME_NOT_INIT;
    int32 actual   = 99;

    actual = TM_SDLP_AddIdlePacket(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_AddIdlePacket_NotReady(void)
{
    int32 expected = TM_SDLP_FRAME_NOT_READY;
    int32 actual   = 99;

    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 10;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    actual = TM_SDLP_AddIdlePacket(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_AddIdlePacket_BadMid(void)
{
    int32 expected = TM_SDLP_ERROR;
    int32 actual   = 99;

    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 10;
    TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                        &globConfig, &chnlConfig);
    frameInfo.isReady = TRUE;
    
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0901, 55, TRUE);

    actual = TM_SDLP_AddIdlePacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_AddIdlePacket_Overflow(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;
    
    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 10;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    /* Force as ready. Normally done in StartFrame */
    frameInfo.isReady = TRUE;

    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 90, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    expected = 4;
    UtAssert_True(actual == expected, "properly initialized");
    
    expected = 0;
    actual = TM_SDLP_AddIdlePacket(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "Idle Packet Added with overflow");
    UtAssert_True(frameInfo.overflowInfo.freeOctets == (10-3), "Overflow freeOctets check");
    UtAssert_True(frameInfo.overflowInfo.partialOctets == 3, "Overflow partialOctets check");
    
    expected = 0;
    actual = TM_SDLP_AddIdlePacket(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "Idle Packet Added twice - ignored.");
    UtAssert_True(frameInfo.overflowInfo.freeOctets == (10-3), "Overflow freeOctets check");
    UtAssert_True(frameInfo.overflowInfo.partialOctets == 3, "Overflow freeOctets check");
}

void Test_TM_SDLP_AddIdlePacket(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;
    
    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 10;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    /* Force as ready. Normally done in StartFrame */
    frameInfo.isReady = TRUE;
    UtAssert_True(actual == expected, "properly initialized");
    
    expected = 0;
    actual = TM_SDLP_AddIdlePacket(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "Idle Packet Added without overflow");
    UtAssert_True(frameInfo.overflowInfo.freeOctets == 10, "Overflow freeOctets check");
    UtAssert_True(frameInfo.overflowInfo.partialOctets == 0, "Overflow partialOctets check");
    
    expected = 0;
    actual = TM_SDLP_AddIdlePacket(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "Idle Packet Added twice - no effect.");
    UtAssert_True(frameInfo.overflowInfo.freeOctets == 10, "Overflow freeOctets check");
}


/*******************************************************************************
**
**  TM_SDLP_AddVcaData
**
*******************************************************************************/
void Test_TM_SDLP_AddVcaData_NullPtr(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual   = 99;

    actual = TM_SDLP_AddVcaData(NULL, NULL, 0);
    UtAssert_True(actual == expected, "return value == expected");

    actual = TM_SDLP_AddVcaData(&frameInfo, NULL, 0);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_AddVcaData_NotInit(void)
{
    int32 expected = TM_SDLP_FRAME_NOT_INIT;
    int32 actual   = 99;

    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 55, TRUE);

    actual = TM_SDLP_AddVcaData(&frameInfo, (uint8 *) dataPacket, 55);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_AddVcaData_TooLarge(void)
{
    int32 expected = TM_SDLP_INVALID_LENGTH;
    int32 actual;
    
    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 10;
    chnlConfig.dataType = 1; // VCA Data type
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    /* Force as ready. Normally done in StartFrame */
    frameInfo.isReady = TRUE;
    
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 200, TRUE);
    actual = TM_SDLP_AddVcaData(&frameInfo, (uint8 *) dataPacket, 200);
    UtAssert_True(actual == expected, "Data too large");
}


void Test_TM_SDLP_AddVcaData(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;
    
    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 10;
    chnlConfig.dataType = 1; // VCA Data type
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    /* Force as ready. Normally done in StartFrame */
    frameInfo.isReady = TRUE;
    UtAssert_True(actual == expected, "properly initialized");
    
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 55, TRUE);
    
    expected = 94 - 55;
    actual = TM_SDLP_AddVcaData(&frameInfo, (uint8 *) dataPacket, 55);
    UtAssert_True(actual == expected, "55-byte Data added");
    UtAssert_True(frameInfo.isFirstHdrPtrSet == FALSE, "First hdr pointer not set check");
}



/*******************************************************************************
**
**  TM_SDLP_StartFrame
**
*******************************************************************************/
void Test_TM_SDLP_StartFrame_NullPtr(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual;

    actual = TM_SDLP_StartFrame(NULL);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_StartFrame_NotInit(void)
{
    int32 expected = TM_SDLP_FRAME_NOT_INIT;
    int32 actual;

    actual = TM_SDLP_StartFrame(&frameInfo);
    UtAssert_True(actual == expected, "return value == expected");
}

void Test_TM_SDLP_StartFrame_EmptyOverflow(void)
{
    int32 expected;
    int32 actual;

    globConfig.frameLength = 20;
    chnlConfig.overflowSize = 20;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    expected = 0;
    actual = TM_SDLP_StartFrame(&frameInfo);
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(frameInfo.isReady == TRUE, "IsReady is TRUE.");
    UtAssert_True(frameInfo.freeOctets == 14, "Is empty check.");

    expected = TM_SDLP_SUCCESS;
    actual = TM_SDLP_StartFrame(&frameInfo);
    UtAssert_True(actual == expected, "re-entrant check");
    UtAssert_True(frameInfo.freeOctets == 14, "Is empty check.");
}


void Test_TM_SDLP_StartFrame_WithOverflow(void)
{
    int32 expected;
    int32 actual;

    globConfig.frameLength = 26;
    chnlConfig.overflowSize = 60;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    expected = 20; 
    actual = TM_SDLP_StartFrame(&frameInfo);
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(frameInfo.isReady == TRUE, "IsReady is TRUE.");
    
    

    expected = TM_SDLP_ERROR;
    actual = TM_SDLP_StartFrame(&frameInfo);
    UtAssert_True(actual == expected, "re-entrant error check");
}



/*******************************************************************************
**
**  TM_SDLP_SetOidFrame
**
*******************************************************************************/
void Test_TM_SDLP_SetOidFrame_NullPtr(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual;
    
    actual = TM_SDLP_SetOidFrame(NULL, NULL);
    UtAssert_True(actual == expected, "return value == expected");
    
    actual = TM_SDLP_SetOidFrame(&frameInfo, NULL);
    UtAssert_True(actual == expected, "return value == expected");
    
    actual = TM_SDLP_SetOidFrame(NULL, idlePacket);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_SetOidFrame_NotInit(void)
{
    int32 expected = TM_SDLP_FRAME_NOT_INIT;
    int32 actual;
    
    actual = TM_SDLP_SetOidFrame(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_SetOidFrame_NotEmpty(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;
    
    globConfig.frameLength = 20;
    chnlConfig.overflowSize = 20;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    TM_SDLP_StartFrame(&frameInfo); 

    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 10, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    UtAssert_True(actual == 4, "Packet Added");
    
    expected = TM_SDLP_ERROR;
    actual = TM_SDLP_SetOidFrame(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_SetOidFrame(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;
    
    globConfig.frameLength = 20;
    chnlConfig.overflowSize = 20;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    TM_SDLP_StartFrame(&frameInfo); 

    expected = TM_SDLP_SUCCESS;
    actual = TM_SDLP_SetOidFrame(&frameInfo, idlePacket);
    UtAssert_True(actual == expected, "return value == expected");
}

/*******************************************************************************
**
**  TM_SDLP_CompleteFrame
**
*******************************************************************************/
void Test_TM_SDLP_CompleteFrame_NullPtr(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual;
    
    actual = TM_SDLP_CompleteFrame(NULL, NULL, NULL);
    UtAssert_True(actual == expected, "return value == expected");
    
    actual = TM_SDLP_CompleteFrame(&frameInfo, NULL, NULL);
    UtAssert_True(actual == expected, "return value == expected");
    
    actual = TM_SDLP_CompleteFrame(NULL, &mcFrameCnt, NULL);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_CompleteFrame_NotInit(void)
{
    int32 expected = TM_SDLP_FRAME_NOT_INIT;
    int32 actual;

    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_CompleteFrame_NullOcf(void)
{
    int32 expected = TM_SDLP_INVALID_POINTER;
    int32 actual;

    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 200;
    chnlConfig.ocfFlag = TRUE;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_CompleteFrame_WithOcf(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;
    uint8 ocfData[4];

    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 200;
    chnlConfig.ocfFlag = TRUE;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, &ocfData);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_CompleteFrame_WithErrCtrl(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;

    globConfig.frameLength = 100;
    globConfig.hasErrCtrl = TRUE;
    chnlConfig.overflowSize = 200;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    UtAssert_True(actual == expected, "return value == expected");
}


void Test_TM_SDLP_CompleteFrame(void)
{
    int32 expected = TM_SDLP_SUCCESS;
    int32 actual;

    globConfig.frameLength = 100;
    chnlConfig.overflowSize = 20;
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(frameInfo.freeOctets == 94 && frameInfo.isReady == FALSE, 
                  "Frame Meta reset.");
}



/*******************************************************************************
**
**  TM_SDLP_NominalTest
**
*******************************************************************************/
void Test_TM_SDLP_NominalTest(void)
{
    int32 expected;
    int32 actual;

    globConfig.frameLength = 36;
    chnlConfig.overflowSize = 80;
    /* Initialize */
    actual = TM_SDLP_InitChannel(&frameInfo, tframe, overflowBuff, 
                                 &globConfig, &chnlConfig);
    
    /* Start Frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Start frame 1");
    UtAssert_True(frameInfo.isReady == TRUE, "IsReady is TRUE.");
    expected = 30; 
    UtAssert_True(frameInfo.freeOctets == expected, "FreeOctets check.");
    

    /* Add small packet */
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 10, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    expected = 20; 
    UtAssert_True(actual == expected, "Add small packet");

    /* Add Idle packet */
    actual = TM_SDLP_AddIdlePacket(&frameInfo, idlePacket);
    expected = 0; 
    UtAssert_True(actual == expected, "Add IdlePacket");
    expected = 80;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");

    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 1");

    /* Start new frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 30;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 2");

    /* Add large packet (10 bytes overflows) */
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 40, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    expected = 0; 
    UtAssert_True(actual == expected, "Add large packet");
    expected = 70;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");

    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 2");

    /* Start new frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 20;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 3");
    expected = 80;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Empty Overflow Buffer check.");

    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 3");

    /* Start new frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 30;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 4");

    /* Set OID Frame */
    actual = TM_SDLP_SetOidFrame(&frameInfo, idlePacket);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Set OID Frame Successful.");
    expected = 80;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Empty Overflow Buffer check.");

    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 4");

    /* Start Frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 30;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 5");
    UtAssert_True(frameInfo.isReady == TRUE, "IsReady is TRUE.");

    /* Add small packet */
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 26, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    expected = 4; 
    UtAssert_True(actual == expected, "Add packet");

    /* Add Idle packet */
    actual = TM_SDLP_AddIdlePacket(&frameInfo, idlePacket);
    expected = 0; 
    UtAssert_True(actual == expected, "Add overflowing IdlePacket");
    expected = 77;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
    expected = 3;
    UtAssert_True(frameInfo.overflowInfo.partialOctets == expected, 
                  "Overflow Buffer data size check.");

    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 5");

    /* Start Frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 27;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 6");
    expected = 80;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Empty Overflow Buffer check.");

    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 6");

    /* Start Frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 30;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 7");

    /* Add very large packet (50 bytes overflows) */
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 80, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    expected = 0; 
    UtAssert_True(actual == expected, "Add large packet");
    expected = 30;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
    
    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 7");

    /* Start Frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 0;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 8");
    expected = 60;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");

    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 8");
    
    /* Start Frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 10;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 9");
    expected = 80;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
    
    /* Add very large packet */
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 90, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    expected = 0; 
    UtAssert_True(actual == expected, "Add large packet");
    expected = 0;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer Full check.");
    
    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 9");
    
    /* Start Frame */
    actual = TM_SDLP_StartFrame(&frameInfo);
    expected = 0;
    UtAssert_True(frameInfo.freeOctets == expected, "Start frame 10");
    expected = 30;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
    
    /* Add small packet in overflow */
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 10, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    expected = 0; 
    UtAssert_True(actual == expected, "Add packet");
    expected = 20;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");

    /* Complete Frame */
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = TM_SDLP_SUCCESS;
    UtAssert_True(actual == expected, "Complete Frame 9");
    
    /* Cycle 2 times */
    actual = TM_SDLP_StartFrame(&frameInfo);
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = 50;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
    
    actual = TM_SDLP_StartFrame(&frameInfo);
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = 80;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");

    actual = TM_SDLP_StartFrame(&frameInfo);
    
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 28, TRUE);
    actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    
    CFE_SB_InitMsg((CFE_SB_Msg_t *) dataPacket, 0x0801, 6, TRUE);
    uint8 ii = 0;
    for (ii = 0; ii < 13; ++ii)
    {
        actual = TM_SDLP_AddPacket(&frameInfo, (CFE_SB_Msg_t *) dataPacket);
    }           
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = 4;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
    
    actual = TM_SDLP_StartFrame(&frameInfo);
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = 34;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
    
    actual = TM_SDLP_StartFrame(&frameInfo);
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = 64;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
    
    actual = TM_SDLP_StartFrame(&frameInfo);
    actual = TM_SDLP_CompleteFrame(&frameInfo, &mcFrameCnt, NULL);
    expected = 80;
    UtAssert_True(frameInfo.overflowInfo.freeOctets == expected, 
                  "Overflow Buffer data size check.");
}
/* ------------------- End of test cases --------------------------------------*/


/*
 * TM_SDLP_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void TM_SDLP_Setup(void)
{
    Test_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_EVS_Reset();
}

/*
 * TM_SDLP_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void TM_SDLP_TearDown(void)
{

}


/*
 * TM_SDLP_Setup_ProcessFrame
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test of TM_SDLP_ProcessFrame
 *
 * Notes:
 */
void TM_SDLP_Setup_ProcessFrame(void)
{
    TM_SDLP_Setup();
}




#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* TM_SDLP_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void TM_SDLP_AddTestCase(void)
{
    /* TM_SDLP_InitIdlePacket */
    ADD_TEST(Test_TM_SDLP_InitIdlePacket_Error,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_InitIdlePacket,  TM_SDLP_Setup, TM_SDLP_TearDown);
    
    /* TM_SDLP_InitChannel */
    ADD_TEST(Test_TM_SDLP_InitChannel_NullPtr,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_InitChannel_SecHdr_BadLen,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_InitChannel_Frame_BadLen,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_InitChannel,  TM_SDLP_Setup, TM_SDLP_TearDown);

    /* TM_SDLP_FrameHasData */
    ADD_TEST(Test_TM_SDLP_FrameHasData_NullPtr,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_FrameHasData,  TM_SDLP_Setup, TM_SDLP_TearDown);

    /* TM_SDLP_AddPacket */
    ADD_TEST(Test_TM_SDLP_AddPacket_NullPtr,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddPacket_NotInit,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddPacket_NotReady, TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddPacket,  TM_SDLP_Setup, TM_SDLP_TearDown);
    
    /* TM_SDLP_AddIdlePacket */
    ADD_TEST(Test_TM_SDLP_AddIdlePacket_NullPtr,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddIdlePacket_NotInit,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddIdlePacket_NotReady,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddIdlePacket_BadMid,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddIdlePacket_Overflow,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddIdlePacket,  TM_SDLP_Setup, TM_SDLP_TearDown);

    /* TM_SDLP_AddVcaData */
    ADD_TEST(Test_TM_SDLP_AddVcaData_NullPtr,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddVcaData_NotInit,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddVcaData_TooLarge,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_AddVcaData,  TM_SDLP_Setup, TM_SDLP_TearDown);

    /* TM_SDLP_StartFrame */
    ADD_TEST(Test_TM_SDLP_StartFrame_NullPtr,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_StartFrame_NotInit,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_StartFrame_EmptyOverflow,  TM_SDLP_Setup, TM_SDLP_TearDown);
    
    /* TM_SDLP_SetOidFrame */
    ADD_TEST(Test_TM_SDLP_SetOidFrame_NullPtr,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_SetOidFrame_NotInit,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_SetOidFrame_NotEmpty,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_SetOidFrame,  TM_SDLP_Setup, TM_SDLP_TearDown);
    
    /* TM_SDLP_CompleteFrame */
    ADD_TEST(Test_TM_SDLP_CompleteFrame_NullPtr,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_CompleteFrame_NotInit,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_CompleteFrame_NullOcf,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_CompleteFrame_WithOcf,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_CompleteFrame_WithErrCtrl,  TM_SDLP_Setup, TM_SDLP_TearDown);
    ADD_TEST(Test_TM_SDLP_CompleteFrame,  TM_SDLP_Setup, TM_SDLP_TearDown);

    /* TM_SDLP_NominalTest */
    ADD_TEST(Test_TM_SDLP_NominalTest,  TM_SDLP_Setup, TM_SDLP_TearDown);
}
