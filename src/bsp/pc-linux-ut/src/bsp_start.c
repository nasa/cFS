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
** History:
**   2005/07/26  A. Cudmore      | Initial version for linux 
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

/*
** OSAL includes 
*/
#include "common_types.h"
#include "osapi.h"

/*
** Types and prototypes for this module
*/

/*
**  External Declarations
*/
void OS_Application_Startup(void);
                                                                           
/*
** Global variables
*/
                                                                                                                                                               
                                                                                                                                                
/******************************************************************************
**  Function:  main()
**
**  Purpose:
**    BSP Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

int main(void)
{
   int      mode;
   int      status; 

   /*
   ** Create local directories for "disk" mount points
   **  See bsp_voltab for the values 
   */
   printf("Making directories: ram0, ram1, eeprom1 for OSAL mount points\n");
   mode = S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO;
   status = mkdir("ram0", mode);
   status = mkdir("ram1", mode);
   status = mkdir("eeprom1", mode); 

   /*
   ** OS_API_Init is called by OS_Application_Startup
   */

   /*
   ** Call application specific entry point.
   */
   OS_Application_Startup();
  
   return(0);
}

