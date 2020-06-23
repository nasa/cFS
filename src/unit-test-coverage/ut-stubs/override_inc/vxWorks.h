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
 * \file     vxWorks.h
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_VXWORKS_H_
#define INCLUDE_VXWORKS_H_

#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in vxWorks.h */
/* ----------------------------------------- */
#define ERROR                OCS_ERROR
#define OK                   OCS_OK
#define WAIT_FOREVER         OCS_WAIT_FOREVER
#define NO_WAIT              OCS_NO_WAIT


#define STATUS          OCS_STATUS
#define BOOL            OCS_BOOL
#define FUNCPTR         OCS_FUNCPTR
#define VOIDFUNCPTR     OCS_VOIDFUNCPTR

#define UINT      OCS_UINT
#define INT8      OCS_INT8
#define UINT8     OCS_UINT8
#define INT16     OCS_INT16
#define UINT16    OCS_UINT16
#define INT32     OCS_INT32
#define UINT32    OCS_UINT32

#define _Vx_usr_arg_t   OCS_Vx_usr_arg_t


#endif  /* INCLUDE_VXWORKS_H_ */

