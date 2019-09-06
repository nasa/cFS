/*
 *  Copyright (c) 2004-2018, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"


/*****************************************************************************
 *
 * Stub function for OS_API_Init()
 *
 *****************************************************************************/
int32 OS_API_Init(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_API_Init);

    return status;
}


/*****************************************************************************
 *
 * Stub function for OS_ApplicationExit()
 *
 *****************************************************************************/
void OS_ApplicationExit(int32 Status)
{
    /* just call the default so a hook can be attached */
    UT_DEFAULT_IMPL(OS_ApplicationExit);
}

/*****************************************************************************
 *
 * Stub function for OS_CleanUpObject()
 *
 *****************************************************************************/
void OS_CleanUpObject(uint32 object_id, void *arg)
{
    UT_Stub_RegisterContext(UT_KEY(OS_CleanUpObject), &object_id);
    UT_Stub_RegisterContext(UT_KEY(OS_CleanUpObject), arg);
    UT_DEFAULT_IMPL(OS_CleanUpObject);
}

/*****************************************************************************
 *
 * Stub function for OS_DeleteAllObjects()
 *
 *****************************************************************************/
void OS_DeleteAllObjects(void)
{
    UT_DEFAULT_IMPL(OS_DeleteAllObjects);
}

/*****************************************************************************
 *
 * Stub function for OS_IdleLoop()
 *
 *****************************************************************************/
void OS_IdleLoop(void)
{
    UT_DEFAULT_IMPL(OS_IdleLoop);
}

/*****************************************************************************
 *
 * Stub function for OS_ApplicationShutdown()
 *
 *****************************************************************************/
void OS_ApplicationShutdown(uint8 flag)
{
    UT_Stub_RegisterContext(UT_KEY(OS_ApplicationShutdown), &flag);
    UT_DEFAULT_IMPL(OS_ApplicationShutdown);
}

