/******************************************************************************
** File:  bsp_start.c
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
**      Copyright (c) 2004-2006, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved.
**
** Purpose:
**
**  OSAL main entry point.
**
** History:
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdlib.h>
#include <string.h>

#include "mcp750_bsp_internal.h"

/******************************************************************************
**  Function:  OS_BSPMain()
**
**  Purpose:
**    vxWorks/BSP Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    integer return code, with zero indicating normal exit, nonzero
**    indicating an off-nominal condition
*/

int OS_BSPMain(void)
{
    /*
     * Initially clear the global object (this contains return code)
     */
    memset(&OS_BSP_Global, 0, sizeof(OS_BSP_Global));

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

    /*
     * Return to shell with the current status code
     */
    return OS_BSP_Global.AppStatus;
}
