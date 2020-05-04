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
 * \file     os-shared-heap.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_HEAP_H_
#define INCLUDE_OS_SHARED_HEAP_H_

#include <os-shared-globaldefs.h>


/****************************************************************************************
                    MEMORY HEAP API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * This may also not be implementable on some platforms
 */

/*----------------------------------------------------------------
   Function: OS_HeapGetInfo_Impl

    Purpose: Return current info on the heap

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop);

#endif  /* INCLUDE_OS_SHARED_HEAP_H_ */

