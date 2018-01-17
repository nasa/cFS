/*
 * Filename: tctf_testcase.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains a unit test cases for transfer frames
 *
 * Modification History:
 *   04/02/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "tctf_stubs.h"
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

#include "tctf.h"


#define TCTF_MAX_LENGTH   1024


/* Prototypes for non-exported functions */


/* -------------------- Special Test Case Variables ------------------------- */
static uint8 testToBuf[TCTF_MAX_LENGTH];
static uint8 testFromBuf[TCTF_MAX_LENGTH];
static TCTF_ChannelService_t testChannelService;


TCTF_Hdr_t testHdr =
{
    {0x00,   /* tf vers no.=0, bypass flag=0, ctl cmd flag=0 */
     0x01,   /* spacecraft id=1 */
     0x04,   /* vc id=1 */
     0x04    /* length=4 (1 less than actual length) */
    },
     0x01    /* seq. no.=1 */
             /* since length = 5, assume no segment header which would go here */
};

static TCTF_ChannelService_t initChannelService =
{
    TCTF_SERVICE_MCF,  /* service */
    0,                 /* packet version number */
    1,                 /* spacecraft id */
    0,                 /* virtual channel id */
    0,                 /* map id */
    0,                 /* has seg hdr */
    0                  /* has frame error control */
};


/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  TCTF_GetVersion Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetVersion_Zero(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;

    /* Execute Test */
    actual = TCTF_GetVersion(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF version no. == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetVersion_Max(void)
{
    uint16 expected = 3;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0xFF;

    /* Execute Test */
    actual = TCTF_GetVersion(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF version no. == max");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetVersion_Null(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetVersion(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Null TF version no. == 0");
}


/*******************************************************************************
**
**  TCTF_GetBypassFlag Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetBypassFlag_False(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;

    /* Execute Test */
    actual = TCTF_GetBypassFlag(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF bypass flag == false");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetBypassFlag_True(void)
{
    uint16 expected = 1;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0x20;

    /* Execute Test */
    actual = TCTF_GetBypassFlag(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF bypass flag == true");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetBypassFlag_Null(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetBypassFlag(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Null TF bypass flag == 0");
}



/*******************************************************************************
**
**  TCTF_GetCtlCmdFlag Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetCtlCmdFlag_False(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;

    /* Execute Test */
    actual = TCTF_GetCtlCmdFlag(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF control command flag == false");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetCtlCmdFlag_True(void)
{
    uint16 expected = 1;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0x10;

    /* Execute Test */
    actual = TCTF_GetCtlCmdFlag(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF control command flag == true");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetCtlCmdFlag_Null(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetCtlCmdFlag(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Nul TF control command flag == 0");
}


/*******************************************************************************
**
**  TCTF_GetScId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetScId_Zero(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[1] = 0x00;

    /* Execute Test */
    actual = TCTF_GetScId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF spacecraft id == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetScId_Max(void)
{
    uint16 expected = 1023;  /* 2^10 - 1 */
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0xFF;
    testMsg.Octet[1] = 0xFF;

    /* Execute Test */
    actual = TCTF_GetScId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF spacecraft id == max");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetScId_Misc(void)
{
    uint16 expected = 513;
    uint16 actual   = 0;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0x02;
    testMsg.Octet[1] = 0x01;

    /* Execute Test */
    actual = TCTF_GetScId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF spacecraft id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetScId_Null(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetScId(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Null TF spacecraft id == 0");
}


/*******************************************************************************
**
**  TCTF_GetVcId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetVcId_Zero(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0x00;

    /* Execute Test */
    actual = TCTF_GetVcId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF virtual channel id == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetVcId_Max(void)
{
    uint16 expected = 63;   /* 2^6 -1 */
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0xFF;

    /* Execute Test */
    actual = TCTF_GetVcId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF virtual channel id == max");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetVcId_Misc(void)
{
    uint16 expected = 33;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0x84;

    /* Execute Test */
    actual = TCTF_GetVcId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF virtual channel id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetVcId_Null(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetVcId(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Null TF virtual channel id == 0");
}


/*******************************************************************************
**
**  TCTF_GetLength Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetLength_Min(void)
{
    uint16 expected = 5;   /* min length=header size */
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;

    /* Execute Test */
    actual = TCTF_GetLength(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF length == min");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetLength_Max(void)
{
    uint16 expected = 1024;   /* 2^10 */
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0xFF;
    testMsg.Octet[3] = 0xFF;

    /* Execute Test */
    actual = TCTF_GetLength(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF length == max");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetLength_Misc(void)
{
    uint16 expected = 514;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0x02;
    testMsg.Octet[3] = 0x01;

    /* Execute Test */
    actual = TCTF_GetLength(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF length == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetLength_Null(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetLength(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Null TF length == 0");
}


/*******************************************************************************
**
**  TCTF_GetSeqNum Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSeqNum_Zero(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Sequence = 0x00;

    /* Execute Test */
    actual = TCTF_GetSeqNum(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seq. no. == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSeqNum_Max(void)
{
    uint16 expected = 255;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Sequence = 0xFF;

    /* Execute Test */
    actual = TCTF_GetSeqNum(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seq. no. == max");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSeqNum_Misc(void)
{
    uint16 expected = 129;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Sequence = 0x81;

    /* Execute Test */
    actual = TCTF_GetSeqNum(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seq. no. == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSeqNum_Null(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetSeqNum(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Null TF seq. no. == 0");
}


/*******************************************************************************
**
**  TCTF_GetSegHdrSeqFlags Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrSeqFlags_Zero(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.SegHdr = 0x00;

    /* Execute Test */
    actual = TCTF_GetSegHdrSeqFlags(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seg hdr seq flags == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrSeqFlags_Max(void)
{
    uint16 expected = 3;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.SegHdr = 0xFF;

    /* Execute Test */
    actual = TCTF_GetSegHdrSeqFlags(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seg hdr seq flags == max");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrSeqFlags_One(void)
{
    uint16 expected = 1;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.SegHdr = 0x7F;

    /* Execute Test */
    actual = TCTF_GetSegHdrSeqFlags(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seg hdr seq flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrSeqFlags_Null(void)
{
    uint16 expected = TCTF_NO_SEGHDR;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetSegHdrSeqFlags(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Null TF seg hdr seq flags == TCTF_NO_SEGHDR");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrSeqFlags_CmdFrm(void)
{
    uint16 expected = TCTF_NO_SEGHDR;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0x10;      /* set the control command flag */

    /* Execute Test */
    actual = TCTF_GetSegHdrSeqFlags(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Command TF seg hdr seq flags == TCTF_NO_SEGHDR");
}


/*******************************************************************************
**
**  TCTF_GetSegHdrMapId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrMapId_Zero(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.SegHdr = 0x00;

    /* Execute Test */
    actual = TCTF_GetSegHdrMapId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seg hdr map id == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrMapId_Max(void)
{
    uint16 expected = 63;  /* 2^6 - 1 */
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.SegHdr = 0xFF;

    /* Execute Test */
    actual = TCTF_GetSegHdrMapId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seg hdr map id == max");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrMapId_Misc(void)
{
    uint16 expected = 33;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.SegHdr = 0x21;

    /* Execute Test */
    actual = TCTF_GetSegHdrMapId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF seg hdr map id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrMapId_Null(void)
{
    uint16 expected = TCTF_NO_SEGHDR;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;

    /* Execute Test */
    actual = TCTF_GetSegHdrMapId(testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Null TF seg hdr map id == TCTF_NO_SEGHDR");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetSegHdrMapId_CmdFrm(void)
{
    uint16 expected = TCTF_NO_SEGHDR;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0x10;      /* set the control command flag */

    /* Execute Test */
    actual = TCTF_GetSegHdrMapId(&testMsg);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Command TF seg hdr map id == TCTF_NO_SEGHDR");
}


/*******************************************************************************
**
**  TCTF_GetPayloadLength Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_GetPayloadLength_Zero(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;

    /* Execute Test */
    actual = TCTF_GetPayloadLength(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF payload length == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetPayloadLength_Max(void)
{
    uint16 expected = 1024 - TCTF_PRIHDR_SIZE;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0x03;
    testMsg.Octet[3] = 0xFF;
    TCTF_ChannelService_t testService = testChannelService;

    /* Execute Test */
    actual = TCTF_GetPayloadLength(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF payload length == max");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetPayloadLength_woSegHdr(void)
{
    uint16 expected = 514 - TCTF_PRIHDR_SIZE;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0x02;
    testMsg.Octet[3] = 0x01;
    TCTF_ChannelService_t testService = testChannelService;

    /* Execute Test */
    actual = TCTF_GetPayloadLength(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF payload length without seg. hdr == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetPayloadLength_wSegHdr(void)
{
    uint16 expected = 514 - TCTF_PRIHDR_SIZE - TCTF_SEGHDR_SIZE;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0x02;
    testMsg.Octet[3] = 0x01;
    TCTF_ChannelService_t testService = testChannelService;
    testService.HasSegHdr = TRUE;

    /* Execute Test */
    actual = TCTF_GetPayloadLength(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF payload length with seg. hdr == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetPayloadLength_wFrmErrCtl(void)
{
    uint16 expected = 514 - TCTF_PRIHDR_SIZE - TCTF_FRAME_ERROR_CONTROL_SIZE;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[2] = 0x02;
    testMsg.Octet[3] = 0x01;
    TCTF_ChannelService_t testService = testChannelService;
    testService.HasFrameErrCtl = TRUE;

    /* Execute Test */
    actual = TCTF_GetPayloadLength(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "TF payload length with frm err ctl == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetPayloadLength_NullTfPtr(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = NULL;
    TCTF_ChannelService_t testService = testChannelService;

    /* Execute Test */
    actual = TCTF_GetPayloadLength(testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Length for null TF pointer == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_GetPayloadLength_NullCsPtr(void)
{
    uint16 expected = 0;
    uint16 actual   = 99;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t *testService = NULL;

    /* Execute Test */
    actual = TCTF_GetPayloadLength(&testMsg, testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Length for null Channel Service == 0");
}


/*******************************************************************************
**
**  TCTF_CopyData Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_CopyData_MaxData(void)
{
    uint16 expectedRetVal = TCTF_MAX_LENGTH - TCTF_PRIHDR_SIZE;
    uint16 actualRetVal   = 99;
    int i = 0;

    /* Setup Inputs */
    for (i = TCTF_PRIHDR_SIZE; i < TCTF_MAX_LENGTH; ++i)
    {
        testFromBuf[i] = i;
    }
    memcpy(testFromBuf, (uint8 *)(&testHdr), TCTF_PRIHDR_SIZE);
    testFromBuf[2] = 0x03;  /* set TF size to max */
    testFromBuf[3] = 0xFF;

    /* Execute Test */
    actualRetVal = TCTF_CopyData(testToBuf, (TCTF_Hdr_t *)testFromBuf, &testChannelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_MemCmp(testToBuf, &(testFromBuf[TCTF_PRIHDR_SIZE]),
                    (TCTF_MAX_LENGTH - TCTF_PRIHDR_SIZE), "toBuffer == TF payload data");
    UtAssert_MemCmpValue(&(testToBuf[TCTF_MAX_LENGTH - TCTF_PRIHDR_SIZE]), 0,
                         TCTF_PRIHDR_SIZE, "extra toBuffer == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_CopyData_MaxData_wSegHdr(void)
{
    uint16 expectedRetVal = TCTF_MAX_LENGTH - TCTF_PRIHDR_SIZE - TCTF_SEGHDR_SIZE;
    uint16 actualRetVal   = 99;
    int i = 0;

    /* Setup Inputs */
    for (i = TCTF_PRIHDR_SIZE; i < TCTF_MAX_LENGTH; ++i)
    {
        testFromBuf[i] = i;
    }
    memcpy(testFromBuf, (uint8 *)(&testHdr), TCTF_PRIHDR_SIZE);
    testFromBuf[2] = 0x03;  /* set TF size to max */
    testFromBuf[3] = 0xFF;
    testFromBuf[5] = 0xAA;  /* seg. hdr included for this test, set to unexpected value */

    testChannelService.HasSegHdr = TRUE;

    /* Execute Test */
    actualRetVal = TCTF_CopyData(testToBuf, (TCTF_Hdr_t *)testFromBuf, &testChannelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_MemCmp(testToBuf, &(testFromBuf[TCTF_PRIHDR_SIZE + TCTF_SEGHDR_SIZE]),
                    (TCTF_MAX_LENGTH - TCTF_PRIHDR_SIZE - TCTF_SEGHDR_SIZE),
                    "toBuffer == TF payload data");
    UtAssert_MemCmpValue(&(testToBuf[TCTF_MAX_LENGTH - TCTF_PRIHDR_SIZE - TCTF_SEGHDR_SIZE]), 0,
                         (TCTF_PRIHDR_SIZE + TCTF_SEGHDR_SIZE), "extra toBuffer == 0");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_CopyData_NoData(void)
{
    uint16 expectedRetVal = 0;
    uint16 actualRetVal   = 99;

    /* Setup Inputs */
    memcpy(testFromBuf, (uint8 *)(&testHdr), TCTF_PRIHDR_SIZE);
    testFromBuf[2] = 0x00;  /* set TF size to min */
    testFromBuf[3] = 0x04;

    /* Execute Test */
    actualRetVal = TCTF_CopyData(testToBuf, (TCTF_Hdr_t *)testFromBuf, &testChannelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_MemCmpValue(testToBuf, '\0', TCTF_MAX_LENGTH, "toBuffer == unchanged");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_CopyData_NullTf(void)
{
    uint16 expectedRetVal = 0;
    uint16 actualRetVal   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actualRetVal = TCTF_CopyData(testToBuf, NULL, &testChannelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_CopyData_NullDest(void)
{
    uint16 expectedRetVal = 0;
    uint16 actualRetVal   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actualRetVal = TCTF_CopyData(NULL, (TCTF_Hdr_t *)testFromBuf, &testChannelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_CopyData_NullService(void)
{
    uint16 expectedRetVal = 0;
    uint16 actualRetVal   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actualRetVal = TCTF_CopyData(testToBuf, (TCTF_Hdr_t *)testFromBuf, NULL);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
}


/*******************************************************************************
**
**  TCTF_IsValidTf Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_BadScId(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;
    testService.SpacecraftId = 2;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_BadPvId(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;
    testService.PacketVersionNumber = 1;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_GoodMcfId(void)
{
    boolean expected = TRUE;
    boolean actual   = FALSE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_BadVcpId(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_VCP;
    testService.VirtualChannelId = 2;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_GoodVcpId(void)
{
    boolean expected = TRUE;
    boolean actual   = FALSE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_VCP;
    testService.VirtualChannelId = 1;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_BadVcaId(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_VCA;
    testService.VirtualChannelId = 2;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_GoodVcaId(void)
{
    boolean expected = TRUE;
    boolean actual   = FALSE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_VCA;
    testService.VirtualChannelId = 1;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_BadVcfId(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_VCF;
    testService.VirtualChannelId = 2;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_GoodVcfId(void)
{
    boolean expected = TRUE;
    boolean actual   = FALSE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_VCF;
    testService.VirtualChannelId = 1;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}


/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_BadMappId(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[3] = 0x05;       /* increase size to include segment header */
    testMsg.SegHdr   = 0x01;       /* seq flags=0, map id=1 */
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_MAPP;
    testService.VirtualChannelId = 1;
    testService.MapId = 2;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_GoodMappId(void)
{
    boolean expected = TRUE;
    boolean actual   = FALSE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[3] = 0x05;       /* increase size to include segment header */
    testMsg.SegHdr   = 0x01;       /* seq flags=0, map id=1 */
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_MAPP;
    testService.VirtualChannelId = 1;
    testService.MapId = 1;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_BadMapaId(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[3] = 0x05;       /* increase size to include segment header */
    testMsg.SegHdr   = 0x01;       /* seq flags=0, map id=1 */
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_MAPA;
    testService.VirtualChannelId = 1;
    testService.MapId = 2;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_GoodMapaId(void)
{
    boolean expected = TRUE;
    boolean actual   = FALSE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[3] = 0x05;       /* increase size to include segment header */
    testMsg.SegHdr   = 0x01;       /* seq flags=0, map id=1 */
    TCTF_ChannelService_t testService = testChannelService;
    testService.Service = TCTF_SERVICE_MAPA;
    testService.VirtualChannelId = 1;
    testService.MapId = 1;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_Bypass0_CtlCmd1(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0x10;  /* tfvn=0, bypass flag=0, ctl cmd flag=1 */
    TCTF_ChannelService_t testService = testChannelService;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_Bypass1_CtlCmd1(void)
{
    boolean expected = TRUE;
    boolean actual   = FALSE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0x30;  /* tfvn=0, bypass flag=1, ctl cmd flag=1 */
    TCTF_ChannelService_t testService = testChannelService;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_Bypass1_CtlCmd0(void)
{
    boolean expected = TRUE;
    boolean actual   = FALSE;

    /* Setup Inputs */
    TCTF_Hdr_t testMsg = testHdr;
    testMsg.Octet[0] = 0x20;  /* tfvn=0, bypass flag=1, ctl cmd flag=0 */
    TCTF_ChannelService_t testService = testChannelService;

    /* Execute Test */
    actual = TCTF_IsValidTf(&testMsg, &testService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_NullTfPtr(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */

    /* Execute Test */
    actual = TCTF_IsValidTf(NULL, &testChannelService);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TCTF_IsValidTF_NullCsPtr(void)
{
    boolean expected = FALSE;
    boolean actual   = TRUE;

    /* Setup Inputs */
    TCTF_Hdr_t *testMsg = &testHdr;

    /* Execute Test */
    actual = TCTF_IsValidTf(testMsg, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "is valid Tf == expected");
}


/* ------------------- End of test cases --------------------------------------*/





/*
 * TCTF_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void TCTF_Setup(void)
{
    Ut_OSAPI_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_CFE_EVS_Reset();
    Ut_CFE_TBL_Reset();

    memset(testToBuf, '\0', sizeof(testToBuf));
    memset(testFromBuf, '\0', sizeof(testFromBuf));
    testChannelService = initChannelService;
}

/*
 * TCTF_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void TCTF_TearDown(void)
{

}


#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* TCTF_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void TCTF_AddTestCase(void)
{
    /* TCTF_GetVersion Tests */
    ADD_TEST(Test_TCTF_GetVersion_Zero,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetVersion_Max,        TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetVersion_Null,       TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetBypassFlag Tests */
    ADD_TEST(Test_TCTF_GetBypassFlag_False,   TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetBypassFlag_True,    TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetBypassFlag_Null,    TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetCtlCmdFlag Tests */
    ADD_TEST(Test_TCTF_GetCtlCmdFlag_False,   TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetCtlCmdFlag_True,    TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetCtlCmdFlag_Null,    TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetScId Tests */
    ADD_TEST(Test_TCTF_GetScId_Zero,          TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetScId_Max,           TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetScId_Misc,          TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetScId_Null,          TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetVcId Tests */
    ADD_TEST(Test_TCTF_GetVcId_Zero,          TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetVcId_Max,           TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetVcId_Misc,          TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetVcId_Null,          TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetLength Tests */
    ADD_TEST(Test_TCTF_GetLength_Min,         TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetLength_Max,         TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetLength_Misc,        TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetLength_Null,        TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetSeqNum Tests */
    ADD_TEST(Test_TCTF_GetSeqNum_Zero,        TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSeqNum_Max,         TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSeqNum_Misc,        TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSeqNum_Null,        TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetSegHdrSeqFlags Tests */
    ADD_TEST(Test_TCTF_GetSegHdrSeqFlags_Zero,   TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSegHdrSeqFlags_Max,    TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSegHdrSeqFlags_One,    TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSegHdrSeqFlags_Null,   TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSegHdrSeqFlags_CmdFrm, TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetSegHdrMapId Tests */
    ADD_TEST(Test_TCTF_GetSegHdrMapId_Zero,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSegHdrMapId_Max,        TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSegHdrMapId_Misc,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSegHdrMapId_Null,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetSegHdrMapId_CmdFrm,     TCTF_Setup, TCTF_TearDown);

    /* TCTF_GetPayloadLength Tests */
    ADD_TEST(Test_TCTF_GetPayloadLength_Zero,      TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetPayloadLength_Max,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetPayloadLength_woSegHdr,  TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetPayloadLength_wSegHdr,   TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetPayloadLength_wFrmErrCtl,TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetPayloadLength_NullTfPtr, TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_GetPayloadLength_NullCsPtr, TCTF_Setup, TCTF_TearDown);

    /* TCTF_CopyData_MaxData Tests */
    ADD_TEST(Test_TCTF_CopyData_MaxData,          TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_CopyData_MaxData_wSegHdr,  TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_CopyData_NoData,           TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_CopyData_NullTf,           TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_CopyData_NullDest,         TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_CopyData_NullService,      TCTF_Setup, TCTF_TearDown);

    /* TCTF_IsValidTf Tests */
    ADD_TEST(Test_TCTF_IsValidTF_BadScId,         TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_BadPvId,         TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_GoodMcfId,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_BadVcpId,        TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_GoodVcpId,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_BadVcaId,        TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_GoodVcaId,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_BadVcfId,        TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_GoodVcfId,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_BadMappId,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_GoodMappId,      TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_BadMapaId,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_GoodMapaId,      TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_Bypass0_CtlCmd1, TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_Bypass1_CtlCmd1, TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_Bypass1_CtlCmd0, TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_NullTfPtr,       TCTF_Setup, TCTF_TearDown);
    ADD_TEST(Test_TCTF_IsValidTF_NullCsPtr,       TCTF_Setup, TCTF_TearDown);
}




