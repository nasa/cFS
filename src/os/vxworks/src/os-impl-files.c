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
 * \file     os-impl-files.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-files.h"
#include "os-shared-file.h"

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
OS_VxWorks_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];


/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_StreamAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_StreamAPI_Impl_Init(void)
{
    uint32 local_id;

    /*
     * init all filehandles to -1, which is always invalid.
     * this isn't strictly necessary but helps when debugging.
     */
    for (local_id = 0; local_id <  OS_MAX_NUM_OPEN_FILES; ++local_id)
    {
        OS_impl_filehandle_table[local_id].fd = -1;
        OS_impl_filehandle_table[local_id].selectable = false;
    }

    return OS_SUCCESS;
} /* end OS_VxWorks_StreamAPI_Impl_Init */


