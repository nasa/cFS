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
 * \file     ut-adaptor-filesys.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_FILESYS_H_
#define INCLUDE_UT_ADAPTOR_FILESYS_H_

#include <common_types.h>
#include <OCS_xbdBlkDev.h>

extern void*  const UT_Ref_OS_impl_filesys_table;
extern size_t const UT_Ref_OS_impl_filesys_table_SIZE;


void UT_FileSysTest_SetupFileSysEntry(uint32 id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts);


#endif  /* INCLUDE_UT_ADAPTOR_FILESYS_H_ */

