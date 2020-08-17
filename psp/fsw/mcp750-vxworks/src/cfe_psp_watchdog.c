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

/************************************************************************************************
** File:  cfe_psp_watchdog.c
**
** Purpose:
**   This file contains glue routines between the cFE and the OS Board Support Package ( BSP ).
**   The functions here allow the cFE to interface functions that are board and OS specific
**   and usually dont fit well in the OS abstraction layer.
**
** History:
**   2009/07/20  A. Cudmore    | Initial version,
**
*************************************************************************************************/

/*
**  Include Files
*/


/*
** cFE includes
*/
#include "common_types.h"
#include "osapi.h"

/*
**  System Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vxWorks.h"
#include "sysLib.h"
#include "vxLib.h"
#include "taskLib.h"
#include "ramDrv.h"
#include "dosFsLib.h"
#include "errnoLib.h"
#include "usrLib.h"
#include "cacheLib.h"
#include "mcpx750.h"
#include "drv/hdisk/ataDrv.h"

/*
** Types and prototypes for this module
*/
#include "cfe_psp.h"
#include "cfe_psp_config.h"

/*
** Global data
*/

/*
** The watchdog time in milliseconds
*/
uint32 CFE_PSP_WatchdogValue = CFE_PSP_WATCHDOG_MAX;

/*  Function:  CFE_PSP_WatchdogInit()
**
**  Purpose:
**    To setup the timer resolution and/or other settings custom to this platform.
**
**  Arguments:
**
**  Return:
*/
void CFE_PSP_WatchdogInit(void)
{

   /*
   ** Just set it to a value right now
   ** The pc-linux desktop platform does not actually implement a watchdog
   ** timeout ( but could with a signal )
   */
   CFE_PSP_WatchdogValue = CFE_PSP_WATCHDOG_MAX;

}


/******************************************************************************
**  Function:  CFE_PSP_WatchdogEnable()
**
**  Purpose:
**    Enable the watchdog timer
**
**  Arguments:
**
**  Return:
*/
void CFE_PSP_WatchdogEnable(void)
{
  /* Arm the WDT2 control register */
  PCI_OUT_BYTE(0xFEFF0068, 0x55);

  /* The enable/disable bit is bit 15, a setting of 1 enables the timer.*/
  PCI_OUT_LONG(0xFEFF0068,0xFFFFFFAA);

}


/******************************************************************************
**  Function:  CFE_PSP_WatchdogDisable()
**
**  Purpose:
**    Disable the watchdog timer
**
**  Arguments:
**
**  Return:
*/
void CFE_PSP_WatchdogDisable(void)
{
  /* Arm the WDT2 control register */
  PCI_OUT_BYTE(0xFEFF0068, 0x55);

  /* The enable/disable bit is bit 15, a setting of 0 disables the timer.*/
  PCI_OUT_LONG(0xFEFF0068,0xFFFF7FAA);

}

/******************************************************************************
**  Function:  CFE_PSP_WatchdogService()
**
**  Purpose:
**    Load the watchdog timer with a count that corresponds to the millisecond
**    time given in the parameter.
**
**  Arguments:
**    None.
**
**  Return:
**    None
**
**  Note:
**    Currently an ExpireTime value of zero will result in the minimum reset time
**    of 4.5 seconds. All other ExpireTime values will result in a reset time of
**    5.5 seconds. See comments below.
*/
void CFE_PSP_WatchdogService(void)
{
  /* Arm the WDT2 control register */
  PCI_OUT_BYTE(0xFEFF0068, 0x55);

  /*
  ** The mcp750 watchdog register is settable to time values between 0 to 1.024
  ** seconds in increments of 16us. The resolution can be set to less than 16us, but
  ** the result is a max time of less than 1 second.
  ** This setting is always added to the fixed delay of 4.5 seconds internal to the
  ** board. So essentially the watchdog timer on this board can be programmed to
  ** expire between 4.5 seconds and 5.5 seconds.
  ** The actual watchdog timer counter is the upper 16 bits of the data word.
  ** The enable/disable bit is bit 15, a setting of 1 enables the timer.
  ** The 32 bit word shown below may get byte swapped and/or word swapped before it
  ** gets to the actual register on the board.
  */
  if( CFE_PSP_WatchdogValue == CFE_PSP_WATCHDOG_MIN)
  {
    PCI_OUT_LONG(0xFEFF0068,0x0000FFAA);
  }
  else
  {
    PCI_OUT_LONG(0xFEFF0068,0xFFFFFFAA);
  }

}
/******************************************************************************
**  Function:  CFE_PSP_WatchdogGet
**
**  Purpose:
**    Get the current watchdog value. 
**
**  Arguments:
**    none 
**
**  Return:
**    the current watchdog value 
**
**  Notes:
**
*/
uint32 CFE_PSP_WatchdogGet(void)
{
   return(CFE_PSP_WatchdogValue);
}


/******************************************************************************
**  Function:  CFE_PSP_WatchdogSet
**
**  Purpose:
**    Get the current watchdog value. 
**
**  Arguments:
**    The new watchdog value 
**
**  Return:
**    nothing 
**
**  Notes:
**
*/
void CFE_PSP_WatchdogSet(uint32 WatchdogValue)
{
    CFE_PSP_WatchdogValue = WatchdogValue;
}

