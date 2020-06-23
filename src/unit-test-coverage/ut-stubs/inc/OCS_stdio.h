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

/* OSAL coverage stub replacement for stdio.h */
#ifndef _OSAL_STUB_STDIO_H_
#define _OSAL_STUB_STDIO_H_

#include <OCS_basetypes.h>
#include <OCS_stdarg.h>

/* ----------------------------------------- */
/* constants normally defined in stdio.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in stdio.h */
/* ----------------------------------------- */
typedef struct OCS_FILE OCS_FILE;


/* ----------------------------------------- */
/* prototypes normally declared in stdio.h */
/* ----------------------------------------- */

extern int OCS_fclose (OCS_FILE * stream);
extern char *OCS_fgets (char * s, int n, OCS_FILE * stream);
extern OCS_FILE *OCS_fopen (const char * filename, const char * modes);
extern int OCS_fputs (const char * s, OCS_FILE * stream);
extern int OCS_remove (const char * filename);
extern int OCS_rename (const char * old, const char * nw);
extern int OCS_snprintf (char * s, size_t maxlen, const char * format, ...);
extern int OCS_vsnprintf (char * s, size_t maxlen, const char * format, OCS_va_list arg);
extern int OCS_printf (const char * format, ...);
extern int OCS_putchar (int c);

extern OCS_FILE*    OCS_stdin;
extern OCS_FILE*    OCS_stdout;
extern OCS_FILE*    OCS_stderr;


#endif /* _OSAL_STUB_STDIO_H_ */

