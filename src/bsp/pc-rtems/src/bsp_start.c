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
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <bsp.h>
#include <rtems.h>
#include <rtems/mkrootfs.h>
#include <rtems/bdbuf.h>
#include <rtems/blkdev.h>
#include <rtems/diskdevs.h>
#include <rtems/bdpart.h>
#include <rtems/error.h>
#include <rtems/ramdisk.h>
#include <rtems/dosfs.h>
#include <rtems/fsmount.h>
#include <rtems/shell.h>
#include <rtems/rtl/dlfcn-shell.h>

#include "pcrtems_bsp_internal.h"

/*
**  External Declarations
*/
extern rtems_status_code rtems_ide_part_table_initialize(const char *);
extern int rtems_rtl_shell_command (int argc, char* argv[]);

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
 * Additional shell commands for the RTL functionality
 */
rtems_shell_cmd_t rtems_shell_RTL_Command = {
  .name = "rtl",
  .usage = "rtl COMMAND...",
  .topic = "misc",
  .command = rtems_rtl_shell_command
};
rtems_shell_cmd_t rtems_shell_dlopen_Command = {
  .name = "dlopen",
  .usage = "dlopen COMMAND...",
  .topic = "misc",
  .command = shell_dlopen
};
rtems_shell_cmd_t rtems_shell_dlsym_Command = {
  .name = "dlsym",
  .usage = "dlsym COMMAND...",
  .topic = "misc",
  .command = shell_dlsym
};


/*
** Global variables
*/
OS_BSP_PcRtemsGlobalData_t OS_BSP_PcRtemsGlobal;

void OS_BSP_Setup(void)
{
    int          status;
    unsigned int i;
    struct stat  statbuf;
    const char * cfpart;
    const char * cmdlinestr;
    const char * cmdp;
    char *       cmdi, *cmdo;

    cmdlinestr = bsp_cmdline();

    printf("\n\n*** RTEMS Info ***\n");
    printf("%s", _Copyright_Notice);
    printf("%s\n\n", _RTEMS_version);
    printf(" Stack size=%d\n", (int)Configuration.stack_space_size);
    printf(" Workspace size=%d\n", (int)Configuration.work_space_size);
    if (cmdlinestr != NULL)
    {
        printf(" Bootloader Command Line: %s\n", cmdlinestr);
    }

    printf("\n");
    printf("*** End RTEMS info ***\n\n");

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
                    cmdo = OS_BSP_PcRtemsGlobal.UserArgBuffer;
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
                if (strcmp(cmdi, "--batch-mode") == 0)
                {
                    OS_BSP_PcRtemsGlobal.BatchMode = true;
                }
                else if (OS_BSP_Global.ArgC < RTEMS_MAX_USER_OPTIONS)
                {
                    /* save other args for app */
                    OS_BSP_Global.ArgV[OS_BSP_Global.ArgC] = cmdi;
                    ++OS_BSP_Global.ArgC;
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
        printf("Creating Root file system failed: %s\n", rtems_status_text(status));
    }

    /*
     * Register the IDE partition table.
     */
    status = rtems_ide_part_table_initialize("/dev/hda");
    if (status != RTEMS_SUCCESSFUL)
    {
        /* note this is not necessarily an error, it just means there
         * will be no persistent storage in this instance.  The IMFS
         * is still available. */
        BSP_DEBUG("warning: /dev/hda partition table not found: %s / %s\n", rtems_status_text(status), strerror(errno));
        BSP_DEBUG("Persistent storage will NOT be mounted\n");
        cfpart = NULL;
    }
    else
    {
        cfpart = "/dev/hda1";
    }

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
    for (i = 0; i < NUM_TABLE_ENTRIES; ++i)
    {
        if (OS_VolumeTable[i].VolumeType == FS_BASED && OS_VolumeTable[i].PhysDevName[0] != 0 &&
            OS_VolumeTable[i].DeviceName[0] == '/')

        {
            if (stat(OS_VolumeTable[i].PhysDevName, &statbuf) < 0)
            {
                status = mkdir(OS_VolumeTable[i].PhysDevName,
                               S_IFDIR | S_IRWXU | S_IRWXG | S_IRWXO); /* For ramdisk mountpoint */
                if (status < 0)
                {
                    printf("mkdir failed: %s\n", strerror(errno));
                }
            }
            if (cfpart != NULL && strcmp(OS_VolumeTable[i].MountPoint, "/cf") == 0)
            {
                status = mount(cfpart, OS_VolumeTable[i].PhysDevName, RTEMS_FILESYSTEM_TYPE_DOSFS,
                               RTEMS_FILESYSTEM_READ_WRITE, NULL);
                if (status < 0)
                {
                    printf("mount failed: %s\n", strerror(errno));
                }
            }
        }
    }

    /*
     * Start the shell now, before any application starts.
     * This way, if there is an issue with the application startup,
     * the shell can still be used to debug the system.
     *
     * The shell is _NOT_ started if the "--batch-mode" switch is
     * given (this means to run completely autonomous)
     */
    if (!OS_BSP_PcRtemsGlobal.BatchMode)
    {
        status = rtems_shell_init("SHLL", RTEMS_MINIMUM_STACK_SIZE * 4, RTEMS_SHELL_PRIORITY, "/dev/console", false,
                                  false, NULL);
        if (status < 0)
        {
            printf("shell init failed: %d / %s\n", status, strerror(errno));
        }

        /* give a small delay to let the shell start,
           avoids having the login prompt show up mid-test,
           and gives a little time for pending output to actually
           be sent to the console in case of a slow port */
        rtems_task_wake_after(50);
    }

    printf("\n\n");
}

/* ---------------------------------------------------------
    OS_BSP_GetReturnStatus()

     Helper function to convert an OSAL status code into
     a code suitable for returning to the OS.
   --------------------------------------------------------- */
rtems_status_code OS_BSP_GetReturnStatus(void)
{
    rtems_status_code retcode;
    const char *      StatusStr;

    switch (OS_BSP_Global.AppStatus)
    {
    case OS_SUCCESS:
        /* translate OS_SUCCESS to the system RTEMS_SUCCESSFUL value */
        StatusStr = "SUCCESS";
        retcode = RTEMS_SUCCESSFUL;
        break;

    default:
        /* translate anything else to a generic non-success code,
         * this basically just means the main task exited */
        StatusStr = "ERROR";
        retcode = RTEMS_TASK_EXITTED;
        break;
    }

    printf("\nApplication exit status: %s (%d)\n\n", StatusStr, (int)OS_BSP_Global.AppStatus);
    rtems_task_wake_after(100);

    return retcode;
}

/*
 ** A simple entry point to start from the loader
 */
rtems_task Init(rtems_task_argument ignored)
{
    /*
     * Initially clear the global object
     */
    memset(&OS_BSP_Global, 0, sizeof(OS_BSP_Global));
    memset(&OS_BSP_PcRtemsGlobal, 0, sizeof(OS_BSP_PcRtemsGlobal));

    /*
     * Perform BSP setup -
     * Initialize the root file system, create mount points, etc.
     */
    OS_BSP_Setup();

    /*
     * Call application specific entry point.
     * This should set up all user tasks and resources, then return
     */
    OS_Application_Startup();

    /*
     * OS_Application_Run() implements the background task.
     * The user application may provide this, or a default implementation
     * is used which just calls OS_IdleLoop().
     */
    OS_Application_Run();

    /*
     * Not calling exit() under RTEMS, this simply shuts down the executive,
     * forcing the user to reboot the system.
     *
     * Calling suspend causes execution to get stuck here, but the RTEMS
     * shell thread will still be active so the user can poke around, read results,
     * then use a shell command to reboot when ready.
     */
    while (!OS_BSP_PcRtemsGlobal.BatchMode)
    {
        printf("\n\nInit thread idle.\nPress <enter> for shell or reset machine...\n\n");
        rtems_task_suspend(rtems_task_self());
    }

    rtems_shutdown_executive(OS_BSP_GetReturnStatus());
}

/* configuration information */

/*
** RTEMS OS Configuration defintions
*/
#define TASK_INTLEVEL 0
#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_ATTRIBUTES  (RTEMS_FLOATING_POINT | RTEMS_PREEMPT | RTEMS_NO_TIMESLICE | RTEMS_ASR | RTEMS_INTERRUPT_LEVEL(TASK_INTLEVEL))
#define CONFIGURE_INIT_TASK_STACK_SIZE  (20*1024)
#define CONFIGURE_INIT_TASK_PRIORITY    10

/*
 * Note that these resources are shared with RTEMS itself (e.g. the init task, the shell)
 * so they should be allocated slightly higher than the user limits in osconfig.h
 *
 * Many RTEMS services use tasks internally, including the idle task, BSWP, ATA driver,
 * low level console I/O, the shell, TCP/IP network stack, and DHCP (if enabled).
 * Many of these also use semaphores for synchronization.
 *
 * Budgeting for additional:
 *   8 internal tasks
 *   2 internal timers
 *   4 internal queues
 *   16 internal semaphores
 *
 */
#define CONFIGURE_MAXIMUM_TASKS                      (OS_MAX_TASKS + 8)
#define CONFIGURE_MAXIMUM_TIMERS                     (OS_MAX_TIMERS + 2)
#define CONFIGURE_MAXIMUM_SEMAPHORES                 (OS_MAX_BIN_SEMAPHORES + OS_MAX_COUNT_SEMAPHORES + OS_MAX_MUTEXES + 16)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES             (OS_MAX_QUEUES + 4)
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS     (OS_MAX_NUM_OPEN_FILES + 8)
#define CONFIGURE_MAXIMUM_DRIVERS                    10
#define CONFIGURE_MAXIMUM_POSIX_KEYS                 4

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_FILESYSTEM_RFS
#define CONFIGURE_FILESYSTEM_IMFS
#define CONFIGURE_FILESYSTEM_DOSFS
#define CONFIGURE_FILESYSTEM_DEVFS
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
#define CONFIGURE_APPLICATION_NEEDS_IDE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER

#define CONFIGURE_EXECUTIVE_RAM_SIZE                 (8*1024*1024)
#define CONFIGURE_MICROSECONDS_PER_TICK              10000
#define CONFIGURE_ATA_DRIVER_TASK_PRIORITY           9

#include <rtems/confdefs.h>

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS

#define CONFIGURE_SHELL_USER_COMMANDS   \
    &rtems_shell_RTL_Command,           \
    &rtems_shell_dlopen_Command,        \
    &rtems_shell_dlsym_Command


#include <rtems/shellconfig.h>
