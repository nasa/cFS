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
 * \file     os-impl-posix-network.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains the network functionality for
 * systems which implement the POSIX-defined network hostname/id functions.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must include whatever is required to get the prototypes of these functions:
 *
 *  gethostname()
 *  gethostid()
 *
 * Both of these routines should conform to X/Open 5 definition.
 */
#include <string.h>
#include <errno.h>

#include "os-impl-network.h"
#include "os-shared-network.h"

/****************************************************************************************
                                    Network API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetHostName_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl       (char *host_name, uint32 name_len)
{
    int32 return_code;

    if ( gethostname(host_name, name_len) < 0 )
    {
        return_code = OS_ERROR;
    }
    else
    {
        /*
         * posix does not say that the name is always
         * null terminated, so its worthwhile to ensure it
         */
        host_name[name_len - 1] = 0;
        return_code = OS_SUCCESS;
    }

    return(return_code);
} /* end OS_NetworkGetHostName_Impl */



/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetID_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl             (int32 *IdBuf)
{
    /* gethostid() has no failure modes */
    *IdBuf = gethostid();
    return OS_SUCCESS;
} /* end OS_NetworkGetID_Impl */


