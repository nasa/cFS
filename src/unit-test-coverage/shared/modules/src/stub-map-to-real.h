/*
 *  Copyright (c) 2004-2015, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file stub-map-to-real.h
 *
 *  Created on: Oct 29, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Placeholder for file content description
 */

#ifndef STUB_MAP_TO_REAL_H_
#define STUB_MAP_TO_REAL_H_

/*******************************************************************************
 * OCS VALUE MAPPINGS for constants normally supplied by libc headers
 *******************************************************************************/

#define EXIT_SUCCESS                 OCS_EXIT_SUCCESS
#define EXIT_FAILURE                 OCS_EXIT_FAILURE


/*******************************************************************************
 * OCS TYPE MAPPINGS for APIs normally supplied by libc headers
 *******************************************************************************/
#define va_list                  OCS_va_list

/*******************************************************************************
 * OCS FUNCTION MAPPINGS for APIs normally supplied by libc headers
 *******************************************************************************/

#define va_start                 OCS_va_start
#define va_end                   OCS_va_end
#define strrchr                  OCS_strrchr
#define strchr                   OCS_strchr
#define exit                     OCS_exit
#define strncpy                  OCS_strncpy
#define strlen                   OCS_strlen
#define strcmp                   OCS_strcmp
#define strcpy                   OCS_strcpy
#define strncpy                  OCS_strncpy
#define vsnprintf                OCS_vsnprintf
#define memset                   OCS_memset
#define memcpy                   OCS_memcpy
#define strcat                   OCS_strcat
#define strncat                  OCS_strncat
#define strncmp                  OCS_strncmp
#define snprintf                 OCS_snprintf
#define printf(...)              OCS_printf(__VA_ARGS__)
#define isgraph                  OCS_isgraph

#endif /* STUB_MAP_TO_REAL_H_ */
