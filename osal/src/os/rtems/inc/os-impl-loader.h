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
 * \file     os-impl-loader.h
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_LOADER_H_
#define INCLUDE_OS_IMPL_LOADER_H_

#include <osconfig.h>
#include <dlfcn.h>

#include <rtems/rtl/rtl.h>
#include <rtems/rtl/rtl-unresolved.h>


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/*
 * A local lookup table for RTEMS-specific information.
 * This is not directly visible to the outside world.
 */
typedef struct
{
    /* cppcheck-suppress unusedStructMember */
    void *dl_handle;
} OS_impl_module_internal_record_t;

extern OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];



#endif  /* INCLUDE_OS_IMPL_LOADER_H_ */

