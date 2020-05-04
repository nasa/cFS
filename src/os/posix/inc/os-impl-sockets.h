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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_SOCKETS_H_
#define INCLUDE_OS_IMPL_SOCKETS_H_

#include "os-impl-io.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>


#define OS_NETWORK_SUPPORTS_IPV6

/*
 * A full POSIX-compliant I/O layer should support using
 * nonblocking I/O calls in combination with select().
 */
#define OS_IMPL_SOCKET_FLAGS        O_NONBLOCK



#endif  /* INCLUDE_OS_IMPL_SOCKETS_H_ */

