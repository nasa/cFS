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


