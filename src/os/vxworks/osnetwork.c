/*
** File   : osnetwork.c
**
**      Copyright (c) 2004-2015, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved. This software was created at NASA Glenn
**      Research Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Joe Hickey based on original RTEMS implementation by Nicholas Yanchik
**
** Purpose: This file contains the network functionality for the osapi.
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#ifdef OS_INCLUDE_NETWORK

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <hostLib.h>

/* Leverage the portable BSD sockets implementation */
#define BSDSOCKET_HANDLE_OPTIONS        0
#define OS_HOST_NAME_LEN                48

/*
 * Use the O_NONBLOCK flag on sockets
 */
const int OS_IMPL_SOCKET_FLAGS = O_NONBLOCK;


/* The "in.h" header file supplied in VxWorks 6.9 is missing the "in_port_t" typedef */
typedef u_short in_port_t;


#include "../portable/os-impl-bsd-sockets.c"


                        
/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetID_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl             (int32 *IdBuf)
{
    int    host_id;
    int32  status;
    char   host_name [OS_HOST_NAME_LEN];

    status = OS_NetworkGetHostName_Impl(host_name, sizeof(host_name));
    if (status == OS_SUCCESS)
    {
        host_id = hostGetByName(host_name);
        if (host_id == ERROR)
        {
            status = OS_ERROR;
        }
        else
        {
            *IdBuf = (int32)host_id;
            status = OS_SUCCESS;
        }
    }

    return status;

} /* end OS_NetworkGetID_Impl */


#endif

