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

/* OSAL coverage stub replacement for string.h */
#ifndef _OSAL_STUB_STRING_H_
#define _OSAL_STUB_STRING_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in string.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in string.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in string.h */
/* ----------------------------------------- */

extern void *OCS_memcpy (void * dest, const void * src, size_t n);
extern void *OCS_memset (void * s, int c, size_t n);
extern int OCS_strcmp (const char * s1, const char * s2);
extern char *OCS_strcpy (char * dest, const char * src);
extern size_t OCS_strlen (const char * s);
extern int OCS_strncmp (const char * s1, const char * s2, size_t n);
extern char *OCS_strncpy (char * dest, const char * src, size_t n);
extern char *OCS_strchr(const char *s, int c);
extern char *OCS_strrchr (const char * s, int c);
extern char *OCS_strcat(char *dest, const char *src);
extern char *OCS_strncat(char *dest, const char *src, size_t n);
extern char *OCS_strerror(int errnum);

#endif /* _OSAL_STUB_STRING_H_ */

