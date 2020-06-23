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
 * \file     ut-adaptor-files.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_FILES_H_
#define INCLUDE_UT_ADAPTOR_FILES_H_

#include <common_types.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/


/*
 * Prototype for table init function (needs to be called from UT)
 */
extern int32 UT_Call_OS_VxWorks_StreamAPI_Impl_Init(void);

/*
 * Allow UT to get the value of the OS_IMPL_SELF_EUID and
 * OS_IMPL_SELF_EGID constants.  These might be assigned
 * at runtime by the init function (above) or they might be
 * defined at compile time.
 */
unsigned int UT_FileTest_GetSelfEUID(void);
unsigned int UT_FileTest_GetSelfEGID(void);

void UT_FileTest_Set_Selectable(uint32 local_id, bool is_selectable);

#endif  /* INCLUDE_UT_ADAPTOR_FILES_H_ */

