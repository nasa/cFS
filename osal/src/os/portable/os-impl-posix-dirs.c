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
 * \file     os-impl-posix-dirs.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file Contains all of the api calls for manipulating files
 * in a file system / C library that implements the UNIX-style file API
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must include whatever is required to get the prototypes of these functions:
 *
 *   stat()
 *   mkdir()
 *   rmdir()
 *   opendir()
 *   readdir()
 *   closedir()
 *   rewinddir()
 */
#include <string.h>
#include <errno.h>

#include "os-impl-dirs.h"
#include "os-shared-dir.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/***************************************************************************************
                                 FUNCTION PROTOTYPES
 **************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_DirCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirCreate_Impl(const char *local_path, uint32 access)
{
   struct stat st;
   int32 return_code;

   if ( mkdir(local_path, S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO) < 0 )
   {
      return_code = OS_ERROR;

      if (errno == EEXIST)
      {
         /* it exists, but not necessarily a directory */
         if ( stat(local_path, &st) == 0  && S_ISDIR(st.st_mode) )
         {
            return_code = OS_SUCCESS;
         }
      }
   }
   else
   {
      return_code = OS_SUCCESS;
   }

   return return_code;
} /* end OS_DirCreate_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_DirOpen_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirOpen_Impl(uint32 local_id, const char *local_path)
{
   DIR *dp = opendir(local_path);
   if (dp == NULL)
   {
      return OS_ERROR;
   }
   OS_impl_dir_table[local_id].dp = dp;
   return OS_SUCCESS;
} /* end OS_DirOpen_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_DirClose_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirClose_Impl(uint32 local_id)
{
   closedir(OS_impl_dir_table[local_id].dp);
   OS_impl_dir_table[local_id].dp = NULL;
   return OS_SUCCESS;
} /* end OS_DirClose_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_DirRead_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRead_Impl(uint32 local_id, os_dirent_t *dirent)
{
   struct dirent *de;

   /* NOTE - the readdir() call is non-reentrant ....
    * However, this is performed while the global dir table lock is taken.
    * Therefore this ensures that only one such call can occur at any given time.
    *
    * Static analysis tools may warn about this because they do not know
    * this function is externally serialized via the global lock.
    */
   /* cppcheck-suppress readdirCalled */
   /* cppcheck-suppress nonreentrantFunctionsreaddir */
   de = readdir(OS_impl_dir_table[local_id].dp);
   if (de == NULL)
   {
      return OS_ERROR;
   }

   strncpy(dirent->FileName, de->d_name, sizeof(dirent->FileName) - 1);
   dirent->FileName[sizeof(dirent->FileName) - 1] = 0;

   return OS_SUCCESS;
} /* end OS_DirRead_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_DirRewind_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRewind_Impl(uint32 local_id)
{
   rewinddir(OS_impl_dir_table[local_id].dp);
   return OS_SUCCESS;
} /* end OS_DirRewind_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_DirRemove_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRemove_Impl(const char *local_path)
{
   if ( rmdir(local_path) < 0 )
   {
      return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_DirRemove_Impl */
