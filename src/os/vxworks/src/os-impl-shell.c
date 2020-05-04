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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-io.h"
#include "os-shared-shell.h"

#include <shellLib.h>
#include <taskLib.h>
#include <sysLib.h>

#define OS_REDIRECTSTRSIZE           15
#define OS_SHELL_TMP_FILE_EXT        ".out"
#define OS_SHELL_TMP_FILE_EXT_LEN    4
#define OS_SHELL_CMD_TASK_STACK_SIZE 16384
#define OS_SHELL_CMD_TASK_PRIORITY   250



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

