/******************************************************************************
** File:  utbsp.c
**
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
**      Copyright (c) 2004-2015, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved.
**
**
** Purpose:
**   Unit test BSP interface functions.
**
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utbsp.h"
#include "uttest.h"

#include "bsp-impl.h"

/*
**  Local Variables
*/
typedef struct
{
    uint32 CurrVerbosity;
} BSP_UT_GlobalData_t;

BSP_UT_GlobalData_t BSP_UT_Global;

void UT_BSP_Setup(void)
{
    uint8        UserShift;
    uint32       ArgC;
    char *const *ArgV;

    memset(&BSP_UT_Global, 0, sizeof(BSP_UT_Global));

    UserShift = UTASSERT_CASETYPE_NONE;
    ArgC      = OS_BSP_GetArgC();
    if (ArgC > 0)
    {
        ArgV = OS_BSP_GetArgV();
        while (ArgC > 0)
        {
            --ArgC;

            if (strcmp(ArgV[0], "-d") == 0)
            {
                UserShift = UTASSERT_CASETYPE_DEBUG;
            }
            if (strcmp(ArgV[0], "-q") == 0)
            {
                UserShift = UTASSERT_CASETYPE_FAILURE;
            }
            if (strcmp(ArgV[0], "-v") == 0 && ArgC > 0)
            {
                --ArgC;
                UserShift = strtoul(ArgV[1], NULL, 0);
                ++ArgV;
            }

            ++ArgV;
        }
    }

    if (UserShift == UTASSERT_CASETYPE_NONE || UserShift >= UTASSERT_CASETYPE_MAX)
    {
        UserShift = UTASSERT_CASETYPE_PASS;
    }

    BSP_UT_Global.CurrVerbosity = (2 << UserShift) - 1;

    UT_BSP_DoText(UTASSERT_CASETYPE_BEGIN, "UNIT TEST");
}

void UT_BSP_StartTestSegment(uint32 SegmentNumber, const char *SegmentName)
{
    char ReportBuffer[128];

    snprintf(ReportBuffer, sizeof(ReportBuffer), "%02u %s", (unsigned int)SegmentNumber, SegmentName);
    UT_BSP_DoText(UTASSERT_CASETYPE_BEGIN, ReportBuffer);
}

void UT_BSP_DoText(uint8 MessageType, const char *OutputMessage)
{
    const char *Prefix;
    char        Buffer[16];
    uint32      TermModeBits = OS_BSP_CONSOLEMODE_NORMAL;
    uint32      MsgEnabled   = BSP_UT_Global.CurrVerbosity >> MessageType;

    if (MsgEnabled & 1)
    {
        switch (MessageType)
        {
            case UTASSERT_CASETYPE_ABORT:
                TermModeBits = OS_BSP_CONSOLEMODE_HIGHLIGHT | OS_BSP_CONSOLEMODE_RED;
                Prefix       = "ABORT";
                break;
            case UTASSERT_CASETYPE_FAILURE:
                TermModeBits = OS_BSP_CONSOLEMODE_HIGHLIGHT | OS_BSP_CONSOLEMODE_RED;
                Prefix       = "FAIL";
                break;
            case UTASSERT_CASETYPE_MIR:
                TermModeBits = OS_BSP_CONSOLEMODE_HIGHLIGHT | OS_BSP_CONSOLEMODE_RED | OS_BSP_CONSOLEMODE_GREEN;
                Prefix       = "MIR";
                break;
            case UTASSERT_CASETYPE_TSF:
                TermModeBits = OS_BSP_CONSOLEMODE_HIGHLIGHT | OS_BSP_CONSOLEMODE_RED | OS_BSP_CONSOLEMODE_BLUE;
                Prefix       = "TSF";
                break;
            case UTASSERT_CASETYPE_TTF:
                Prefix = "TTF";
                break;
            case UTASSERT_CASETYPE_NA:
                Prefix = "N/A";
                break;
            case UTASSERT_CASETYPE_BEGIN:
                OS_BSP_ConsoleOutput_Impl("\n", 1); /* add a bit of extra whitespace between tests */
                Prefix = "BEGIN";
                break;
            case UTASSERT_CASETYPE_END:
                Prefix = "END";
                break;
            case UTASSERT_CASETYPE_PASS:
                TermModeBits = OS_BSP_CONSOLEMODE_HIGHLIGHT | OS_BSP_CONSOLEMODE_GREEN;
                Prefix       = "PASS";
                break;
            case UTASSERT_CASETYPE_INFO:
                Prefix = "INFO";
                break;
            case UTASSERT_CASETYPE_DEBUG:
                Prefix = "DEBUG";
                break;
            default:
                Prefix = "OTHER";
                break;
        }

        if (MsgEnabled <= 1)
        {
            TermModeBits = OS_BSP_CONSOLEMODE_NORMAL;
        }

        snprintf(Buffer, sizeof(Buffer), "[%5s]", Prefix);

        if (TermModeBits != OS_BSP_CONSOLEMODE_NORMAL)
        {
            OS_BSP_ConsoleSetMode_Impl(TermModeBits);
        }

        OS_BSP_ConsoleOutput_Impl(Buffer, strlen(Buffer));

        if (TermModeBits != OS_BSP_CONSOLEMODE_NORMAL)
        {
            OS_BSP_ConsoleSetMode_Impl(OS_BSP_CONSOLEMODE_NORMAL);
        }

        OS_BSP_ConsoleOutput_Impl(" ", 1);
        OS_BSP_ConsoleOutput_Impl(OutputMessage, strlen(OutputMessage));
        OS_BSP_ConsoleOutput_Impl("\n", 1);
    }

    /*
     * If any ABORT (major failure) message is thrown,
     * then actually call abort() to stop the test and dump a core
     */
    if (MessageType == UTASSERT_CASETYPE_ABORT)
    {
        abort();
    }
}

void UT_BSP_DoReport(const char *File, uint32 LineNum, uint32 SegmentNum, uint32 TestSeq, uint8 MessageType,
                     const char *SubsysName, const char *ShortDesc)
{
    uint32      FileLen;
    const char *BasePtr;
    char        ReportBuffer[128];

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

void UT_BSP_DoTestSegmentReport(const char *SegmentName, const UtAssert_TestCounter_t *TestCounters)
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

void UT_BSP_EndTest(const UtAssert_TestCounter_t *TestCounters)
{
    char Message[128];

    /*
     * Only output a "summary" if there is more than one test Segment.
     * Otherwise it is a duplicate of the report already given.
     */
    if (TestCounters->TestSegmentCount > 1)
    {
        UT_BSP_DoTestSegmentReport("SUMMARY", TestCounters);
    }

    snprintf(Message, sizeof(Message), "COMPLETE: %u tests Segment(s) executed\n\n",
             (unsigned int)TestCounters->TestSegmentCount);
    OS_BSP_ConsoleOutput_Impl(Message, strlen(Message));

    if (TestCounters->CaseCount[UTASSERT_CASETYPE_FAILURE] > 0)
    {
        OS_BSP_SetExitCode(OS_ERROR);
    }
    else
    {
        OS_BSP_SetExitCode(OS_SUCCESS);
    }
}
