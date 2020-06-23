/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

