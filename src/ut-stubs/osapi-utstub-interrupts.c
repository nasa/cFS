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
 * Stub function for OS_IntAttachHandler()
 *
 *****************************************************************************/
int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
    UT_Stub_RegisterContext(UT_KEY(OS_IntAttachHandler), &InterruptHandler);
    UT_Stub_RegisterContext(UT_KEY(OS_IntAttachHandler), &parameter);
    return UT_DEFAULT_IMPL(OS_IntAttachHandler);
}


/*****************************************************************************/
/**
** \brief OS_IntLock stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_IntLock.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_IntLock(void)
{
    return UT_DEFAULT_IMPL(OS_IntLock);
}

/*****************************************************************************/
/**
** \brief OS_IntUnlock stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_IntUnlock.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_IntUnlock(int32 IntFlags)
{
    return UT_DEFAULT_IMPL(OS_IntUnlock);
}

/*****************************************************************************
 *
 * Stub function for OS_IntEnable()
 *
 *****************************************************************************/
int32 OS_IntEnable(int32 Level)
{
    return UT_DEFAULT_IMPL(OS_IntEnable);
}

/*****************************************************************************
 *
 * Stub function for OS_IntDisable()
 *
 *****************************************************************************/
int32 OS_IntDisable(int32 Level)
{
    return UT_DEFAULT_IMPL(OS_IntDisable);
}

/*****************************************************************************
 *
 * Stub function for OS_IntSetMask()
 *
 *****************************************************************************/
int32 OS_IntSetMask ( uint32 MaskSetting )
{
    UT_Stub_RegisterContext(UT_KEY(OS_IntSetMask), &MaskSetting);
    return UT_DEFAULT_IMPL(OS_IntSetMask);
}

/*****************************************************************************
 *
 * Stub function for OS_IntGetMask()
 *
 *****************************************************************************/
int32 OS_IntGetMask ( uint32 * MaskSettingPtr )
{
    UT_Stub_RegisterContext(UT_KEY(OS_IntGetMask), MaskSettingPtr);
    return UT_DEFAULT_IMPL(OS_IntGetMask);
}


