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
   
   if ( SizeOfCDS == NULL || CFE_PSP_ReservedMemoryMap.TotalSize == 0 )
   {
       return_code = OS_ERROR;
   }
   else
   {
       *SizeOfCDS = CFE_PSP_ReservedMemoryMap.CDSSize;
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
         
   if ( PtrToDataToWrite == NULL || CFE_PSP_ReservedMemoryMap.TotalSize == 0 )
   {
       return_code = OS_ERROR;
   }
   else
   {
       if ( (CDSOffset < CFE_PSP_ReservedMemoryMap.CDSSize ) && ( (CDSOffset + NumBytes) <= CFE_PSP_ReservedMemoryMap.CDSSize ))
       {
          CopyPtr = &(CFE_PSP_ReservedMemoryMap.CDSMemory[CDSOffset]);
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
      
   if ( PtrToDataToRead == NULL || CFE_PSP_ReservedMemoryMap.TotalSize == 0 )
   {
       return_code = OS_ERROR;
   }
   else
   {
       if ( (CDSOffset < CFE_PSP_ReservedMemoryMap.CDSSize ) && ( (CDSOffset + NumBytes) <= CFE_PSP_ReservedMemoryMap.CDSSize ))
       {
          CopyPtr = &(CFE_PSP_ReservedMemoryMap.CDSMemory[CDSOffset]);
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
   
   if ( SizeOfResetArea == NULL || PtrToResetArea == NULL || CFE_PSP_ReservedMemoryMap.TotalSize == 0 )
   {
      return_code = OS_ERROR;
   }
   else
   {
      *PtrToResetArea = (cpuaddr)(CFE_PSP_ReservedMemoryMap.ResetMemory);
      *SizeOfResetArea = CFE_PSP_ReservedMemoryMap.ResetSize;
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
   
   if ( SizeOfUserArea == NULL || PtrToUserArea == NULL || CFE_PSP_ReservedMemoryMap.TotalSize == 0 )
   {
      return_code = OS_ERROR;
   }
   else
   {
     *PtrToUserArea = (cpuaddr)(CFE_PSP_ReservedMemoryMap.UserReservedMemory);
      *SizeOfUserArea = CFE_PSP_ReservedMemoryMap.UserReservedSize;
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
   
   if ( SizeOfVolDisk == NULL || PtrToVolDisk == NULL || CFE_PSP_ReservedMemoryMap.TotalSize == 0 )
   {
      return_code = OS_ERROR;
   }
   else
   {
      *PtrToVolDisk = (cpuaddr)(CFE_PSP_ReservedMemoryMap.VolatileDiskMemory);
      *SizeOfVolDisk = CFE_PSP_ReservedMemoryMap.VolatileDiskSize;
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
   uint32 RequiredSize;
   uint32 SizeAlignMask;
   uint8 *ReservedMemoryPtr;

   /*
   ** Allocate memory for the cFE memory. Note that this is malloced on
   ** the COTS board, but will be a static location in the ETU. 
   */
   SizeAlignMask = sizeof(uint32) - 1;

   CFE_PSP_ReservedMemoryMap.ResetSize = CFE_PSP_RESET_AREA_SIZE;
   CFE_PSP_ReservedMemoryMap.VolatileDiskSize = (CFE_PSP_RAM_DISK_SECTOR_SIZE * CFE_PSP_RAM_DISK_NUM_SECTORS);
   CFE_PSP_ReservedMemoryMap.CDSSize = CFE_PSP_CDS_SIZE;
   CFE_PSP_ReservedMemoryMap.UserReservedSize = CFE_PSP_USER_RESERVED_SIZE;

   /*  Calculate the required size, adding padding so that each element is aligned */
   RequiredSize = (sizeof(CFE_PSP_ReservedFixedInfo_t) + SizeAlignMask) & ~SizeAlignMask;
   RequiredSize += (CFE_PSP_ReservedMemoryMap.ResetSize + SizeAlignMask) & ~SizeAlignMask;
   RequiredSize += (CFE_PSP_ReservedMemoryMap.CDSSize + SizeAlignMask) & ~SizeAlignMask;
   RequiredSize += (CFE_PSP_ReservedMemoryMap.VolatileDiskSize + SizeAlignMask) & ~SizeAlignMask;
   RequiredSize += (CFE_PSP_ReservedMemoryMap.UserReservedSize + SizeAlignMask) & ~SizeAlignMask;

   OS_printf("Sizeof BSP reserved memory = %u bytes\n",(unsigned int)RequiredSize);

   ReservedMemoryPtr = (uint8 *)malloc(RequiredSize);

   if ( ReservedMemoryPtr == NULL )
   {
      OS_printf("CFE_PSP: Error: Cannot malloc BSP reserved memory!\n");
      return_code = OS_ERROR;
   }
   else
   {
      OS_printf("CFE_PSP: Allocated %u bytes for PSP reserved memory at: 0x%08X\n",
                (unsigned int)RequiredSize, (unsigned int)ReservedMemoryPtr);
 
      /* FIXME: On real HW clearing would be only done if RestartType != CFE_PSP_PROCESSOR_RESET,
         but in this impl we are using malloc'ed memory so it always must be done */
      
      OS_printf("CFE_PSP: Clearing Processor Reserved Memory.\n");
      memset(ReservedMemoryPtr, 0, RequiredSize);
      
      CFE_PSP_ReservedMemoryMap.FixedInfo = (CFE_PSP_ReservedFixedInfo_t *)ReservedMemoryPtr;
      ReservedMemoryPtr += (sizeof(CFE_PSP_ReservedFixedInfo_t) + SizeAlignMask) & ~SizeAlignMask;
      
      CFE_PSP_ReservedMemoryMap.ResetMemory = ReservedMemoryPtr;
      ReservedMemoryPtr += (CFE_PSP_ReservedMemoryMap.ResetSize + SizeAlignMask) & ~SizeAlignMask;
      
      CFE_PSP_ReservedMemoryMap.VolatileDiskMemory = ReservedMemoryPtr;
      ReservedMemoryPtr += (CFE_PSP_ReservedMemoryMap.VolatileDiskSize + SizeAlignMask) & ~SizeAlignMask;
      
      CFE_PSP_ReservedMemoryMap.CDSMemory = ReservedMemoryPtr;
      ReservedMemoryPtr += (CFE_PSP_ReservedMemoryMap.CDSSize + SizeAlignMask) & ~SizeAlignMask;
      
      CFE_PSP_ReservedMemoryMap.UserReservedMemory = ReservedMemoryPtr;
      ReservedMemoryPtr += (CFE_PSP_ReservedMemoryMap.UserReservedSize + SizeAlignMask) & ~SizeAlignMask;
      
      /*
      ** Set the default reset type in case a watchdog reset occurs 
      */
      CFE_PSP_ReservedMemoryMap.FixedInfo->bsp_reset_type = CFE_PSP_RST_TYPE_PROCESSOR;
      
      /* Keep the actual size for future reference.
         This is also something that can be checked to see if things were initialized successfully.
         (i.e. If this reads zero then none of the other pointers are useful)
      */
      CFE_PSP_ReservedMemoryMap.TotalSize = RequiredSize;
            
      return_code = OS_SUCCESS;
   }
   return(return_code);

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

