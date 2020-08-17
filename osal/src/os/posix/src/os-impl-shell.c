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
 * \file     os-impl-shell.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "os-posix.h"
#include "os-impl-io.h"

#include "os-shared-shell.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/



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
int32 OS_ShellOutputToFile_Impl(uint32 file_id, const char* Cmd)
{
   pid_t cpid;
   uint32 local_id;
   int wstat;
   const char *shell = getenv("SHELL");

   if (shell == NULL)
   {
       shell = "/bin/sh";
   }

   cpid = fork();
   if (cpid < 0)
   {
       OS_DEBUG("%s(): Error during fork(): %s\n", __func__, strerror(errno));
       return OS_ERROR;
   }

   if (cpid == 0)
   {
       /* child process */
       dup2(OS_impl_filehandle_table[file_id].fd, STDOUT_FILENO);
       dup2(OS_impl_filehandle_table[file_id].fd, STDERR_FILENO);

       /* close all _other_ filehandles */
       for (local_id = 0; local_id < OS_MAX_NUM_OPEN_FILES; ++local_id)
       {
           if (OS_global_stream_table[local_id].active_id != 0)
           {
               close(OS_impl_filehandle_table[local_id].fd);
           }
       }

       execl(shell, "sh", "-c", Cmd, NULL); /* does not return if successful */
       exit(EXIT_FAILURE);
   }

   if (waitpid(cpid, &wstat, 0) != cpid)
   {
       OS_DEBUG("%s(): Error during waitpid(): %s\n", __func__, strerror(errno));
       return OS_ERROR;
   }

   if (!WIFEXITED(wstat) || WEXITSTATUS(wstat) != 0)
   {
       OS_DEBUG("%s(): Error from child process: %d\n", __func__, wstat);
       return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_ShellOutputToFile_Impl */

