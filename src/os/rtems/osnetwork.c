/*
** File   : osnetwork.c
** 
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Nicholas Yanchik
**
** Purpose: This file contains the network functionality for the osapi.
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

#include "common_types.h"
#include "osapi.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/


/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/


/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/


/****************************************************************************************
                                    Network API
****************************************************************************************/

/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetID
    
    Purpose: Gets the ID of the current Network 

    Returns: OS_ERROR if the  host id could not be found
             a 32 bit host id if success
---------------------------------------------------------------------------------------*/
int32 OS_NetworkGetID             (void)
{
#ifdef OS_INCLUDE_NETWORK
     int    host_id;

     host_id = gethostid();
     if (host_id == -1)
     {
       return OS_ERROR;
     }
     else
     { 
        return (host_id);
     }
 #else
    return(OS_ERR_NOT_IMPLEMENTED);
 #endif
    
}/* end OS_NetworkGetID */
/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetHostName
    
    Purpose: Gets the name of the current host

    Returns: OS_ERROR if the  host name could not be found
             OS_SUCCESS if the name was copied to host_name successfully
---------------------------------------------------------------------------------------*/

int32 OS_NetworkGetHostName       (char *host_name, uint32 name_len)
{
#ifdef OS_INCLUDE_NETWORK

   int    retval = OS_SUCCESS;
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
      retval = gethostname( host_name, name_len); 
      if ( retval == -1 )
      {
         return_code = OS_ERROR;
      }
      else
      {
         return_code = OS_SUCCESS;
      }
   }

   return(return_code);
#else
   return(OS_ERR_NOT_IMPLEMENTED);
#endif
}/* end OS_NetworkGetHostName */


