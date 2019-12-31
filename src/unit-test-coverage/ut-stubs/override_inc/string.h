/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for string.h */
#ifndef _OSAL_OVERRIDE_STRING_H_
#define _OSAL_OVERRIDE_STRING_H_

#include <OCS_string.h>

/* ----------------------------------------- */
/* mappings for declarations in string.h */
/* ----------------------------------------- */
#define memcpy       OCS_memcpy
#define memset       OCS_memset
#define strcmp       OCS_strcmp
#define strcpy       OCS_strcpy
#define strlen       OCS_strlen
#define strncmp      OCS_strncmp
#define strncpy      OCS_strncpy
#define strchr       OCS_strchr
#define strrchr      OCS_strrchr
#define strcat       OCS_strcat
#define strncat      OCS_strncat
#define strerror     OCS_strerror

#endif /* _OSAL_OVERRIDE_STRING_H_ */
