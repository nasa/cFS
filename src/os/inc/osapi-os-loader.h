/*
** File: osapi-os-loader.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author:  Alan Cudmore - Code 582
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, Object file loader API
**
*/

#ifndef _osapi_loader_
#define _osapi_loader_

/*
** Defines
*/


/*
** Typedefs
*/

typedef struct
{
   uint32 valid;
   uint32 flags;  
   cpuaddr code_address;
   cpuaddr code_size;
   cpuaddr data_address;
   cpuaddr data_size;
   cpuaddr bss_address;
   cpuaddr bss_size;
} OS_module_address_t;

typedef struct
{
   cpuaddr             entry_point;
   cpuaddr             host_module_id;
   char                filename[OS_MAX_PATH_LEN];
   char                name[OS_MAX_API_NAME];
   OS_module_address_t addr;
} OS_module_prop_t;

/**
 * Associates a single symbol name with a memory address.
 *
 * If the OS_STATIC_SYMBOL_TABLE feature is enabled, then
 * an array of these structures should be provided by the
 * application.  When the application needs to find a symbol
 * address, the static table will be checked in addition
 * to (or instead of) the OS/library-provided lookup function.
 *
 * This static symbol allows systems that do not implement
 * dynamic module loading to maintain the same semantics
 * as dynamically loaded modules.
 */
typedef const struct
{
   const char *Name;
   void (*Address)(void);
   const char *Module;
} OS_static_symbol_record_t;


/*
 * Define the former "OS_module_record_t" type as equivalent
 * to the OS_module_prop_t.  This is what the OS_ModuleInfo()
 * will output.  It used to be the same as the internal record
 * just without all the fields filled in.  This has been changed
 * to make it a separate structure, which will allow the internal
 * implementation to change without further changing the API.
 *
 * Ideally OS_module_record_t type should be removed to avoid confusion,
 * but this would break existing code that calls OS_ModuleInfo().
 */
#ifndef OSAL_OMIT_DEPRECATED
typedef OS_module_prop_t OS_module_record_t;
#endif

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find the Address of a Symbol
 *
 * This calls to the OS dynamic symbol lookup implementation,
 * and/or checks a static symbol table for a matching symbol name.
 *
 * The static table is intended to support embedded targets that do
 * not have module loading capability or have it disabled.
 *
 * @param[out] symbol_address Set to the address of the symbol
 * @param[in]  symbol_name    Name of the symbol to look up
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if the symbol could not be found
 * OS_INVALID_POINTER if one of the pointers passed in are NULL
 */
int32 OS_SymbolLookup (cpuaddr *symbol_address, const char *symbol_name );

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Dumps the system symbol table to a file
 *
 * @param[in] filename  File to write to
 * @param[in] size_limit Maximum number of bytes to write
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED if the system does not support this function
 * OS_ERROR if the symbol table could not be read or dumped
 * OS_INVALID_FILE  if the file could not be opened or written
 */
int32 OS_SymbolTableDump ( const char *filename, uint32 size_limit );

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Loads an object file
 *
 * Loads an object file into the running operating system
 *
 * @param[out] module_id    OSAL ID corresponding to the loaded module
 * @param[in]  module_name  Name of module
 * @param[in]  filename     File containing the object code to load
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if the module cannot be loaded
 * OS_INVALID_POINTER if one of the parameters is NULL
 * OS_ERR_NO_FREE_IDS if the module table is full
 * OS_ERR_NAME_TAKEN if the name is in use
 */
int32 OS_ModuleLoad ( uint32 *module_id, const char *module_name, const char *filename );

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Unloads the module file
 *
 * Unloads the module file from the running operating system
 *
 * @param[in] module_id    OSAL ID of the previously the loaded module
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if the module is invalid or cannot be unloaded
 */
int32 OS_ModuleUnload ( uint32 module_id );

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about a module
 *
 * Returns information about the loadable module
 *
 * @param[in]  module_id    OSAL ID of the previously the loaded module
 * @param[out] module_info  Buffer to store module information
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the module id invalid
 * OS_INVALID_POINTER if the pointer to the ModuleInfo structure is invalid
 */
int32 OS_ModuleInfo ( uint32 module_id, OS_module_prop_t *module_info );


#endif
