/*
 *  Copyright (c) 2004-2018, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"


/*****************************************************************************
 *
 * Stub function for OS_HeapGetInfo()
 *
 *****************************************************************************/
int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_HeapGetInfo);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_HeapGetInfo), heap_prop, sizeof(*heap_prop)) < sizeof(*heap_prop))
    {
        /* Return some random data */
        heap_prop->free_bytes = (uint32) 12345;
        heap_prop->free_blocks = (uint32) 6789;
        heap_prop->largest_free_block = (uint32) 100;
    }

    return status;
}


