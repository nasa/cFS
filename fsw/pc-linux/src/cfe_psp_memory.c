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
**      OSX x86 Linux Version
**
** Purpose:
**   cFE PSP Memory related functions. This is the implementation of the cFE 
**   memory areas that have to be preserved, and the API that is designed to allow
**   acccess to them. It also contains memory related routines to return the
**   address of the kernel code used in the cFE checksum.
**
** History:
**   2006/09/29  A. Cudmore      | Initial version for OS X 
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

/*
** cFE includes 
*/
#include "common_types.h"
#include "osapi.h"

/*
** Types and prototypes for this module
*/
#include "cfe_psp.h"

/*
** PSP Specific defines
*/
#include "cfe_psp_config.h"

#define CFE_PSP_CDS_KEY_FILE ".cdskeyfile"
#define CFE_PSP_RESET_KEY_FILE ".resetkeyfile"
#define CFE_PSP_RESERVED_KEY_FILE ".reservedkeyfile"

#include <target_config.h>

/*
 * Define the PSP-supported capacities to be the maximum allowed,
 * (since the PC-linux PSP has the advantage of abundant disk space to hold this)
 */
#define CFE_PSP_CDS_SIZE            (GLOBAL_CONFIGDATA.CfeConfig->CdsSize)
#define CFE_PSP_RESET_AREA_SIZE     (GLOBAL_CONFIGDATA.CfeConfig->ResetAreaSize)
#define CFE_PSP_USER_RESERVED_SIZE  (GLOBAL_CONFIGDATA.CfeConfig->UserReservedSize)

/*
** Internal prototypes for this module
*/
int32 CFE_PSP_InitCDS(uint32 RestartType );
int32 CFE_PSP_InitResetArea(uint32 RestartType);
int32 CFE_PSP_InitVolatileDiskMem(uint32 RestartType );
int32 CFE_PSP_InitUserReservedArea(uint32 RestartType );

/*
**  External Declarations
*/
extern unsigned int _init;
extern unsigned int _fini;

/*
** Global variables
*/
uint8 *CFE_PSP_CDSPtr = 0;
uint8 *CFE_PSP_ResetAreaPtr = 0;
uint8 *CFE_PSP_UserReservedAreaPtr = 0;
int    ResetAreaShmId;
int    CDSShmId;
int    UserShmId;
                                                                              
                                                                              
                                                                              
/*
*********************************************************************************
** CDS related functions
*********************************************************************************
*/
/*
**  
*/
/******************************************************************************
**  Function: CFE_PSP_InitCDS
**
**  Purpose: This function is used by the ES startup code to initialize the 
**            Critical Data store area
**  
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

int32 CFE_PSP_InitCDS(uint32 RestartType )
{
   int32 return_code;
   key_t key;

   /* 
   ** Make the Shared memory key
   */
   if ((key = ftok(CFE_PSP_CDS_KEY_FILE, 'R')) == -1) 
   {
        OS_printf("CFE_PSP: Cannot Create CDS Shared memory key!\n");
        exit(-1);
   }

   /* 
   ** connect to (and possibly create) the segment: 
   */
   if ((CDSShmId = shmget(key, CFE_PSP_CDS_SIZE, 0644 | IPC_CREAT)) == -1) 
   {
        OS_printf("CFE_PSP: Cannot shmget CDS Shared memory Segment!\n");
        exit(-1);
   }

   /* 
   ** attach to the segment to get a pointer to it: 
   */
   CFE_PSP_CDSPtr = shmat(CDSShmId, (void *)0, 0);
   if (CFE_PSP_CDSPtr == (uint8 *)(-1)) 
   {
        OS_printf("CFE_PSP: Cannot shmat to CDS Shared memory Segment!\n");
        exit(-1);
   }

   if ( RestartType == CFE_PSP_RST_TYPE_POWERON )
   {
      OS_printf("CFE_PSP: Clearing out CFE CDS Shared memory segment.\n");
      memset(CFE_PSP_CDSPtr, 0, CFE_PSP_CDS_SIZE);
   }
   
   return_code = CFE_PSP_SUCCESS;
   return(return_code);

}


/******************************************************************************
**  Function: CFE_PSP_DeleteCDS
**
**  Purpose:
**   This is an internal function to delete the CDS Shared memory segment. 
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
void CFE_PSP_DeleteCDS(void)
{

   int    ReturnCode;
   struct shmid_ds ShmCtrl;
   
   ReturnCode = shmctl(CDSShmId, IPC_RMID, &ShmCtrl);
   
   if ( ReturnCode == 0 )
   {
      printf("CFE_PSP: Critical Data Store Shared memory segment removed\n");
   }
   else
   {
      printf("CFE_PSP: Error Removing Critical Data Store Shared memory Segment.\n");
      printf("CFE_PSP: It can be manually checked and removed using the ipcs and ipcrm commands.\n");
   }


}

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
       *SizeOfCDS = CFE_PSP_CDS_SIZE;
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
          CopyPtr = &(CFE_PSP_CDSPtr[CDSOffset]);
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
          CopyPtr = &(CFE_PSP_CDSPtr[CDSOffset]);
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
**  Function: CFE_PSP_InitESResetArea
**
**  Purpose:
**    This function is used by the ES startup code to initialize the
**     ES Reset Area.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_InitResetArea(uint32 RestartType)
{

   int32 return_code;
   key_t key;

   /* 
   ** Make the Shared memory key
   */
   if ((key = ftok(CFE_PSP_RESET_KEY_FILE, 'R')) == -1) 
   {
        OS_printf("CFE_PSP: Cannot Create Reset Area Shared memory key!\n");
        exit(-1);
   }

   /* 
   ** connect to (and possibly create) the segment: 
   */
   if ((ResetAreaShmId = shmget(key, CFE_PSP_RESET_AREA_SIZE, 0644 | IPC_CREAT)) == -1) 
   {
        OS_printf("CFE_PSP: Cannot shmget Reset Area Shared memory Segment!\n");
        exit(-1);
   }

   /* 
   ** attach to the segment to get a pointer to it: 
   */
   CFE_PSP_ResetAreaPtr = shmat(ResetAreaShmId, (void *)0, 0);
   if (CFE_PSP_ResetAreaPtr == (uint8 *)(-1)) 
   {
        OS_printf("CFE_PSP: Cannot shmat to Reset Area Shared memory Segment!\n");
        exit(-1);
   }

   if ( RestartType == CFE_PSP_RST_TYPE_POWERON )
   {
      OS_printf("CFE_PSP: Clearing out CFE Reset Shared memory segment.\n");
      memset(CFE_PSP_ResetAreaPtr, 0, CFE_PSP_RESET_AREA_SIZE);
   }
   
   return_code = CFE_PSP_SUCCESS;
   return(return_code);
}


/******************************************************************************
**  Function: CFE_PSP_DeleteResetArea
**
**  Purpose:
**   This is an internal function to delete the Reset Area Shared memory segment. 
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
void CFE_PSP_DeleteResetArea(void)
{
   int    ReturnCode;
   struct shmid_ds ShmCtrl;
   
   ReturnCode = shmctl(ResetAreaShmId, IPC_RMID, &ShmCtrl);
   
   if ( ReturnCode == 0 )
   {
      printf("Reset Area Shared memory segment removed\n");
   }
   else
   {
      printf("Error Removing Reset Area Shared memory Segment.\n");
      printf("It can be manually checked and removed using the ipcs and ipcrm commands.\n");
   }


}


/*
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
   int32 return_code;
   
   if ( SizeOfResetArea == NULL )
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      *PtrToResetArea = (cpuaddr)CFE_PSP_ResetAreaPtr;
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
**  Function: CFE_PSP_InitUserReservedArea 
**
**  Purpose:
**    This function is used by the ES startup code to initialize the
**      ES user reserved area.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_InitUserReservedArea(uint32 RestartType )
{
   int32 return_code;
   key_t key;

   /* 
   ** Make the Shared memory key
   */
   if ((key = ftok(CFE_PSP_RESERVED_KEY_FILE, 'R')) == -1) 
   {
        OS_printf("CFE_PSP: Cannot Create User Reserved Area Shared memory key!\n");
        exit(-1);
   }

   /* 
   ** connect to (and possibly create) the segment: 
   */
   if ((UserShmId = shmget(key, CFE_PSP_USER_RESERVED_SIZE, 0644 | IPC_CREAT)) == -1) 
   {
        OS_printf("CFE_PSP: Cannot shmget User Reserved Area Shared memory Segment!\n");
        exit(-1);
   }

   /* 
   ** attach to the segment to get a pointer to it: 
   */
   CFE_PSP_UserReservedAreaPtr = shmat(UserShmId, (void *)0, 0);
   if (CFE_PSP_UserReservedAreaPtr == (uint8 *)(-1)) 
   {
        OS_printf("CFE_PSP: Cannot shmat to User Reserved Area Shared memory Segment!\n");
        exit(-1);
   }

   if ( RestartType == CFE_PSP_RST_TYPE_POWERON )
   {
      OS_printf("CFE_PSP: Clearing out CFE User Reserved Shared memory segment.\n");
      memset(CFE_PSP_UserReservedAreaPtr, 0, CFE_PSP_USER_RESERVED_SIZE);
   }
   
   return_code = CFE_PSP_SUCCESS;
   return(return_code);

}

/******************************************************************************
**  Function: CFE_PSP_DeleteUserReservedArea
**
**  Purpose:
**   This is an internal function to delete the User Reserved Shared memory segment. 
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
void CFE_PSP_DeleteUserReservedArea(void)
{
   int    ReturnCode;
   struct shmid_ds ShmCtrl;
   
   ReturnCode = shmctl(UserShmId, IPC_RMID, &ShmCtrl);
   
   if ( ReturnCode == 0 )
   {
      printf("User Reserved Area Shared memory segment removed\n");
   }
   else
   {
      printf("Error Removing User Reserved Area Shared memory Segment.\n");
      printf("It can be manually checked and removed using the ipcs and ipcrm commands.\n");
   }
}


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
   int32 return_code;
   
   if ( SizeOfUserArea == NULL )
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      *PtrToUserArea = (cpuaddr)(&CFE_PSP_UserReservedAreaPtr);
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
**  Function: CFE_PSP_InitVolatileDiskMem
**
**  Purpose:
**   This function is used by the ES startup code to initialize the memory
**   used by the volatile disk. On a desktop/posix platform this is currently
**   a no-op, because the volatile disk is being mapped to the desktop.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 CFE_PSP_InitVolatileDiskMem(uint32 RestartType )
{
   int32 return_code;
    
   /*
   ** Here, we want to clear out the volatile ram disk contents
   ** on a power on reset 
   */
    
   return_code = CFE_PSP_SUCCESS;
   return(return_code);

}

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
   int32 return_code;
   
   if ( SizeOfVolDisk == NULL )
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      *PtrToVolDisk = 0;
      *SizeOfVolDisk = 0;
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
   int   tempFd;
   
   /*
   ** Create the key files for the shared memory segments
   ** The files are not needed, so they are closed right away.
   */
   tempFd = open(CFE_PSP_CDS_KEY_FILE, O_RDONLY | O_CREAT, S_IRWXU );
   close(tempFd);
   tempFd = open(CFE_PSP_RESET_KEY_FILE, O_RDONLY | O_CREAT, S_IRWXU );
   close(tempFd);
   tempFd = open(CFE_PSP_RESERVED_KEY_FILE, O_RDONLY | O_CREAT, S_IRWXU );
   close(tempFd);
      
   if ( RestartType == CFE_PSP_RST_TYPE_PROCESSOR )
   {
      return_code = CFE_PSP_InitCDS( CFE_PSP_RST_TYPE_PROCESSOR );
      return_code = CFE_PSP_InitResetArea( CFE_PSP_RST_TYPE_PROCESSOR );
      return_code = CFE_PSP_InitVolatileDiskMem( CFE_PSP_RST_TYPE_PROCESSOR );
      return_code = CFE_PSP_InitUserReservedArea( CFE_PSP_RST_TYPE_PROCESSOR );
   }
   else 
   {
      return_code = CFE_PSP_InitCDS( CFE_PSP_RST_TYPE_POWERON );
      return_code = CFE_PSP_InitResetArea( CFE_PSP_RST_TYPE_POWERON );
      return_code = CFE_PSP_InitVolatileDiskMem( CFE_PSP_RST_TYPE_POWERON );
      return_code = CFE_PSP_InitUserReservedArea( CFE_PSP_RST_TYPE_POWERON );
   }

   return(return_code);
}

/******************************************************************************
**  Function: CFE_PSP_DeleteProcessorReservedMemory
**
**  Purpose:
**    This function cleans up all of the shared memory segments in the 
**     Linux/OSX ports.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
void CFE_PSP_DeleteProcessorReservedMemory(void)
{
   
   CFE_PSP_DeleteCDS();
   CFE_PSP_DeleteResetArea();
   CFE_PSP_DeleteUserReservedArea();
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
   /*
   ** Prevent warnings by referencing parameters
   */
   if ( PtrToKernelSegment == NULL || SizeOfKernelSegment == NULL )
   {
      return(CFE_PSP_ERROR);
   }
   
   return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
   
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
   
   if ( SizeOfCFESegment == NULL )
   {
      return_code = CFE_PSP_ERROR;
   }
   else
   {
      *PtrToCFESegment = (cpuaddr)(&_init);
      *SizeOfCFESegment = (uint32) (((cpuaddr) &_fini) - ((cpuaddr) &_init));
      
      return_code = CFE_PSP_SUCCESS;
   }
   
   return(return_code);
}

