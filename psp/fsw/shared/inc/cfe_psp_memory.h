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
#include "cfe_psp_config.h"
#include "cfe_psp_exceptionstorage_types.h"

typedef struct
{
    void*  BlockPtr;
    size_t BlockSize;

} CFE_PSP_MemoryBlock_t;


typedef struct
{
    CFE_PSP_ReservedMemoryBootRecord_t *BootPtr;
    CFE_PSP_ExceptionStorage_t *ExceptionStoragePtr;

    CFE_PSP_MemoryBlock_t ResetMemory;
    CFE_PSP_MemoryBlock_t VolatileDiskMemory;
    CFE_PSP_MemoryBlock_t CDSMemory;
    CFE_PSP_MemoryBlock_t UserReservedMemory;
} CFE_PSP_ReservedMemoryMap_t;



/**
 * \brief Initialize the CFE_PSP_ReservedMemoryMap global object
 *
 * This must be called by the startup code before the map is accessed.
 */
extern void CFE_PSP_SetupReservedMemoryMap(void);


/*
 * CFE_PSP_InitProcessorReservedMemory initializes all of the memory in the
 * BSP that is preserved on a processor reset. The memory includes the
 * Critical Data Store, the ES Reset Area, the Volatile Disk Memory, and
 * the User Reserved Memory.
 *
 * This initializes based on the reset type.  Typically, the information
 * is preserved on a processor reset, and cleared/reinitialized on a power-on
 * reset.
 */
extern int32 CFE_PSP_InitProcessorReservedMemory(uint32 RestartType );


/**
 * \brief Unlink the memory segments within the CFE_PSP_ReservedMemoryMap global object
 *
 * This function is only relevant on systems where the objects are implemented
 * as kernel shared memory segments.  The segments will be marked for deletion
 * but the local maps remain usable until the process ends.
 */
extern void CFE_PSP_DeleteProcessorReservedMemory(void);


/*
** External variables
*/

/*
** Map to the reserved memory area(s)
** Contains a pointer to each of the separate memory blocks
*/
extern CFE_PSP_ReservedMemoryMap_t CFE_PSP_ReservedMemoryMap;

#endif  /* _cfe_psp_memory_ */
