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
 * \file     os-shared-clock.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_CLOCK_H_
#define INCLUDE_OS_SHARED_CLOCK_H_

#include <os-shared-globaldefs.h>


/*
 * Clock API low-level handlers
 * These simply get/set the kernel RTC (if it has one)
 */

/*----------------------------------------------------------------
   Function: OS_GetLocalTime_Impl

    Purpose: Get the time from the RTC

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_GetLocalTime_Impl(OS_time_t *time_struct);

/*----------------------------------------------------------------
   Function: OS_SetLocalTime_Impl

    Purpose: Set the time in the RTC

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SetLocalTime_Impl(const OS_time_t *time_struct);


#endif  /* INCLUDE_OS_SHARED_CLOCK_H_ */

