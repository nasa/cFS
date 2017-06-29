/******************************************************************************
** File:  bsp_start.c
**
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement. 
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. 
**
**
** Purpose:
**   OSAL BSP main entry point.
**
******************************************************************************/

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

/*
**  External Declarations
*/
void OS_Application_Startup(void);

extern rtems_status_code rtems_ide_part_table_initialize (const char* );

/*
** cFE includes
*/
#include "common_types.h"
#include "osapi.h"

#ifdef _UNIT_TEST_
#include "utbsp.h"
#include "uttest.h"
#endif

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

rtems_id          RtemsTimerId;

/*
 * The RTEMS shell needs a function to check the validity of a login username/password
 * This is just a stub that always passes.
 */
bool BSP_Login_Check(const char *user, const char *passphrase)
{
   return TRUE;
}


/*
** 1 HZ Timer "ISR"
*/
int timer_count = 0;

void BSP_Setup(void)
{
   int status;

#ifdef _UNIT_TEST_
   UT_BSP_Setup("PC-RTEMS UNIT TEST");
#endif

   OS_printf( "\n\n*** RTEMS Info ***\n" );
   OS_printf("%s", _Copyright_Notice );
   OS_printf("%s\n\n", _RTEMS_version );
   OS_printf(" Stack size=%d\n", (int)Configuration.stack_space_size );
   OS_printf(" Workspace size=%d\n",   (int) Configuration.work_space_size );
   OS_printf("\n");
   OS_printf( "*** End RTEMS info ***\n\n" );

   /*
   ** Create the RTEMS Root file system
   */
   status = rtems_create_root_fs();
   if (status != RTEMS_SUCCESSFUL)
   {
       OS_printf("Creating Root file system failed: %s\n",rtems_status_text(status));
   }

   /*
   ** create the directory mountpoints
   */
   status = mkdir("/ram", S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO); /* For ramdisk mountpoint */
   if (status != RTEMS_SUCCESSFUL)
   {
       OS_printf("mkdir failed: %s\n", strerror (errno));
   }

   status = mkdir("/cf", S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO); /* For EEPROM mountpoint */
   if (status != RTEMS_SUCCESSFUL)
   {
       OS_printf("mkdir failed: %s\n", strerror (errno));
       return;
   }

   /*
    * Register the IDE partition table.
    */
   status = rtems_ide_part_table_initialize ("/dev/hda");
   if (status != RTEMS_SUCCESSFUL)
   {
     OS_printf ("error: ide partition table not found: %s / %s\n",
             rtems_status_text (status),strerror(errno));
   }

   status = mount("/dev/hda1", "/cf",
         RTEMS_FILESYSTEM_TYPE_DOSFS,
         RTEMS_FILESYSTEM_READ_WRITE,
         NULL);
   if (status < 0)
   {
     OS_printf ("mount failed: %s\n", strerror (errno));
   }

}

/*
** A simple entry point to start from the loader
*/
rtems_task Init(
  rtems_task_argument ignored
)
{
   BSP_Setup();

   if (rtems_shell_init("SHLL", RTEMS_MINIMUM_STACK_SIZE * 4, 100, "/dev/console", false, false, BSP_Login_Check) < 0)
   {
     OS_printf ("shell init failed: %s\n", strerror (errno));
   }

   /*
   ** Call application specific entry point.
   ** This is supposed to call OS_API_Init()
   */
   OS_Application_Startup();

#ifdef _UNIT_TEST_

   /*
   ** In unit test mode, call the UtTest_Run function (part of UT Assert library)
   */
   UtTest_Run();
   UT_BSP_EndTest(UtAssert_GetCounters());

#else

   /*
   ** OS_IdleLoop() will wait forever and return if
   ** someone calls OS_ApplicationShutdown(TRUE)
   */
   OS_IdleLoop();

#endif

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


