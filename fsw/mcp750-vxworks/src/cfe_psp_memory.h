/*
**  GSC-18128-1, "Core Flight Executive Version 6.6"
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

/*
 * the CFE platform config is required
 * to size the ReservedMemory structure, below
 */
#include <cfe_platform_cfg.h>

#include <target_config.h>

/*
** Macro Definitions
*/
#define CFE_PSP_CDS_SIZE            (GLOBAL_CONFIGDATA.CfeConfig->CdsSize)
#define CFE_PSP_RESET_AREA_SIZE     (GLOBAL_CONFIGDATA.CfeConfig->ResetAreaSize)
#define CFE_PSP_USER_RESERVED_SIZE  (GLOBAL_CONFIGDATA.CfeConfig->UserReservedSize)

#define CFE_PSP_VOLATILE_DISK_SIZE  (CFE_PLATFORM_ES_RAM_DISK_SECTOR_SIZE * CFE_PLATFORM_ES_RAM_DISK_NUM_SECTORS)


/*
** Typedef for the layout of the vxWorks USER_RESERVED_MEM
** Note that the structure below reserves memory sizes defined
** at compile time directly from cfe_platform_cfg.h above.
** A future enhancement should reserve blocks based on the runtime 
** size in GLOBAL_CONFIGDATA.
*/
typedef struct
{
   uint32 bsp_reset_type;
   uint32 spare1;
   uint32 spare2;
   uint32 spare3;
   uint8  ResetMemory[CFE_PLATFORM_ES_RESET_AREA_SIZE];
   uint8  VolatileDiskMemory[CFE_PSP_VOLATILE_DISK_SIZE];
   uint8  CDSMemory[CFE_PLATFORM_ES_CDS_SIZE];
   uint8  UserReservedMemory[CFE_PLATFORM_ES_USER_RESERVED_SIZE];

} CFE_PSP_ReservedMemory_t;

/*
** External variables
*/

/*
** Pointer to the vxWorks USER_RESERVED_MEMORY area
** The sizes of each memory area is defined in os_processor.h for this architecture.
*/
extern CFE_PSP_ReservedMemory_t *CFE_PSP_ReservedMemoryPtr; 

#endif  /* _cfe_psp_memory_ */
