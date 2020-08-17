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

/**
 * \file cfe_psp_eeprom_mmap_file.c
 *
 *  Created on: Jul 17, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * This is an implementation of the PSP EEPROM API calls that operates on a
 * memory-mapped disk file, therefore emulating the persistence of a real eeprom device.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "cfe_psp.h"
#include "cfe_psp_module.h"

/*
** Defines
*/
#define EEPROM_FILE "EEPROM.DAT"


CFE_PSP_MODULE_DECLARE_SIMPLE(eeprom_mmap_file);

/*
** Simulate EEPROM by mapping in a file
*/
int32 CFE_PSP_SetupEEPROM(uint32 EEPROMSize, cpuaddr *EEPROMAddress)
{

   int          FileDescriptor;
   int          ReturnStatus;
   char        *DataBuffer;
   struct stat  StatBuf;

   /*
   ** Check to see if the file has been created.
   ** If not, create it.
   ** If so, then open it for read/write
   */
   ReturnStatus = stat(EEPROM_FILE, &StatBuf);
   if ( ReturnStatus == -1 )
   {
      /*
      ** File does not exist, create it.
      */
      FileDescriptor = open(EEPROM_FILE, O_RDWR | O_CREAT, S_IRWXU);
      if ( FileDescriptor == -1 )
      {
         OS_printf("CFE_PSP: Cannot open EEPROM File: %s\n",EEPROM_FILE);
         return(-1);
      }
      else
      {
         /*
         ** Need to seek to the desired EEPROM size
         */
         if (lseek (FileDescriptor, EEPROMSize - 1, SEEK_SET) == -1)
         {
            OS_printf("CFE_PSP: Cannot Seek to end of EEPROM file.\n");
            close(FileDescriptor);
            return(-1);
         }

         /*
         ** Write a byte at the end of the File
         */
         if (write (FileDescriptor, "", 1) != 1)
         {
            OS_printf("CFE_PSP: Cannot write to EEPROM file\n");
            close(FileDescriptor);
            return(-1);
         }
      }
   }
   else
   {
      /*
      ** File exists
      */
      FileDescriptor = open(EEPROM_FILE, O_RDWR);
      if ( FileDescriptor == -1 )
      {
         OS_printf("CFE_PSP: Cannot open EEPROM File: %s\n",EEPROM_FILE);
         perror("CFE_PSP: open");
         return(-1);
      }
   }

   /*
   ** Map the file to a memory space
   */
   if ((DataBuffer = mmap(NULL, EEPROMSize, PROT_READ | PROT_WRITE, MAP_SHARED, FileDescriptor, 0)) == (void*)(-1))
   {
      OS_printf("CFE_PSP: mmap to EEPROM File failed\n");
      close(FileDescriptor);
      return(-1);
   }

   /*
   ** Return the address to the caller
   */
   *EEPROMAddress = (cpuaddr)DataBuffer;

   return(0);
}

/* For read/write - As this is mmap'ed we dereference the pointer directly.
 * Hopefully the caller didn't get it wrong.
 * No need to anything special for 8/16/32 width access in this mode.
 */
int32 CFE_PSP_EepromWrite32( cpuaddr MemoryAddress, uint32 uint32Value )
{
    *((uint32*)MemoryAddress) = uint32Value;
    return(CFE_PSP_SUCCESS);
}

int32 CFE_PSP_EepromWrite16( cpuaddr MemoryAddress, uint16 uint16Value )
{
    *((uint16*)MemoryAddress) = uint16Value;
    return(CFE_PSP_SUCCESS);
}

int32 CFE_PSP_EepromWrite8( cpuaddr MemoryAddress, uint8 ByteValue )
{
    *((uint8*)MemoryAddress) = ByteValue;
    return(CFE_PSP_SUCCESS);
}

int32 CFE_PSP_EepromWriteEnable(uint32 Bank)
{
    return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

int32 CFE_PSP_EepromWriteDisable(uint32 Bank)
{
    return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

int32 CFE_PSP_EepromPowerUp(uint32 Bank)
{
    return(CFE_PSP_SUCCESS);
}

int32 CFE_PSP_EepromPowerDown(uint32 Bank)
{
   return(CFE_PSP_SUCCESS);
}

void eeprom_mmap_file_Init(uint32 PspModuleId)
{
    int32  Status;
    cpuaddr eeprom_address;
    uint32 eeprom_size;

    /*
    ** Create the simulated EEPROM segment by mapping a memory segment to a file.
    ** Since the file will be saved, the "EEPROM" contents will be preserved.
    ** Set up 512Kbytes of EEPROM
    */
    eeprom_size = 0x80000;
    Status = CFE_PSP_SetupEEPROM(eeprom_size, &eeprom_address);

    if ( Status == 0 )
    {
       /*
       ** Install the 2nd memory range as the mapped file ( EEPROM )
       */
       Status = CFE_PSP_MemRangeSet (1, CFE_PSP_MEM_EEPROM, eeprom_address,
                                     eeprom_size, CFE_PSP_MEM_SIZE_DWORD, 0 );
       OS_printf("CFE_PSP: EEPROM Range (2) created: Start Address = %08lX, Size = %08X Status = %d\n", (unsigned long)eeprom_address, (unsigned int)eeprom_size, Status);

    }
    else
    {
       OS_printf("CFE_PSP: Cannot create EEPROM Range from Memory Mapped file.\n");
    }
}

