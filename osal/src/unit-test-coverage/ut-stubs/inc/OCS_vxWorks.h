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
 * \file     OCS_vxWorks.h
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 * OSAL coverage stub replacement for vxWorks.h 
 */

#ifndef INCLUDE_OCS_VXWORKS_H_
#define INCLUDE_OCS_VXWORKS_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in vxWorks.h */
/* ----------------------------------------- */
enum
{
    OCS_ERROR = -1,
    OCS_OK    =  0
};

enum
{
    OCS_WAIT_FOREVER = -1,
    OCS_NO_WAIT      = 0
};



/* ----------------------------------------- */
/* types normally defined in vxWorks.h */
/* ----------------------------------------- */
typedef int           OCS_STATUS;
typedef bool          OCS_BOOL;
typedef unsigned int  OCS_UINT;
typedef int8_t        OCS_INT8;
typedef uint8_t       OCS_UINT8;
typedef int16_t       OCS_INT16;
typedef uint16_t      OCS_UINT16;
typedef int32_t       OCS_INT32;
typedef uint32_t      OCS_UINT32;

typedef long          OCS_Vx_usr_arg_t;

/* Function pointers are used in many VxWorks modules. */
/*
 * NOTE: The FUNCPTR type in the actual library may be defined
 * without arguments, e.g. "int (*FUNCPTR)()".  This is acceptable
 * by some compilers but generally incompatible with the
 * "-Wstrict-prototype" gcc warning option.  So in this override it
 * is defined as a int argument.  This means that application code
 * may need to cast it at the time of use (which is generally done anyway).
 */
typedef int    (*OCS_FUNCPTR)(int);
typedef void   (*OCS_VOIDFUNCPTR)(void);


/* ----------------------------------------- */
/* prototypes normally declared in vxWorks.h */
/* ----------------------------------------- */


#endif  /* INCLUDE_OCS_VXWORKS_H_ */

