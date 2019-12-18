/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
**  File Name:  cfe_psp_memory.h
**
**  Author:  A. Cudmore
**
**  Purpose:  This file contains PSP support routine internal prototypes 
**            and typedefs. The routines and typedefs are intended for 
**            the local PSP routines only. 
**
**  Modification History:
**
*/

#ifndef _cfe_psp_memory_
#define _cfe_psp_memory_

/*
** Include Files
*/

#include "common_types.h"
#include "cfe_psp.h"


/*
** Typedef for the layout of the vxWorks USER_RESERVED_MEM
*/

typedef struct
{
   uint32 bsp_reset_type;
   uint32 spare1;
   uint32 spare2;
   uint32 spare3;
} CFE_PSP_ReservedFixedInfo_t;


typedef struct
{
   uint32 TotalSize;
   CFE_PSP_ReservedFixedInfo_t *FixedInfo;
   uint32 ResetSize;
   uint8  *ResetMemory;
   uint32 VolatileDiskSize;
   uint8  *VolatileDiskMemory; 
   uint32 CDSSize;
   uint8  *CDSMemory;
   uint32 UserReservedSize;
   uint8  *UserReservedMemory;
} CFE_PSP_ReservedMemoryMap_t;

/*
** External variables
*/

/*
** Pointer to the vxWorks USER_RESERVED_MEMORY area
** The sizes of each memory area is defined in os_processor.h for this architecture.
*/
extern CFE_PSP_ReservedMemoryMap_t CFE_PSP_ReservedMemoryMap; 

#endif  /* _cfe_psp_memory_ */
