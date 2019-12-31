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
#include "ut-osfileapi.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osfileapi.c"

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };


unsigned int Osapi_Internal_GetSelfEUID(void)
{
    return OS_IMPL_SELF_EUID;
}

unsigned int Osapi_Internal_GetSelfEGID(void)
{
    return OS_IMPL_SELF_EGID;
}

void Osapi_Internal_Set_Selectable(uint32 local_id, bool is_selectable)
{
    OS_impl_filehandle_table[local_id].selectable = is_selectable;
}
