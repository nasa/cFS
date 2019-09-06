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

/*
** Loader API
*/
int32 OS_ModuleTableInit ( void );

int32 OS_SymbolLookup (cpuaddr *symbol_address, const char *symbol_name );

int32 OS_SymbolTableDump ( const char *filename, uint32 size_limit );

int32 OS_ModuleLoad ( uint32 *module_id, const char *module_name, const char *filename );

int32 OS_ModuleUnload ( uint32 module_id );

int32 OS_ModuleInfo ( uint32 module_id, OS_module_prop_t *module_info );


#endif
