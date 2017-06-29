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
** OSAL includes 
*/
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

int main(int argc, char *argv[])
{
   /*
   ** OS_API_Init is called by OS_Application_Startup
   ** Also note that OS_API_Init now also takes care of signal masking
   */

   /*
   ** Call application specific entry point.
   */
   OS_Application_Startup();

   /*
   ** OS_IdleLoop() will wait forever and return if
   ** someone calls OS_ApplicationShutdown(TRUE)
   */
   OS_IdleLoop();

   /* Should typically never get here */
   return(EXIT_SUCCESS);
}

