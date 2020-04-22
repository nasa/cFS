/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osshell.c
 *
 * Purpose: Implements shell-related calls that can be optionally built
 *          for distributions that choose to support them.  Alternatively
 *          build the portable no-shell implementation to exclude this
 *          functionality.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"

#include <sys/types.h>
#include <sys/wait.h>

/****************************************************************************************
                         IMPLEMENTATION-SPECIFIC ROUTINES
             These are specific to this particular operating system
 ****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_ShellOutputToFile_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
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

