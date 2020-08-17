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

