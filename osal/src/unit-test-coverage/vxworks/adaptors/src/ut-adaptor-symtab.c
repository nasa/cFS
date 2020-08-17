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
 * \file     ut-adaptor-symtab.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-loader.h"

#include <os-vxworks.h>
#include <os-impl-symtab.h>

/*
 * A UT-specific wrapper function to invoke the Symbol Table Iterator.
 * This is normally static so it needs this wrapper to call it.
 */
int32 UT_SymTabTest_CallIteratorFunc(const char *name, void* val, uint32 TestSize, uint32 SizeLimit)
{
    OS_VxWorks_SymbolDumpState.Sizelimit = SizeLimit;
    OS_VxWorks_SymbolDumpState.CurrSize = TestSize;
    /*
     * note the internal function is takes a name declared as "char*" to be
     * consistent with the VxWorks API, however the implementation does not
     * modify this argument.
     */
    return OS_SymTableIterator_Impl((char*)name,(OCS_SYM_VALUE)val,0,0,0);
}

