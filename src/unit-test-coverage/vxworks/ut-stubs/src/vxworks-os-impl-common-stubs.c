/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     vxworks-os-impl-common-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <os-shared-common.h>
#include <OCS_semLib.h>

UT_DEFAULT_STUB(OS_API_Impl_Init, (uint32 idtype))

int OS_VxWorks_GenericSemTake(OCS_SEM_ID vxid, int sys_ticks)
{
    return UT_DEFAULT_IMPL(OS_VxWorks_GenericSemTake);
}

int OS_VxWorks_GenericSemGive(OCS_SEM_ID vxid)
{
    return UT_DEFAULT_IMPL(OS_VxWorks_GenericSemGive);
}
