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
 * \file     osapi-timer-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"
#include "os-shared-timebase.h"
#include "os-shared-clock.h"

/*
** OS Time/Tick related API
*/
UT_DEFAULT_STUB(OS_TimeBaseCreate_Impl,(uint32 timer_id))
UT_DEFAULT_STUB(OS_TimeBaseSet_Impl,(uint32 timer_id, int32 start_time, int32 interval_time))
UT_DEFAULT_STUB(OS_TimeBaseDelete_Impl,(uint32 timer_id))
void  OS_TimeBaseLock_Impl          (uint32 timebase_id)
{
    UT_DEFAULT_IMPL(OS_TimeBaseLock_Impl);
}

void  OS_TimeBaseUnlock_Impl        (uint32 timebase_id)
{
    UT_DEFAULT_IMPL(OS_TimeBaseUnlock_Impl);
}

UT_DEFAULT_STUB(OS_TimeBaseGetInfo_Impl,(uint32 timer_id, OS_timebase_prop_t *timer_prop))

UT_DEFAULT_STUB(OS_TimeBaseRegister_Impl,(uint32 timebase_id))
/*
 * Clock API low-level handlers
 */
UT_DEFAULT_STUB(OS_GetLocalTime_Impl,(OS_time_t *time_struct))
UT_DEFAULT_STUB(OS_SetLocalTime_Impl,(const OS_time_t *time_struct))


