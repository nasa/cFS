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

/* OSAL coverage stub replacement for unldLib.h */
#ifndef _OSAL_STUB_UNLDLIB_H_
#define _OSAL_STUB_UNLDLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>
#include <OCS_moduleLib.h>

/* ----------------------------------------- */
/* constants normally defined in unldLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in unldLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in unldLib.h */
/* ----------------------------------------- */
extern OCS_STATUS OCS_unldByModuleId(OCS_MODULE_ID moduleId, int options);



#endif /* _OSAL_STUB_UNLDLIB_H_ */

