/*
** File   : osloader.c
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Alan Cudmore
**
** Purpose: This file contains the module loader and symbol lookup functions for the OSAL.
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>

#include "vxWorks.h"
#include "sysLib.h"
#include "symLib.h"
#include "loadLib.h"
#include "unldLib.h"
#include "ioLib.h"

#include "common_types.h"
#include "osapi.h"


/*
** If OS_INCLUDE_MODULE_LOADER is not defined, skip the module
*/
#ifdef OS_INCLUDE_MODULE_LOADER

/****************************************************************************************
                                     TYPEDEFS 
****************************************************************************************/

typedef struct 
{
    char    SymbolName[OS_MAX_SYM_LEN];
    uint32  SymbolAddress;
} SymbolRecord_t;

/****************************************************************************************
                                     DEFINES
****************************************************************************************/
#undef  OS_DEBUG_PRINTF
#define OS_SYMBOL_RECORD_SIZE sizeof(SymbolRecord_t)

/****************************************************************************************
                                   EXTERNAL GLOBAL REFERENCES 
****************************************************************************************/
extern SYMTAB_ID sysSymTbl;

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/
int    OS_symbol_table_size = 0;

/*
** Need to define the OS Module table here. 
** osconfig.h will have the maximum number of loadable modules defined.
*/
OS_module_record_t OS_module_table[OS_MAX_MODULES];

/*
** The Mutex for protecting the above table
*/
SEM_ID     OS_module_table_mut;

/*
** The File descriptor for the symbol table dump function
*/
int    OS_sym_table_file_fd;

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/
int32  OS_ModuleTableInit ( void )
{
   int i;
   
   /* 
   ** Initialize Module Table 
   */
   for(i = 0; i < OS_MAX_MODULES; i++)
   {
      OS_module_table[i].free        = TRUE;
      OS_module_table[i].entry_point = 0; 
      OS_module_table[i].host_module_id = 0;
      OS_module_table[i].addr.valid = FALSE;
      strcpy(OS_module_table[i].name,"");
      strcpy(OS_module_table[i].filename,"");
   }

   /*
   ** Create the Module Table mutex
   */
   OS_module_table_mut = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE); 
   if ( OS_module_table_mut == NULL )
   {
      return(OS_ERROR);
   }
   else
   {
      return(OS_SUCCESS);
   }
   
}

/****************************************************************************************
                                    Symbol table API
****************************************************************************************/
/*--------------------------------------------------------------------------------------
    Name: OS_SymbolLookup
    
    Purpose: Find the Address of a Symbol 

    Parameters: 

    Returns: OS_ERROR if the symbol could not be found
             OS_SUCCESS if the symbol is found 
             OS_INVALID_POINTER if the pointer passed in is invalid
             
             The address of the symbol will be stored in the pointer that is passed in.
---------------------------------------------------------------------------------------*/
int32 OS_SymbolLookup( uint32 *SymbolAddress, char *SymbolName )
{
   STATUS    vxStatus;
   SYM_TYPE  SymType;
   char *    SymValue; 

   /*
   ** Check parameters
   */
   if (( SymbolAddress == NULL ) || (SymbolName == NULL ))
   {
      return(OS_INVALID_POINTER);
   }

   /*
   ** Lookup the entry point
   */
   vxStatus = symFindByName ( sysSymTbl, SymbolName, &SymValue, &SymType );
   if( vxStatus == ERROR )
   {
      return(OS_ERROR);
   }
   *SymbolAddress = (uint32)SymValue;   
   
   return(OS_SUCCESS);
    
}/* end OS_SymbolLookup */


/*--------------------------------------------------------------------------------------
    Name: OS_SymTableIterator ( local function, not part of the public API )
    
    Purpose: Function called by vxWorks to iterate the vxworks symbol table  

    Parameters: 
             name - The symbol name
             val  - The symbol address value
             type - The vxWorks symbol type ( not used )
             max_size - The maximum size of the file that is written to.
             group - The vxWorks symbol group ( not used )

    Returns: TRUE to tell vxWorks to continue to iterate the symbol table 
             FALSE to tell vxWorks to stop iterating the symbol table
             
             The address of the symbol will be stored in the pointer that is passed in.
---------------------------------------------------------------------------------------*/
BOOL  OS_SymTableIterator ( char *name, int val,  SYM_TYPE type,  int max_size, UINT16 group )
{
   SymbolRecord_t symRecord;   
   int            status;
  
   /*
   ** Copy symbol name 
   */
   strncpy(symRecord.SymbolName, name, OS_MAX_SYM_LEN);

   /*
   ** Save symbol address
   */
   symRecord.SymbolAddress = (uint32)val;

   /*
   ** Write entry in file
   */
   status = write(OS_sym_table_file_fd, (char *)&symRecord, sizeof(symRecord));
   if ( status == -1 )
   {
       return(FALSE);
   }

   /*
   ** Check to see if the maximum size of the file has been reached 
   */
   OS_symbol_table_size = OS_symbol_table_size + OS_SYMBOL_RECORD_SIZE;
   if ( OS_symbol_table_size >= max_size )
   {
      /*
      ** We reached the maximum size, so tell vxWorks to stop
      */
      return(FALSE);
   }
   /*
   ** It's OK to continue
   */
   return(TRUE);
} 

/*--------------------------------------------------------------------------------------
    Name: OS_SymbolTableDump
    
    Purpose: Dumps the system symbol table to a file

    Parameters: 

    Returns: OS_ERROR if the symbol table could not be read or dumped
             OS_INVALID_POINTER if the filename is NULL
             OS_FS_ERR_PATH_INVALID  if the filename/path is invalid 
             OS_SUCCESS if the symbol is found 
---------------------------------------------------------------------------------------*/
int32 OS_SymbolTableDump ( char *filename, uint32 SizeLimit )
{
   char   local_path_name[OS_MAX_LOCAL_PATH_LEN];
   int32  return_status;
   SYMBOL *last_symbol;

   /*
   ** Check parameters
   */
   if ( filename == NULL ) 
   {
      return OS_INVALID_POINTER;
   }
   if (SizeLimit < OS_SYMBOL_RECORD_SIZE)
   {
      return(OS_ERROR);
   }

   /*
   ** Get local path name
   */
   if ( OS_TranslatePath(filename, local_path_name) != OS_FS_SUCCESS )
   {
      return(OS_FS_ERR_PATH_INVALID);
   }

   /*
   ** Open file
   */
   OS_sym_table_file_fd = open(local_path_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
   if ( OS_sym_table_file_fd  != -1 )
   {
      /*
      ** Reset the global
      */
      OS_symbol_table_size = 0;

      /*
      ** Iterate the symbol table
      */
      last_symbol = symEach( sysSymTbl, (FUNCPTR)OS_SymTableIterator, SizeLimit );
      close(OS_sym_table_file_fd);
      return_status = OS_SUCCESS;
   }
   else
   {
      return_status = OS_ERROR;
   }
   return(return_status);
   
}/* end OS_SymbolTableDump */

/****************************************************************************************
                                    Module Loader API
****************************************************************************************/

/*--------------------------------------------------------------------------------------
    Name: OS_ModuleLoad
    
    Purpose: Loads an object file into the running operating system

    Parameters: 

    Returns: OS_ERROR if the module cannot be loaded
             OS_INVALID_POINTER if one of the parameters is NULL
             OS_ERR_NO_FREE_IDS if the module table is full
             OS_ERR_NAME_TAKEN if the name is in use
             OS_SUCCESS if the module is loaded successfuly

---------------------------------------------------------------------------------------*/
int32 OS_ModuleLoad ( uint32 *module_id, char *module_name, char *filename )
{
   int         i;
   uint32      possible_moduleid;
   char        translated_path[OS_MAX_LOCAL_PATH_LEN];
   int32       return_code;
   int         fd;
   MODULE_ID   vxModuleId;
   
   /*
   ** Check parameters
   */
   if (( filename == NULL ) || (module_id == NULL ) || (module_name == NULL))
   {
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: Error, invalid parameters to OS_ModuleLoad\n");
      #endif
      return(OS_INVALID_POINTER);
   }
  
   semTake(OS_module_table_mut,WAIT_FOREVER); 

   /*
   ** Find a free module id
   */
   for( possible_moduleid = 0; possible_moduleid < OS_MAX_MODULES; possible_moduleid++)
   {
       if (OS_module_table[possible_moduleid].free == TRUE)
       {
           break;
       }
   }

   /* 
   ** Check to see if the id is out of bounds 
   */
   if( possible_moduleid >= OS_MAX_MODULES || OS_module_table[possible_moduleid].free != TRUE)
   {
       semGive(OS_module_table_mut);
       return OS_ERR_NO_FREE_IDS;
   }

   /* 
   ** Check to see if the module file is already loaded 
   */
   for (i = 0; i < OS_MAX_MODULES; i++)
   {
       if ((OS_module_table[i].free == FALSE) &&
          ( strcmp((char*) module_name, OS_module_table[i].name) == 0)) 
       {       
           semGive(OS_module_table_mut);
           return OS_ERR_NAME_TAKEN;
       }
   }

   /* 
   ** Set the possible task Id to not free so that
   ** no other task can try to use it 
   */
   OS_module_table[possible_moduleid].free = FALSE ;
   semGive(OS_module_table_mut);
 
   /*
   ** Translate the filename to the Host System
   */     
   return_code = OS_TranslatePath((const char *)filename, (char *)translated_path);
   if ( return_code != OS_SUCCESS )
   {
      return(return_code);
   }
   /*
   ** File is ready to load
   */
   
   /* 
   ** Open the file
   */
   fd = open (translated_path, O_RDONLY, 0);
   if( fd < 0 )
   {
      OS_module_table[possible_moduleid].free = TRUE ;
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: Error, cannot open application file: %s\n",translated_path);
      #endif
      return(OS_ERROR);
   }
   #ifdef OS_DEBUG_PRINTF
   else
   {
      OS_printf("OSAL: Opened application file OK.\n");
   }
   #endif
   
   /*
   ** Load the module
   */
   vxModuleId = loadModule (fd, LOAD_ALL_SYMBOLS);
   if( vxModuleId == NULL )
   {
      OS_module_table[possible_moduleid].free = TRUE ;
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: Error, cannot load module: %s\n",translated_path);
      #endif
      close(fd);
      return(OS_ERROR);
   }
   #ifdef OS_DEBUG_PRINTF
   else
   {
         OS_printf("OSAL: Loaded Module OK\n");
   }
   #endif
   
   /*
   ** Close the file
   */
   close(fd);
 
   /*
   ** fill out the OS_module_table entry for this new module
   */
   OS_module_table[possible_moduleid].entry_point = 0; /* Only for certain targets */
   OS_module_table[possible_moduleid].host_module_id = (uint32) vxModuleId;
   strncpy(OS_module_table[possible_moduleid].filename , filename, OS_MAX_PATH_LEN);
   strncpy(OS_module_table[possible_moduleid].name , module_name, OS_MAX_API_NAME);

   /*
   ** For now, do not store the module address information
   ** Let the OS_ModuleInfo function fetch that information and return it.
   */
   OS_module_table[possible_moduleid].addr.valid = FALSE; 
 
   /*
   ** Return the OSAL Module ID
   */
   *module_id = possible_moduleid;
     
   return(OS_SUCCESS);
   
}/* end OS_ModuleLoad */

/*--------------------------------------------------------------------------------------
    Name: OS_ModuleUnload
    
    Purpose: Unloads the module file from the running operating system

    Parameters: 

    Returns: OS_ERROR if the module is invalid or cannot be unloaded
             OS_SUCCESS if the module was unloaded successfuly 
---------------------------------------------------------------------------------------*/
int32 OS_ModuleUnload ( uint32 module_id )
{
   STATUS vxStatus;
   
   /*
   ** Check the module_id
   */
   if ( module_id >= OS_MAX_MODULES || OS_module_table[module_id].free == TRUE )
   {
      return(OS_ERR_INVALID_ID);
   }
  
   /*
   ** Attempt to close/unload the module
   */ 
   vxStatus = unldByModuleId((MODULE_ID )OS_module_table[module_id].host_module_id, 0);
   if ( vxStatus == ERROR )   
   {
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: Error, Cannot Close/Unload application file: %d\n",vxStatus);
      #endif
      return(OS_ERROR);
   }
   OS_module_table[module_id].free = TRUE ;
   #ifdef OS_DEBUG_PRINTF
      OS_printf("OS_ModuleUnload: Object file unloaded OK\n");
   #endif
   
   return(OS_SUCCESS);
      
}/* end OS_ModuleUnload */

/*--------------------------------------------------------------------------------------
    Name: OS_ModuleInfo
    
    Purpose: Returns information about the loadable module

    Parameters: 

    Returns: OS_ERR_INVALID_ID if the module id invalid
             OS_INVALID_POINTER if the pointer to the ModuleInfo structure is invalid
             OS_SUCCESS if the module info was filled out successfuly 
---------------------------------------------------------------------------------------*/
int32 OS_ModuleInfo ( uint32 module_id, OS_module_record_t *module_info )
{
   MODULE_INFO  vxModuleInfo;
   STATUS       vxStatus;

   /*
   ** Check the parameter
   */
   if ( module_info == NULL )
   {
      return(OS_INVALID_POINTER);
   }

   /*
   ** Check the module_id
   */
   if ( module_id >= OS_MAX_MODULES || OS_module_table[module_id].free == TRUE )
   {
      return(OS_ERR_INVALID_ID);
   }

   /*
   ** Fill out the module info
   */
   module_info->entry_point = OS_module_table[module_id].entry_point;
   module_info->host_module_id = OS_module_table[module_id].host_module_id;
   strncpy(module_info->filename, OS_module_table[module_id].filename , OS_MAX_PATH_LEN);
   strncpy(module_info->name, OS_module_table[module_id].name, OS_MAX_API_NAME);

   /*
   ** Get the module info from vxWorks
   */
   vxStatus = moduleInfoGet((MODULE_ID)OS_module_table[module_id].host_module_id, &vxModuleInfo);
   if ( vxStatus == ERROR )
   {
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: OS_ModuleInfoGet Error from vxWorks: %d\n",vxStatus);  
      #endif
      module_info->addr.valid = FALSE;
      module_info->addr.code_address = 0;
      module_info->addr.code_size = 0;
      module_info->addr.data_address = 0;
      module_info->addr.data_size = 0;
      module_info->addr.bss_address = 0;
      module_info->addr.bss_size = 0;
      module_info->addr.flags = 0; 
   }
   else
   {
      module_info->addr.valid = TRUE;
      module_info->addr.code_address = (uint32)(vxModuleInfo.segInfo.textAddr);
      module_info->addr.code_size = vxModuleInfo.segInfo.textSize;
      module_info->addr.data_address = (uint32)(vxModuleInfo.segInfo.dataAddr);
      module_info->addr.data_size = vxModuleInfo.segInfo.dataSize;
      module_info->addr.bss_address = (uint32)(vxModuleInfo.segInfo.bssAddr);
      module_info->addr.bss_size = vxModuleInfo.segInfo.bssSize;
      module_info->addr.flags = 0; 
   }

   return(OS_SUCCESS);
   
}/* end OS_ModuleInfo */

#endif
