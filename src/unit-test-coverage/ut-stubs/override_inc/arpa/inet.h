/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for arpa/inet.h */
#ifndef _OSAL_OVERRIDE_ARPA_INET_H_
#define _OSAL_OVERRIDE_ARPA_INET_H_

#include <OCS_arpa_inet.h>

/* ----------------------------------------- */
/* mappings for declarations in arpa/inet.h */
/* ----------------------------------------- */
#define inet_ntop    OCS_inet_ntop
#define inet_pton    OCS_inet_pton


#endif /* _OSAL_OVERRIDE_ARPA_INET_H_ */
