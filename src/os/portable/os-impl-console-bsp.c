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
 * \file   os-impl-console-bsp.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *      Uses the BSP-provided "console write" function
 *      Note this only supports a single console
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <string.h>
#include <errno.h>

#include "bsp-impl.h"

#include "os-impl-console.h"
#include "os-shared-printf.h"

/****************************************************************************************
                                CONSOLE OUTPUT
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleOutput_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void  OS_ConsoleOutput_Impl(uint32 local_id)
{
    uint32 StartPos;
    uint32 EndPos;
    long WriteSize;
    OS_console_internal_record_t *console;

    console = &OS_console_table[local_id];
    StartPos = console->ReadPos;
    EndPos = console->WritePos;
    while (StartPos != EndPos)
    {
        if (StartPos > EndPos)
        {
            /* handle wrap */
            WriteSize = console->BufSize - StartPos;
        }
        else
        {
            WriteSize = EndPos - StartPos;
        }

        OS_BSP_ConsoleOutput_Impl(&console->BufBase[StartPos],
                WriteSize);

        StartPos += WriteSize;
        if (StartPos >= console->BufSize)
        {
            /* handle wrap */
            StartPos = 0;
        }
    }

    /* Update the global with the new read location */
    console->ReadPos = StartPos;
} /* end OS_ConsoleOutput_Impl */





