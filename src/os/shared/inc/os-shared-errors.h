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
 * \file     os-shared-errors.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_ERRORS_H_
#define INCLUDE_OS_SHARED_ERRORS_H_

#include <os-shared-globaldefs.h>


/* Mapping of integer error number to name */
typedef struct
{
   int32 Number;
   const char *Name;
} OS_ErrorTable_Entry_t;

extern const OS_ErrorTable_Entry_t  OS_IMPL_ERROR_NAME_TABLE[];

#endif  /* INCLUDE_OS_SHARED_ERRORS_H_ */

