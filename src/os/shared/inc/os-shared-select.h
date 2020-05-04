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
 * \file     os-shared-select.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_SELECT_H_
#define INCLUDE_OS_SHARED_SELECT_H_

#include <os-shared-globaldefs.h>


/*
 * Select API
 * Blocks until specified readable/writable conditions
 * are met on a file id or set of file ids
 */

/*----------------------------------------------------------------
   Function: OS_SelectSingle_Impl

   Purpose: Waits for activity on a single file descriptor
            This wrapper is usable by the File or Socket API
            The type of activity to wait for is indicated by "SelectFlags"
            msecs indicates the timeout.  Positive values will wait up to that many milliseconds.
            Zero will not wait (poll) or negative values will wait forever (pend)

            Bits in "SelectFlags" will be unset according to activity

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SelectSingle_Impl(uint32 stream_id, uint32 *SelectFlags, int32 msecs);

/*----------------------------------------------------------------

   Function: OS_SelectMultiple_Impl

   Purpose: Waits for activity on multiple file descriptors
            This wrapper is usable by the File or Socket API
            Will wait for any file descriptor in "ReadSet" to be readable OR
            any descriptor in "WriteSet" to be writable.
            Time-Limited to "msecs" (negative to wait forever, zero to poll)

   Notes: It is not possible for this function to verify that the file descriptors
          passed in are actually valid.  In order to do so would require a different
          approach to the OS_FdSet structure (this is currently just a bitmask so
          the actual file descriptor value is lost in translation).

          Using an array of uint32's would solve the problem but make the structures
          much bigger.

          File descriptors in sets be modified according to activity

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);


#endif  /* INCLUDE_OS_SHARED_SELECT_H_ */

