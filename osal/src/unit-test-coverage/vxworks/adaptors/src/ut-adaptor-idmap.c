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

