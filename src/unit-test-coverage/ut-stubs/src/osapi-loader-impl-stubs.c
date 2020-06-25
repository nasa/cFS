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
 * \file     osapi-loader-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"
#include "os-shared-module.h"


/*
 * Module Loader API
 */
UT_DEFAULT_STUB(OS_ModuleLoad_Impl,( uint32 module_id, const char *translated_path ))
UT_DEFAULT_STUB(OS_ModuleUnload_Impl,( uint32 module_id ))
UT_DEFAULT_STUB(OS_ModuleGetInfo_Impl,( uint32 module_id, OS_module_prop_t *module_prop ))
UT_DEFAULT_STUB(OS_SymbolLookup_Impl,( cpuaddr *SymbolAddress, const char *SymbolName ))
UT_DEFAULT_STUB(OS_SymbolTableDump_Impl,( const char *filename, uint32 size_limit ))

