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

#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"

#include "common_types.h"
#include "osapi.h"

#ifdef OS_INCLUDE_NETWORK
  #include "hostLib.h"
#endif 

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

#define OS_HOST_NAME_LEN 35


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
  int    host_id = 0;

#ifdef OS_INCLUDE_NETWORK
  int    retval;
  char   host_name [OS_HOST_NAME_LEN];

  retval = gethostname( host_name, OS_HOST_NAME_LEN);
  if ( retval == -1 )
     return OS_ERROR;

  host_id = hostGetByName(host_name);
  if (host_id == -1)
       return OS_ERROR;
#endif  
 
  return (host_id);
    
}/* end OS_NetworkGetID */
/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetHostName
    
    Purpose: Gets the name of the current host

    Returns: OS_ERROR if the  host name could not be found
             OS_SUCCESS if the name was copied to host_name successfully
---------------------------------------------------------------------------------------*/

int32 OS_NetworkGetHostName       (char *host_name, uint32 name_len)
{
   uint32 return_code = OS_ERROR;
   
   if ( host_name == NULL)
   {
      return_code = OS_INVALID_POINTER;
   }
   else if ( name_len == 0 )
   {
      return_code = OS_ERROR;
   }
#ifdef OS_INCLUDE_NETWORK
   else
   {
      int retval;

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
#endif

   return(return_code);

}/* end OS_NetworkGetHostName */



