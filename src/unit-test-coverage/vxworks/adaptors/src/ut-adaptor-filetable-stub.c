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
 * \file     ut-adaptor-filetable-stub.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-filetable-stub.h"

#include <os-vxworks.h>
#include <os-impl-files.h>




OS_VxWorks_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];


void*  const UT_FileTableTest_OS_impl_filehandle_table = OS_impl_filehandle_table;
size_t const UT_FileTableTest_OS_impl_filehandle_table_SIZE = sizeof(OS_impl_filehandle_table);
