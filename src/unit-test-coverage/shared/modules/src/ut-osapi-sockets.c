/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-osapi-sockets.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi-sockets.c"

/* Global variables to satisfy external link symbols */
OS_stream_internal_record_t OS_stream_table[OS_MAX_NUM_OPEN_FILES];
OS_common_record_t OS_stub_socket_table[OS_MAX_NUM_OPEN_FILES];
OS_common_record_t * const OS_global_stream_table   = OS_stub_socket_table;

void Osapi_Call_CreateSocketName_Static(OS_stream_internal_record_t *sock, const OS_SockAddr_t *Addr, const char *parent_name)
{
    OS_CreateSocketName(sock, Addr, parent_name);
}

