/*
**
** File: utassert.c
**
** $Id: utassert.c 1.3 2015/06/16 16:14:04EDT sstrege Exp  $
**
** Copyright 2012-2013 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: This code implements a standard set of asserts for use in unit tests.
**
** $Log: utassert.c  $
** Revision 1.3 2015/06/16 16:14:04EDT sstrege 
** Added copyright information
** Revision 1.2 2015/03/10 15:21:43EDT czogby 
** Add Missing Functionality to UT Assert Library
** --- Added comments ---  czogby [2015/03/31 18:36:49Z]
** Added a #include and a #ifdef
**
*/

/*
 * Includes
 */
#include <stdlib.h>
#include <stdarg.h>

#include "common_types.h"
#include "utassert.h"
#include "uttools.h"
#include "utbsp.h"

/*
 * Local Data
 */

UtAssert_CaseType_t DefaultContext = UTASSERT_CASETYPE_FAILURE;
UtAssert_TestCounter_t UT_SegmentCounters = { 0 };
UtAssert_TestCounter_t UT_TotalCounters = { 0 };
static char CurrentSegment[32];

/*
 * Function Definitions
 */

uint32 UtAssert_GetPassCount(void)
{
    return(UT_TotalCounters.CaseCount[UTASSERT_CASETYPE_PASS]);
}

uint32 UtAssert_GetFailCount(void)
{
    return(UT_TotalCounters.CaseCount[UTASSERT_CASETYPE_FAILURE]);
}

const UtAssert_TestCounter_t *UtAssert_GetCounters(void)
{
    return &UT_TotalCounters;
}

void UtAssert_BeginTest(const char *SegmentName)
{
    memset(&UT_SegmentCounters, 0, sizeof (UT_SegmentCounters));
    strncpy(CurrentSegment, SegmentName, sizeof(CurrentSegment) - 1);
    CurrentSegment[sizeof(CurrentSegment) - 1] = 0;
    UT_BSP_StartTestSegment(1 + UT_TotalCounters.TestSegmentCount, SegmentName);
}

void UtAssert_EndTest(void)
{
    uint32 Ct;

    if (UT_SegmentCounters.TotalTestCases > 0)
    {
        ++UT_TotalCounters.TestSegmentCount;
        UT_SegmentCounters.TestSegmentCount =  UT_TotalCounters.TestSegmentCount;
        UT_TotalCounters.TotalTestCases += UT_SegmentCounters.TotalTestCases;
        for (Ct = 0; Ct < UTASSERT_CASETYPE_MAX; ++Ct)
        {
            UT_TotalCounters.CaseCount[Ct] += UT_SegmentCounters.CaseCount[Ct];
        }
    }

    UT_BSP_DoTestSegmentReport(CurrentSegment, &UT_SegmentCounters);
    memset(&UT_SegmentCounters, 0, sizeof (UT_SegmentCounters));
}

void UtAssert_SetContext(UtAssert_CaseType_t Context)
{
    DefaultContext = Context;
}

UtAssert_CaseType_t UtAssert_GetContext(void)
{
    return DefaultContext;
}

osalbool UtAssert(osalbool Expression, const char *Description, const char *File, uint32 Line)
{
    return UtAssertEx(Expression, UtAssert_GetContext(), File, Line, "%s", Description);
}

osalbool UtAssertEx(osalbool Expression, UtAssert_CaseType_t CaseType, const char *File, uint32 Line, const char *MessageFormat, ...)
{
    va_list va;
    char FinalMessage[128];

    ++UT_SegmentCounters.TotalTestCases;

    if (Expression)
    {
        CaseType = UTASSERT_CASETYPE_PASS;
    }

    if ((uint32)CaseType < UTASSERT_CASETYPE_MAX)
    {
        ++UT_SegmentCounters.CaseCount[(uint32)CaseType];
    }

    va_start(va, MessageFormat);
    vsnprintf(FinalMessage, sizeof(FinalMessage), MessageFormat, va);
    va_end(va);

    UT_BSP_DoReport(File, Line, 1 + UT_TotalCounters.TestSegmentCount, UT_SegmentCounters.TotalTestCases, CaseType, CurrentSegment, FinalMessage);

    return Expression;
}

void UtAssert_Abort(const char *Message)
{
    UT_BSP_DoText(UTASSERT_CASETYPE_ABORT, Message);
}

void UtAssert_Message(uint8 MessageType, const char *File, uint32 Line, const char *Spec, ...)
{
    va_list va;
    char FinalMessage[128];

    va_start(va, Spec);
    vsnprintf(FinalMessage, sizeof(FinalMessage), Spec, va);
    va_end(va);

    UT_BSP_DoText(MessageType, FinalMessage);
}
