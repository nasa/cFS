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
 * \file     os-shared-network.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_NETWORK_H_
#define INCLUDE_OS_SHARED_NETWORK_H_

#include <os-shared-globaldefs.h>



/****************************************************************************************
              NETWORK / SOCKET API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_NetworkAPI_Init

   Purpose: Initialize the OS-independent layer for network services

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_NetworkAPI_Init             (void);


/*----------------------------------------------------------------

   Function: OS_NetworkGetHostName_Impl

    Purpose: Gets the name of the current host

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl (char *host_name, uint32 name_len);

/*----------------------------------------------------------------
   Function: OS_NetworkGetID_Impl

    Purpose: Gets the ID of the host on the network

    Returns: the ID value on success, or -1 on error.
 ------------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl       (int32 *IdBuf);

#endif  /* INCLUDE_OS_SHARED_NETWORK_H_ */

