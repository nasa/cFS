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

#include "osconfig.h"
#include "utbsp.h"
#include "uttest.h"

/*
**  External Declarations
*/
void OS_Application_Startup(void);

extern rtems_status_code rtems_ide_part_table_initialize (const char* );


#define RTEMS_NUMBER_OF_RAMDISKS 1


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
uint32 CurrVerbosity = (2 << UTASSERT_CASETYPE_PASS) - 1;

/*
 * The RTEMS shell needs a function to check the validity of a login username/password
 * This is just a stub that always passes.
 */
bool BSP_Login_Check(const char *user, const char *passphrase)
{
   return TRUE;
}


void UT_BSP_Setup(const char *Name)
{
    int status;

    printf( "\n\n*** RTEMS Info ***\n" );
    printf("%s", Name );
    printf("%s", _Copyright_Notice );
    printf("%s\n\n", _RTEMS_version );
    printf(" Stack size=%d\n", (int)Configuration.stack_space_size );
    printf(" Workspace size=%d\n",   (int) Configuration.work_space_size );
    printf("\n");
    printf( "*** End RTEMS info ***\n\n" );

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
    status = mkdir("/ram", S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO); /* For ramdisk mountpoint */
    if (status != RTEMS_SUCCESSFUL)
    {
        printf("mkdir failed: %s\n", strerror (errno));
    }

    status = mkdir("/cf", S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO); /* For EEPROM mountpoint */
    if (status != RTEMS_SUCCESSFUL)
    {
        printf("mkdir failed: %s\n", strerror (errno));
        return;
    }

    /*
     * Register the IDE partition table.
     */
    status = rtems_ide_part_table_initialize ("/dev/hda");
    if (status != RTEMS_SUCCESSFUL)
    {
      printf ("error: ide partition table not found: %s / %s\n",
              rtems_status_text (status),strerror(errno));
    }

    status = mount("/dev/hda1", "/cf",
          RTEMS_FILESYSTEM_TYPE_DOSFS,
          RTEMS_FILESYSTEM_READ_WRITE,
          NULL);
    if (status < 0)
    {
      printf ("mount failed: %s\n", strerror (errno));
    }

    status = rtems_shell_init("SHLL", RTEMS_MINIMUM_STACK_SIZE * 4, 100, "/dev/console", false, false, BSP_Login_Check);
    if (status < 0)
    {
      printf ("shell init failed: %s\n", strerror (errno));
    }


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
   /*
    * Only output a "summary" if there is more than one test Segment.
    * Otherwise it is a duplicate of the report already given.
    */
   if (TestCounters->TestSegmentCount > 1)
   {
       UT_BSP_DoTestSegmentReport("SUMMARY", TestCounters);
   }

   printf("COMPLETE: %u test segment(s) executed\n\n", (unsigned int)TestCounters->TestSegmentCount);

   /*
    * Not calling exit() under RTEMS, this simply shuts down the executive,
    * forcing the user to reboot the system.
    *
    * Calling "pause()" in a loop causes execution to get stuck here, but the RTEMS
    * shell thread will still be active so the user can poke around, read results,
    * then use a shell command to reboot when ready.
    */
   while (TRUE)
   {
       pause();
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
#define CONFIGURE_MAXIMUM_SEMAPHORES                 (OS_MAX_BIN_SEMAPHORES + OS_MAX_COUNT_SEMAPHORES + OS_MAX_MUTEXES + 4)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES             (OS_MAX_QUEUES + 4)


#define CONFIGURE_EXECUTIVE_RAM_SIZE    (1024*1024)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS     100

#define CONFIGURE_FILESYSTEM_RFS
#define CONFIGURE_FILESYSTEM_IMFS
#define CONFIGURE_FILESYSTEM_DOSFS
#define CONFIGURE_FILESYSTEM_DEVFS

#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_MICROSECONDS_PER_TICK              10000

#define CONFIGURE_MAXIMUM_DRIVERS                   10

#define CONFIGURE_APPLICATION_NEEDS_IDE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER
#define CONFIGURE_ATA_DRIVER_TASK_PRIORITY         9

#define CONFIGURE_MAXIMUM_POSIX_KEYS               4

#include <rtems/confdefs.h>

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS

#include <rtems/shellconfig.h>


