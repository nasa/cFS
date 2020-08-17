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
 * \file     osapi-network.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
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
#include "os-shared-network.h"


/****************************************************************************************
                                  NETWORK API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_NetworkAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkAPI_Init(void)
{
    return OS_SUCCESS;
} /* end OS_NetworkAPI_Init */



/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetHostName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetHostName (char *host_name, uint32 name_len)
{
   uint32 return_code;

   if ( host_name == NULL)
   {
      return_code = OS_INVALID_POINTER;
   }
   else if ( name_len == 0 )
   {
      return_code = OS_ERROR;
   }
   else
   {
      /* delegate to low-level API */
      return_code = OS_NetworkGetHostName_Impl(host_name, name_len);

      if (return_code != OS_SUCCESS)
      {
          /* return an empty string on failure, just in case */
          host_name[0] = 0;
      }
   }

   return(return_code);
} /* end OS_NetworkGetHostName */




/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetID
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetID             (void)
{
    int32 IdBuf;

    /* always delegate to low-level API */
    if (OS_NetworkGetID_Impl(&IdBuf) != OS_SUCCESS)
    {
        /* return a hardcoded value on failure */
        return (-1);
    }

    return IdBuf;

} /* end OS_NetworkGetID */


