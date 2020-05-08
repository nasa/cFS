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
**      mcf5235 Coldfire RTEMS version
**
** Purpose:
**   cFE PSP Memory related functions. This is the implementation of the cFE 
**   memory areas that have to be preserved, and the API that is designed to allow
**   acccess to them. It also contains memory related routines to return the
**   address of the kernel code used in the cFE checksum.
**
** History:
**   2007/09/23  A. Cudmore      | m5235bcc Coldfire RTEMS version
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <string.h>

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
#include "cfe_psp_config.h"

/*
** Macro Definitions
*/
#include <target_config.h>

/*
 * Define the PSP-supported capacities to be the maximum allowed,
 * (since the PC-linux PSP has the advantage of abundant disk space to hold this)
 */
#define CFE_PSP_CDS_SIZE               (GLOBAL_CONFIGDATA.CfeConfig->CdsSize)
#define CFE_PSP_RESET_AREA_SIZE        (GLOBAL_CONFIGDATA.CfeConfig->ResetAreaSize)
#define CFE_PSP_USER_RESERVED_SIZE     (GLOBAL_CONFIGDATA.CfeConfig->UserReservedSize)
#define CFE_PSP_RAM_DISK_SECTOR_SIZE   (GLOBAL_CONFIGDATA.CfeConfig->RamDiskSectorSize)
#define CFE_PSP_RAM_DISK_NUM_SECTORS   (GLOBAL_CONFIGDATA.CfeConfig->RamDiskTotalSectors)


typedef struct
{
    CFE_PSP_ReservedMemoryBootRecord_t BootRecord;
    CFE_PSP_ExceptionStorage_t ExceptionStorage;
} CFE_PSP_RtemsReservedAreaFixedLayout_t;



/*
**  External Declarations
*/

/*
** Global variables
*/

/*
** Dynamic map of the reserved memory area
*/
CFE_PSP_ReservedMemoryMap_t CFE_PSP_ReservedMemoryMap = { 0 }; 


CFE_PSP_MemoryBlock_t PcRtems_ReservedMemBlock;


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
       return_code = OS_ERROR;
   }
   else
   {
       *SizeOfCDS = CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize;
       return_code = OS_SUCCESS;
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
       return_code = OS_ERROR;
   }
   else
   {
       if ( (CDSOffset < CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize ) &&
               ( (CDSOffset + NumBytes) <= CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize ))
       {
           CopyPtr = CFE_PSP_ReservedMemoryMap.CDSMemory.BlockPtr;
           CopyPtr += CDSOffset;
           memcpy((char *)CopyPtr, (char *)PtrToDataToWrite,NumBytes);
          
           return_code = OS_SUCCESS;
       }
       else
       {
          return_code = OS_ERROR;
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
       return_code = OS_ERROR;
   }
   else
   {
       if ( (CDSOffset < CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize ) &&
               ( (CDSOffset + NumBytes) <= CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize ))
       {
           CopyPtr = CFE_PSP_ReservedMemoryMap.CDSMemory.BlockPtr;
           CopyPtr += CDSOffset;
           memcpy((char *)PtrToDataToRead, (char *)CopyPtr, NumBytes);
          
           return_code = OS_SUCCESS;
       }
       else
       {
          return_code = OS_ERROR;
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
   
   if ( SizeOfResetArea == NULL || PtrToResetArea == NULL )
   {
      return_code = OS_ERROR;
   }
   else
   {
      *PtrToResetArea = (cpuaddr)(CFE_PSP_ReservedMemoryMap.ResetMemory.BlockPtr);
      *SizeOfResetArea = CFE_PSP_ReservedMemoryMap.ResetMemory.BlockSize;
      return_code = OS_SUCCESS;
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
   
   if ( SizeOfUserArea == NULL || PtrToUserArea == NULL )
   {
      return_code = OS_ERROR;
   }
   else
   {
      *PtrToUserArea = (cpuaddr)(CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockPtr);
      *SizeOfUserArea = CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockSize;
      return_code = OS_SUCCESS;
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
   
   if ( SizeOfVolDisk == NULL || PtrToVolDisk == NULL )
   {
      return_code = OS_ERROR;
   }
   else
   {
      *PtrToVolDisk = (cpuaddr)(CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockPtr);
      *SizeOfVolDisk = CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockSize;
      return_code = OS_SUCCESS;

   }
   
   return(return_code);
   
}

/*
*********************************************************************************
** ES BSP Top Level Reserved memory initialization
*********************************************************************************
*/

/******************************************************************************
**  Function: CFE_PSP_SetupReservedMemoryMap
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
void CFE_PSP_SetupReservedMemoryMap(void)
{
   CFE_PSP_RtemsReservedAreaFixedLayout_t *FixedPtr;
   cpuaddr ReservedMemoryAddr;
   size_t FixedSize;
   size_t ResetSize;
   size_t CDSSize;
   size_t UserReservedSize;
   size_t VolatileDiskSize;
   size_t RequiredSize;

   /*
   ** Allocate memory for the cFE memory. Note that this is malloced on
   ** the COTS board, but will be a static location in the ETU. 
   */
   FixedSize = sizeof(CFE_PSP_RtemsReservedAreaFixedLayout_t);
   ResetSize = CFE_PSP_RESET_AREA_SIZE;
   VolatileDiskSize = (CFE_PSP_RAM_DISK_SECTOR_SIZE * CFE_PSP_RAM_DISK_NUM_SECTORS);
   CDSSize = CFE_PSP_CDS_SIZE;
   UserReservedSize = CFE_PSP_USER_RESERVED_SIZE;

   FixedSize = (FixedSize + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;
   ResetSize = (ResetSize + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;
   CDSSize = (CDSSize + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;
   VolatileDiskSize = (VolatileDiskSize + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;
   UserReservedSize = (UserReservedSize + CFE_PSP_MEMALIGN_MASK) & ~CFE_PSP_MEMALIGN_MASK;

   /*  Calculate the required size, adding padding so that each element is aligned */
   RequiredSize = FixedSize;
   RequiredSize += ResetSize;
   RequiredSize += VolatileDiskSize;
   RequiredSize += CDSSize;
   RequiredSize += UserReservedSize;

   OS_printf("Sizeof BSP reserved memory = %u bytes\n",(unsigned int)RequiredSize);

   PcRtems_ReservedMemBlock.BlockPtr = malloc(RequiredSize);

   if ( PcRtems_ReservedMemBlock.BlockPtr == NULL )
   {
      OS_printf("CFE_PSP: Error: Cannot malloc BSP reserved memory!\n");
      abort();
   }

   PcRtems_ReservedMemBlock.BlockSize = RequiredSize;
   ReservedMemoryAddr = (cpuaddr)PcRtems_ReservedMemBlock.BlockPtr;

   OS_printf("CFE_PSP: Allocated %u bytes for PSP reserved memory at: 0x%08lX\n",
           (unsigned int)RequiredSize, (unsigned long)ReservedMemoryAddr);

   FixedPtr = (CFE_PSP_RtemsReservedAreaFixedLayout_t*)ReservedMemoryAddr;

   CFE_PSP_ReservedMemoryMap.BootPtr = &FixedPtr->BootRecord;
   CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr = &FixedPtr->ExceptionStorage;
   ReservedMemoryAddr += FixedSize;

   CFE_PSP_ReservedMemoryMap.ResetMemory.BlockPtr = (void*)ReservedMemoryAddr;
   CFE_PSP_ReservedMemoryMap.ResetMemory.BlockSize = CFE_PSP_RESET_AREA_SIZE;
   ReservedMemoryAddr += ResetSize;

   CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockPtr = (void*)ReservedMemoryAddr;
   CFE_PSP_ReservedMemoryMap.VolatileDiskMemory.BlockSize = (CFE_PSP_RAM_DISK_SECTOR_SIZE * CFE_PSP_RAM_DISK_NUM_SECTORS);
   ReservedMemoryAddr += VolatileDiskSize;

   CFE_PSP_ReservedMemoryMap.CDSMemory.BlockPtr = (void*)ReservedMemoryAddr;
   CFE_PSP_ReservedMemoryMap.CDSMemory.BlockSize = CFE_PSP_CDS_SIZE;
   ReservedMemoryAddr += CDSSize;

   CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockPtr = (void*)ReservedMemoryAddr;
   CFE_PSP_ReservedMemoryMap.UserReservedMemory.BlockSize = CFE_PSP_USER_RESERVED_SIZE;
   ReservedMemoryAddr += UserReservedSize;

   /*
    * displaying the final address shows how much was actually used,
    * and additionally avoids a warning about the result of the final increment not being used.
    * (prefer this over removing the increment, as it is safer if another block is added)
    */
   OS_printf("CFE_PSP: PSP reserved memory ends at: 0x%08lX\n",
           (unsigned long)ReservedMemoryAddr);
}

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
    OS_printf("CFE_PSP: Clearing Processor Reserved Memory.\n");
    memset(PcRtems_ReservedMemBlock.BlockPtr, 0, PcRtems_ReservedMemBlock.BlockSize);
    return CFE_PSP_SUCCESS;
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
   
   if ( SizeOfKernelSegment == NULL || PtrToKernelSegment == NULL )
   {
      return_code = OS_ERROR;
   }
   else
   {
      /*
      ** Get the kernel start and end
      ** addresses from the BSP, because the 
      ** symbol table does not contain the symbls we need for this
      */
      StartAddress = (cpuaddr) 0x0;
      EndAddress = (cpuaddr) 0x1024;

      *PtrToKernelSegment = StartAddress;
      *SizeOfKernelSegment = (uint32) (EndAddress - StartAddress);
      
      return_code = OS_SUCCESS;
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
   int32 return_code;
   cpuaddr StartAddress;
   cpuaddr EndAddress;
   
   if ( SizeOfCFESegment == NULL )
   {
      return_code = OS_ERROR;
   }
   else
   {
      /*
      ** Get the kernel start and end
      ** addresses from the BSP, because the 
      ** symbol table does not contain the symbls we need for this
      */
      StartAddress = (cpuaddr) 0x0;
      EndAddress = (cpuaddr) 0x1024;

      *PtrToCFESegment = StartAddress;
      *SizeOfCFESegment = (uint32) (EndAddress - StartAddress);
      
      return_code = OS_SUCCESS;
   }
   
   return(return_code);
}


//extern int rtems_shell_main_wkspace_info(int argc,char *argv[]);
//extern int rtems_shell_main_malloc_info( int argc, char *argv[]);


void CFE_PSP_MemReport(char *message)
{

   //int MemStatus;
   //char *malloc_argv[] = {"malloc", "info"};
   //char *wkspace_argv[] = {"wkspace", "info"};
   
   OS_printf("------------------------ Memory Stat Report ----------------------------\n");
   OS_printf(" Called from: %s\n",message);
   OS_printf("------------------------ Dumping Malloc Stats: ----------------------------\n");
   //MemStatus = rtems_shell_main_malloc_info(2, malloc_argv);
   //OS_printf("rtems_shell_main_malloc_info() rc=%d\n",(int)MemStatus);
   OS_printf("------------------------ Dumping Wkspace Stats: ----------------------------\n");
   //MemStatus = rtems_shell_main_wkspace_info(2, wkspace_argv);
   //OS_printf("rtems_shell_main_wkspace_info() rc=%d\n",(int)MemStatus);
   OS_printf("------------------------ Done ----------------- ----------------------------\n");
   
}

