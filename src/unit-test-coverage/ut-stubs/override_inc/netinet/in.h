/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for netinet/in.h */
#ifndef _OSAL_OVERRIDE_NETINET_IN_H_
#define _OSAL_OVERRIDE_NETINET_IN_H_

#include <OCS_netinet_in.h>

/* ----------------------------------------- */
/* mappings for declarations in netinet/in.h */
/* ----------------------------------------- */

#define htons    OCS_htons
#define ntohs    OCS_ntohs
#define htonl    OCS_htonl
#define ntohl    OCS_ntohl


#endif /* _OSAL_OVERRIDE_NETINET_IN_H_ */
