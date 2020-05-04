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
 * \file     ut-adaptor-idmap.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/* pull in the OSAL configuration */
#include <osconfig.h>
#include <stdlib.h>
#include <string.h>

#include <os-vxworks.h>
#include "ut-adaptor-idmap.h"

int32 UT_Call_OS_VxWorks_TableMutex_Init(uint32 idtype)
{
    return OS_VxWorks_TableMutex_Init(idtype);
}

void UT_IdMapTest_SetImplTableMutex(uint32 idtype, OCS_SEM_ID vxid)
{
    VX_MUTEX_TABLE[idtype].vxid = vxid;
}

