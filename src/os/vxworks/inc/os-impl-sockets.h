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
 * \file     os-impl-sockets.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_SOCKETS_H_
#define INCLUDE_OS_IMPL_SOCKETS_H_

#include "os-impl-io.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <hostLib.h>

/*
 * Use the O_NONBLOCK flag on sockets
 */
#define  OS_IMPL_SOCKET_FLAGS       O_NONBLOCK


/* The "in.h" header file supplied in VxWorks 6.9 is missing the "in_port_t" typedef */
typedef u_short in_port_t;


#endif  /* INCLUDE_OS_IMPL_SOCKETS_H_ */

