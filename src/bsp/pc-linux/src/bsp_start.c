/******************************************************************************
** File:  bsp_start.c
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
**   OSAL BSP main entry point.
**
** History:
**   2005/07/26  A. Cudmore      | Initial version for linux
**
******************************************************************************/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pclinux_bsp_internal.h"

OS_BSP_PcLinuxGlobalData_t OS_BSP_PcLinuxGlobal;

/* ---------------------------------------------------------
    OS_BSP_Initialize()

     Helper function to auto-create any missing FS_BASED mount
     points listed in OS_VolumeTable.  If these do not actually
     exist then app code may fail.
   --------------------------------------------------------- */
void OS_BSP_Initialize(void)
{
    mode_t      mode;
    uint32      i;
    struct stat statbuf;
    FILE *fp;
    char buffer[32];

    /*
    ** Create local directories for "disk" mount points
    **  See bsp_voltab for the values
    **
    ** NOTE - the voltab table is poorly designed here; values of "0" are valid
    ** and will translate into an entry that is actually used.  In particular the
    ** "free" flag has to be actually initialized to TRUE to say its NOT valid.
    ** So in the case of an entry that has been zeroed out (i.e. bss section) it
    ** will be treated as a valid entry.
    **
    ** Checking that the DeviceName starts with a leading slash '/' is a workaround
    ** for this, and may be the only way to detect an entry that is uninitialized.
    */
    mode = S_IFDIR | S_IRWXU | S_IRWXG | S_IRWXO;
    for (i = 0; i < NUM_TABLE_ENTRIES; ++i)
    {
        if (OS_VolumeTable[i].VolumeType == FS_BASED &&
                OS_VolumeTable[i].PhysDevName[0] != 0 &&
                OS_VolumeTable[i].DeviceName[0] == '/')

        {
            if (stat(OS_VolumeTable[i].PhysDevName, &statbuf) < 0)
            {
                BSP_DEBUG("Creating mount point: %s\n", OS_VolumeTable[i].PhysDevName);
                mkdir(OS_VolumeTable[i].PhysDevName, mode);
            }
        }
    }

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
    memset(&OS_BSP_PcLinuxGlobal, 0, sizeof(OS_BSP_PcLinuxGlobal));

    /*
     * Save the argc/argv arguments for future use.
     * In particular the UT-specific logic uses this
     * to control verbosity.
     *
     * Note that the first argument (0) is the command name.  The
     * first "real" argument is at position 1.
     *
     * The first arg is ignored to be more consistent with other platforms
     * where this is not passed in.
     */
    if (argc > 1)
    {
        OS_BSP_Global.ArgC = argc - 1;
        OS_BSP_Global.ArgV = &argv[1];
    }

    /*
     * Only attempt terminal control if the stdout is a TTY
     * and the TERM environment variable is set
     */
    if (getenv("TERM") != NULL)
    {
        OS_BSP_PcLinuxGlobal.EnableTermControl = isatty(STDOUT_FILENO);
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
