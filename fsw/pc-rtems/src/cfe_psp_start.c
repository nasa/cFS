/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/******************************************************************************
** File:  cfe_psp_start.c
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
#include <rtems/rtems_bsdnet.h>
#include <rtems/rtems_dhcp_failsafe.h>
#include <bsp.h>

extern int rtems_fxp_attach(struct rtems_bsdnet_ifconfig *config, int attaching);

/*
** cFE includes 
*/
#include "common_types.h"
#include "osapi.h"
#include "cfe_psp.h" 
#include "cfe_psp_memory.h"
#include "cfe_psp_module.h"


/*
 * The preferred way to obtain the CFE tunable values at runtime is via
 * the dynamically generated configuration object.  This allows a single build
 * of the PSP to be completely CFE-independent.
 */
#include <target_config.h>

#define CFE_PSP_MAIN_FUNCTION        (*GLOBAL_CONFIGDATA.CfeConfig->SystemMain)
#define CFE_PSP_NONVOL_STARTUP_FILE  (GLOBAL_CONFIGDATA.CfeConfig->NonvolStartupFile)

/*
** Global variables
*/


rtems_id          RtemsTimerId;

static unsigned char ethernet_address[6] = {0x00, 0x04, 0x9F, 0x00, 0x27, 0x61 };
static char net_name_str[] = "fxp1";
static char ip_addr_str[] = "10.0.2.17";
static char ip_netmask_str[] = "255.255.255.0";

static struct rtems_bsdnet_ifconfig netdriver_config = {
        .name = net_name_str,
        .attach = rtems_fxp_attach,
        .next = NULL,
        .ip_address = ip_addr_str,
        .ip_netmask = ip_netmask_str,
        .hardware_address = ethernet_address
        /* more options can follow */
};

struct rtems_bsdnet_config rtems_bsdnet_config = {
        .ifconfig = &netdriver_config,
        .bootp = rtems_bsdnet_do_dhcp_failsafe, /* fill if DHCP is used*/
};



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
**    This function is invoked before OSAL is initialized.
**      NO OSAL CALLS SHOULD BE USED YET.
**
**    The root file system is created, and mount points are created and mounted:
**     - /ram as ramdisk (RFS), read-write
**     - /boot from /dev/hda1, read-only, contain the boot executable(s) (CFE core)
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
   rtems_status_code status;

   /*
    * Initialize the network.  This is also optional and only
    * works if an appropriate network device is present.
    */
   status = rtems_bsdnet_initialize_network();
   if (status != RTEMS_SUCCESSFUL)
   {
     printf ("Network init not successful: %s / %s (continuing)\n",
             rtems_status_text (status),strerror(errno));
   }

   return RTEMS_SUCCESSFUL;
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
** A simple entry point to start from the BSP loader
**
** This entry point is used when building an RTEMS+CFE monolithic
** image, which is a single executable containing the RTEMS
** kernel and Core Flight Executive in one file.  In this mode
** the RTEMS BSP invokes the "Init" function directly.
**
** This sets up the root fs and the shell prior to invoking CFE via
** the CFE_PSP_Main() routine.
**
** In a future version this code may be moved into a separate bsp
** integration unit to be more symmetric with the VxWorks implementation.
*/
void OS_Application_Startup(void)
{
   if (CFE_PSP_Setup() != RTEMS_SUCCESSFUL)
   {
       CFE_PSP_Panic(CFE_PSP_ERROR);
   }

   /*
   ** Run the PSP Main - this will return when init is complete
   */
   CFE_PSP_Main();
}

/******************************************************************************
**  Function:  CFE_PSP_Main()
**
**  Purpose:
**    Application entry point.
**
**    The basic RTEMS system including the root FS and shell (if used) should
**    be running prior to invoking this function.
**
**    This entry point is used when building a separate RTEMS kernel/platform
**    boot image and Core Flight Executive image.  This is the type of deployment
**    used on e.g. VxWorks platforms.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

void CFE_PSP_Main(void)
{
   uint32            reset_type;
   uint32            reset_subtype;
   int32 Status;



   /*
   ** Initialize the OS API
   */
   Status = OS_API_Init();
   if (Status != OS_SUCCESS)
   {
       /* irrecoverable error if OS_API_Init() fails. */
       /* note: use printf here, as OS_printf may not work */
       printf("CFE_PSP: OS_API_Init() failure\n");
       CFE_PSP_Panic(Status);
   }

   /*
    * Initialize the CFE reserved memory map
    */
   CFE_PSP_SetupReservedMemoryMap();

   /*
   ** Initialize the statically linked modules (if any)
   */
   CFE_PSP_ModuleInit();

   /* Prepare the system timing resources */
   CFE_PSP_SetupSystemTimer();

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
   CFE_PSP_MAIN_FUNCTION(reset_type,reset_subtype, 1, CFE_PSP_NONVOL_STARTUP_FILE);

}
