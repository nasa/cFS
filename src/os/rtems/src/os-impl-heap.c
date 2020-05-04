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
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-shared-heap.h"


/****************************************************************************************
                                     HEAP API
 ***************************************************************************************/


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
    region_information_block info;
    int                      status;

    status = malloc_info( &info );

    if ( status != 0 )
    {
       return(OS_ERROR);
    }

    heap_prop->free_bytes         = (uint32) info.Free.total;
    heap_prop->free_blocks        = (uint32) info.Free.number;
    heap_prop->largest_free_block = (uint32) info.Free.largest;

    return (OS_SUCCESS);
} /* end OS_HeapGetInfo_Impl */

