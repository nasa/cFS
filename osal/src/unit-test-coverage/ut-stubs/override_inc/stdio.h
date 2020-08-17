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
#ifndef _OSAL_OVERRIDE_STDIO_H_
#define _OSAL_OVERRIDE_STDIO_H_

#include <OCS_stdio.h>

/* ----------------------------------------- */
/* mappings for declarations in stdio.h */
/* ----------------------------------------- */

#define FILE           OCS_FILE
#define fclose         OCS_fclose
#define fgets          OCS_fgets
#define fopen          OCS_fopen
#define fputs          OCS_fputs
#define remove         OCS_remove
#define rename         OCS_rename
#define snprintf       OCS_snprintf
#define vsnprintf      OCS_vsnprintf
#define printf(...)    OCS_printf(__VA_ARGS__)
#define putchar        OCS_putchar

#define stdin          OCS_stdin
#define stdout         OCS_stdout
#define stderr         OCS_stderr


#endif /* _OSAL_OVERRIDE_STDIO_H_ */

