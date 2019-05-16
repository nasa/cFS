/******************************************************************************
** File:  cfe_psp_start.c
**
**
**      GSC-18128-1, "Core Flight Executive Version 6.6"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
** 
**
**
** Purpose:
**   cFE BSP main entry point.
**
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
#include <cexp.h>


extern rtems_status_code rtems_ide_part_table_initialize (const char* );

/*
** cFE includes 
*/
#include "common_types.h"
#include "osapi.h"
#include "cfe_psp.h" 
#include "cfe_psp_memory.h"

#define RTEMS_NUMBER_OF_RAMDISKS 1


/*
 * The preferred way to obtain the CFE tunable values at runtime is via
 * the dynamically generated configuration object.  This allows a single build
 * of the PSP to be completely CFE-independent.
 */
#include <target_config.h>

#define CFE_ES_MAIN_FUNCTION        (*GLOBAL_CONFIGDATA.CfeConfig->SystemMain)
#define CFE_ES_NONVOL_STARTUP_FILE  (GLOBAL_CONFIGDATA.CfeConfig->NonvolStartupFile)
#define CFE_CPU_ID                  (GLOBAL_CONFIGDATA.Default_CpuId)
#define CFE_CPU_NAME                (GLOBAL_CONFIGDATA.Default_CpuName)
#define CFE_SPACECRAFT_ID           (GLOBAL_CONFIGDATA.Default_SpacecraftId)

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

bool CFE_PSP_Login_Check(const char *user, const char *passphrase)
{
   return TRUE;
}

/*
** 1 HZ Timer "ISR"
*/
int timer_count = 0;

/******************************************************************************
**  Function:  CFE_PSP_Setup()
**
**  Purpose:
**    Perform initial setup.
**
**    The root file system is created, and mount points are created and mounted:
**     - /ram as ramdisk (RFS), read-write
**     - /boot from /dev/hda1, read-only, contain the boot executable(s) (CFE core)
**     - /cf from /dev/hdb1, read-write, containing loadable apps & tables (eeprom directory).
**
**  Arguments:
**    (none)
**
**  Return:
**    OS error code.  RTEMS_SUCCESSFUL if everything worked.
**
**  Note:
**    If this fails then CFE will not run properly, so a non-success here should
**    stop the boot so the issue can be fixed.  Trying to continue booting usually
**    just obfuscates the issue when something does not work later on.
*/
int CFE_PSP_Setup(void)
{
   int status;
   
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
       return status;
   }

   /*
   ** create the directory mountpoints
   */
   status = mkdir("/ram", S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO); /* For ramdisk mountpoint */
   if (status != RTEMS_SUCCESSFUL)
   {
       OS_printf("mkdir failed: %s\n", strerror (errno));
       return status;
   }

   status = mkdir("/boot", S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO); /* For boot disk mountpoint */
   if (status != RTEMS_SUCCESSFUL)
   {
       OS_printf("mkdir failed: %s\n", strerror (errno));
       return status;
   }

   status = mkdir("/cf", S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO); /* For EEPROM mountpoint */
   if (status != RTEMS_SUCCESSFUL)
   {
       OS_printf("mkdir failed: %s\n", strerror (errno));
       return status;
   }

   /*
    * Register the IDE partition table.
    */
   status = rtems_ide_part_table_initialize ("/dev/hda");
   if (status != RTEMS_SUCCESSFUL)
   {
     OS_printf ("error: ide partition table not found: %s / %s\n",
             rtems_status_text (status),strerror(errno));
     return status;
   }

   status = rtems_ide_part_table_initialize ("/dev/hdb");
   if (status != RTEMS_SUCCESSFUL)
   {
     OS_printf ("error: ide partition table not found: %s / %s\n",
             rtems_status_text (status),strerror(errno));
     return status;
   }

   status = mount("/dev/hda1", "/boot",
         RTEMS_FILESYSTEM_TYPE_DOSFS,
         RTEMS_FILESYSTEM_READ_ONLY,
         NULL);
   if (status < 0)
   {
     OS_printf ("mount failed: %s\n", strerror (errno));
     return status;
   }

   status = mount("/dev/hdb1", "/cf",
         RTEMS_FILESYSTEM_TYPE_DOSFS,
         RTEMS_FILESYSTEM_READ_WRITE,
         NULL);
   if (status < 0)
   {
     OS_printf ("mount failed: %s\n", strerror (errno));
     return status;
   }

   return RTEMS_SUCCESSFUL;
}

/******************************************************************************
**  Function:  CFE_PSP_SetupSymbolTable()
**
**  Purpose:
**    Load the symbol table from the EXE file representing the currently running code.
**    This is required in order to make the module loading work properly.  Without this,
**    all symbols defined within the core executable will get flagged as undefined
**    when loading a module that tries to reference them.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
**
**  Note:
**    Not calling CFE_PSP_Panic() if this doesn't work, because CFE itself will still
**    run and any apps that are statically linked should still run.
*/
void CFE_PSP_SetupSymbolTable(void)
{
    char cfe_exec_file[OS_MAX_LOCAL_PATH_LEN];

    snprintf(cfe_exec_file, sizeof(cfe_exec_file), "/boot/core-%s.exe",
            GLOBAL_CONFIGDATA.Default_CpuName);
    OS_printf ("CFE_PSP: Loading symbol table from: %s\n", cfe_exec_file);
    cexpModuleLoad(cfe_exec_file, 0);
}

/******************************************************************************
**  Function:  CFE_PSP_SetupSystemTimer
**
**  Purpose:
**    BSP system time base and timer object setup.
**    This does the necessary work to start the 1Hz time tick required by CFE
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
**
** NOTE:
**      The handles to the timebase/timer objects are "start and forget"
**      as they are supposed to run forever as long as CFE runs.
**
**      If needed for e.g. additional timer creation, they can be recovered
**      using an OSAL GetIdByName() call.
**
**      This is preferred anyway -- far cleaner than trying to pass the uint32 value
**      up to the application somehow.
*/

void CFE_PSP_SetupSystemTimer(void)
{
    uint32 SystemTimebase;
    int32  Status;

    Status = OS_TimeBaseCreate(&SystemTimebase, "cFS-Master", NULL);
    if (Status == OS_SUCCESS)
    {
        Status = OS_TimeBaseSet(SystemTimebase, 250000, 250000);
    }


    /*
     * If anything failed, cFE/cFS will not run properly, so a panic is appropriate
     */
    if (Status != OS_SUCCESS)
    {
        OS_printf("CFE_PSP: Error configuring cFS timing: %d\n", (int)Status);
        CFE_PSP_Panic(Status);
    }
}


/*
** A simple entry point to start from the loader
*/
rtems_task Init(
  rtems_task_argument ignored
)
{
   if (CFE_PSP_Setup() != RTEMS_SUCCESSFUL)
   {
       CFE_PSP_Panic(CFE_PSP_ERROR);
   }

   /*
   ** Start the shell early, so it can be be used in case a problem occurs
   */
   if (rtems_shell_init("SHLL", RTEMS_MINIMUM_STACK_SIZE * 4, 100, "/dev/console", false, false, CFE_PSP_Login_Check) < 0)
   {
     OS_printf ("shell init failed: %s\n", strerror (errno));
   }

   /*
   ** Initialize the OS API
   */
   OS_API_Init();

   CFE_PSP_SetupSymbolTable();

   /* Prepare the system timing resources */
   CFE_PSP_SetupSystemTimer();

   /*
   ** Run the PSP Main - this will return when init is complete
   */
   CFE_PSP_Main(1, "/cf/cfe_es_startup.scr");


   /*
   ** Wait for anything interesting to happen
   **  (any real work should be done by threads spawned during startup)
   */
   OS_IdleLoop();
}

/******************************************************************************
**  Function:  CFE_PSP_Main()
**
**  Purpose:
**    Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

void CFE_PSP_Main(uint32 ModeId, char *StartupFilePath )
{
   uint32            reset_type;
   uint32            reset_subtype;


   /*
   ** Determine Reset type by reading the hardware reset register.
   */
   reset_type = CFE_PSP_RST_TYPE_POWERON;
   reset_subtype = CFE_PSP_RST_SUBTYPE_POWER_CYCLE;

   /*
   ** Initialize the reserved memory 
   */
   CFE_PSP_InitProcessorReservedMemory(reset_type);

   /*
   ** Call cFE entry point. This will return when cFE startup
   ** is complete.
   */
   CFE_ES_MAIN_FUNCTION(reset_type,reset_subtype, 1, CFE_ES_NONVOL_STARTUP_FILE);

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
 * Note that these resources are shared with RTEMS itself (e.g. the init task, the shell)
 * so they should be allocated slightly higher than the user limits in osconfig.h
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


