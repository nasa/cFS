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
 * \file     osapi-filesys-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"
#include "os-shared-filesys.h"

/*
 * File system abstraction layer
 */
UT_DEFAULT_STUB(OS_FileSysStartVolume_Impl,(uint32 filesys_id))
UT_DEFAULT_STUB(OS_FileSysStopVolume_Impl, (uint32 filesys_id))
UT_DEFAULT_STUB(OS_FileSysFormatVolume_Impl, (uint32 filesys_id))
UT_DEFAULT_STUB(OS_FileSysCheckVolume_Impl, (uint32 filesys_id, bool repair))
UT_DEFAULT_STUB(OS_FileSysMountVolume_Impl, (uint32 filesys_id))
UT_DEFAULT_STUB(OS_FileSysUnmountVolume_Impl, (uint32 filesys_id))

int32 OS_FileSysStatVolume_Impl (uint32 filesys_id, OS_statvfs_t *result)
{
    int32 Status = UT_DEFAULT_IMPL(OS_FileSysStatVolume_Impl);

    if (Status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_FileSysStatVolume_Impl), result, sizeof(*result)) < sizeof(*result))
    {
        memset(result, 0, sizeof(*result));
    }

    return Status;
}

