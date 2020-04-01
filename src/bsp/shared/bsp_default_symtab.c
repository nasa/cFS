/******************************************************************************
** File:  bsp_symtab.c
**
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
**      Copyright (c) 2004-2015, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved.
**
**
** Purpose:
**   Application static symbol table default implementation.
**
** NOTE: This is isolated in a separate compilation unit, so that a user
**   application may directly provide an OS_STATIC_SYMBOL_TABLE definition
**   which will override this default.
**
******************************************************************************/

#include "osapi.h"
#include "bsp-impl.h"


OS_static_symbol_record_t OS_STATIC_SYMBOL_TABLE[] =
{
        { "OS_Application_Startup", OS_Application_Startup },
        { "OS_Application_Run", OS_Application_Run },
        { NULL, NULL }
};
