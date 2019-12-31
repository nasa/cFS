/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-osapi-filesys.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi-filesys.c"

/*
 * Stub objects to satisfy linking requirements -
 */
OS_common_record_t OS_stub_filesys_table[OS_MAX_FILE_SYSTEMS];
OS_common_record_t OS_stub_stream_table[OS_MAX_NUM_OPEN_FILES];
OS_common_record_t * const OS_global_filesys_table  = OS_stub_filesys_table;
OS_common_record_t * const OS_global_stream_table   = OS_stub_stream_table;

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };

/*
 * Provide a wrapper so the UT can invoke OS_FileSys_FindVirtMountPoint()
 */
bool Osapi_Internal_FileSys_FindVirtMountPoint(void *ref, uint32 local_id, const OS_common_record_t *obj)
{
    return OS_FileSys_FindVirtMountPoint(ref, local_id, obj);
}

/*
 * Provide a wrapper so the UT can invoke OS_FileSys_InitLocalFromVolTable()
 */
int32 Osapi_Internal_FileSys_InitLocalFromVolTable(OS_filesys_internal_record_t *local, const OS_VolumeInfo_t *Vol)
{
    return OS_FileSys_InitLocalFromVolTable(local, Vol);
}

