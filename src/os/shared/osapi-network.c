/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osapi-network.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *
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
#include "common_types.h"
#include "os-impl.h"


/****************************************************************************************
                                  NETWORK API
 ***************************************************************************************/

/* Initialization function */
int32 OS_NetworkAPI_Init(void)
{
    return OS_SUCCESS;
}


/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetHostName

    Purpose: Gets the name of the current host

    Returns: OS_ERROR if the  host name could not be found
             OS_SUCCESS if the name was copied to host_name successfully
---------------------------------------------------------------------------------------*/

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
}/* end OS_NetworkGetHostName */



/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetID

    Purpose: Gets the ID of the current Network

    Returns: The ID or fixed value of -1 if the host id could not be found
             Note this is _not_ an OS_ERROR code -- it is not possible
             to differentiate between error codes and valid network IDs here.
             It is assumed, however, that -1 is never a valid ID.

---------------------------------------------------------------------------------------*/
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

}/* end OS_NetworkGetID */


