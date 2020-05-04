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
 * \file     osapi-clock.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         Contains the code related to clock getting / setting.
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
#include "os-shared-clock.h"


/*----------------------------------------------------------------
 *
 * Function: OS_GetLocalTime
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GetLocalTime(OS_time_t *time_struct)
{
    if (time_struct == NULL)
    {
       return OS_INVALID_POINTER;
    }

    return OS_GetLocalTime_Impl(time_struct);

} /* end OS_GetLocalTime */


/*----------------------------------------------------------------
 *
 * Function: OS_SetLocalTime
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SetLocalTime(OS_time_t *time_struct)
{
    if (time_struct == NULL)
    {
       return OS_INVALID_POINTER;
    }

    return OS_SetLocalTime_Impl(time_struct);

} /* end OS_SetLocalTime */
