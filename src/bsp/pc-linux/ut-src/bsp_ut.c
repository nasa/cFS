/******************************************************************************
** File:  bsp_ut.c
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
**   BSP unit test implementation functions.
**
** History:
**   Created on: Feb 10, 2015
**
******************************************************************************/

/*
 * NOTE - This entire source file is only relevant for unit testing.
 * It should not be included in a "normal" BSP build.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "osapi.h"
#include "utbsp.h"
#include "uttest.h"

static const char BSP_TERMCODE_HIGHLIGHT[] = "\x1b[31m";
static const char BSP_TERMCODE_NORMAL[] =    "\x1b[0m";
static const char BSP_TERMCODE_NONE[] = "";

/*
**  External Declarations
*/
extern OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES];

/*
**  Local Variables
*/
static int32 NumUserOptions = 0;
static char **FirstUserOption = NULL;
static uint32 CurrVerbosity = (2 << UTASSERT_CASETYPE_PASS) - 1;
static bool EnableTermCodes = false;

/*
 * UT_BSP_GetTotalOptions: See details in prototype
 */
int32 UT_BSP_GetTotalOptions(void)
{
    return NumUserOptions;
}

/*
 * UT_BSP_GetOptionString: See details in prototype
 */
const char * UT_BSP_GetOptionString(int32 OptionNum)
{
    if (OptionNum >= NumUserOptions)
    {
        return NULL;
    }

    return FirstUserOption[OptionNum];
}



void UT_BSP_ParseCommandLine(int argc, char *argv[])
{
    uint8 UserShift;
    int opt;

    UserShift = UTASSERT_CASETYPE_NONE;
    while ((opt = getopt(argc, argv, "v:qd")) != -1)
    {
        switch (opt) {
        case 'd':
            UserShift = UTASSERT_CASETYPE_DEBUG;
            break;
        case 'q':
            UserShift = UTASSERT_CASETYPE_FAILURE;
            break;
        case 'v':
            UserShift = atoi(optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-v verbosity] [-d] [-q]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
        if (UserShift > 0 && UserShift < UTASSERT_CASETYPE_MAX)
        {
            CurrVerbosity = (2 << UserShift) - 1;
        }
    }

    if (optind < argc)
    {
        NumUserOptions = argc - optind;
        FirstUserOption = &argv[optind];
    }
}

void UT_BSP_Setup(const char *Name)
{
    int      mode;
    uint32   i;
    struct stat statbuf;

    /*
     * Enable terminal codes only if stdout is actually terminal.
     * This should prevent log files from having escape codes, for
     * when output is redirected to a file.
     */
    EnableTermCodes = isatty(STDOUT_FILENO);


    UT_BSP_DoText(UTASSERT_CASETYPE_BEGIN, Name);

    /*
    ** Create local directories for "disk" mount points
    **  See bsp_voltab for the values
    **
    ** NOTE - the voltab table is poorly designed here; values of "0" are valid
    ** and will translate into an entry that is actually used.  In particular the
    ** "free" flag has to be actually initialized to TRUE to say its NOT valid.
    ** So in the case of an entry that has been zeroed out (i.e. bss section) it
    ** will be treated as a valid entry.
    **
    ** Checking that the DeviceName starts with a leading slash '/' is a workaround
    ** for this, and may be the only way to detect an entry that is uninitialized.
    */
    mode = S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO;
    for (i=0; i < NUM_TABLE_ENTRIES; ++i)
    {
        if (OS_VolumeTable[i].VolumeType == FS_BASED &&
                OS_VolumeTable[i].PhysDevName[0] != 0 &&
                OS_VolumeTable[i].DeviceName[0] == '/')

        {
            if (stat(OS_VolumeTable[i].PhysDevName, &statbuf) < 0)
            {
                printf("Creating mount point directory: %s\n",
                        OS_VolumeTable[i].PhysDevName);
                mkdir(OS_VolumeTable[i].PhysDevName, mode);
            }
        }
    }
}

void UT_BSP_StartTestSegment(uint32 SegmentNumber, const char *SegmentName)
{
    char ReportBuffer[128];

    snprintf(ReportBuffer,sizeof(ReportBuffer), "%02u %s", (unsigned int)SegmentNumber, SegmentName);
    UT_BSP_DoText(UTASSERT_CASETYPE_BEGIN, ReportBuffer);
}

void UT_BSP_DoText(uint8 MessageType, const char *OutputMessage)
{
   const char *Prefix;
   const char *ControlCodeStart = NULL;
   const char *ControlCodeEnd = NULL;

   if ((CurrVerbosity >> MessageType) & 1)
   {
      switch(MessageType)
      {
      case UTASSERT_CASETYPE_ABORT:
         Prefix = "ABORT";
         break;
      case UTASSERT_CASETYPE_FAILURE:
         ControlCodeStart = BSP_TERMCODE_HIGHLIGHT;
         Prefix = "FAIL";
         break;
      case UTASSERT_CASETYPE_MIR:
         Prefix = "MIR";
         break;
      case UTASSERT_CASETYPE_TSF:
         Prefix = "TSF";
         break;
      case UTASSERT_CASETYPE_TTF:
          Prefix = "TTF";
          break;
      case UTASSERT_CASETYPE_NA:
         Prefix = "N/A";
         break;
      case UTASSERT_CASETYPE_BEGIN:
         printf("\n"); /* add a bit of extra whitespace between tests */
         Prefix = "BEGIN";
         break;
      case UTASSERT_CASETYPE_END:
         Prefix = "END";
         break;
      case UTASSERT_CASETYPE_PASS:
         Prefix = "PASS";
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

      if (!EnableTermCodes || ControlCodeStart == NULL)
      {
          ControlCodeStart = BSP_TERMCODE_NONE;
          ControlCodeEnd = BSP_TERMCODE_NONE;
      }
      else if (ControlCodeEnd == NULL)
      {
          ControlCodeEnd = BSP_TERMCODE_NORMAL;
      }
      printf("[%s%5s%s] %s\n",ControlCodeStart,Prefix,ControlCodeEnd,OutputMessage);
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

void UT_BSP_DoReport(const char *File, uint32 LineNum, uint32 SegmentNum, uint32 TestSeq, uint8 MessageType, const char *SubsysName, const char *ShortDesc)
{
    uint32 FileLen;
    const char *BasePtr;
    char ReportBuffer[128];

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

    snprintf(ReportBuffer,sizeof(ReportBuffer), "%02u.%03u %s:%u - %s",
            (unsigned int)SegmentNum, (unsigned int)TestSeq,
            BasePtr, (unsigned int)LineNum, ShortDesc);

    UT_BSP_DoText(MessageType, ReportBuffer);
}

void UT_BSP_DoTestSegmentReport(const char *SegmentName, const UtAssert_TestCounter_t *TestCounters)
{
    char ReportBuffer[128];

    snprintf(ReportBuffer,sizeof(ReportBuffer),
            "%02u %-20s TOTAL::%-4u  PASS::%-4u  FAIL::%-4u   MIR::%-4u   TSF::%-4u   N/A::%-4u\n",
            (unsigned int)TestCounters->TestSegmentCount,
            SegmentName,
            (unsigned int)TestCounters->TotalTestCases,
            (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_PASS],
            (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_FAILURE],
            (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_MIR],
            (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_TSF],
            (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_NA]);


    UT_BSP_DoText(UTASSERT_CASETYPE_END, ReportBuffer);
}

void UT_BSP_EndTest(const UtAssert_TestCounter_t *TestCounters)
{
   int status = 0;

   /*
    * Only output a "summary" if there is more than one test Segment.
    * Otherwise it is a duplicate of the report already given.
    */
   if (TestCounters->TestSegmentCount > 1)
   {
       UT_BSP_DoTestSegmentReport("SUMMARY", TestCounters);
   }

   printf("COMPLETE: %u tests Segment(s) executed\n\n", (unsigned int)TestCounters->TestSegmentCount);

   /*
    * The Linux UT BSP allows at least a 7 bit status code to be returned to the OS (i.e. the exit status
    * of the process).  This is useful to report pass/fail.  Because we have multiple bits, we can make
    * descriptive exit status codes to indicate what went wrong.  Anything nonzero represents failure.
    *
    * Consider Failures as well as "TSF" (setup failures) to be grounds for returning nonzero (bad) status.
    * Also the lack of ANY test cases should produce a bad status.
    *
    * "MIR" results should not produce a bad status -- these may have worked fine, we do not know.
    *
    * Likewise "N/A" tests are simply not applicable, so we just ignore them.
    */

   if (TestCounters->TotalTestCases == 0)
   {
      status |= 0x01;
   }

   if (TestCounters->CaseCount[UTASSERT_CASETYPE_FAILURE] > 0)
   {
      status |= 0x02;
   }

   if (TestCounters->CaseCount[UTASSERT_CASETYPE_TSF] > 0)
   {
      status |= 0x04;
   }

   exit(status);
}

/******************************************************************************
**  Function:  main()
**
**  Purpose:
**    BSP Unit Test Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

int main(int argc, char *argv[])
{
   UT_BSP_Setup("PC-LINUX UNIT TEST");
   UT_BSP_ParseCommandLine(argc, argv);

   /*
   ** Call application specific entry point.
   */
   OS_Application_Startup();

   /*
   ** In unit test mode, call the UtTest_Run function (part of UT Assert library)
   */
   UtTest_Run();
   UT_BSP_EndTest(UtAssert_GetCounters());

   /* Should typically never get here */
   return(EXIT_SUCCESS);
}

