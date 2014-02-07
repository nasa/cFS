/******************************************************************************
** File:  os_bspstart.c
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
** History:
**   2004/09/23  J.P. Swinski    | Initial version,
**   2004/10/01  P.Kutt          | Replaced OS API task delay with VxWorks functions
**                                 since OS API is initialized later.
**
******************************************************************************/

#define _USING_RTEMS_INCLUDES_

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <bsp.h>
#include <rtems.h>
#include <netinet/in.h>
#include <rtems/mkrootfs.h>
#include <rtems/bdbuf.h>
#include <rtems/blkdev.h>
#include <rtems/diskdevs.h>
#include <rtems/error.h>
#include <rtems/ramdisk.h>

/*
** OSAL includes 
*/
#include "common_types.h"
#include "osapi.h"

/*
** Defines
*/
#define RTEMS_DRIVER_AUTO_MAJOR (0)
#define RTEMS_NUMBER_OF_RAMDISKS 1


/*
**  External Declarations
*/
void OS_Application_Startup(void);

/*
** Global data
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
  initialization_entry: ramdisk_initialize,
  open_entry:           rtems_blkdev_generic_open,
  close_entry:          rtems_blkdev_generic_close,
  read_entry:           rtems_blkdev_generic_read,
  write_entry:          rtems_blkdev_generic_write,
  control_entry:        rtems_blkdev_generic_ioctl
};

/******************************************************************************
**  Function:  OS_BSPMain()
**
**  Purpose:
**    vxWorks/BSP Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

void OS_BSPMain( void )
{

   /*
   ** Call OSAL entry point.    
   */
   OS_Application_Startup();
   
   /*
   ** Just sleep while OSAL code runs
   */
   for ( ;; )
      sleep(1);

}

rtems_task Init(
  rtems_task_argument ignored
)
{
   int status;
   int mode;

   printf("\n\n");
   printf( "\n\n*** RTEMS Info ***\n" );
   printf("%s", _Copyright_Notice );
   printf("%s\n\n", _RTEMS_version );
   printf(" Workspace base %08X\n", (unsigned int)Configuration.work_space_start );
   printf(" Workspace size %d\n",   (int) Configuration.work_space_size );
   printf("  Workspace top %08X\n", (unsigned int)(Configuration.work_space_start 
                                    + Configuration.work_space_size) );
   printf("\n");
   printf( "*** End RTEMS info ***\n\n" );

   /*
   ** Create the RTEMS Root file system
   */
   printf("Creating Root file system\n");
   status = rtems_create_root_fs();


   /*
   ** create the directory mountpoints
   */
   mode = S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO;
   status = mkdir("/drive0", mode);
   status = mkdir("/drive1", mode);
   status = mkdir("/ram", mode); /* For symtest mountpoint */


   /*
   ** Call OSAL entry point
   */
   OS_BSPMain();

   printf( "*** END OF OSAL TEST ***\n" );

  exit( 0 );
}

/* configuration information */

/*
** RTEMS OS Configuration defintions
*/
#define TASK_INTLEVEL 0
#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_ATTRIBUTES	(RTEMS_FLOATING_POINT | RTEMS_PREEMPT | RTEMS_NO_TIMESLICE | RTEMS_ASR | RTEMS_INTERRUPT_LEVEL(TASK_INTLEVEL))
#define CONFIGURE_INIT_TASK_STACK_SIZE	(20*1024)
#define CONFIGURE_INIT_TASK_PRIORITY	120

#define CONFIGURE_MAXIMUM_TASKS                      64 
#define CONFIGURE_MAXIMUM_TIMERS                     10
#define CONFIGURE_MAXIMUM_SEMAPHORES                 64 
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES             64

#define CONFIGURE_EXECUTIVE_RAM_SIZE	(1024*1024)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS     100

#define CONFIGURE_FILESYSTEM_RFS
#define CONFIGURE_FILESYSTEM_IMFS

#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_MICROSECONDS_PER_TICK              10000

#define CONFIGURE_MAXIMUM_DRIVERS                   10

#include <rtems/confdefs.h>


