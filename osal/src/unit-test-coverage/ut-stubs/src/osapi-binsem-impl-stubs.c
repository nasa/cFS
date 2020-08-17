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
 * \file     osapi-binsem-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-binsem.h"


/*
** Semaphore API
*/

UT_DEFAULT_STUB(OS_BinSemCreate_Impl,(uint32 sem_id, uint32 sem_initial_value, uint32 options))
UT_DEFAULT_STUB(OS_BinSemFlush_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemGive_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemTake_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemTimedWait_Impl,(uint32 sem_id, uint32 msecs))
UT_DEFAULT_STUB(OS_BinSemDelete_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemGetInfo_Impl,(uint32 sem_id, OS_bin_sem_prop_t *bin_prop))

