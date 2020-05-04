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
 * \file     OCS_sys_select.h
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OCS_SYS_SELECT_H_
#define INCLUDE_OCS_SYS_SELECT_H_

#include <OCS_basetypes.h>
#include <OCS_time.h>

/* ----------------------------------------- */
/* constants normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/select.h */
/* ----------------------------------------- */
typedef struct { unsigned int fds; }    OCS_fd_set;

/* ----------------------------------------- */
/* prototypes normally declared in sys/select.h */
/* ----------------------------------------- */

extern int OCS_select (int nfds, OCS_fd_set * readfds, OCS_fd_set * writefds, OCS_fd_set * exceptfds, struct OCS_timeval * timeout);

extern void OCS_FD_SET(int fd, OCS_fd_set *set);
extern int OCS_FD_ISSET(int fd, OCS_fd_set *set);
extern void OCS_FD_CLR(int fd, OCS_fd_set *set);
extern void OCS_FD_ZERO(OCS_fd_set *set);

#endif  /* INCLUDE_OCS_SYS_SELECT_H_ */

