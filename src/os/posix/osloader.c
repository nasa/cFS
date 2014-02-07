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

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

#include "common_types.h"
#include "osapi.h"

#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <pthread.h>

#include <dlfcn.h>

/*
** If OS_INCLUDE_MODULE_LOADER is not defined, skip the whole module
*/
#ifdef OS_INCLUDE_MODULE_LOADER

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                     FUNCTION PROTOTYPES 
****************************************************************************************/
extern int    OS_InterruptSafeLock(pthread_mutex_t *lock, sigset_t *set, sigset_t *previous);
extern void   OS_InterruptSafeUnlock(pthread_mutex_t *lock, sigset_t *previous);

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
pthread_mutex_t    OS_module_table_mut;

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/
int32  OS_ModuleTableInit ( void )
{
   int i;
   int return_code;
   
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
   return_code = pthread_mutex_init((pthread_mutex_t *) & OS_module_table_mut,NULL); 
   if ( return_code == 0)
   {
      return(OS_SUCCESS);
   }
   else 
   {
      return(OS_ERROR);
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
   const char   *dlError;           /*  Pointer to error string   */
   uint32        Function;

   /*
   ** Check parameters
   */
   if (( SymbolAddress == NULL ) || ( SymbolName == NULL ))
   {
      return(OS_INVALID_POINTER);
   }

   /*
   ** Lookup the entry point
   */
   Function = (uint32 )dlsym((void *)0, SymbolName);
   dlError = dlerror();
   if( dlError )
   {
      return(OS_ERROR);
   }

   *SymbolAddress = Function;
   
   return(OS_SUCCESS);
    
}/* end OS_SymbolLookup */

/*--------------------------------------------------------------------------------------
    Name: OS_SymbolTableDump
    
    Purpose: Dumps the system symbol table to a file

    Parameters: 

    Returns: OS_ERROR if the symbol table could not be read or dumped
             OS_INVALID_FILE  if the file could not be opened or written
             OS_SUCCESS if the symbol is found 
---------------------------------------------------------------------------------------*/
int32 OS_SymbolTableDump ( char *filename, uint32 SizeLimit )
{

   return(OS_ERR_NOT_IMPLEMENTED);
   
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
   void       *function_lib;     /*  Handle to shared lib file */
   const char *dl_error;    /*  Pointer to error string   */
   sigset_t    previous;
   sigset_t    mask;

   /*
   ** Check parameters
   */
   if (( filename == NULL ) || (module_id == NULL ) || (module_name == NULL))
   {
      return(OS_INVALID_POINTER);
   }
 
   OS_InterruptSafeLock(&OS_module_table_mut, &mask, &previous); 

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
       OS_InterruptSafeUnlock(&OS_module_table_mut, &previous); 
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
           OS_InterruptSafeUnlock(&OS_module_table_mut, &previous); 
           return OS_ERR_NAME_TAKEN;
       }
   }

   /* 
   ** Set the possible task Id to not free so that
   ** no other task can try to use it 
   */
   OS_module_table[possible_moduleid].free = FALSE ;
   OS_InterruptSafeUnlock(&OS_module_table_mut, &previous); 
 
   /*
   ** Translate the filename to the Host System
   */     
   return_code = OS_TranslatePath((const char *)filename, (char *)translated_path); 
   if ( return_code != OS_SUCCESS )
   {
      OS_module_table[possible_moduleid].free = TRUE;
      return(return_code);
   }

   /*
   ** File is ready to load
   */
   
   /* 
   ** Open the loadble bundle .. just opening it loads it into the system.
   */
   function_lib = dlopen(translated_path, RTLD_LAZY | RTLD_GLOBAL);
   dl_error = dlerror();
   if( dl_error )
   {
      OS_module_table[possible_moduleid].free = TRUE;
      return(OS_ERROR);
   }

   /*
   ** fill out the OS_module_table entry for this new module
   */
   OS_module_table[possible_moduleid].entry_point = 0; /* Only for certain targets */
   OS_module_table[possible_moduleid].host_module_id = (uint32) function_lib;
   strncpy(OS_module_table[possible_moduleid].filename , filename, OS_MAX_PATH_LEN);
   strncpy(OS_module_table[possible_moduleid].name , module_name, OS_MAX_API_NAME);
 
   /*
   ** For now, do not store the module address information
   ** Let the OS_ModuleInfo function fetch that information and return it.
   */
   OS_module_table[possible_moduleid].addr.valid = FALSE;
 
   /*
   ** Return the OSAPI Module ID
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

   int         ReturnCode;
   const char *dlError;   

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
   ReturnCode = dlclose((void *)OS_module_table[module_id].host_module_id);
   dlError = dlerror();
   if( dlError )
   {
      OS_module_table[module_id].free = TRUE;  
      return(OS_ERROR);
   }
   OS_module_table[module_id].free = TRUE;
 
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

   /*
   ** Check the parameter
   */
   if ( module_info == 0 )
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
   ** Address info is currently not valid on Linux 
   */
   module_info->addr.valid = FALSE;
   module_info->addr.code_address = 0;
   module_info->addr.code_size = 0;
   module_info->addr.data_address = 0;
   module_info->addr.data_size = 0;
   module_info->addr.bss_address = 0;
   module_info->addr.bss_size = 0;
   module_info->addr.flags = 0;

   return(OS_SUCCESS);
 
}/* end OS_ModuleInfo */

#endif
