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
 * \file     osapi-shell.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "os-shared-shell.h"
#include "os-shared-file.h"
#include "os-shared-idmap.h"



/*----------------------------------------------------------------
 *
 * Function: OS_ShellOutputToFile
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ShellOutputToFile(const char* Cmd, uint32 filedes)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check Parameters */
   if (Cmd == NULL)
   {
       return OS_INVALID_POINTER;
   }

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_STREAM, filedes, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_ShellOutputToFile_Impl(local_id, Cmd);
      OS_ObjectIdRefcountDecr(record);
   }

   return return_code;
} /* end OS_ShellOutputToFile */

