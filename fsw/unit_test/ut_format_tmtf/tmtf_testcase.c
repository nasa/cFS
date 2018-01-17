/*
 * Filename: tmtf_testcase.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains unit test cases for TM transfer frames
 *
 * TODO:
 *   Add additional asserts for TMTF_InitFrame tests
 *   Add additional TMTF_InitFrame tests
 *
 * Modification History:
 *   04/26/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "tmtf_stubs.h"
#include "utassert.h"
#include "uttest.h"
#include "utlist.h"

#include "tmtf.h"


#define TEST_FRAME_LENGTH     500  

static const uint16 TMTF_PRIHDR_SIZE = 6;


/* Prototypes for non-exported functions */


/* -------------------- Special Test Case Variables ------------------------- */
static const TMTF_PriHdr_t initHdr =
{
    {0x00, 0x12},  /* tfvn=0, scid=1, vcid=1, ocf flag=0 */
    0x00,          /* mc frame count=0 */
    0x00,          /* vc frame count=0 */
    {0x00, 0x00}   /* status=0 */
};

uint8 tfBuffer[TEST_FRAME_LENGTH];

static TMTF_PriHdr_t   *testFramePtr = (TMTF_PriHdr_t *)tfBuffer;
static uint8            testFrame[TEST_FRAME_LENGTH];
static uint8            testData[TEST_FRAME_LENGTH];


/*******************************************************************************
**
**  TMTF_SetVersion Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVersion_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetVersion(NULL, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVersion_Zero(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[0] = 0xFF;

    /* Execute test */
    actual = TMTF_SetVersion(testFramePtr, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->Id[0] == 0x3F, "tfvn == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVersion_One(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[0] = 0x00;

    /* Execute test */
    actual = TMTF_SetVersion(testFramePtr, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->Id[0] == 0x40, "tfvn == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVersion_Max(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[0] = 0x00;

    /* Execute test */
    actual = TMTF_SetVersion(testFramePtr, 0xFF);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->Id[0] == 0xC0, "tfvn == expected");
}


/*******************************************************************************
**
**  TMTF_SetScId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetScId_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetScId(NULL, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetScId_Min(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[0] = 0xFF;
    testFramePtr->Id[1] = 0xFF;

    /* Execute test */
    actual = TMTF_SetScId(testFramePtr, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True((testFramePtr->Id[0] == 0xC0) && (testFramePtr->Id[1] == 0x0F), "sc id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetScId_Max(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[0] = 0x00;
    testFramePtr->Id[1] = 0x00;

    /* Execute test */
    actual = TMTF_SetScId(testFramePtr, 0xFFFF);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True((testFramePtr->Id[0] == 0x3F) && (testFramePtr->Id[1] == 0xF0), "sc id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetScId_Misc(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[0] = 0xC0;
    testFramePtr->Id[1] = 0x0F;

    /* Execute test */
    actual = TMTF_SetScId(testFramePtr, 0x2AA);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True((testFramePtr->Id[0] == 0xEA) && (testFramePtr->Id[1] == 0xAF), "sc id == expected");
}


/*******************************************************************************
**
**  TMTF_GetMcId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_GetMcId(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[0] = 0x00;
    TMTF_SetScId(testFramePtr, 0x01);

    /* Execute test */
    actual = TMTF_GetMcId(NULL);
    UtAssert_True(actual == expected, "Null Pointer Error");
    
    expected = 0x01;
    actual = TMTF_GetMcId(testFramePtr);
    UtAssert_True(actual == expected, "McId == expected");
}



/*******************************************************************************
**
**  TMTF_SetVcId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVcId_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetVcId(NULL, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVcId_Min(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[1] = 0xFF;

    /* Execute test */
    actual = TMTF_SetVcId(testFramePtr, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->Id[1] == 0xF1, "vc id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVcId_Max(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[1] = 0x00;

    /* Execute test */
    actual = TMTF_SetVcId(testFramePtr, 0xFFFF);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->Id[1] == 0x0E, "vc id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVcId_Misc(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[1] = 0x00;

    /* Execute test */
    actual = TMTF_SetVcId(testFramePtr, 0x05);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->Id[1] == 0x0A, "vc id == expected");
}

/*******************************************************************************
**
**  TMTF_GetGlobalVcId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_GetGlobalVcId(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[0] = 0x00;
    testFramePtr->Id[1] = 0x00;
    TMTF_SetScId(testFramePtr, 1);
    TMTF_SetVcId(testFramePtr, 7);

    /* Execute test */
    actual = TMTF_GetGlobalVcId(NULL);
    UtAssert_True(actual == expected, "return value == expected");
    
    expected = 0x1e;
    actual = TMTF_GetGlobalVcId(testFramePtr);
    UtAssert_True(actual == expected, "GlobalVcId == expected");
}

/*******************************************************************************
**
**  TMTF_SetMcFrameCount Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetMcFrameCount_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetMcFrameCount(NULL, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetMcFrameCount_Min(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

   /* Setup inputs */
    testFramePtr->McFrameCount = 0xFF;

    /* Execute test */
    actual = TMTF_SetMcFrameCount(testFramePtr, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->McFrameCount == 0, "mc frame count == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetMcFrameCount_Max(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->McFrameCount = 0;

    /* Execute test */
    actual = TMTF_SetMcFrameCount(testFramePtr, 0xFFFF);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->McFrameCount == 0xFF, "mc frame count == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetMcFrameCount_Misc(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->McFrameCount = 0;

    /* Execute test */
    actual = TMTF_SetMcFrameCount(testFramePtr, 0xAA);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->McFrameCount == 0xAA, "mc frame count == expected");
}


/*******************************************************************************
**
**  TMTF_SetVcFrameCount Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVcFrameCount_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetVcFrameCount(NULL, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVcFrameCount_Min(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->VcFrameCount = 0xFF;

    /* Execute test */
    actual = TMTF_SetVcFrameCount(testFramePtr, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->VcFrameCount == 0, "vc frame count == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVcFrameCount_Max(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->VcFrameCount = 0;

    /* Execute test */
    actual = TMTF_SetVcFrameCount(testFramePtr, 0xFFFF);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->VcFrameCount == 0xFF, "vc frame count == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetVcFrameCount_Misc(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->VcFrameCount = 0;

    /* Execute test */
    actual = TMTF_SetVcFrameCount(testFramePtr, 0xAA);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->VcFrameCount == 0xAA, "vc frame count == expected");
}


/*******************************************************************************
**
**  TMTF_IncrVcFrameCount Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_IncrVcFrameCount_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_IncrVcFrameCount(NULL);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_IncrVcFrameCount(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->VcFrameCount = 0xFF;

    /* Execute test */
    actual = TMTF_IncrVcFrameCount(testFramePtr);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->VcFrameCount == 0, "vc frame count == expected");

    /* Setup inputs */
    testFramePtr->VcFrameCount = 0x00;

    /* Execute test */
    actual = TMTF_IncrVcFrameCount(testFramePtr);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->VcFrameCount == 1, "vc frame count == expected");
}

/*******************************************************************************
**
**  TMTF_SetOcfFlag Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetOcfFlag_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetOcfFlag(NULL, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetOcfFlag_True(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[1] = 0x00;

    /* Execute test */
    actual = TMTF_SetOcfFlag(testFramePtr, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->Id[1] == 0x01, "ocf flag == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetOcfFlag_False(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->Id[1] = 0xFF;

    /* Execute test */
    actual = TMTF_SetOcfFlag(testFramePtr, FALSE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->Id[1] == 0xFE, "ocf flag == expected");
}


/*******************************************************************************
**
**  TMTF_SetSecHdrFlag Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrFlag_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetSecHdrFlag(NULL, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrFlag_True(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x00;

    /* Execute test */
    actual = TMTF_SetSecHdrFlag(testFramePtr, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x80, "secondary hdr flag == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrFlag_False(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0xFF;

    /* Execute test */
    actual = TMTF_SetSecHdrFlag(testFramePtr, FALSE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x7F, "secondary hdr flag == expected");
}


/*******************************************************************************
**
**  TMTF_SetSyncFlag Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSyncFlag_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetSyncFlag(NULL, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSyncFlag_True_PoFlgOn(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x20;  /* packet order flag = 1, seg length id = '00' */

    /* Execute test */
    actual = TMTF_SetSyncFlag(testFramePtr, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x60, "sync flag == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSyncFlag_True_PoFlgOff(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x00;  /* packet order flag = 0, seg length id = '00' */

    /* Execute test */
    actual = TMTF_SetSyncFlag(testFramePtr, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x40, "sync flag == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSyncFlag_False_PoFlgOn(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0xE7;  /* packet order flag = 1, seg length id = '00' */

    /* Execute test */
    actual = TMTF_SetSyncFlag(testFramePtr, FALSE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x9F, "sync flag == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSyncFlag_False_PoFlgOff(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0xDF;  /* packet order flag = 0, seg length id = '11' */

    /* Execute test */
    actual = TMTF_SetSyncFlag(testFramePtr, FALSE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x9F, "sync flag == expected");
}


/*******************************************************************************
**
**  TMTF_SetPacketOrderFlag Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetPacketOrderFlag_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetPacketOrderFlag(NULL, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetPacketOrderFlag_True(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x00;

    /* Execute test */
    actual = TMTF_SetPacketOrderFlag(testFramePtr, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x20, "packet order flag == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetPacketOrderFlag_False(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

   /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0xFF;

    /* Execute test */
    actual = TMTF_SetPacketOrderFlag(testFramePtr, FALSE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0xDF, "packet order flag == expected");
}


/*******************************************************************************
**
**  TMTF_SetSegLengthId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSegLengthId_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetSegLengthId(NULL, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSegLengthId_Min(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0xFF;

    /* Execute test */
    actual = TMTF_SetSegLengthId(testFramePtr, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0xE7, "seg length id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSegLengthId_Max(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

   /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x00;

    /* Execute test */
    actual = TMTF_SetSegLengthId(testFramePtr, 0xFF);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x18, "seg length id == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSegLengthId_Misc(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

   /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x00;

    /* Execute test */
    actual = TMTF_SetSegLengthId(testFramePtr, 2);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(testFramePtr->DataFieldStatus[0] == 0x10, "seg length id == expected");
}


/*******************************************************************************
**
**  TMTF_SetFirstHdrPtr Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetFirstHdrPtr_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetFirstHdrPtr(NULL, TRUE);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetFirstHdrPtr_Min(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0xFF;
    testFramePtr->DataFieldStatus[1] = 0xFF;

    /* Execute test */
    actual = TMTF_SetFirstHdrPtr(testFramePtr, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True((testFramePtr->DataFieldStatus[0] == 0xF8) && (testFramePtr->DataFieldStatus[1] == 0x00),
                  "first hdr ptr == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetFirstHdrPtr_Max(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x00;
    testFramePtr->DataFieldStatus[1] = 0x00;

    /* Execute test */
    actual = TMTF_SetFirstHdrPtr(testFramePtr, 0xFFFF);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True((testFramePtr->DataFieldStatus[0] == 0x07) && (testFramePtr->DataFieldStatus[1] == 0xFF),
                  "first hdr ptr == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetFirstHdrPtr_NoFirstHdrPtr(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    uint16 noFirstHdrPtrVal = 0x07FF;
    testFramePtr->DataFieldStatus[0] = 0x00;
    testFramePtr->DataFieldStatus[1] = 0x00;

    /* Execute test */
    actual = TMTF_SetFirstHdrPtr(testFramePtr, noFirstHdrPtrVal);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(((testFramePtr->DataFieldStatus[0] * 256 +
                    testFramePtr->DataFieldStatus[1]) == noFirstHdrPtrVal),
                  "first hdr ptr == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetFirstHdrPtr_OidOnly(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    uint16 oidFirstHdrPtrVal = 0x07FE;
    testFramePtr->DataFieldStatus[0] = 0x00;
    testFramePtr->DataFieldStatus[1] = 0x00;

    /* Execute test */
    actual = TMTF_SetFirstHdrPtr(testFramePtr, oidFirstHdrPtrVal);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(((testFramePtr->DataFieldStatus[0] * 256 +
                    testFramePtr->DataFieldStatus[1]) == oidFirstHdrPtrVal),
                  "first hdr ptr == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetFirstHdrPtr_Misc(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x00;
    testFramePtr->DataFieldStatus[1] = 0x00;

    /* Execute test */
    actual = TMTF_SetFirstHdrPtr(testFramePtr, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(((testFramePtr->DataFieldStatus[0] * 256 +
                    testFramePtr->DataFieldStatus[1]) == 1),
                  "first hdr ptr == expected");
}


/*******************************************************************************
**
**  TMTF_SetSecHdrLength Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrLength_NullPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

   /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetSecHdrLength(NULL, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrLength_SecHdrFlagFalse(void)
{
    int32 expected = TMTF_INVALID_SECHDR;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x00;  /* sec hdr flag = 0 */

    /* Execute test */
    actual = TMTF_SetSecHdrLength(testFramePtr, 0xFF);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(*((uint8 *)testFramePtr + TMTF_PRIHDR_SIZE) == 0x00,
                  "sec hdr length == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrLength_Zero(void)
{
    int32 expected = TMTF_INVALID_LENGTH;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x80;  /* sec hdr flag = 1 */
    *((uint8 *)testFramePtr + TMTF_PRIHDR_SIZE) = 0x3F;

    /* Execute test */
    actual = TMTF_SetSecHdrLength(testFramePtr, 0);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(*((uint8 *)testFramePtr + TMTF_PRIHDR_SIZE) == 0x3F,
                  "sec hdr length == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrLength_Min(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x80;  /* sec hdr flag = 1 */
    *((uint8 *)testFramePtr + TMTF_PRIHDR_SIZE) = 0x3F;

    /* Execute test */
    actual = TMTF_SetSecHdrLength(testFramePtr, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(*((uint8 *)testFramePtr + TMTF_PRIHDR_SIZE) == 0x01,
                  "sec hdr length == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrLength_Max(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x80;  /* sec hdr flag = 1 */

    /* Execute test */
    actual = TMTF_SetSecHdrLength(testFramePtr, 63);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(*((uint8 *)testFramePtr + TMTF_PRIHDR_SIZE) == 0x3F,
                  "sec hdr length == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrLength_TooBig(void)
{
    int32 expected = TMTF_INVALID_LENGTH;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x80;  /* sec hdr flag = 1 */

    /* Execute test */
    actual = TMTF_SetSecHdrLength(testFramePtr, 64);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(*((uint8 *)testFramePtr + TMTF_PRIHDR_SIZE) == 0x00,
                  "sec hdr length == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrLength_Misc(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    testFramePtr->DataFieldStatus[0] = 0x80;  /* sec hdr flag = 1 */

    /* Execute test */
    actual = TMTF_SetSecHdrLength(testFramePtr, 42);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True(*((uint8 *)testFramePtr + TMTF_PRIHDR_SIZE) == 42,
                  "sec hdr length == expected");
}


/*******************************************************************************
**
**  TMTF_SetSecHdrData Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrData_NullTfPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

   /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetSecHdrData(NULL, testData, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrData_NullDataPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

   /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetSecHdrData((TMTF_PriHdr_t *)(&testFrame), NULL, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrData_NoSecHdr(void)
{
    int32 expected = TMTF_INVALID_SECHDR;
    int32 actual   = 99;

   /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetSecHdrData((TMTF_PriHdr_t *)(&testFrame), testData, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrData_InvalidLen(void)
{
    int32 expected = TMTF_INVALID_LENGTH;
    int32 actual   = 99;

   /* Setup inputs */
    TMTF_SetSecHdrFlag((TMTF_PriHdr_t *)(&testFrame), 1);
    TMTF_SetSecHdrLength((TMTF_PriHdr_t *)(&testFrame), 1);

    /* Execute test */
    actual = TMTF_SetSecHdrData((TMTF_PriHdr_t *)(&testFrame), testData, 2);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetSecHdrData(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

   /* Setup inputs */
    TMTF_SetSecHdrFlag((TMTF_PriHdr_t *)(&testFrame), 1);
    TMTF_SetSecHdrLength((TMTF_PriHdr_t *)(&testFrame), 2);

    /* Execute test */
    actual = TMTF_SetSecHdrData((TMTF_PriHdr_t *)(&testFrame), testData, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}


/*******************************************************************************
**
**  TMTF_SetOcf Test
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_SetOcf_NullTfPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

   /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetOcf(NULL, testData, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetOcf_NullDataPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;

   /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetOcf((TMTF_PriHdr_t *)(&testFrame), NULL, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetOcf_NoOcf(void)
{
    int32 expected = TMTF_ERROR;
    int32 actual   = 99;

   /* Setup inputs */

    /* Execute test */
    actual = TMTF_SetOcf((TMTF_PriHdr_t *)(&testFrame), testData, 1);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_SetOcf(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;

    /* Setup inputs */
    TMTF_SetOcfFlag(testFramePtr, 1);

    /* Execute test */
    actual = TMTF_SetOcf(testFramePtr, testData, 496);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*******************************************************************************
**
**  TMTF UpdateErrCtrlField Test
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TMTF_UpdateErrCtrlField_NullTfPtr(void)
{
    int32 expected = TMTF_INVALID_POINTER;
    int32 actual   = 99;
    
    TMTF_LibInit();
    actual = TMTF_UpdateErrCtrlField(NULL, 9);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_UpdateErrCtrlField_InvlLen(void)
{
    int32 expected = TMTF_INVALID_LENGTH;
    int32 actual   = 99;
    
    TMTF_LibInit();
    actual = TMTF_UpdateErrCtrlField(&testFrame, 5);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_TMTF_UpdateErrCtrlField(void)
{
    int32 expected = TMTF_SUCCESS;
    int32 actual   = 99;
    
    /* Standard "123456789" test for CRC16-CCITT as in following reference:
     * http::/srecord.sourceforge.net/crc16-ccitt.html 
     * Expected: 0xE5CC. */
    
    uint8 crcFrame[11] = "123456789";

    TMTF_LibInit();
    actual = TMTF_UpdateErrCtrlField((TMTF_PriHdr_t *) crcFrame, 9);

    /* Verify results */
    UtAssert_True(actual == expected, "return value == expected");
    UtAssert_True((crcFrame[9] == 0xE5) && (crcFrame[10] == 0xCC),
                  "CRC == expected");
}


/* ------------------- End of test cases --------------------------------------*/





/*
 * TMTF_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void TMTF_Setup(void)
{
    memset(testFramePtr, 0, TEST_FRAME_LENGTH);
    memcpy(testFramePtr, &initHdr, sizeof(initHdr));
    memset(&testFrame, 0, sizeof(testFrame));
    memset(&testData, 0, sizeof(testData));
}

/*
 * TMTF_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void TMTF_TearDown(void)
{

}


#define ADD_TEST(test) UtTest_Add((test), TMTF_Setup, TMTF_TearDown, #test)

/* TMTF_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void TMTF_AddTestCase(void)
{
    /* TMTF_SetVersion Tests */
    ADD_TEST(Test_TMTF_SetVersion_NullPtr);
    ADD_TEST(Test_TMTF_SetVersion_Zero);
    ADD_TEST(Test_TMTF_SetVersion_One);
    ADD_TEST(Test_TMTF_SetVersion_Max);

    /* TMTF_SetScId Tests */
    ADD_TEST(Test_TMTF_SetScId_NullPtr);
    ADD_TEST(Test_TMTF_SetScId_Min);
    ADD_TEST(Test_TMTF_SetScId_Max);
    ADD_TEST(Test_TMTF_SetScId_Misc);

    ADD_TEST(Test_TMTF_GetMcId);

    /* TMTF_SetVcId Tests */
    ADD_TEST(Test_TMTF_SetVcId_NullPtr);
    ADD_TEST(Test_TMTF_SetVcId_Min);
    ADD_TEST(Test_TMTF_SetVcId_Max);
    ADD_TEST(Test_TMTF_SetVcId_Misc);
    
    ADD_TEST(Test_TMTF_GetGlobalVcId);

    /* TMTF_SetMcFrameCount Tests */
    ADD_TEST(Test_TMTF_SetMcFrameCount_NullPtr);
    ADD_TEST(Test_TMTF_SetMcFrameCount_Min);
    ADD_TEST(Test_TMTF_SetMcFrameCount_Max);
    ADD_TEST(Test_TMTF_SetMcFrameCount_Misc);

    /* TMTF_SetVcFrameCount Tests */
    ADD_TEST(Test_TMTF_SetVcFrameCount_NullPtr);
    ADD_TEST(Test_TMTF_SetVcFrameCount_Min);
    ADD_TEST(Test_TMTF_SetVcFrameCount_Max);
    ADD_TEST(Test_TMTF_SetVcFrameCount_Misc);
    
    /* TMTF_IncrVcFrameCount Tests */
    ADD_TEST(Test_TMTF_IncrVcFrameCount_NullPtr);
    ADD_TEST(Test_TMTF_IncrVcFrameCount);

    /* TMTF_SetOcfFlag Tests */
    ADD_TEST(Test_TMTF_SetOcfFlag_NullPtr);
    ADD_TEST(Test_TMTF_SetOcfFlag_True);
    ADD_TEST(Test_TMTF_SetOcfFlag_False);
    
    /* TMTF_SetSecHdrFlag Tests */
    ADD_TEST(Test_TMTF_SetSecHdrFlag_NullPtr);
    ADD_TEST(Test_TMTF_SetSecHdrFlag_True);
    ADD_TEST(Test_TMTF_SetSecHdrFlag_False);

    /* TMTF_SetSyncFlag Tests */
    ADD_TEST(Test_TMTF_SetSyncFlag_NullPtr);
    ADD_TEST(Test_TMTF_SetSyncFlag_True_PoFlgOn);
    ADD_TEST(Test_TMTF_SetSyncFlag_True_PoFlgOff);
    ADD_TEST(Test_TMTF_SetSyncFlag_False_PoFlgOn);
    ADD_TEST(Test_TMTF_SetSyncFlag_False_PoFlgOff);

    /* TMTF_SetPacketOrderFlag Tests */
    ADD_TEST(Test_TMTF_SetPacketOrderFlag_NullPtr);
    ADD_TEST(Test_TMTF_SetPacketOrderFlag_True);
    ADD_TEST(Test_TMTF_SetPacketOrderFlag_False);

    /* TMTF_SetSegLengthId Tests */
    ADD_TEST(Test_TMTF_SetSegLengthId_NullPtr);
    ADD_TEST(Test_TMTF_SetSegLengthId_Min);
    ADD_TEST(Test_TMTF_SetSegLengthId_Max);
    ADD_TEST(Test_TMTF_SetSegLengthId_Misc);

    /* TMTF_SetFirstHdrPtr Tests */
    ADD_TEST(Test_TMTF_SetFirstHdrPtr_NullPtr);
    ADD_TEST(Test_TMTF_SetFirstHdrPtr_Min);
    ADD_TEST(Test_TMTF_SetFirstHdrPtr_Max);
    ADD_TEST(Test_TMTF_SetFirstHdrPtr_NoFirstHdrPtr);
    ADD_TEST(Test_TMTF_SetFirstHdrPtr_OidOnly);
    ADD_TEST(Test_TMTF_SetFirstHdrPtr_Misc);

    /* TMTF_SetSecHdrLength Tests */
    ADD_TEST(Test_TMTF_SetSecHdrLength_NullPtr);
    ADD_TEST(Test_TMTF_SetSecHdrLength_SecHdrFlagFalse);
    ADD_TEST(Test_TMTF_SetSecHdrLength_Zero);
    ADD_TEST(Test_TMTF_SetSecHdrLength_Min);
    ADD_TEST(Test_TMTF_SetSecHdrLength_Max);
    ADD_TEST(Test_TMTF_SetSecHdrLength_TooBig);
    ADD_TEST(Test_TMTF_SetSecHdrLength_Misc);

    /* TMTF_SetSecHdrData Tests */
    ADD_TEST(Test_TMTF_SetSecHdrData_NullTfPtr);
    ADD_TEST(Test_TMTF_SetSecHdrData_NullDataPtr);
    ADD_TEST(Test_TMTF_SetSecHdrData_NoSecHdr);
    ADD_TEST(Test_TMTF_SetSecHdrData_InvalidLen);
    ADD_TEST(Test_TMTF_SetSecHdrData);

    /* TMTF_SetOcf Tests */
    ADD_TEST(Test_TMTF_SetOcf_NullTfPtr);
    ADD_TEST(Test_TMTF_SetOcf_NullDataPtr);
    ADD_TEST(Test_TMTF_SetOcf_NoOcf);
    ADD_TEST(Test_TMTF_SetOcf);

    /* TMTF_UpdateErrCtrlField Tests */
    ADD_TEST(Test_TMTF_UpdateErrCtrlField_NullTfPtr);
    ADD_TEST(Test_TMTF_UpdateErrCtrlField_InvlLen);
    ADD_TEST(Test_TMTF_UpdateErrCtrlField);


}
