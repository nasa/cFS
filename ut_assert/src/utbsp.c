/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * File:  utbsp.c
 *
 * Purpose:
 *   Unit test BSP interface functions.
 *
 *   This file provides the bindings between the OSAL BSP and UT assert
 *   when directly running a test program as a standalone OSAL application.
 *
 *   It is not used when loading UT assert into another application (e.g. CFE).
 */

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
     * then call a BSP-provided routine to stop the test and possibly dump a core
     */
    if (MessageType == UTASSERT_CASETYPE_ABORT)
    {
        OS_BSP_Shutdown_Impl();
    }
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
        UtAssert_DoTestSegmentReport("SUMMARY", TestCounters);
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

/*
 * -------------------------------------------------------
 * ENTRY POINTS from OSAL BSP
 * -------------------------------------------------------
 */

void OS_Application_Run(void)
{
    UtTest_Run();
}

/*
 * Entry point from the BSP.
 * When linking with UT-Assert, the test framework (this library) serves
 * the role of the "application" being executed.
 *
 * There is a separate entry point (UT_Test_Setup) to configure the test cases.
 */
void OS_Application_Startup(void)
{

    UT_BSP_Setup();

    /*
     * Wrap the UtTest_Setup() function in a UT segment called "SETUP"
     * This allows any assert calls to be used and recorded during setup
     */
    UtAssert_BeginTest("SETUP");
    UtTest_Setup();
    UtAssert_EndTest();
}



