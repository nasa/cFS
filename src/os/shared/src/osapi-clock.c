/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
