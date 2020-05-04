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
 * \file     os-impl-heap.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-shared-heap.h"

#include <memPartLib.h>

/****************************************************************************************
                                     HEAP API
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_HeapGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_HeapGetInfo_Impl       (OS_heap_prop_t *heap_prop)
{
    MEM_PART_STATS stats;
    STATUS status;

    status = memPartInfoGet(memSysPartId, &stats);

    if (status != OK)
    {
        return OS_ERROR;
    }

    heap_prop->free_bytes         = stats.numBytesFree;
    heap_prop->free_blocks        = stats.numBlocksFree;
    heap_prop->largest_free_block = stats.maxBlockSizeFree;

    return (OS_SUCCESS);
} /* end OS_HeapGetInfo_Impl */

