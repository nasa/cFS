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
 * \file     ut-adaptor-binsem.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_BINSEM_H_
#define INCLUDE_UT_ADAPTOR_BINSEM_H_

#include <common_types.h>
#include <utstubs.h>
#include <OCS_taskLib.h>
#include <OCS_semLib.h>


extern void*  const UT_Ref_OS_impl_bin_sem_table;
extern size_t const UT_Ref_OS_impl_bin_sem_table_SIZE;

/*
 * This also needs to expose the keys for the stubs to
 * helper functions that the test case needs to configure.
 *
 * This is because the test case cannot directly include
 * the internal header file which provides this API.
 */
extern const UT_EntryKey_t UT_StubKey_GenericSemTake;
extern const UT_EntryKey_t UT_StubKey_GenericSemGive;

extern int32 UT_Call_OS_VxWorks_BinSemAPI_Impl_Init(void);

#endif  /* INCLUDE_UT_ADAPTOR_BINSEM_H_ */

