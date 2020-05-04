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

