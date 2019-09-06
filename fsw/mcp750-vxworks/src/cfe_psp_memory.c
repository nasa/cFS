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

/*
** Define the cFE Core loadable module name
*/
#define CFE_MODULE_NAME "cfe-core.o"


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
CFE_PSP_ReservedMemory_t *CFE_PSP_ReservedMemoryPtr; 

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
       *SizeOfCDS =  CFE_PSP_CDS_SIZE;
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
       if ( (CDSOffset < CFE_PSP_CDS_SIZE ) && ( (CDSOffset + NumBytes) <= CFE_PSP_CDS_SIZE ))
       {
          CopyPtr = &(CFE_PSP_ReservedMemoryPtr->CDSMemory[CDSOffset]);
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
       if ( (CDSOffset < CFE_PSP_CDS_SIZE ) && ( (CDSOffset + NumBytes) <= CFE_PSP_CDS_SIZE ))
       {
          CopyPtr = &(CFE_PSP_ReservedMemoryPtr->CDSMemory[CDSOffset]);
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
      *PtrToResetArea = (cpuaddr)&(CFE_PSP_ReservedMemoryPtr->ResetMemory[0]);
      *SizeOfResetArea = CFE_PSP_RESET_AREA_SIZE;
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
      *PtrToUserArea = (cpuaddr)&(CFE_PSP_ReservedMemoryPtr->UserReservedMemory[0]);
      *SizeOfUserArea = CFE_PSP_USER_RESERVED_SIZE;
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
      *PtrToVolDisk = (cpuaddr)&(CFE_PSP_ReservedMemoryPtr->VolatileDiskMemory[0]);
      *SizeOfVolDisk = CFE_PSP_VOLATILE_DISK_SIZE;
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
      memset((void *)CFE_PSP_ReservedMemoryPtr, 0, sizeof(CFE_PSP_ReservedMemory_t));
      
      /*
      ** Set the default reset type in case a watchdog reset occurs 
      */
      CFE_PSP_ReservedMemoryPtr->bsp_reset_type = CFE_PSP_RST_TYPE_PROCESSOR;

   }      
   return_code = CFE_PSP_SUCCESS;
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

