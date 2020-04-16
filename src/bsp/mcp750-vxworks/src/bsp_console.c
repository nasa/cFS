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
#include <stdio.h>

#include "mcp750_bsp_internal.h"
#include "bsp-impl.h"

/****************************************************************************************
                    BSP CONSOLE IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*----------------------------------------------------------------
   OS_BSP_ConsoleOutput_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleOutput_Impl(const char *Str, uint32 DataLen)
{
    while (DataLen > 0)
    {
        putchar(*Str);
        ++Str;
        --DataLen;
    }
}

/*----------------------------------------------------------------
   OS_BSP_ConsoleSetMode_Impl() definition
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleSetMode_Impl(uint32 ModeBits)
{
    /* ignored; not implemented */
}


