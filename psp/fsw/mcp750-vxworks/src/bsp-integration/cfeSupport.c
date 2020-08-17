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
** cFE Support routines
**
** This module is a collection of support routines needed to run the cFE on vxWorks.
** 
*/

#include <stdio.h>
#include <string.h>
#include "vxWorks.h"
#include "taskLib.h"
#include "sysLib.h"
#include "symLib.h"
#include "loadLib.h"
#include "ioLib.h"
#include "dosFsLib.h"
#include "xbdBlkDev.h"
#include "errnoLib.h"
#include "usrLib.h"
#include "cacheLib.h"
#include "drv/hdisk/ataDrv.h"

/*
** External reference to wrs_kernel_text_start and wrs_kernel_text_end
** The cFE core needs these symbols, but when the cfe-core.o is loaded as a 
** dynamic module, these symbols are not in the symbol table. 
*/

extern unsigned int wrs_kernel_text_start;
extern unsigned int wrs_kernel_text_end;

/*
** External reference to the vxWorks symbol table
*/
extern SYMTAB_ID sysSymTbl;

/*
** External variable for Disk Number
*/
extern int DriveNum;

/*
** Function: GetWrsKernelTextStart
** Purpose:  This function returns the start address of the kernel code.
**
*/
unsigned int GetWrsKernelTextStart (void)
{
   return (unsigned int) &wrs_kernel_text_start;
}

/*
** Function: GetWrsKernelTextEnd
** Purpose:  This function returns the end address of the kernel code.
**
*/
unsigned int GetWrsKernelTextEnd (void)
{
   return (unsigned int) &wrs_kernel_text_end;
}


/*
** Function: loadCfeCore
** Purpose:  This function unzips ( if needed ) , loads, and starts the cFE core.
**
*/
int startCfeCore ( char *cfevolume, char *cfepath )
{
   int        fd;
   int        status;
   MODULE_ID  moduleID;
   SYM_TYPE   symType;
   char       *symValue;
   void       (*cFEFuncPtr)(void);
   char       cfeCorePath[64];

   if ( cfevolume == NULL || cfepath == NULL )
   {
      printf("Error: No cFE volume or path/filename given.\n");
      return(-1);
   }

   /*
   ** Recreate cFE core path name
   ** This is a kludge to account for the vxWorks XBD volume name
   ** ( CF:0, CF:1, etc )
   ** This assumes that the CFE_PSP_InitFlashDisk function has been called.
   */
   snprintf(cfeCorePath,64,"%s:%d%s",cfevolume,DriveNum,cfepath);

   /*
   ** Open the cFE core module
   */
   fd = open( cfeCorePath, O_RDONLY, 0);
   if ( fd < 0 )
   {
      printf("Error: Cannot open cFE core file: %s!\n",cfeCorePath);
      return(-1);
   }
   else
   {
      printf("Opened %s.\n",cfepath);
   }
    
   /*
   ** Load the cFE core
   */
   moduleID = loadModule ( fd, LOAD_ALL_SYMBOLS );
   if ( moduleID == NULL )
   { 
      printf("Error: Cannot load cFE core module.\n");
      close(fd);
      return(-1);
   }
   else
   {
      printf("Loaded %s module OK.\n",cfepath);
   }
   
   /*
   ** Close the file
   */
   close(fd); 
   
   /*
   ** Lookup the cFE core entry point
   */
   status = symFindByName(sysSymTbl, "CFE_PSP_Main", &symValue, &symType);
   if ( status == ERROR )
   {
      printf("Cannot locate CFE_PSP_Main symbol, trying OS_BSPMain\n");
      status = symFindByName(sysSymTbl, "OS_BSPMain", &symValue, &symType);
      if ( status == ERROR )
      {
         printf("Error: Cannot locate CFE_PSP_Main or OS_BSPMain symbols.\n");
         return(-1);
      }
   }

   /*
   ** Call the cFE startup routine 
   */
    cFEFuncPtr = (void *)symValue;
    (*cFEFuncPtr)();
 
   /*
   ** Return to the vxWorks shell
   */
   return(0);
   
}

/******************************************************************************
**  Function:  CFE_PSP_InitFlashDisk()
**
**  Purpose:
**    Initialize the Compact flash disk in vxWorks
**
**  Arguments:
**    (none)
**
**  Return:
**    Returns -1 for error, or Compact flash device number on success ( 0 or 1 ) 
*/

int CFE_PSP_InitFlashDisk( void)
{
    BLK_DEV    *ataBlkDev;  
    int         Status = 0;
    device_t    xbd;
    int         i;
    int         cfNameFound;
    int         tempFd;
    char        CompactFlashName[32];

    printf("CFE_PSP: Re-mounting the ATA/FLASH DISK\n");
   
    /*
    ** Create the Flash disk device
    */
    if( (ataBlkDev = ataDevCreate( 0, 0, 0, 0)) == NULL)
    {
       printf("CFE_PSP: Error Creating flash disk device.\n");
       Status = -1;
    }
    else
    {
       /*
       **  Attach to the XBD layer 
       */
       xbd = xbdBlkDevCreate(ataBlkDev,"CF");
       if (xbd == NULLDEV)
       {
          printf("CFE_PSP: Error Creating XBD device on ATA/FLASH disk.\n");
          Status = -1;
       }
       else
       {
          /*
          ** Delay to allow the XBD operation to complete
          */
          (void) taskDelay(100);

          /*
          ** Determine the name of the Compact flash drive
          */
          cfNameFound = 0;
          for ( i = 0; i < 4; i++ )
          {
             snprintf(CompactFlashName, 32, "%s:%d","CF", i);
             tempFd = open ( CompactFlashName, O_RDONLY, 0644 );
             if ( tempFd != ERROR )
             {
                close(tempFd);
                cfNameFound = 1;
                break;
             }
          }
          if ( cfNameFound == 1 ) 
          {
             printf("CFE_PSP: FLASH device initialized. vxWorks name = %s\n",CompactFlashName);
             Status = i;
          }
          else
          {
             printf("CFE_PSP: Warning: Compact flash name not found! Run devs command in shell\n");
             Status = -1;
          } 

       } /* end if */
       
    } /* end if */
   
    return(Status);
}

