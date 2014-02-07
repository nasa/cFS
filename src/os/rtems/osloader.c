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
**          Currently, we are using RTEMS with two different loaders: 
**          1. The CEXP loader 
**          2. An internally developed static module loader ( which should be open source soon ).
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#define _USING_RTEMS_INCLUDES_

#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>
#include <fcntl.h>

#include <rtems.h>
#include "common_types.h"
#include "osapi.h"

/*
** If OS_INCLUDE_MODULE_LOADER is not defined, then skip the module 
*/
#ifdef OS_INCLUDE_MODULE_LOADER

/*
** Select the static or dynamic loader
*/
#ifdef OS_STATIC_LOADER
   #include "loadstaticloadfile.h"
#else
   #include <cexp.h>
#endif

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
#define OS_SYMBOL_RECORD_SIZE sizeof(SymbolRecord_t)

#undef OS_DEBUG_PRINTF 

#define OSAL_TABLE_MUTEX_ATTRIBS \
 (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | \
  RTEMS_INHERIT_PRIORITY | RTEMS_NO_PRIORITY_CEILING | RTEMS_LOCAL)

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

/*
** Need to define the OS Module table here. 
** osconfig.h will have the maximum number of loadable modules defined.
*/
OS_module_record_t OS_module_table[OS_MAX_MODULES];

/*
** The Mutex for protecting the above table
*/
rtems_id     OS_module_table_sem;


#ifdef OS_STATIC_LOADER
   /*
   ** In addition to the module table, this is the static loader specific data.
   ** It is a mini symbol table with all of the information for the static loaded modules.
   */
   static_load_file_header_t OS_symbol_table[OS_MAX_MODULES];
#endif


/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

int32  OS_ModuleTableInit ( void )
{
   int               i;
   rtems_status_code rtems_sc; 

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

#ifdef OS_STATIC_LOADER   
   /*
   ** Initialize Static file headers table
   */
   memset(&(OS_symbol_table[0]),0,sizeof(static_load_file_header_t) * OS_MAX_MODULES);
#endif

   /*
   ** Create the Module Table mutex
   */
   rtems_sc = rtems_semaphore_create (rtems_build_name ('M', 'U', 'T', '7'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_module_table_sem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return(OS_ERROR);
   }
   
   return(OS_SUCCESS);
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
             OS_INVALID_POINTER if one of the pointers passed in are NULL 
---------------------------------------------------------------------------------------*/
int32 OS_SymbolLookup( uint32 *SymbolAddress, char *SymbolName )
{
#ifdef OS_STATIC_LOADER

   int               i;
   rtems_status_code status;

   /*
   ** Check parameters
   */
   if (( SymbolAddress == NULL ) || (SymbolName == NULL ))
   {
      return(OS_INVALID_POINTER);
   }

   /*
   ** Static load lookup. Since we are not maintaining a symbol table
   ** in a static linked implementation, only the entry point symbols are saved.
   */
   status = rtems_semaphore_obtain (OS_module_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    
   /*
   ** Lookup the symbol 
   */
   for (i = 0; i < OS_MAX_MODULES; i++ )
   {
       if ((OS_module_table[i].free == FALSE) &&
          ( strcmp((char*) SymbolName, OS_symbol_table[i].entry_point_name) == 0)) 
       {    
           *SymbolAddress = (uint32)OS_symbol_table[i].entry_point;   
           status = rtems_semaphore_release (OS_module_table_sem);
           return OS_SUCCESS;
       }
   }
   status = rtems_semaphore_release (OS_module_table_sem);
   return(OS_ERROR);

#else

   CexpSym     CexpSymbol;

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
   CexpSymbol = cexpSymLookup(SymbolName, NULL);
   if ( CexpSymbol == NULL )
   {
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: ERROR cexpSymLookup error when resolving address of %s\n",SymbolName);
      #endif
      return(OS_ERROR);
   }
   #ifdef OS_DEBUG_PRINTF
      OS_printf("OSAL: Symbol table lookup OK\n");
   #endif

   /*
   ** Return the code start address and module ID back to the code
   */
   *SymbolAddress = ( void *) cexpSymValue(CexpSymbol);
   
   return(OS_SUCCESS);

#endif
 
}/* end OS_SymbolLookup */

/*--------------------------------------------------------------------------------------
    Name: OS_SymbolTableDump
    
    Purpose: Dumps the system symbol table to a file

    Parameters: 

    Returns: OS_ERROR if the symbol table could not be read or dumped
             OS_FS_ERR_PATH_INVALID  if the file and/or path is invalid 
             OS_SUCCESS if the file is written correctly 
---------------------------------------------------------------------------------------*/
int32 OS_SymbolTableDump ( char *filename, uint32 SizeLimit )
{
#ifdef OS_STATIC_LOADER
   char            local_path_name[OS_MAX_LOCAL_PATH_LEN];
   int             sym_table_file_fd;
   int32           return_status;
   int             status;
   SymbolRecord_t  symRecord;
   int             i;

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
   ** Open file. "open" returns -1 on error. 
   */
   sym_table_file_fd = open(local_path_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
   if ( sym_table_file_fd  != -1  )
   {
      /*
      ** Iterate the symbol table
      */
      status = rtems_semaphore_obtain (OS_module_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

      /*
      ** Lookup the symbol
      */
      for (i = 0; i < OS_MAX_MODULES; i++ )
      {
          if (OS_module_table[i].free == FALSE) 
          {
             /*
             ** Copy symbol name
             */
             strncpy(symRecord.SymbolName, OS_symbol_table[i].entry_point_name, OS_MAX_SYM_LEN);

             /*
             ** Save symbol address
             */
             symRecord.SymbolAddress = (uint32)OS_symbol_table[i].entry_point;

             /*
             ** Write entry in file
             */
             status = write(sym_table_file_fd, &symRecord, sizeof(symRecord));
             if ( status == -1 )
             {
                status = rtems_semaphore_release (OS_module_table_sem);
                close(sym_table_file_fd);
                return(OS_ERROR);
             }
          } /* end if */
      } /* end for */
      status = rtems_semaphore_release (OS_module_table_sem);
      return_status = OS_SUCCESS;
   }
   else
   {
      return_status = OS_ERROR;
   }

   close(sym_table_file_fd);

   return(return_status);

#else
   /*
   ** Need to do a CEXP implementation
   ** But CEXP support will be removed when RTEMS
   ** gets a native dynamic load API
   */
   return(OS_ERR_NOT_IMPLEMENTED);
#endif

}/* end OS_SymbolTableDump */

/****************************************************************************************
                                    Module Loader API
****************************************************************************************/

/*--------------------------------------------------------------------------------------
    Name: OS_ModuleLoad
    
    Purpose: Loads an ELF object file into the running operating system

    Parameters: 

    Returns: OS_ERROR if the module cannot be loaded
             OS_INVALID_POINTER if one of the parameters is NULL
             OS_ERR_NO_FREE_IDS if the module table is full
             OS_ERR_NAME_TAKEN if the name is in use
             OS_SUCCESS if the module is loaded successfuly 
---------------------------------------------------------------------------------------*/
int32 OS_ModuleLoad ( uint32 *module_id, char *module_name, char *filename )
{
   int                       i;
   uint32                    possible_moduleid;
   char                      translated_path[OS_MAX_LOCAL_PATH_LEN];
   int32                     return_code;
   rtems_status_code         status;

#ifdef OS_STATIC_LOADER
   boolean                   StaticLoadStatus;
#else
   CexpModule                CexpModuleId;
#endif
 
   /*
   ** Check parameters
   */
   if (( filename == NULL ) || (module_id == NULL ) || ( module_name == NULL))
   {
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: Error, invalid parameters to OS_ModuleLoad\n");
      #endif
      return(OS_INVALID_POINTER);
   }
  
   status = rtems_semaphore_obtain (OS_module_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

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
   if( possible_moduleid >= OS_MAX_MODULES )
   {
       status = rtems_semaphore_release (OS_module_table_sem);
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
           status = rtems_semaphore_release (OS_module_table_sem);
           return OS_ERR_NAME_TAKEN;
       }
   }

   /* 
   ** Set the possible Module ID to "not free" so that
   ** another caller will not try to use it. 
   */
   OS_module_table[possible_moduleid].free = FALSE ;
   status = rtems_semaphore_release (OS_module_table_sem);
 
   /*
   ** Translate the filename to the Host System
   */
   return_code = OS_TranslatePath((const char *)filename, (char *)translated_path); 
   if ( return_code != OS_SUCCESS )
   {
      OS_module_table[possible_moduleid].free = TRUE ;
      return(return_code);
   }

#ifdef OS_STATIC_LOADER
  
   /*
   ** Load the module
   */
   StaticLoadStatus = LoadStaticLoadFile(translated_path, &OS_symbol_table[possible_moduleid]);
   if ( StaticLoadStatus == FALSE )
   {
      OS_module_table[possible_moduleid].free = TRUE ;
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: Error, cannot load static module: %s\n",translated_path);
      #endif
      return(OS_ERROR);
   }
   #ifdef OS_DEBUG_PRINTF
       OS_printf("OSAL: Loaded Module OK.\n");
   #endif 
 
#else  

   /*
   ** Load the module
   */
   CexpModuleId = cexpModuleLoad(translated_path, module_name);   
   if ( CexpModuleId == 0 )
   {
      OS_module_table[possible_moduleid].free = TRUE ;
      #ifdef OS_DEBUG_PRINTF
         OS_printf("OSAL: Error, cannot load module: %s\n",translated_path);
      #endif
      return(OS_ERROR);
   }
   #ifdef OS_DEBUG_PRINTF
       OS_printf("OSAL: Loaded Module OK.\n");
   #endif 

 #endif


   /*
   ** fill out the OS_module_table entry for this new module
   */
#ifdef OS_STATIC_LOADER
   OS_module_table[possible_moduleid].entry_point = OS_symbol_table[possible_moduleid].entry_point;
   OS_module_table[possible_moduleid].host_module_id = possible_moduleid;
#else
   OS_module_table[possible_moduleid].entry_point = 0; /* Only for certain targets */
   OS_module_table[possible_moduleid].host_module_id = (uint32) CexpModuleId;
#endif

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
#ifndef OS_STATIC_LOADER
   int CexpUnloadStatus;
#endif

   /*
   ** Check the module_id
   */
   if ( module_id >= OS_MAX_MODULES || OS_module_table[module_id].free == TRUE )
   {
      return(OS_ERR_INVALID_ID);
   }

#ifndef OS_STATIC_LOADER
   CexpUnloadStatus = cexpModuleUnload((CexpModule )OS_module_table[module_id].host_module_id);
   if ( CexpUnloadStatus != 0 )   
   {
      #ifdef OS_DEBUG_PRINTF
         OS_printf("BSP: Error, Cannot Close/Unload application file: %d\n",CexpUnloadStatus);
      #endif
      return(OS_ERROR);
   }
   #ifdef OS_DEBUG_PRINTF
      OS_printf("OSAL: Unloaded module OK\n");
   #endif 

#endif
   /*
   ** Free the module entry
   */
   OS_module_table[module_id].free = TRUE; 

   return(OS_SUCCESS);
   
}/* end OS_ModuleUnload */

/*--------------------------------------------------------------------------------------
    Name: OS_ModuleInfo
    
    Purpose: Returns information about the loadable module

    Parameters: 

    Returns: OS_INVALID_POINTER if the pointer to the ModuleInfo structure is NULL 
             OS_ERR_INVALID_ID if the module ID is not valid
             OS_SUCCESS if the module info was filled out successfuly 
---------------------------------------------------------------------------------------*/
int32 OS_ModuleInfo ( uint32 module_id, OS_module_record_t *module_info )
{

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

#ifdef OS_STATIC_LOADER
      module_info->addr.valid = TRUE;
      module_info->addr.code_address = OS_symbol_table[module_id].code_target;
      module_info->addr.code_size = OS_symbol_table[module_id].code_size;
      module_info->addr.data_address = OS_symbol_table[module_id].data_target;
      module_info->addr.data_size = OS_symbol_table[module_id].data_size;
      module_info->addr.bss_address = OS_symbol_table[module_id].bss_target;
      module_info->addr.bss_size = OS_symbol_table[module_id].bss_size;
      module_info->addr.flags = OS_symbol_table[module_id].flags;
#else /* CEXP info */
      module_info->addr.valid = FALSE;
      module_info->addr.code_address = 0;
      module_info->addr.code_size = 0;
      module_info->addr.data_address = 0;
      module_info->addr.data_size = 0;
      module_info->addr.bss_address = 0;
      module_info->addr.bss_size = 0;
      module_info->addr.flags = 0;
#endif
   
   return(OS_SUCCESS);

}/* end OS_ModuleInfo */

#endif /* OS_INCLUDE_MODULE_LOADER */
