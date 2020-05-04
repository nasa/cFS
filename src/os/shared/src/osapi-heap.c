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
 * \file     osapi-heap.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         Contains the code related to heap.
 *         Implementation of these are mostly in the lower layer; however
 *         a wrapper must exist at this level which allows for unit testing.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * User defined include files
 */
#include "os-shared-heap.h"



/*----------------------------------------------------------------
 *
 * Function: OS_HeapGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_HeapGetInfo       (OS_heap_prop_t *heap_prop)
{
    if (heap_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_HeapGetInfo_Impl(heap_prop);
} /* end OS_HeapGetInfo */

