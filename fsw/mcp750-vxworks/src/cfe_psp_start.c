/*
**  GSC-18128-1, "Core Flight Executive Version 6.6"
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
**   cFE PSP main entry point.
**
** History:
**   2004/09/23  J.P. Swinski    | Initial version,
**   2004/10/01  P.Kutt          | Replaced OS API task delay with VxWorks functions
**                                 since OS API is initialized later.
**   2016/04/07  M.Grubb         | Updated for PSP version 1.3
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vxWorks.h"
#include "sysLib.h"
#include "taskLib.h"
#include "ramDrv.h"
#include "dosFsLib.h"
#include "xbdBlkDev.h"
#include "errnoLib.h"
#include "usrLib.h"
#include "cacheLib.h"
#include "drv/hdisk/ataDrv.h"
/* #include "config.h" */
#include "mcpx750.h"

/*
** cFE includes 
*/
#include "common_types.h"
#include "osapi.h"

#include "cfe_psp.h" 
#include "cfe_psp_memory.h"           

/*
**  External Declarations
*/
IMPORT void sysPciWrite32 (UINT32, UINT32);


/*
 * The preferred way to obtain the CFE tunable values at runtime is via
 * the dynamically generated configuration object.  This allows a single build
 * of the PSP to be completely CFE-independent.
 */
#include <target_config.h>

#define CFE_PSP_MAIN_FUNCTION        (*GLOBAL_CONFIGDATA.CfeConfig->SystemMain)
#define CFE_PSP_NONVOL_STARTUP_FILE  (GLOBAL_CONFIGDATA.CfeConfig->NonvolStartupFile)
#define CFE_PSP_1HZ_FUNCTION         (*GLOBAL_CONFIGDATA.CfeConfig->System1HzISR)



/******************************************************************************
**  Function:  CFE_PSP_Main()
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

void CFE_PSP_Main( void )
{
   int    TicksPerSecond;
   uint32 reset_type;
   uint32 reset_subtype;
   char   reset_register;
   int32  Status;


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
   ** Delay for one second. 
   */
   TicksPerSecond = sysClkRateGet();
   (void) taskDelay( TicksPerSecond );

   /* 
   ** This starts up the hardware timer on the board that
   ** will be used to get the local time 
   */
   sysPciWrite32(0xFC0011D0, 0x0D6937E5);

   /*
   ** Setup the pointer to the reserved area in vxWorks.
   ** This must be done before any of the reset variables are used.
   */
   CFE_PSP_ReservedMemoryPtr = (CFE_PSP_ReservedMemory_t *)sysMemTop();

   /*
   ** Determine Reset type by reading the hardware reset register.
   */
   reset_register = *(SYS_REG_BLRR);
   OS_printf("CFE_PSP: Reset Register = %02X\n",reset_register);
   
   if ( reset_register & SYS_REG_BLRR_PWRON )
   {    
      OS_printf("CFE_PSP: POWERON Reset: Power Switch ON.\n");
      reset_type = CFE_PSP_RST_TYPE_POWERON;
      reset_subtype = CFE_PSP_RST_SUBTYPE_POWER_CYCLE;
   }
   else if ( reset_register & SYS_REG_BLRR_PBRST )
   {    
      OS_printf("CFE_PSP: POWERON Reset: CPCI Push Button Reset.\n");
      reset_type = CFE_PSP_RST_TYPE_POWERON;
      reset_subtype = CFE_PSP_RST_SUBTYPE_PUSH_BUTTON;
   }
   else if ( reset_register & SYS_REG_BLRR_FBTN )
   {    
      OS_printf("CFE_PSP: POWERON Reset: Front Panel Push Button Reset.\n");
      reset_type = CFE_PSP_RST_SUBTYPE_PUSH_BUTTON;
      reset_subtype = 3;
   }
   else if ( reset_register & SYS_REG_BLRR_WDT2 )
   {    
      OS_printf("CFE_PSP: PROCESSOR Reset: Watchdog level 2 Reset.\n");
      reset_type = CFE_PSP_RST_TYPE_PROCESSOR;
      reset_subtype = CFE_PSP_RST_SUBTYPE_HW_WATCHDOG;
   }
   else if ( reset_register & SYS_REG_BLRR_SWSRST )
   {    
      OS_printf("CFE_PSP: PROCESSOR Reset: Software Soft Reset.\n");
      reset_type = CFE_PSP_RST_TYPE_PROCESSOR;
      reset_subtype = CFE_PSP_RST_SUBTYPE_RESET_COMMAND;
   }
   else if ( reset_register & SYS_REG_BLRR_SWHRST )
   {
   
      /*
      ** For a Software hard reset, we want to look at the special 
      ** BSP reset variable to determine if we wanted a 
      ** Power ON or a Processor reset. Because the vxWorks sysToMonitor and
      ** reboot functions use this reset type, we want to use this for a software
      ** commanded processor or Power on reset.
      */
      if ( CFE_PSP_ReservedMemoryPtr->bsp_reset_type == CFE_PSP_RST_TYPE_POWERON)
      {
         OS_printf("CFE_PSP: POWERON Reset: Software Hard Reset.\n");
         reset_type = CFE_PSP_RST_TYPE_POWERON;
         reset_subtype = CFE_PSP_RST_SUBTYPE_RESET_COMMAND;
      }
      else
      {
         OS_printf("CFE_PSP: PROCESSOR Reset: Software Hard Reset.\n");
         reset_type = CFE_PSP_RST_TYPE_PROCESSOR;
         reset_subtype = CFE_PSP_RST_SUBTYPE_RESET_COMMAND;
      }
   }
   else 
   {
      OS_printf("CFE_PSP: POWERON Reset: UNKNOWN Reset.\n");
      reset_type = CFE_PSP_RST_TYPE_POWERON;
      reset_subtype = CFE_PSP_RST_SUBTYPE_UNDEFINED_RESET; 
   }   

   /*
   ** Initialize the reserved memory 
   */
   CFE_PSP_InitProcessorReservedMemory(reset_type);


   /*
   ** Call cFE entry point. This will return when cFE startup
   ** is complete.
   */
   CFE_PSP_MAIN_FUNCTION(reset_type,reset_subtype, 1, CFE_PSP_NONVOL_STARTUP_FILE);

   /*
   ** Main loop for default task and simulated 1hz 
   */
   for ( ;; )
   {
      TicksPerSecond = sysClkRateGet();
      (void) taskDelay( TicksPerSecond );
      CFE_PSP_1HZ_FUNCTION();
   }
}

