/******************************************************************************
** File:  bsp_start.c
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement. 
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. 
**
** Purpose:
**   
**  OSAL main entry point.
**
** History:
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

/*
** OSAL includes 
*/
#include "common_types.h"
#include "osapi.h"

/*
**  External Declarations
*/
void OS_Application_Startup(void);

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
   int    TicksPerSecond;
   
   /* 
   ** Delay for one second. 
   */
   TicksPerSecond = sysClkRateGet();
   (void) taskDelay( TicksPerSecond );

   OS_printf("Starting Up OSAPI App.\n");
   
   /*
   ** Call OSAL entry point.    
   */
   OS_Application_Startup();

   /*
   ** Exit the main thread.
   ** in VxWorks we can delete all of the OS tasks if we want.
   */

}

