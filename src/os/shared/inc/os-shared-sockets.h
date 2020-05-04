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
 * \file     os-shared-sockets.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_SOCKETS_H_
#define INCLUDE_OS_SHARED_SOCKETS_H_

#include <os-shared-globaldefs.h>

/*
 * Sockets API abstraction layer
 */

/*---------------------------------------------------------------------------------------
   Name: OS_SocketAPI_Init

   Purpose: Initialize the OS-independent layer for network sockets

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_SocketAPI_Init              (void);

/*----------------------------------------------------------------
   Function: OS_SocketOpen_Impl

    Purpose: Opens the OS socket indicated by the sock_id table entry

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketOpen_Impl(uint32 sock_id);

/*----------------------------------------------------------------
   Function: OS_SocketBind_Impl

   Purpose: Binds the indicated socket table entry to the passed-in address

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketBind_Impl(uint32 sock_id, const OS_SockAddr_t *Addr);

/*----------------------------------------------------------------
   Function: OS_SocketAccept_Impl

   Purpose: Accept an incoming connection on the indicated socket (must be a STREAM socket)
            Will wait up to "timeout" milliseconds for an incoming connection
            Will wait forever if timeout is negative

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAccept_Impl(uint32 sock_id, uint32 connsock_id, OS_SockAddr_t *Addr, int32 timeout);

/*----------------------------------------------------------------
   Function: OS_SocketConnect_Impl

    Purpose: Connects the socket to a remote address.
             Socket must be of the STREAM variety.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketConnect_Impl(uint32 sock_id, const OS_SockAddr_t *Addr, int32 timeout);

/*----------------------------------------------------------------
   Function: OS_SocketRecvFrom_Impl

    Purpose: Receives a datagram from the specified socket (must be of the DATAGRAM type)
             Stores the datagram in "buffer" which has a maximum size of "buflen"
             Stores the remote address (sender of the datagram) in "RemoteAddr"
             Will wait up to "timeout" milliseconds to receive a packet
             (zero to poll, negative to wait forever)

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketRecvFrom_Impl(uint32 sock_id, void *buffer, uint32 buflen, OS_SockAddr_t *RemoteAddr, int32 timeout);

/*----------------------------------------------------------------
   Function: OS_SocketSendTo_Impl

    Purpose: Sends a datagram from the specified socket (must be of the DATAGRAM type)
             to the remote address specified by "RemoteAddr"
             The datagram to send must be stored in "buffer" with a size of "buflen"

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketSendTo_Impl(uint32 sock_id, const void *buffer, uint32 buflen, const OS_SockAddr_t *RemoteAddr);

/*----------------------------------------------------------------

   Function: OS_SocketGetInfo_Impl

    Purpose: Get OS-specific information about a socket

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketGetInfo_Impl (uint32 sock_id, OS_socket_prop_t *sock_prop);

/*----------------------------------------------------------------

   Function: OS_SocketAddrInit_Impl

    Purpose: Initializes an OSAL SockAddr structure to the given address domain

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrInit_Impl(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain);

/*----------------------------------------------------------------
   Function: OS_SocketAddrToString_Impl

   Purpose: Converts a Socket Address structure to a printable string
            Useful for including a dotted-decimal IP address in a message or log

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrToString_Impl(char *buffer, uint32 buflen, const OS_SockAddr_t *Addr);

/*----------------------------------------------------------------
   Function: OS_SocketAddrFromString_Impl

   Purpose: Sets the Address portion of the SockAddr structure according to the string
            For IPV4 (SocketDomain_INET) this will parse the dotted decimal IP address.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrFromString_Impl(OS_SockAddr_t *Addr, const char *string);

/*----------------------------------------------------------------
   Function: OS_SocketAddrGetPort_Impl

   Purpose: Retrieve the TCP/UDP port number from the SockAddr structure

   NOTE: The port number is output to the caller in native byte order
            (the value is converted from network order before return)

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrGetPort_Impl(uint16 *PortNum, const OS_SockAddr_t *Addr);

/*----------------------------------------------------------------
   Function: OS_SocketAddrSetPort_Impl

    Purpose: Set the TCP/UDP port number in the SockAddr structure

   NOTE: The port number should be passed in native byte order
             (this function will convert to network order)

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrSetPort_Impl(OS_SockAddr_t *Addr, uint16 PortNum);

/*
 * Internal helper functions
 * Not normally called outside the local unit, except during unit test
 */
void OS_CreateSocketName(uint32 local_id, const OS_SockAddr_t *Addr, const char *parent_name);

#endif  /* INCLUDE_OS_SHARED_SOCKETS_H_ */

