/*
** File: osapi-os-net.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author:  Alan Cudmore Code 582
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, Network Module
**
** $Revision: 1.2 $ 
**
** $Date: 2010/11/12 12:00:19GMT-05:00 $
**
** $Log: osapi-os-net.h  $
** Revision 1.2 2010/11/12 12:00:19GMT-05:00 acudmore 
** replaced copyright character with (c) and added open source notice where needed.
** Revision 1.1 2008/04/20 22:36:02EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/os/inc/project.pj
** Revision 1.1 2007/10/16 16:14:52EDT apcudmore 
** Initial revision
** Member added to project d:/mksdata/MKS-OSAL-REPOSITORY/src/os/inc/project.pj
** Revision 1.1 2007/08/24 13:43:25EDT apcudmore 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-PROJECT/fsw/cfe-core/os/inc/project.pj
** Revision 1.3 2006/01/20 11:56:18EST njyanchik 
** Fixed header file information to match api document
** Revision 1.4  2005/06/07 16:49:31  nyanchik
** changed returns code for osapi.c to all int32 from uint32
**
** Revision 1.3  2005/03/22 19:04:54  acudmore
** fixed uint type
**
** Revision 1.2  2005/03/22 18:59:33  acudmore
** updated prototype
**
** Revision 1.1  2005/03/22 18:58:51  acudmore
** added osapi network interface
**
** Revision 1.1  2005/03/15 18:26:32  nyanchik
** *** empty log message ***
**
**
** Date Written:
**
**    
*/
#ifndef _osapi_network_
#define _osapi_network_

/*
 * Sockets API wrapper functions
 */

typedef enum
{
   OS_SocketDomain_INVALID,
#ifdef OS_INCLUDE_NETWORK
   OS_SocketDomain_INET,
   OS_SocketDomain_INET6,
#endif
   OS_SocketDomain_MAX
} OS_SocketDomain_t;

typedef enum
{
   OS_SocketType_INVALID,
#ifdef OS_INCLUDE_NETWORK
   OS_SocketType_DATAGRAM,
   OS_SocketType_STREAM,
#endif
   OS_SocketType_MAX
} OS_SocketType_t;

#ifdef OS_INCLUDE_NETWORK

/* NOTE - osconfig.h may optionally specify the value for OS_SOCADDR_MAX_LEN */
#include <osconfig.h>

#ifndef OS_SOCKADDR_MAX_LEN
#define OS_SOCKADDR_MAX_LEN     32
#endif

typedef struct
{
   uint32 ActualLength;
   char AddrData[OS_SOCKADDR_MAX_LEN];
} OS_SockAddr_t;

typedef struct
{
    char name [OS_MAX_API_NAME];
    uint32 creator;
} OS_socket_prop_t;


int32 OS_SocketOpen(uint32 *sock_id, OS_SocketDomain_t Domain, OS_SocketType_t Type);
int32 OS_SocketClose(uint32 sock_id);
int32 OS_SocketBind(uint32 sock_id, const OS_SockAddr_t *Addr);
int32 OS_SocketConnect(uint32 sock_id, const OS_SockAddr_t *Addr, int32 timeout);
int32 OS_SocketAccept(uint32 sock_id, uint32 *connsock_id, OS_SockAddr_t *Addr, int32 timeout);
int32 OS_SocketRecvFrom(uint32 sock_id, void *buffer, uint32 buflen, OS_SockAddr_t *RemoteAddr, int32 timeout);
int32 OS_SocketSendTo(uint32 sock_id, const void *buffer, uint32 buflen, const OS_SockAddr_t *RemoteAddr);
int32 OS_SocketGetIdByName (uint32 *sock_id, const char *sock_name);
int32 OS_SocketGetInfo (uint32 sock_id, OS_socket_prop_t *sock_prop);

int32 OS_SocketAddrInit(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain);
int32 OS_SocketAddrToString(char *buffer, uint32 buflen, const OS_SockAddr_t *Addr);
int32 OS_SocketAddrFromString(OS_SockAddr_t *Addr, const char *string);
int32 OS_SocketAddrGetPort(uint16 *PortNum, const OS_SockAddr_t *Addr);
int32 OS_SocketAddrSetPort(OS_SockAddr_t *Addr, uint16 PortNum);

/*
** OS_NetworkGetID is currently [[deprecated]] as its behavior is
** unknown and not consistent across operating systems.
*/
int32 OS_NetworkGetID             (void);
int32 OS_NetworkGetHostName       (char *host_name, uint32 name_len);

#endif  /* OS_INCLUDE_NETWORK */

#endif
