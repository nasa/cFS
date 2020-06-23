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

/**
 * \file     bsd-select-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* OSAL coverage stub replacement for functions in sys/stat.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_sys_select.h>

int OCS_select (int nfds, OCS_fd_set * readfds, OCS_fd_set * writefds, OCS_fd_set * exceptfds, struct OCS_timeval * timeout)
{
    return UT_DEFAULT_IMPL_RC(OCS_select, 1);
}

void OCS_FD_SET(int fd, OCS_fd_set *set)
{
    UT_DEFAULT_IMPL(OCS_FD_SET);
}

int OCS_FD_ISSET(int fd, OCS_fd_set *set)
{
    return UT_DEFAULT_IMPL(OCS_FD_ISSET);
}

void OCS_FD_CLR(int fd, OCS_fd_set *set)
{
    UT_DEFAULT_IMPL(OCS_FD_CLR);
}

void OCS_FD_ZERO(OCS_fd_set *set)
{
    UT_DEFAULT_IMPL(OCS_FD_ZERO);
}

