/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-impl-shell.c
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-impl-files.h"
#include "os-shared-shell.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define OS_REDIRECTSTRSIZE           15



/****************************************************************************************
                         IMPLEMENTATION-SPECIFIC ROUTINES
             These are specific to this particular operating system
 ****************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_ShellOutputToFile_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ShellOutputToFile_Impl(uint32 file_id, const char *Cmd)
{
   /*
   ** this is a #define to avoid a 'variable length array' warning
   ** 15 is for the size of the redirection string that is added
   ** to the command
   */
   char LocalCmd [OS_MAX_CMD_LEN + OS_REDIRECTSTRSIZE];
   int32 Result;

   strncpy(LocalCmd,Cmd,OS_MAX_CMD_LEN +OS_REDIRECTSTRSIZE);

   /* Make sure that we are able to access this file */
   fchmod(OS_impl_filehandle_table[file_id].fd, 0666);

   /*
   ** add in the extra chars necessary to perform the redirection
   ** 1 for stdout and 2 for stderr. they are redirected to the
   ** file descriptor passed in
   */
   snprintf(LocalCmd, sizeof(LocalCmd), "%s 1>&%d 2>&%d",
         Cmd,
         OS_impl_filehandle_table[file_id].fd,
         OS_impl_filehandle_table[file_id].fd);

   Result = system(LocalCmd);

   if (Result != 0)
   {
      return OS_ERROR;
   }
   return OS_SUCCESS;
} /* end OS_ShellOutputToFile_Impl */

