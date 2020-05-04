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
 * \file     os-impl-files.h
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_FILES_H_
#define INCLUDE_OS_IMPL_FILES_H_

#include "os-impl-io.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


/*
 * These two constants (EUID and EGID) are local cache of the
 * euid and egid of the user running the OSAL application.  They
 * assist the "stat" implementation in determination of permissions.
 *
 * For an OS that does not have multiple users, these could be
 * defined as 0.  Otherwise they should be populated via the system
 * geteuid/getegid calls.
 */
extern uid_t OS_IMPL_SELF_EUID;
extern gid_t OS_IMPL_SELF_EGID;


extern const int OS_IMPL_REGULAR_FILE_FLAGS;

#endif  /* INCLUDE_OS_IMPL_FILES_H_ */

