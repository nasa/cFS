/******************************************************************************
** File:  bsp_console.c
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
**   OSAL BSP debug console abstraction
**
******************************************************************************/

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "pclinux_bsp_internal.h"
#include "bsp-impl.h"

/*----------------------------------------------------------------
   OS_BSP_ExecTput()

   Helper function: Use the system "tput" utility to set the given
   console capability.

   This uses a fork/exec to invoke the external command which outputs
   the control sequence directly to the controlling terminal.

   It is assumed that this will only be used during debug/testing.
   Otherwise it would be preferable to cache the control strings to
   avoid repetitive fork/exec operations.
 ------------------------------------------------------------------*/
static void OS_BSP_ExecTput(const char *cap, const char *param)
{
    pid_t cpid;
    int   status;

    cpid = fork();
    if (cpid < 0)
    {
        return;
    }
    if (cpid == 0)
    {
        execlp("tput", "tput", cap, param, NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(cpid, &status, 0);
}

/****************************************************************************************
                    BSP CONSOLE IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*----------------------------------------------------------------
   OS_BSP_ConsoleOutput_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleOutput_Impl(const char *Str, uint32 DataLen)
{
    ssize_t WriteLen;

    while(DataLen > 0)
    {
        /* writes the raw data directly to STDOUT_FILENO (unbuffered) */
        WriteLen = write(STDOUT_FILENO, Str, DataLen);
        if (WriteLen <= 0)
        {
            /* no recourse if this fails, just stop. */
            break;
        }
        Str += WriteLen;
        DataLen -= WriteLen;
    }
}

/*----------------------------------------------------------------
   OS_BSP_ConsoleSetMode_Impl() definition
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleSetMode_Impl(uint32 ModeBits)
{
    char param[32];

    if (OS_BSP_PcLinuxGlobal.EnableTermControl)
    {
        if (ModeBits == OS_BSP_CONSOLEMODE_NORMAL)
        {
            OS_BSP_ExecTput("sgr0", NULL);
        }
        else
        {
            if ((ModeBits & OS_BSP_CONSOLEMODE_HIGHLIGHT) == 0)
            {
                /* no highlight (standout) text */
                OS_BSP_ExecTput("rmso", NULL);
            }
            else
            {
                /* set highlight (standout) text */
                OS_BSP_ExecTput("smso", NULL);
            }

            snprintf(param, sizeof(param), "%d", OS_BSP_CONSOLEMODE_TO_ANSICOLOR(ModeBits));
            OS_BSP_ExecTput("setaf", param);
        }
    }
}
