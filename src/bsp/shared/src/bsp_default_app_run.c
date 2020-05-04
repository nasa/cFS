/******************************************************************************
** File:  bsp_app_run.c
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
**   Application run default implementation.
**
** NOTE: This is isolated in a separate compilation unit, so that a user
**   application may directly provide an OS_Application_Run() implementation
**   which will override this default.
**
******************************************************************************/

#include "osapi.h"
#include "bsp-impl.h"

/*
 * The default implementation of OS_Application_Run()
 * just calls the OS_IdleLoop() provided by OSAL.
 */
void OS_Application_Run(void)
{
    OS_IdleLoop();
}
