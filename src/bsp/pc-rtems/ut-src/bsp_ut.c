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

#define _USING_RTEMS_INCLUDES_

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <rtems.h>
#include <rtems/mkrootfs.h>
#include <rtems/bdbuf.h>
#include <rtems/blkdev.h>
#include <rtems/diskdevs.h>
#include <rtems/error.h>
#include <rtems/ramdisk.h>
#include <rtems/dosfs.h>
#include <rtems/fsmount.h>
#include <rtems/shell.h>
#include <bsp.h>

#include "osapi.h"
#include "utbsp.h"
#include "uttest.h"

/*
**  External Declarations
*/
extern OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES];

void OS_Application_Startup(void);

#define RTEMS_NUMBER_OF_RAMDISKS  1
#define RTEMS_UT_MAX_USER_OPTIONS 4
#define RTEMS_UT_MAX_CMDLINE      256

/*
** Global variables
*/

/*
 * The RAM Disk configuration.
 */
rtems_ramdisk_config rtems_ramdisk_configuration[RTEMS_NUMBER_OF_RAMDISKS];

/*
 * The number of RAM Disk configurations.
*/
size_t rtems_ramdisk_configuration_size = RTEMS_NUMBER_OF_RAMDISKS;

/*
** RAM Disk IO op table.
*/
rtems_driver_address_table rtems_ramdisk_io_ops =
{
        .initialization_entry = ramdisk_initialize,
        .open_entry =           rtems_blkdev_generic_open,
        .close_entry =          rtems_blkdev_generic_close,
        .read_entry =           rtems_blkdev_generic_read,
        .write_entry =          rtems_blkdev_generic_write,
        .control_entry =        rtems_blkdev_generic_ioctl
};

/*
 * Under RTEMS there is no notion of command-line arguments like in pc-linux,
 * so it is not as easy to change this value at runtime.  For now the default
 * will show all messages except debug.
 *
 * It may be possible to set this value using the shell...
 */
static bool BatchMode = false;
static int32 UserArgc = 0;
static char UserArgBuffer[RTEMS_UT_MAX_CMDLINE];
static char *UserArgv[RTEMS_UT_MAX_USER_OPTIONS] = { NULL };
static uint32 CurrVerbosity = (2 << UTASSERT_CASETYPE_PASS) - 1;

/*
 * UT_BSP_GetTotalOptions: See details in prototype
 */
int32 UT_BSP_GetTotalOptions(void)
{
    return UserArgc;
}

/*
 * UT_BSP_GetOptionString: See details in prototype
 */
const char * UT_BSP_GetOptionString(int32 OptionNum)
{
    if (OptionNum >= UserArgc)
    {
        return NULL;
    }

    return UserArgv[OptionNum];
}


void UT_BSP_Setup(const char *Name)
{
    int status;
    int i;
    struct stat statbuf;
    const char *cmdlinestr;
    const char *cmdp;
    char *cmdi, *cmdo;

    cmdlinestr = bsp_cmdline();
    printf( "\n\n*** RTEMS Info ***\n" );
    printf("%s", _Copyright_Notice );
    printf("%s\n\n", _RTEMS_version );
    printf(" Stack size=%d\n", (int)Configuration.stack_space_size );
    printf(" Workspace size=%d\n",   (int) Configuration.work_space_size );
    if (cmdlinestr != NULL)
    {
        printf(" Bootloader Command Line: %s\n", cmdlinestr);
    }
    printf("\n");
    printf( "*** End RTEMS info ***\n\n" );

    /*
     * Parse command line string (passed in from bootloader)
     *
     * Known arguments are handled here, and unknown args are
     * saved for the UT application.
     *
     * Batch mode is intended for non-interative execution.
     *
     * It does two things:
     * - do not start the shell task
     * - when tests are complete, shutdown the executive
     *
     * The BSP should be configured with these options to
     * make this most useful:
     *   USE_COM1_AS_CONSOLE=1
     *   BSP_PRESS_KEY_FOR_RESET=0
     *   BSP_RESET_BOARD_AT_EXIT=1
     *
     * This way all the test output will be sent to COM1
     * and then immediately resets the CPU when done.
     *
     * When running under QEMU the "-no-reboot" flag is
     * also useful to shutdown QEMU rather than resetting.
     */
    if (cmdlinestr != NULL)
    {
        cmdp = cmdlinestr;
        cmdo = NULL;
        cmdi = NULL;
        while (1)
        {
            if (isgraph((int)*cmdp))
            {
                if (cmdo == NULL)
                {
                    cmdo = UserArgBuffer;
                }
                else
                {
                    ++cmdo;
                }
                if (cmdi == NULL)
                {
                    cmdi = cmdo;
                }
                *cmdo = *cmdp;
            }
            else if (cmdi != NULL)
            {
                ++cmdo;
                *cmdo = 0;
                if (strcmp(cmdi,"--batch-mode") == 0)
                {
                    BatchMode = true;
                }
                else if (UserArgc < RTEMS_UT_MAX_USER_OPTIONS)
                {
                    /* save other args for app */
                    UserArgv[UserArgc] = cmdi;
                    ++UserArgc;
                }
                cmdi = NULL;
            }

            if (*cmdp == 0)
            {
                break;
            }

            ++cmdp;
        }
    }

    /*
    ** Create the RTEMS Root file system
    */
    status = rtems_create_root_fs();
    if (status != RTEMS_SUCCESSFUL)
    {
        printf("Creating Root file system failed: %s\n",rtems_status_text(status));
    }

    /*
    ** create the directory mountpoints
    */

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
                status = mkdir(OS_VolumeTable[i].PhysDevName, S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO); /* For ramdisk mountpoint */
                if (status != RTEMS_SUCCESSFUL)
                {
                    printf("mkdir failed: %s\n", strerror (errno));
                }
            }
        }
    }

    if (!BatchMode)
    {
        status = rtems_shell_init("SHLL", RTEMS_MINIMUM_STACK_SIZE * 4, 100, "/dev/console", false, false, NULL);
        if (status < 0)
        {
          printf ("shell init failed: %s\n", strerror (errno));
        }

    }

    printf("\n\n");

    /* give a small delay to let the shell start,
       avoids having the login prompt show up mid-test,
       and gives a little time for pending output to actually
       be sent to the console in case of a slow port */
    rtems_task_wake_after(50);

    UT_BSP_DoText(UTASSERT_CASETYPE_BEGIN, Name);
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

   if ((CurrVerbosity >> MessageType) & 1)
   {
      switch(MessageType)
      {
      case UTASSERT_CASETYPE_ABORT:
         Prefix = "ABORT";
         break;
      case UTASSERT_CASETYPE_FAILURE:
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
      printf("[%5s] %s\n",Prefix,OutputMessage);
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
   const char *Overall;

   /*
    * Only output a "summary" if there is more than one test Segment.
    * Otherwise it is a duplicate of the report already given.
    */
   if (TestCounters->TestSegmentCount > 1)
   {
       UT_BSP_DoTestSegmentReport("SUMMARY", TestCounters);
   }

   printf("COMPLETE: %u test segment(s) executed\n", (unsigned int)TestCounters->TestSegmentCount);

   /*
    * Since this test is probably not running directly on the
    * host but rather on a separate target or emulator, we cannot
    * rely on a return code / exit status to identify pass/fail
    * as is done on the Linux UT.
    *
    * This outputs a simplified "RESULT: <STATUS>" line as the
    * final output line.  A test script can grep for this line.
    */
   if (TestCounters->CaseCount[UTASSERT_CASETYPE_FAILURE] > 0 ||
           TestCounters->CaseCount[UTASSERT_CASETYPE_ABORT] > 0)
   {
       Overall = "FAILURE";
   }
   else if (TestCounters->CaseCount[UTASSERT_CASETYPE_TSF] > 0 ||
           TestCounters->CaseCount[UTASSERT_CASETYPE_TTF] > 0)
   {
       Overall = "TSF/TTF";
   }
   else if (TestCounters->CaseCount[UTASSERT_CASETYPE_PASS] > 0)
   {
       Overall = "SUCCESS";
   }
   else
   {
       /* no tests failed, but no tests passed either... */
       Overall = "UNKNOWN";
   }

   printf("RESULT: %s\n", Overall);

   /*
    * Not calling exit() under RTEMS, this simply shuts down the executive,
    * forcing the user to reboot the system.
    *
    * Calling suspend causes execution to get stuck here, but the RTEMS
    * shell thread will still be active so the user can poke around, read results,
    * then use a shell command to reboot when ready.
    */
   while (!BatchMode)
   {
       printf("\n\nTesting thread now idle.\nPress <enter> for shell or reset machine...\n\n");
       rtems_task_suspend(rtems_task_self());
   }
}


/*
** A simple entry point to start from the loader
*/
rtems_task Init(
  rtems_task_argument ignored
)
{
   UT_BSP_Setup("PC-RTEMS UNIT TEST");

   /*
   ** Call application specific entry point.
   ** This is supposed to call OS_API_Init()
   */
   OS_Application_Startup();

   /*
   ** In unit test mode, call the UtTest_Run function (part of UT Assert library)
   */
   UtTest_Run();
   UT_BSP_EndTest(UtAssert_GetCounters());

   rtems_shutdown_executive(UtAssert_GetFailCount() != 0);
}


/* configuration information */

/*
** RTEMS OS Configuration defintions
*/
#define TASK_INTLEVEL 0
#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_ATTRIBUTES  (RTEMS_FLOATING_POINT | RTEMS_PREEMPT | RTEMS_NO_TIMESLICE | RTEMS_ASR | RTEMS_INTERRUPT_LEVEL(TASK_INTLEVEL))
#define CONFIGURE_INIT_TASK_STACK_SIZE  (20*1024)
#define CONFIGURE_INIT_TASK_PRIORITY    120

/*
 * Note that these resources are shared with RTEMS itself (e.g. the shell)
 * so they should be allocated slightly higher than the limits in osconfig.h
 */
#define CONFIGURE_MAXIMUM_TASKS                      (OS_MAX_TASKS + 4)
#define CONFIGURE_MAXIMUM_TIMERS                     (OS_MAX_TIMERS + 2)
#define CONFIGURE_MAXIMUM_SEMAPHORES                 (OS_MAX_BIN_SEMAPHORES + OS_MAX_COUNT_SEMAPHORES + OS_MAX_MUTEXES + 2)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES             (OS_MAX_QUEUES + 2)


#define CONFIGURE_EXECUTIVE_RAM_SIZE    (1024*1024)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS     (OS_MAX_NUM_OPEN_FILES + 8)

#define CONFIGURE_FILESYSTEM_RFS
#define CONFIGURE_FILESYSTEM_IMFS
#define CONFIGURE_FILESYSTEM_DEVFS

#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_MICROSECONDS_PER_TICK              10000
#define CONFIGURE_MAXIMUM_DRIVERS                   10
#define CONFIGURE_MAXIMUM_POSIX_KEYS               4

#include <rtems/confdefs.h>

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL

#include <rtems/shellconfig.h>


