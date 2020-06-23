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
 * \file     osapi-queue-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-queue.h"

/*
** Message Queue API
*/

UT_DEFAULT_STUB(OS_QueueCreate_Impl,(uint32 queue_id, uint32 flags))
UT_DEFAULT_STUB(OS_QueueDelete_Impl,(uint32 queue_id))
UT_DEFAULT_STUB(OS_QueueGet_Impl,(uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout))
UT_DEFAULT_STUB(OS_QueuePut_Impl,(uint32 queue_id, const void *data, uint32 size, uint32 flags))
UT_DEFAULT_STUB(OS_QueueGetInfo_Impl,(uint32 queue_id, OS_queue_prop_t *queue_prop))

