/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for sys/socket.h */
#ifndef _OSAL_OVERRIDE_SYS_SOCKET_H_
#define _OSAL_OVERRIDE_SYS_SOCKET_H_

#include <OCS_sys_socket.h>

/* ----------------------------------------- */
/* mappings for declarations in sys/socket.h */
/* ----------------------------------------- */
#define socklen_t    OCS_socklen_t
#define sockaddr     OCS_sockaddr
#define accept       OCS_accept
#define bind         OCS_bind
#define connect      OCS_connect
#define getsockopt   OCS_getsockopt
#define listen       OCS_listen
#define recvfrom     OCS_recvfrom
#define sendto       OCS_sendto
#define setsockopt   OCS_setsockopt
#define socket       OCS_socket


#endif /* _OSAL_OVERRIDE_SYS_SOCKET_H_ */

