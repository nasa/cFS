/*
 *  Copyright (c) 2004-2015, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file utstub-helpers.h
 *
 *  Created on: May 24, 2018
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Internal header file for OSAL UT stub functions
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#ifndef __OSAPI_UTSTUB_INTERNAL_H_
#define __OSAPI_UTSTUB_INTERNAL_H_

/*
 * Commonly used C library headers
 */
#include <string.h>

/*
 * Include all relevant OSAPI (public) and UT-assert headers
 */
#include "osapi.h"
#include "utstubs.h"
#include "utbsp.h"
#include "utassert.h"
#include "uttools.h"



typedef enum
{
    UT_OBJTYPE_NONE = 0,
    UT_OBJTYPE_TASK,
    UT_OBJTYPE_QUEUE,
    UT_OBJTYPE_COUNTSEM,
    UT_OBJTYPE_BINSEM,
    UT_OBJTYPE_MUTEX,
    UT_OBJTYPE_TIMECB,
    UT_OBJTYPE_MODULE,
    UT_OBJTYPE_FILESTREAM,
    UT_OBJTYPE_SOCKET,
    UT_OBJTYPE_TIMEBASE,
    UT_OBJTYPE_DIR,
    UT_OBJTYPE_FILESYS,
    UT_OBJTYPE_MAX
} UT_ObjType_t;

/*
 * Function to adjust any objid into the proper range
 *
 * If building an OSAL that has opaque (nonzero) object IDs,
 * then these stub functions can also replicate that.  This
 * macro will be used to support both opaque and non-opaque IDs
 */
#define UT_FIXUP_ID(id,typ)        id = UT_ObjIdFixup(id,typ)

/*
 * Size of the bitmask for the OSAL fake object ID validity table
 * Set this according to the highest anticipated number of OSAL objects
 * simultaneously created by any given test case (it is reset when the
 * test is reset).
 *
 * Keep as a power of two, and actual number of bits is 8x this value
 * (Default value of 16 allows for up to 128 objects to be created in
 * a single test case, far more than anything I've seen yet)
 */
#define OSAL_MAX_VALID_PER_TYPE     16

typedef struct
{
    uint32 LastIssueNumber;
    uint8 ValidBits[OSAL_MAX_VALID_PER_TYPE];
} UT_ObjTypeState_t;


/**
 * Table lookup for the maximum number of OSAL objects by object type
 */
extern const uint32 UT_MAXOBJS[];

/*
 * Helper function - "allocate" a fake object ID of the given type
 */
uint32 UT_AllocStubObjId(UT_ObjType_t ObjType);

/*
 * Helper function - "deallocate" a fake object ID of the given type
 */
void UT_DeleteStubObjId(UT_ObjType_t ObjType, uint32 ObjId);

/*
 * Helper function - Report any queue objects found open
 * (for compatibility with CFE tests, only checks queues)
 */
void UT_CheckForOpenSockets(void);

/*
 * Helper function - Clear all OSAL UT stub objects
 * Resets the stub object table back to its initial/empty state
 */
void UT_ClearAllStubObjects(void);

/*
 * Takes an ID index and sets the proper ID bits.
 */
uint32 UT_ObjIdFixup(uint32 val,uint32 objtype);

#endif


