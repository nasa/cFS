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
 * \file     os-impl-symtab.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_SYMTAB_H_
#define INCLUDE_OS_IMPL_SYMTAB_H_

#include <osconfig.h>
#include <symLib.h>

typedef struct
{
    uint32  Sizelimit;
    uint32  CurrSize;
    int32   StatusCode;
    int     fd;
} SymbolDumpState_t;


/* A global for storing the state in a SymbolDump call */
extern SymbolDumpState_t OS_VxWorks_SymbolDumpState;

BOOL  OS_SymTableIterator_Impl ( char *name, SYM_VALUE val,  SYM_TYPE type,  _Vx_usr_arg_t arg, SYM_GROUP group );

#endif  /* INCLUDE_OS_IMPL_SYMTAB_H_ */

