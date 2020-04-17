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

#include "os-vxworks.h"

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <taskLib.h>
#include <sysLib.h>
#include <shellLib.h>
#include <errnoLib.h>

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
int32 OS_ShellOutputToFile_Impl(uint32 file_id, const char *Cmd)
{
    int32 ReturnCode = OS_ERROR;
    int32 Result = ERROR;
    int32 fdCmd;
    uint32 cmdidx;
    char * shellName;

    /* Create a file to write the command to (or write over the old one) */
    fdCmd = OS_creat(OS_SHELL_CMD_INPUT_FILE_NAME,OS_READ_WRITE);

    if (fdCmd < OS_SUCCESS)
    {
        return OS_ERROR;
    }

    if (OS_ConvertToArrayIndex(fdCmd, &cmdidx) == OS_SUCCESS)
    {
        /* copy the command to the file, and then seek back to the beginning of the file */
        OS_write(fdCmd, Cmd, strlen(Cmd));
        OS_lseek(fdCmd,0,OS_SEEK_SET);

        /* Create a shell task the will run the command in the file, push output to OS_fd */
        Result = shellGenericInit("INTERPRETER=Cmd", 0, NULL, &shellName, false, false,
                OS_impl_filehandle_table[cmdidx].fd,
                OS_impl_filehandle_table[file_id].fd,
                OS_impl_filehandle_table[file_id].fd);
    }

    if (Result == OK)
    {
        /* Wait for the command to terminate */
        do
        {
            taskDelay(sysClkRateGet());
        }
        while (taskNameToId(shellName) != ((TASK_ID)ERROR));

        ReturnCode =  OS_SUCCESS;
    }

    /* Close the file descriptor */
    OS_close(fdCmd);

    return ReturnCode;

} /* end OS_ShellOutputToFile_Impl */
