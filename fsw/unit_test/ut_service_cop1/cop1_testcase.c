/*
 * Filename: cop1_testcase.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains a unit test cases for the COP-1 procedure
 *
 * Modification History:
 *   03/16/2015  Alan Asp, Odyssey Space Research, LLC
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

#include "cop1.h"
#include "cop1_stubs.h"


/* Prototypes for non-exported functions */


/* -------------------- Special Test Case Variables ------------------------- */
static const uint16 TCTF_MAX_LENGTH = 1024;

static uint8 testTf[1024];
static uint8 toBuffer[1024];
static uint8 defaultTestTf   = 0;
static uint8 defaultToBuffer = 5;

static const TCTF_Hdr_t testHdr =
{
    {0x00,   /* tf vers no.=0, bypass flag=0, ctl cmd flag=0 */
     0x01,   /* spacecraft id=1 */
     0x04,   /* vc id=1 */
     0x04    /* length=4 (1 less than actual length) */
    },
     0x01    /* seq. no.=1 */
             /* since length = 5, assume no segment header which would go here */
};



static const COP1_Clcw_t test_word =
{
    0x01,  /* ctrl word type=0, version=0, status=0, cop in effect=1 */
    0x04,  /* virtual channel ID=1 */
    0x00,  /* no rf=0, no bit lock=0, lockout=0, wait=0, retransmit=0, farmB ctr=0 */
    0x00   /* report value=0 */
};

COP1_Clcw_t test_clcw;

static const TCTF_ChannelService_t testChannelService =
{
    TCTF_SERVICE_MAPP,  /* service type */
    0,                  /* packet version number */
    1,                  /* spacecraft id */
    1,                  /* virtual channel id */
    1,                  /* map id */
    TRUE                /* has segment header */
};



/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  COP1_InitClcw Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_InitClcw(void)
{
    COP1_Clcw_t expected =
    {
        0x01,  /* ctrl word type=0, version=0, status=0, cop in effect=1 */
        0xFC,  /* virtual channel ID=64 (2^6) */
        0x00,  /* no rf=0, no bit lock=0, lockout=0, wait=0, retransmit=0, farmB ctr=0 */
        0x00   /* report value=0 */
    };

    /* Setup Inputs */
    uint16 vcId = 0xFF;
    COP1_Clcw_t test_clcw =
    {
            0xFE,  /* Set all bits the opposite of their expected init values */
            0x03,
            0xFF,
            0xFF
    };

    /* Execute Test */
    COP1_InitClcw(&test_clcw, vcId);

    /* Verify Outputs */
    UtAssert_True(test_clcw.Status  == expected.Status,  "Clcw.Status == expected");
    UtAssert_True(test_clcw.Channel == expected.Channel, "Clcw.Channel == expected");
    UtAssert_True(test_clcw.Flags   == expected.Flags,   "Clcw.Flags == expected");
    UtAssert_True(test_clcw.Report  == expected.Report,  "Clcw.Report == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_InitClcw_NullPtr(void)
{
    /* Setup Inputs */
    uint16 vcId = 0xFF;
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    COP1_InitClcw(test_clcw, vcId);

    /* Verify Outputs - Nothing to verify, no return values, no I/O values updated.
     * This test simply executes the code to verify it doesn't cause an exception. */
    /*UtAssert_True(test_clcw.Status  == expected.Status,  "Clcw.Status == expected");*/
}


/*******************************************************************************
**
**  COP1_GetClcwCtrlWordType Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwCtrlWordType_On(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;

    /* Execute Test */
    actual = COP1_GetClcwCtrlWordType(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Ctrl word type == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwCtrlWordType_Off(void)
{
    uint16 actual   = 0;
    uint16 expected = 1;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0x80;

    /* Execute Test */
    actual = COP1_GetClcwCtrlWordType(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Ctrl word type == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwCtrlWordType_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwCtrlWordType(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Ctrl word type == expected");
}


/*******************************************************************************
**
**  COP1_GetClcwVersion Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwVersion_Zero(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0x9F;

    /* Execute Test */
    actual = COP1_GetClcwVersion(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Version == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwVersion_NonZero(void)
{
    uint16 actual   = 1;
    uint16 expected = 3;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0x60;

    /* Execute Test */
    actual = COP1_GetClcwVersion(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Version == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwVersion_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwVersion(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Version == expected");
}


/*******************************************************************************
**
**  COP1_SetClcwStatus Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwStatus_Zero(void)
{
    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0xFF;

    /* Execute Test */
    COP1_SetClcwStatus(&test_clcw, 0);

    /* Verify Outputs */
    UtAssert_True(test_clcw.Status == 0xE3, "Clcw status == input");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwStatus_NonZero(void)
{
    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0;

    /* Execute Test */
    COP1_SetClcwStatus(&test_clcw, 7);

    /* Verify Outputs */
    UtAssert_True(test_clcw.Status == 0x1C, "Clcw status == input");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwStatus_NullPtr(void)
{
    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    COP1_SetClcwStatus(test_clcw, 7);

    /* Verify Outputs - Nothing to verify, no return values, no I/O values updated.
     * This test simply executes the code to verify it doesn't cause an exception. */
    /*UtAssert_True(test_clcw.Status == 0x1C, "Clcw status == input");*/
}


/*******************************************************************************
**
**  COP1_GetClcwStatus Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwStatus_Zero(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0xE3;

    /* Execute Test */
    actual = COP1_GetClcwStatus(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Status == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwStatus_NonZero(void)
{
    uint16 actual   = 0;
    uint16 expected = 7;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0x1C;

    /* Execute Test */
    actual = COP1_GetClcwStatus(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Status == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwStatus_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwStatus(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "Status == expected");
}


/*******************************************************************************
**
**  COP1_GetClcwCopEffect Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwCopEffect_Zero(void)
{
    uint16 actual   = 0xFFFF;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0xFC;

    /* Execute Test */
    actual = COP1_GetClcwCopEffect(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwCopEffect_Max(void)
{
    uint16 actual   = 0;
    uint16 expected = 3;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Status = 0xFF;

    /* Execute Test */
    actual = COP1_GetClcwCopEffect(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwCopEffect_NullPtr(void)
{
    uint16 actual   = 0xFFFF;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwCopEffect(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_GetClcwVcId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwVcId_Zero(void)
{
    uint16 actual   = 0xFFFF;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Channel = 0x00;

    /* Execute Test */
    actual = COP1_GetClcwVcId(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwVcId_Max(void)
{
    uint16 actual   = 0;
    uint16 expected = 0x3F;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Channel = 0xFF;

    /* Execute Test */
    actual = COP1_GetClcwVcId(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwVcId_NullPtr(void)
{
    uint16 actual   = 0xFFFF;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwVcId(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_SetClcwNoRf Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwNoRf_False(void)
{
    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0xFF;

    /* Execute Test */
    COP1_SetClcwNoRf(&test_clcw, FALSE);

    /* Verify Outputs */
    UtAssert_True(test_clcw.Flags == 0x7F, "test_clcw.Flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwNoRf_True(void)
{
    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x00;

    /* Execute Test */
    COP1_SetClcwNoRf(&test_clcw, TRUE);

    /* Verify Outputs */
    UtAssert_True(test_clcw.Flags == 0x80, "test_clcw.Flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwNoRf_NullPtr(void)
{
    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    COP1_SetClcwNoRf(test_clcw, TRUE);

    /* Verify Outputs - Nothing to verify, no return values, no I/O values updated.
     * This test simply executes the code to verify it doesn't cause an exception. */
    /*UtAssert_True(test_clcw.Flags == 0x80, "test_clcw.Flags == expected");*/
}


/*******************************************************************************
**
**  COP1_GetClcwNoRf Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwNoRf_False(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x7F;

    /* Execute Test */
    actual = COP1_GetClcwNoRf(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwNoRf_True(void)
{
    uint16 actual   = 0;
    uint16 expected = 1;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x80;

    /* Execute Test */
    actual = COP1_GetClcwNoRf(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwNoRf_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwNoRf(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_SetClcwNoBitlock Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwNoBitlock_False(void)
{
    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0xFF;

    /* Execute Test */
    COP1_SetClcwNoBitlock(&test_clcw, FALSE);

    /* Verify Outputs */
    UtAssert_True(test_clcw.Flags == 0xBF, "test_clcw.Flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwNoBitlock_True(void)
{
    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x00;

    /* Execute Test */
    COP1_SetClcwNoBitlock(&test_clcw, TRUE);

    /* Verify Outputs */
    UtAssert_True(test_clcw.Flags == 0x40, "test_clcw.Flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_SetClcwNoBitlock_NullPtr(void)
{
    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    COP1_SetClcwNoBitlock(test_clcw, TRUE);

    /* Verify Outputs - Nothing to verify, no return values, no I/O values updated.
     * This test simply executes the code to verify it doesn't cause an exception. */
    /*UtAssert_True(test_clcw.Flags == 0x40, "test_clcw.Flags == expected");*/
}

/*******************************************************************************
**
**  COP1_GetClcwNoBitlock Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwNoBitlock_False(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0xBF;

    /* Execute Test */
    actual = COP1_GetClcwNoBitlock(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwNoBitlock_True(void)
{
    uint16 actual   = 0;
    uint16 expected = 1;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x40;

    /* Execute Test */
    actual = COP1_GetClcwNoBitlock(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwNoBitlock_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwNoBitlock(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_GetClcwLockout Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwLockout_False(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0xDF;

    /* Execute Test */
    actual = COP1_GetClcwLockout(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwLockout_True(void)
{
    uint16 actual   = 0;
    uint16 expected = 1;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x20;

    /* Execute Test */
    actual = COP1_GetClcwLockout(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwLockout_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwLockout(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_GetClcwWait Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwWait_False(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0xEF;

    /* Execute Test */
    actual = COP1_GetClcwWait(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwWait_True(void)
{
    uint16 actual   = 0;
    uint16 expected = 1;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x10;

    /* Execute Test */
    actual = COP1_GetClcwWait(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwWait_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwWait(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_GetClcwRetransmit Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwRetransmit_False(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0xF7;

    /* Execute Test */
    actual = COP1_GetClcwRetransmit(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwRetransmit_True(void)
{
    uint16 actual   = 0;
    uint16 expected = 1;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x08;

    /* Execute Test */
    actual = COP1_GetClcwRetransmit(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwRetransmit_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwRetransmit(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_GetClcwFarmbCtr Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwFarmbCtr_Zero(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0xF9;

    /* Execute Test */
    actual = COP1_GetClcwFarmbCtr(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwFarmbCtr_Max(void)
{
    uint16 actual   = 0;
    uint16 expected = 3;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Flags = 0x06;

    /* Execute Test */
    actual = COP1_GetClcwFarmbCtr(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwFarmbCtr_NullPtr(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwFarmbCtr(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_GetClcwReport Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwReport_Zero(void)
{
    uint16 actual   = 1;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Report = 0x00;

    /* Execute Test */
    actual = COP1_GetClcwReport(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwReport_Max(void)
{
    uint16 actual   = 0;
    uint16 expected = 0xFF;

    /* Setup Inputs */
    COP1_Clcw_t test_clcw = test_word;
    test_clcw.Report = 0xFF;

    /* Execute Test */
    actual = COP1_GetClcwReport(&test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_GetClcwReport_NullPtr(void)
{
    uint16 actual   = 0xFF;
    uint16 expected = 0;

    /* Setup Inputs */
    COP1_Clcw_t *test_clcw = NULL;

    /* Execute Test */
    actual = COP1_GetClcwReport(test_clcw);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  COP1_ProcessFrame Tests
**
*******************************************************************************/
void TCTF_CopyDataHook(uint8 *toBuffer, TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService)
{
    memcpy(toBuffer, tfPtr, TCTF_MAX_LENGTH);
}


/*----------------------------------------------------------------------------*/
void Test_COP1_ProcessFrame_NullTf(void)
{
    int32 expectedRetVal = COP1_BADINPUT_ERR;
    int32 actualRetVal   = 99;

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    TCTF_Hdr_t *nullTfPtr = NULL;

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, nullTfPtr, &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_MemCmp(&test_clcw, &test_word, sizeof(test_word), "CLCW is unmodified");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_ProcessFrame_NullClcw(void)
{
    int32 expectedRetVal = COP1_BADINPUT_ERR;
    int32 actualRetVal   = 99;

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, NULL, (TCTF_Hdr_t *)testTf, &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_ProcessFrame_InvalidTf(void)
{
    int32 expectedRetVal = COP1_INVALID_TF_ERR;
    int32 actualRetVal   = 99;

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, FALSE, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_MemCmp(&test_clcw, &test_word, sizeof(test_word), "CLCW is unmodified");
}


/*******************************************************************************
**
**  COP1_BypassTf Tests
**
**  Note:
**    The COP1_BypassTf function is a static function, called by COP1_ProcessFrame
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_BypassTf_DataFrm(void)
{
    int32 expectedRetVal = 1499;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x02;   /* lockout flg = 0, retrans flg = 0, FARM-B counter = 1 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    test_clcw.Flags = 0x00;  /* set FARM-B counter = 0 */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_GETCTLCMDFLG_INDEX, 0, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, expectedRetVal, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_BypassTf_Unlock(void)
{
    int32 expectedRetVal = 0;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x02;   /* lockout flg = 0, wait = 0, retrans flg = 0, FARM-B counter = 1 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    test_clcw.Flags = 0x38;  /* set lockout = 1, wait = 1, retran = 1, FARM-B counter = 0 */
    testTf[5]       = 0x00;  /* set control command to "unlock" */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_GETCTLCMDFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 33, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_BypassTf_SetVr_Locked(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x2A;   /* lockout flg = 1, retrans flg = 1, FARM-B counter = 1 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    test_clcw.Flags = 0x28;  /* set lockout flag = 1, retran flag = 1, FARM-B counter = 0 */
    testTf[5] = 0x82;        /* set control command to "setvr" */
    testTf[6] = 0x00;
    testTf[7] = 0x11;        /* set the commanded V(R) */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_GETCTLCMDFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 22, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_BypassTf_SetVr_Unlocked(void)
{
    int32 expectedRetVal = 0;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x02;   /* lockout flg = 0, wait = 0, retrans flg = 0, FARM-B counter = 1 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    test_clcw.Flags = 0x18;  /* set lockout = 0, wait = 1, retran flag = 1, FARM-B counter = 0 */
    test_clcw.Report = 0;    /* zero value to be set */
    testTf[5] = 0x82;        /* set control command to "setvr" */
    testTf[6] = 0x00;
    testTf[7] = 0xAB;        /* set the commanded V(R) */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_GETCTLCMDFLG_INDEX, 1, 1);
    COP1_SetFunctionHook(COP1_TCTF_COPYDATA_INDEX, TCTF_CopyDataHook);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
    UtAssert_True(test_clcw.Report == testTf[7], "V(R) == commanded value");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_BypassTf_SetVr_MalformedCmd(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x00;   /* lockout flg = 0, retrans flg = 0, FARM-B counter = 0 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    test_clcw.Flags  = 0x00; /* lockout flg = 0, retrans flg = 0, FARM-B counter = 0 */
    testTf[5] = 0x82;        /* set control command to "setvr" but with one bad byte */
    testTf[6] = 0x01;        /* bad value - would normally be 0x00 */
    testTf[7] = 0x11;        /* set the commanded V(R) */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_GETCTLCMDFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 22, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}


/*----------------------------------------------------------------------------*/
void Test_COP1_BypassTf_UnknownCmd(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x00;   /* lockout flg = 0, retrans flg = 0, FARM-B counter = 0 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    test_clcw.Flags  = 0x00; /* lockout flg = 0, retrans flg = 0, FARM-B counter = 0 */
    test_clcw.Report = 0;    /* zero value to be set */
    testTf[5] = 0x01;        /* set control command to an unknown value */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_GETCTLCMDFLG_INDEX, 1, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 999, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}


/*******************************************************************************
**
**  COP1_AcceptTf Tests
**
**  Note:
**    The COP1_AcceptTf function is a static function, called by COP1_ProcessFrame
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_Locked(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x20;   /* lockout flg = 1 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    test_clcw.Flags = 0x20;  /* set lockout flag = 1 */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_GoodSeqNoBuf(void)
{
    int32 expectedRetVal = COP1_BADINPUT_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x18;   /* wait flg = 1, retran flg = 1 */

    /* Setup Inputs */
    uint8 *nullPtr = NULL;
    TCTF_ChannelService_t channelService = testChannelService;
    testTf[4]        = 0;    /* set the frame seq. no. and the clcw seq. no. identically */
    test_clcw.Report = 0;

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(nullPtr, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_GoodSeqGoodBuf(void)
{
    int32 expectedRetVal = 777;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x00;   /* wait flg = 0, retran flg = 0 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    test_clcw.Flags  = 0x18; /* set the wait and retransmit flags - should be cleared */
    testTf[4]        = 0;    /* set the frame seq. no. and the clcw seq. no. identically */
    test_clcw.Report = 0;

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, expectedRetVal, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
    UtAssert_True(test_clcw.Report == 1, "CLCW Report incremented");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_PosSeqWindow(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x08;   /* retran flg = 1 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    testTf[4]        = 1;    /* set the frame seq. no. greater than the clcw seq. no. */
    test_clcw.Report = 0;

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 44, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_PosSeqWindowLim(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x08;   /* retran flg = 1 */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    testTf[4]        = 62;   /* set the frame seq. no. max sliding window greater */
    test_clcw.Report = 0;    /* than the clcw seq. no. */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 55, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_NegSeqWindow(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x00;   /* no change to clcw flags expected */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    testTf[4]        = 0;    /* set the frame seq. no. less than the clcw seq. no. */
    test_clcw.Report = 1;

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 66, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_NegSeqWindowLim(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x00;   /* no change to clcw flags expected */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    testTf[4]        = 193;  /* set the frame seq. no. max sliding window less */
    test_clcw.Report = 0;    /* than the clcw seq. no. */

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 77, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW flags == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_LockoutPos(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x20;   /* lockout flg = 1  */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    testTf[4]        = 64;    /* set the frame seq. no. outside pos. sliding window */
    test_clcw.Report = 0;

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 88, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW lockout == expected");
}

/*----------------------------------------------------------------------------*/
void Test_COP1_AcceptTf_LockoutNeg(void)
{
    int32 expectedRetVal = COP1_FARM1_ERR;
    int32 actualRetVal   = 99;
    uint8 expectedClcw   = 0x20;   /* lockout flg = 1  */

    /* Setup Inputs */
    TCTF_ChannelService_t channelService = testChannelService;
    testTf[4]        = 0;    /* set the frame seq. no. outside neg. sliding window */
    test_clcw.Report = 100;

    COP1_SetReturnCode(COP1_TCTF_ISVALIDTF_INDEX, TRUE, 1);
    COP1_SetReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX, 0, 1);
    COP1_SetReturnCode(COP1_TCTF_COPYDATA_INDEX, 222, 1);

    /* Execute Test */
    actualRetVal = COP1_ProcessFrame(toBuffer, &test_clcw, (TCTF_Hdr_t *)(&testTf), &channelService);

    /* Verify Outputs */
    UtAssert_True(actualRetVal == expectedRetVal, "return value == expected");
    UtAssert_True(test_clcw.Flags == expectedClcw, "CLCW lockout == expected");
}


/* ------------------- End of test cases --------------------------------------*/





/*
 * COP1_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void COP1_Setup(void)
{
    COP1_Reset();
    Ut_OSAPI_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_CFE_EVS_Reset();
    Ut_CFE_TBL_Reset();
}

/*
 * COP1_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void COP1_TearDown(void)
{

}


/*
 * COP1_Setup_ProcessFrame
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test of COP1_ProcessFrame
 *
 * Notes:
 *   Initialize testTf and toBuffer to different values so they can be compared for equality
 *   if a TCTF_CopyData function is called.
 */
void COP1_Setup_ProcessFrame(void)
{
    COP1_Setup();

    test_clcw = test_word;

    memset(toBuffer, defaultToBuffer, sizeof(toBuffer));

    memset(testTf, defaultTestTf, sizeof(testTf));
    memcpy(testTf, (uint8 *)(&testHdr), TCTF_PRIHDR_SIZE);
    testTf[2] = 0x03;        /* set TF size to max */
    testTf[3] = 0xFF;
}




#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* COP1_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void COP1_AddTestCase(void)
{
    /* COP1_InitClcw Tests */
    ADD_TEST(Test_COP1_InitClcw,                 COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_InitClcw_NullPtr,         COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwCtrlWordType Tests */
    ADD_TEST(Test_COP1_GetClcwCtrlWordType_On,      COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwCtrlWordType_Off,     COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwCtrlWordType_NullPtr, COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwVersion Tests */
    ADD_TEST(Test_COP1_GetClcwVersion_Zero,      COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwVersion_NonZero,   COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwVersion_NullPtr,   COP1_Setup, COP1_TearDown);

    /* COP1_SetClcwStatus Tests */
    ADD_TEST(Test_COP1_SetClcwStatus_Zero,       COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_SetClcwStatus_NonZero,    COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_SetClcwStatus_NullPtr,    COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwStatus Tests */
    ADD_TEST(Test_COP1_GetClcwStatus_Zero,       COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwStatus_NonZero,    COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwStatus_NullPtr,    COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwCopEffect Tests */
    ADD_TEST(Test_COP1_GetClcwCopEffect_Zero,    COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwCopEffect_Max,     COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwCopEffect_NullPtr, COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwVcId Tests */
    ADD_TEST(Test_COP1_GetClcwVcId_Zero,         COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwVcId_Max,          COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwVcId_NullPtr,      COP1_Setup, COP1_TearDown);

    /* COP1_SetClcwNoRf Tests */
    ADD_TEST(Test_COP1_SetClcwNoRf_False,        COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_SetClcwNoRf_True,         COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_SetClcwNoRf_NullPtr,      COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwNoRf Tests */
    ADD_TEST(Test_COP1_GetClcwNoRf_False,        COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwNoRf_True,         COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwNoRf_NullPtr,      COP1_Setup, COP1_TearDown);

    /* COP1_SetClcwNoBitlock Tests */
    ADD_TEST(Test_COP1_SetClcwNoBitlock_False,   COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_SetClcwNoBitlock_True,    COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_SetClcwNoBitlock_NullPtr, COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwNoBitlock Tests */
    ADD_TEST(Test_COP1_GetClcwNoBitlock_False,   COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwNoBitlock_True,    COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwNoBitlock_NullPtr, COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwLockout Tests */
    ADD_TEST(Test_COP1_GetClcwLockout_False,     COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwLockout_True,      COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwLockout_NullPtr,   COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwWait Tests */
    ADD_TEST(Test_COP1_GetClcwWait_False,        COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwWait_True,         COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwWait_NullPtr,      COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwRetransmit Tests */
    ADD_TEST(Test_COP1_GetClcwRetransmit_False,  COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwRetransmit_True,   COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwRetransmit_NullPtr,COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwFarmbCtr Tests */
    ADD_TEST(Test_COP1_GetClcwFarmbCtr_Zero,     COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwFarmbCtr_Max,      COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwFarmbCtr_NullPtr,  COP1_Setup, COP1_TearDown);

    /* COP1_GetClcwReport Tests */
    ADD_TEST(Test_COP1_GetClcwReport_Zero,       COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwReport_Max,        COP1_Setup, COP1_TearDown);
    ADD_TEST(Test_COP1_GetClcwReport_NullPtr,    COP1_Setup, COP1_TearDown);

    /* COP1_ProcessFrame Tests */
    ADD_TEST(Test_COP1_ProcessFrame_NullTf,      COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_ProcessFrame_NullClcw,    COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_ProcessFrame_InvalidTf,   COP1_Setup_ProcessFrame, COP1_TearDown);

    /* COP1_BypassTf Tests */
    ADD_TEST(Test_COP1_BypassTf_DataFrm,         COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_BypassTf_Unlock,          COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_BypassTf_SetVr_Locked,    COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_BypassTf_SetVr_Unlocked,  COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_BypassTf_SetVr_MalformedCmd, COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_BypassTf_UnknownCmd,      COP1_Setup_ProcessFrame, COP1_TearDown);

    /* COP1_AcceptTf Tests */
    ADD_TEST(Test_COP1_AcceptTf_Locked,          COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_AcceptTf_GoodSeqNoBuf,    COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_AcceptTf_GoodSeqGoodBuf,  COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_AcceptTf_PosSeqWindow,    COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_AcceptTf_PosSeqWindowLim, COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_AcceptTf_NegSeqWindow,    COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_AcceptTf_NegSeqWindowLim, COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_AcceptTf_LockoutPos,      COP1_Setup_ProcessFrame, COP1_TearDown);
    ADD_TEST(Test_COP1_AcceptTf_LockoutNeg,      COP1_Setup_ProcessFrame, COP1_TearDown);

}
