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

/* OSAL coverage stub replacement for sched.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_sched.h>


int OCS_sched_get_priority_max(int policy)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_sched_get_priority_max,9);

    return Status;
}


int OCS_sched_get_priority_min(int policy)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_sched_get_priority_min,1);

    return Status;
}



