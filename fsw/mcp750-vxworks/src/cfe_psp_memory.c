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

/******************************************************************************
** File:  cfe_psp_memory.c
**
**      MCP750 vxWorks 6.x Version
**
** Purpose:
**   cFE PSP Memory related functions. This is the implementation of the cFE 
**   memory areas that have to be preserved, and the API that is designed to allow
**   acccess to them. It also contains memory related routines to return the
**   address of the kernel code used in the cFE checksum.
**
** History:
**   2006/09/29  A. Cudmore      | vxWorks 6.2 MCP750 version
**   2016/04/07  M.Grubb         | Updated for PSP version 1.3
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <string.h>
#include <vxWorks.h>
#include <sysLib.h>
#include <moduleLib.h>

/*
** cFE includes 
*/
#include "common_types.h"
#include "osapi.h"

/*
** Types and prototypes for this module
*/
#include "cfe_psp.h" 
#include "cfe_psp_memory.h"           

#include <target_config.h>

/*
** Define the cFE Core loadable module name
*/
#define CFE_MODULE_NAME_DEFAULT "cfe-core.o"

static char CFE_MODULE_NAME[] = CFE_MODULE_NAME_DEFAULT;


/*
**  External Declarations
*/
extern unsigned int GetWrsKernelTextStart(void);
extern unsigned int GetWrsKernelTextEnd (void);         

/*
** Global variables
*/

/*
** Pointer to the vxWorks USER_RESERVED_MEMORY area
** The sizes of each memory area is defined in os_processor.h for this architecture.
*/
CFE_PSP_ReservedMemoryMap_t CFE_PSP_ReservedMemoryMap;


CFE_PSP_MemoryBlock_t MCP750_ReservedMemBlock;


/*
*********************************************************************************
** CDS related functions
*********************************************************************************
*/

/******************************************************************************
**  Function: CFE_PSP_GetCDSSize
**
**  Purpose: 
**    This function fetches the size of the OS Critical Data Store area.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

int32 CFE_PSP_GetCDSSize(uint32 *SizeOfCDS)
{
   int32 return_code;
   
   if ( SizeOfCDS == NULL )
   {
       return_code = CFE_PSP_ERROR;
   }
   else
   {
       *SizeOfCDS =  CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize;
       return_code = CFE_PSP_SUCCESS;
   }
   return(return_code);
}

/******************************************************************************
**  Function: CFE_PSP_WriteToCDS
**
**  Purpose:
**    This function writes to the CDS Block.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_WriteToCDS(const void *PtrToDataToWrite, uint32 CDSOffset, uint32 NumBytes)
{
   uint8 *CopyPtr;
   int32  return_code;
         
   if ( PtrToDataToWrite == NULL )
   {
       return_code = CFE_PSP_ERROR;
   }
   else
   {
       if ( (CDSOffset < CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize ) &&
               ( (CDSOffset + NumBytes) <= CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize ))
       {
           CopyPtr = CFE_PSP_ReservedMemoryMap.CDSMemory.BlockPtr;
           CopyPtr += CDSOffset;
           memcpy(CopyPtr, (char *)PtrToDataToWrite,NumBytes);
          
           return_code = CFE_PSP_SUCCESS;
       }
       else
       {
          return_code = CFE_PSP_ERROR;
       }
       
   } /* end if PtrToDataToWrite == NULL */
   
   return(return_code);
}


/******************************************************************************
**  Function: CFE_PSP_ReadFromCDS
**
**  Purpose:
**   This function reads from the CDS Block
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

int32 CFE_PSP_ReadFromCDS(void *PtrToDataToRead, uint32 CDSOffset, uint32 NumBytes)
{
   uint8 *CopyPtr;
   int32  return_code;
      
   if ( PtrToDataToRead == NULL )
   {
       return_code = CFE_PSP_ERROR;
   }
   else
   {
       if ( (CDSOffset < CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize ) &&
               ( (CDSOffset + NumBytes) <= CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize ))
       {
           CopyPtr = CFE_PSP_ReservedMemoryMap.CDSMemory.BlockPtr;
           CopyPtr += CDSOffset;
           memcpy((char *)PtrToDataToRead,CopyPtr, NumBytes);
          
           return_code = CFE_PSP_SUCCESS;
       }
       else
       {
          return_code = CFE_PSP_ERROR;
       }
       
   } /* end if PtrToDataToWrite == NULL */
   
   return(return_code);
   
}

/*
*********************************************************************************
** ES Reset Area related functions
*********************************************************************************
*/

/******************************************************************************
**  Function: CFE_PSP_GetResetArea
**
**  Purpose:
**     This function returns the location and size of the ES Reset information area. 
**     This area is preserved during a processor reset and is used to store the 
**     ER Log, System Log and reset related variables
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_GetResetArea (cpuaddr *PtrToResetArea, uint32 *SizeOfResetArea)
{
   int32   return_code;
   
   if ((PtrToResetArea == NULL) || (SizeOfResetArea == NULL))
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      *PtrToResetArea = (cpuaddr)(CFE_PSP_ReservedMemoryMap.ResetMemory.BlockPtr);
      *SizeOfResetArea = CFE_PSP_ReservedMemoryMap.ResetMemory.BlockSize;
      return_code = CFE_PSP_SUCCESS;
   }
   
   return(return_code);
}

/*
*********************************************************************************
** ES User Reserved Area related functions
*********************************************************************************
*/

/******************************************************************************
**  Function: CFE_PSP_GetUserReservedArea
**
**  Purpose:
**    This function returns the location and size of the memory used for the cFE
**     User reserved area.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_GetUserReservedArea(cpuaddr *PtrToUserArea, uint32 *SizeOfUserArea )
{
   int32   return_code;
   
   if ((PtrToUserArea == NULL) || (SizeOfUserArea == NULL))
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      *PtrToUserArea = (cpuaddr)(CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockPtr);
      *SizeOfUserArea = CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockSize;
      return_code = CFE_PSP_SUCCESS;
   }
   
   return(return_code);
}

/*
*********************************************************************************
** ES Volatile disk memory related functions
*********************************************************************************
*/

/******************************************************************************
**  Function: CFE_PSP_GetVolatileDiskMem
**
**  Purpose:
**    This function returns the location and size of the memory used for the cFE
**     volatile disk.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_GetVolatileDiskMem(cpuaddr *PtrToVolDisk, uint32 *SizeOfVolDisk )
{
   int32   return_code;
   
   if ((PtrToVolDisk == NULL) || (SizeOfVolDisk == NULL))
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      *PtrToVolDisk = (cpuaddr)(CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockPtr);
      *SizeOfVolDisk = CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockSize;
      return_code = CFE_PSP_SUCCESS;

   }
   
   return(return_code);
   
}

/*
*********************************************************************************
** ES BSP Top Level Reserved memory initialization
*********************************************************************************
*/

/******************************************************************************
**  Function: CFE_PSP_InitProcessorReservedMemory
**
**  Purpose:
**    This function performs the top level reserved memory initialization.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_InitProcessorReservedMemory( uint32 RestartType )
{
   int32 return_code;
 
   if ( RestartType != CFE_PSP_RST_TYPE_PROCESSOR )
   {
      OS_printf("CFE_PSP: Clearing Processor Reserved Memory.\n");
      memset(MCP750_ReservedMemBlock.BlockPtr, 0, MCP750_ReservedMemBlock.BlockSize);
      
      /*
      ** Set the default reset type in case a watchdog reset occurs 
      */
      CFE_PSP_ReservedMemoryMap.BootPtr->bsp_reset_type = CFE_PSP_RST_TYPE_PROCESSOR;

   }      
   return_code = CFE_PSP_SUCCESS;
   return(return_code);

}

/******************************************************************************
**  Function: CFE_PSP_SetupReservedMemoryMap
**
**  Purpose:
**    Set up the CFE_PSP_ReservedMemoryMap global data structure
**    This only sets the pointers, it does not initialize the data.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
void CFE_PSP_SetupReservedMemoryMap(void)
{
    cpuaddr start_addr;
    cpuaddr end_addr;

    /*
    ** Note: this uses a "cpuaddr" (integer address) as an intermediate
    ** to avoid warnings about alignment.  The output of sysMemTop()
    ** should be aligned to hold any data type, being the very start
    ** of the memory space.
    */
    start_addr = (cpuaddr) sysMemTop();
    end_addr = start_addr;

    memset(&CFE_PSP_ReservedMemoryMap, 0, sizeof(CFE_PSP_ReservedMemoryMap));

    CFE_PSP_ReservedMemoryMap.BootPtr = (CFE_PSP_ReservedMemoryBootRecord_t *)end_addr;
    end_addr += sizeof(CFE_PSP_ReservedMemoryBootRecord_t);
    end_addr = (end_addr + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;

    CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr = (CFE_PSP_ExceptionStorage_t *)end_addr;
    end_addr += sizeof(CFE_PSP_ExceptionStorage_t);
    end_addr = (end_addr + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;

    CFE_PSP_ReservedMemoryMap.ResetMemory.BlockPtr = (void*)end_addr;
    CFE_PSP_ReservedMemoryMap.ResetMemory.BlockSize = GLOBAL_CONFIGDATA.CfeConfig->ResetAreaSize;
    end_addr += CFE_PSP_ReservedMemoryMap.ResetMemory.BlockSize;
    end_addr = (end_addr + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;

    CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockPtr = (void*)end_addr;
    CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockSize =
        GLOBAL_CONFIGDATA.CfeConfig->RamDiskSectorSize * GLOBAL_CONFIGDATA.CfeConfig->RamDiskTotalSectors;
    end_addr += CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockSize;
    end_addr = (end_addr + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;

    CFE_PSP_ReservedMemoryMap.CDSMemory.BlockPtr = (void*)end_addr;
    CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize = GLOBAL_CONFIGDATA.CfeConfig->CdsSize;
    end_addr += CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize;
    end_addr = (end_addr + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;

    CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockPtr = (void*)end_addr;
    CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockSize = GLOBAL_CONFIGDATA.CfeConfig->UserReservedSize;
    end_addr += CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockSize;
    end_addr = (end_addr + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;

    /* The total size of the entire block is the difference in address */
    MCP750_ReservedMemBlock.BlockPtr = (void*)start_addr;
    MCP750_ReservedMemBlock.BlockSize =  end_addr - start_addr;


    OS_printf("CFE_PSP: MCP750 Reserved Memory Block at 0x%08lx, Total Size = 0x%lx\n",
            (unsigned long)MCP750_ReservedMemBlock.BlockPtr,
            (unsigned long)MCP750_ReservedMemBlock.BlockSize);
}

/******************************************************************************
 * Function: CFE_PSP_DeleteProcessorReservedMemory
 *
 * No action on MCP750 - reserved block is statically allocated at sysMemTop.
 * Implemented for API consistency with other PSPs.
 */
void CFE_PSP_DeleteProcessorReservedMemory(void)
{
}

/*
*********************************************************************************
** ES BSP kernel memory segment functions
*********************************************************************************
*/

/******************************************************************************
**  Function: CFE_PSP_GetKernelTextSegmentInfo
**
**  Purpose:
**    This function returns the start and end address of the kernel text segment.
**     It may not be implemented on all architectures.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_GetKernelTextSegmentInfo(cpuaddr *PtrToKernelSegment, uint32 *SizeOfKernelSegment)
{
   int32 return_code;
   cpuaddr StartAddress;
   cpuaddr EndAddress;
   
   if ( SizeOfKernelSegment == NULL )
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      /*
      ** Get the kernel start and end
      ** addresses from the BSP, because the 
      ** symbol table does not contain the symbls we need for this
      */
      StartAddress = (cpuaddr) GetWrsKernelTextStart();
      EndAddress = (cpuaddr) GetWrsKernelTextEnd();

      *PtrToKernelSegment = StartAddress;
      *SizeOfKernelSegment = (uint32) (EndAddress - StartAddress);
      
      return_code = CFE_PSP_SUCCESS;
   }
   
   return(return_code);
}

/******************************************************************************
**  Function: CFE_PSP_GetCFETextSegmentInfo
**
**  Purpose:
**    This function returns the start and end address of the CFE text segment.
**     It may not be implemented on all architectures.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_GetCFETextSegmentInfo(cpuaddr *PtrToCFESegment, uint32 *SizeOfCFESegment)
{
   int32       return_code;
   STATUS      status;
   MODULE_ID   cFEModuleId;
   MODULE_INFO cFEModuleInfo;
   
   if ( SizeOfCFESegment == NULL )
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      cFEModuleId = moduleFindByName(CFE_MODULE_NAME);
      
      if ( cFEModuleId == NULL )
      {
         return_code = CFE_PSP_ERROR;
      }
      else
      {
         status = moduleInfoGet(cFEModuleId, &cFEModuleInfo);
         if ( status != ERROR )
         {
            *PtrToCFESegment = (cpuaddr) (cFEModuleInfo.segInfo.textAddr);
            *SizeOfCFESegment = (uint32) (cFEModuleInfo.segInfo.textSize);
            return_code = CFE_PSP_SUCCESS;
         }
         else
         {
            return_code = CFE_PSP_SUCCESS;
         }
      }
   }
   
   return(return_code);
}

