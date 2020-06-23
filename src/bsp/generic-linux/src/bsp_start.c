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

/*
 * File:  bsp_start.c
 *
 * Purpose:
 *   OSAL BSP main entry point.
 *
 * History:
 *   2005/07/26  A. Cudmore      | Initial version for linux
 */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "generic_linux_bsp_internal.h"

OS_BSP_GenericLinuxGlobalData_t OS_BSP_GenericLinuxGlobal;

/* ---------------------------------------------------------
    OS_BSP_Initialize()

     Helper function to auto-create any missing FS_BASED mount
     points listed in OS_VolumeTable.  If these do not actually
     exist then app code may fail.
   --------------------------------------------------------- */
void OS_BSP_Initialize(void)
{
    FILE *fp;
    char buffer[32];

    /*
     * If not running as root, check /proc/sys/fs/mqueue/msg_max
     *
     * This special file represents the max depth of a POSIX message queue for an unprivileged user.
     *
     * In order to facilitate running in simulation mode without any need for root access --
     * this will allow the OSAL to successfully create message queues by truncating anything larger than this size.
     *
     * No need to check _LINUX_OS_ here; if the file fails to open, i.e. if not on Linux and the file does not exist,
     * then leave well enough alone and don't do anything.
     */
    if (geteuid() != 0)
    {
        fp = fopen("/proc/sys/fs/mqueue/msg_max","r");
        if (fp)
        {
            if (fgets(buffer,sizeof(buffer),fp) != NULL)
            {
                OS_BSP_Global.MaxQueueDepth = strtoul(buffer, NULL, 10);
                BSP_DEBUG("Maximum user msg queue depth = %u\n", (unsigned int)OS_BSP_Global.MaxQueueDepth);
            }
            fclose(fp);
        }
    }
}

/* ---------------------------------------------------------
    OS_BSP_GetReturnStatus()

     Helper function to convert an OSAL status code into
     a code suitable for returning to the OS.
   --------------------------------------------------------- */
int OS_BSP_GetReturnStatus(void)
{
    int retcode;

    switch (OS_BSP_Global.AppStatus)
    {
    case OS_SUCCESS:
        /* translate OS_SUCCESS to the system EXIT_SUCCESS value (usually 0) */
        retcode = EXIT_SUCCESS;
        break;

    case OS_ERROR:
        /* translate OS_ERROR to the system EXIT_FAILURE value (usually 1) */
        retcode = EXIT_FAILURE;
        break;

    default:
        /* any other value will be passed through (implementation-defined) */
        /* Range is limited to 0-127, however */
        retcode = OS_BSP_Global.AppStatus & 0x7F;
        break;

    }

    return retcode;
}

/* ---------------------------------------------------------
    OS_BSP_Shutdown_Impl()

     Helper function to abort the running task
   --------------------------------------------------------- */
void OS_BSP_Shutdown_Impl(void)
{
    abort();
}


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
     * Initially clear the global objects
     */
    memset(&OS_BSP_Global, 0, sizeof(OS_BSP_Global));
    memset(&OS_BSP_GenericLinuxGlobal, 0, sizeof(OS_BSP_GenericLinuxGlobal));

    /*
     * Save the argc/argv arguments for future use.
     * In particular the UT-specific logic uses this
     * to control verbosity.
     *
     * Note that the first argument (0) is the command name.  The
     * first "real" argument is at position 1.
     *
     * However this still needs to pass it through as the appliction
     * might still want to use library "getopt" and this expects the
     * first parameter to be this way.
     */
    OS_BSP_Global.ArgC = argc;
    OS_BSP_Global.ArgV = argv;

    /*
     * Only attempt terminal control if the stdout is a TTY
     * and the TERM environment variable is set
     */
    if (getenv("TERM") != NULL)
    {
        OS_BSP_GenericLinuxGlobal.EnableTermControl = isatty(STDOUT_FILENO);
    }

    /*
     * Auto-Create any missing FS_BASED mount points specified in OS_VolumeTable
     */
    OS_BSP_Initialize();

    /*
     * Call application specific entry point.
     * This should set up all user tasks and resources, then return
     */
    OS_Application_Startup();

    /*
     * OS_Application_Run() implements the background task.
     * The user application may provide this, or a default implementation
     * is used which just calls OS_IdleLoop().
     */
    OS_Application_Run();

    /* Should typically never get here */
    return OS_BSP_GetReturnStatus();
}
