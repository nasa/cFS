/*
**
** File: utassert.c
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
static char CurrentSegment[128];

/*
 * Function Definitions
 */

void UtAssert_DoReport(const char *File, uint32 LineNum, uint32 SegmentNum, uint32 TestSeq, uint8 MessageType,
                     const char *SubsysName, const char *ShortDesc)
{
    uint32      FileLen;
    const char *BasePtr;
    char        ReportBuffer[320];

    FileLen = strlen(File);
    BasePtr = File + FileLen;
    while (FileLen > 0)
    {
        --BasePtr;
        --FileLen;
        if (*BasePtr == '/' || *BasePtr == '\\')
        {
            ++BasePtr;
            break;
        }
    }

    snprintf(ReportBuffer, sizeof(ReportBuffer), "%02u.%03u %s:%u - %s", (unsigned int)SegmentNum,
             (unsigned int)TestSeq, BasePtr, (unsigned int)LineNum, ShortDesc);

    UT_BSP_DoText(MessageType, ReportBuffer);
}

void UtAssert_DoTestSegmentReport(const char *SegmentName, const UtAssert_TestCounter_t *TestCounters)
{
    char ReportBuffer[128];

    snprintf(ReportBuffer, sizeof(ReportBuffer),
             "%02u %-20s TOTAL::%-4u  PASS::%-4u  FAIL::%-4u   MIR::%-4u   TSF::%-4u   N/A::%-4u\n",
             (unsigned int)TestCounters->TestSegmentCount, SegmentName, (unsigned int)TestCounters->TotalTestCases,
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_PASS],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_FAILURE],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_MIR],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_TSF],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_NA]);

    UT_BSP_DoText(UTASSERT_CASETYPE_END, ReportBuffer);
}


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

const char *UtAssert_GetSegmentName(void)
{
    return CurrentSegment;
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
        UtAssert_DoTestSegmentReport(CurrentSegment, &UT_SegmentCounters);
    }
    else
    {
        UT_BSP_DoText(UTASSERT_CASETYPE_END, "No test cases\n");
    }

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

bool UtAssert(bool Expression, const char *Description, const char *File, uint32 Line)
{
    return UtAssertEx(Expression, UtAssert_GetContext(), File, Line, "%s", Description);
}

bool UtAssertEx(bool Expression, UtAssert_CaseType_t CaseType, const char *File, uint32 Line, const char *MessageFormat, ...)
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

    UtAssert_DoReport(File, Line, 1 + UT_TotalCounters.TestSegmentCount, UT_SegmentCounters.TotalTestCases, CaseType, CurrentSegment, FinalMessage);

    return Expression;
}

void UtAssert_Abort(const char *Message)
{
    UT_BSP_DoText(UTASSERT_CASETYPE_ABORT, Message);
}

void UtAssert_Message(uint8 MessageType, const char *File, uint32 Line, const char *Spec, ...)
{
    va_list va;
    char FinalMessage[256];
    const char *BaseName;
    size_t MsgLen;

    if (File != NULL)
    {
        BaseName = strrchr(File, '/');
        if (BaseName == NULL)
        {
            BaseName = File;
        }
        else
        {
            ++BaseName;
        }
        snprintf(FinalMessage, sizeof(FinalMessage), "%s:%u:", BaseName, (unsigned int)Line);
        MsgLen = strlen(FinalMessage);
    }
    else
    {
        MsgLen = 0;
    }

    va_start(va, Spec);
    vsnprintf(&FinalMessage[MsgLen], sizeof(FinalMessage) - MsgLen, Spec, va);
    va_end(va);

    UT_BSP_DoText(MessageType, FinalMessage);
}
