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
 * \file     symLib.h
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_SYMLIB_H_
#define INCLUDE_SYMLIB_H_

#include <OCS_symLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in symLib.h */
/* ----------------------------------------- */
#define LOAD_ALL_SYMBOLS     OCS_LOAD_ALL_SYMBOLS
#define SYM_FIND_BY_NAME     OCS_SYM_FIND_BY_NAME

#define SYM_TYPE             OCS_SYM_TYPE
#define SYMTAB               OCS_SYMTAB
#define SYMBOL               OCS_SYMBOL
#define SYMTAB_ID            OCS_SYMTAB_ID
#define SYMBOL_ID            OCS_SYMBOL_ID
#define SYMBOL_DESC          OCS_SYMBOL_DESC
#define SYM_GROUP            OCS_SYM_GROUP
#define SYM_VALUE            OCS_SYM_VALUE

#define sysSymTbl            OCS_sysSymTbl
#define symFindByName        OCS_symFindByName
#define symEach              OCS_symEach
#define symFind              OCS_symFind


#endif  /* INCLUDE_SYMLIB_H_ */

