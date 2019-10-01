/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osapi-module.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined
 *
 * OS_MAX_MODULES is allowed to be zero in which case the
 * table is not instantiated.
 */
#if !defined(OS_MAX_MODULES) || (OS_MAX_MODULES < 0)
#error "osconfig.h must define OS_MAX_ to a valid value"
#endif


enum
{
   LOCAL_NUM_OBJECTS = OS_MAX_MODULES,
   LOCAL_OBJID_TYPE = OS_OBJECT_TYPE_OS_MODULE
};

/*
 * Only instantiate the "OS_module_table" if
 * OS_MAX_MODULES configuration is greater than 0.
 * It is allowed to be zero on statically linked apps.
 */
#if (OS_MAX_MODULES > 0)
OS_module_internal_record_t OS_module_table[OS_MAX_MODULES];
#endif

/*
 * If the "OS_STATIC_LOADER" directive is enabled,
 * then the user application/BSP must provide a symbol
 * called "OS_STATIC_SYMBOL_TABLE" which will provide
 * user-defined mappings of symbol names to addresses.
 * 
 * Note - when compiling unit tests, the UT code will
 * supply a custom definition for OS_STATIC_SYMTABLE_SOURCE
 */
#if !defined(OS_STATIC_SYMTABLE_SOURCE) && defined(OS_STATIC_LOADER)
/* use default symbol name for static table */
#define OS_STATIC_SYMTABLE_SOURCE   OS_STATIC_SYMBOL_TABLE
#endif

#ifdef OS_STATIC_SYMTABLE_SOURCE
/* the BSP should supply the static symbol table when this is set */
extern OS_static_symbol_record_t OS_STATIC_SYMTABLE_SOURCE[];
#else
/* there is no static symbol table, use NULL */
#define OS_STATIC_SYMTABLE_SOURCE   NULL
#endif /* OS_STATIC_SYMTABLE_SOURCE */

/*----------------------------------------------------------------
 *
 * Function: OS_SymbolLookup_Static
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Checks for a symbol name in the static symbol table
 *
 *-----------------------------------------------------------------*/
static int32 OS_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName)
{
    int32 return_code = OS_ERR_NOT_IMPLEMENTED;
    OS_static_symbol_record_t *StaticSym = OS_STATIC_SYMTABLE_SOURCE;

    while (StaticSym != NULL)
    {
        if (StaticSym->Name == NULL)
        {
            /* end of list --
             * Return "OS_ERROR" to indicate that an actual search was done
             * with a not-found result, vs. not searching at all. */
            return_code = OS_ERROR;
            break;
        }
        if (strcmp(StaticSym->Name, SymbolName) == 0)
        {
            /* found matching symbol */
            *SymbolAddress = (cpuaddr)StaticSym->Address;
            return_code = OS_SUCCESS;
            break;
        }

        ++StaticSym;
    }

    return return_code;
} /* end OS_SymbolLookup_Static */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Static
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Checks for a module name in the static symbol table
 *
 *-----------------------------------------------------------------*/
static int32 OS_ModuleLoad_Static(const char *ModuleName)
{
    int32 return_code = OS_ERR_NAME_NOT_FOUND;
    OS_static_symbol_record_t *StaticSym = OS_STATIC_SYMTABLE_SOURCE;

    while (StaticSym != NULL)
    {
        if (StaticSym->Name == NULL)
        {
            /* end of list  */
            break;
        }
        if (StaticSym->Module != NULL &&
                strcmp(StaticSym->Module, ModuleName) == 0)
        {
            /* found matching module name */
            return_code = OS_SUCCESS;
            break;
        }

        ++StaticSym;
    }

    return return_code;
} /* end OS_ModuleLoad_Static */

/****************************************************************************************
                                   Module API
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_ModuleAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleAPI_Init(void)
{
#if (OS_MAX_MODULES > 0)
   memset(OS_module_table, 0, sizeof(OS_module_table));
#endif
   return OS_SUCCESS;
} /* end OS_ModuleAPI_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad ( uint32 *module_id, const char *module_name, const char *filename )
{
    char        translated_path[OS_MAX_LOCAL_PATH_LEN];
    int32       return_code;
    uint32      local_id;
    OS_common_record_t *record;

    /*
     ** Check parameters
     */
    if (( filename == NULL ) || (module_id == NULL ) || (module_name == NULL))
    {
        return(OS_INVALID_POINTER);
    }

    if (strlen(module_name) >= OS_MAX_API_NAME)
    {
        return(OS_ERR_NAME_TOO_LONG);
    }

    /*
     * Check the statically-linked module list.
     * If a matching entry is found, this means its
     * already effectively "loaded" through static linkage.
     * Return success without any more action.
     */
    return_code = OS_ModuleLoad_Static(module_name);
    if (return_code == OS_SUCCESS)
    {
        *module_id = 0;
        return OS_SUCCESS;
    }

    /*
     ** Translate the filename to the Host System
     */
    return_code = OS_TranslatePath(filename, translated_path);
    if ( return_code != OS_SUCCESS )
    {
        return(return_code);
    }

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, module_name, &local_id, &record);
    if(return_code == OS_SUCCESS)
    {
        /*
         * Save all the data to our own internal module table, but
         * only if OS_MAX_MODULES is greater than 0
         */
#if (OS_MAX_MODULES > 0)
        memset(&OS_module_table[local_id], 0, sizeof(OS_module_internal_record_t));
        strncpy(OS_module_table[local_id].module_name, module_name, OS_MAX_API_NAME);
        strncpy(OS_module_table[local_id].file_name, filename, OS_MAX_PATH_LEN);
        record->name_entry = OS_module_table[local_id].module_name;

        /* Now call the OS-specific implementation.  This reads info from the module table. */
        return_code = OS_ModuleLoad_Impl(local_id, translated_path);
#endif

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, record, module_id);
    }

    return(return_code);

} /* end OS_ModuleLoad */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleUnload
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload ( uint32 module_id )
{
    OS_common_record_t *record;
    int32             return_code;
    uint32            local_id;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE,LOCAL_OBJID_TYPE, module_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
        /*
         * Only call the implementation if the loader is enabled
         */
        return_code = OS_ModuleUnload_Impl(local_id);

        if (return_code == OS_SUCCESS)
        {
            /* Clear the ID to zero */
            record->active_id = 0;
        }

        /* Unlock the global from OS_ObjectIdGetAndLock() */
        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }

    return return_code;
} /* end OS_ModuleUnload */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleInfo ( uint32 module_id, OS_module_prop_t *module_prop )
{
    OS_common_record_t *record;
    int32             return_code;
    uint32            local_id;

    /* Check parameters */
    if (module_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }

    memset(module_prop,0,sizeof(OS_module_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL,LOCAL_OBJID_TYPE, module_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {

#if (OS_MAX_MODULES > 0)
        strncpy(module_prop->name, record->name_entry, OS_MAX_API_NAME - 1);
        strncpy(module_prop->filename, OS_module_table[local_id].file_name, OS_MAX_API_NAME - 1);
        /* module_prop->creator = record->creator; */

        return_code = OS_ModuleGetInfo_Impl(local_id, module_prop);
#endif

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }

    return return_code;

} /* end OS_ModuleInfo */


/*----------------------------------------------------------------
 *
 * Function: OS_SymbolLookup
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolLookup( cpuaddr *SymbolAddress, const char *SymbolName )
{
    int32 return_code;
    int32 status;

    /*
    ** Check parameters
    */
    if (( SymbolAddress == NULL ) || (SymbolName == NULL ))
    {
        return(OS_INVALID_POINTER);
    }

    /*
     * if the module loader is included, then call the
     * OS symbol lookup implementation function first.
     */
    return_code = OS_SymbolLookup_Impl(SymbolAddress, SymbolName);

    /*
     * If the OS call did not find the symbol or the loader is
     * disabled, then check if a static symbol table is present
     */
    if (return_code != OS_SUCCESS)
    {
        status = OS_SymbolLookup_Static(SymbolAddress, SymbolName);

        /*
         * NOTE:
         * The OS_ERR_NOT_IMPLEMENTED code should only be returned
         * if _neither_ the SymbolLookup_Impl _nor_ the static table
         * lookup capabilities are implemented.
         *
         * If either of these are implemented then the returned
         * value should be OS_ERROR for a not-found result.
         */
        if (status == OS_SUCCESS ||
                return_code == OS_ERR_NOT_IMPLEMENTED)
        {
            return_code = status;
        }
    }

    return (return_code);

} /* end OS_SymbolLookup */

/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump ( const char *filename, uint32 SizeLimit )
{
    int32 return_code;
    char translated_path[OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check parameters
    */
    if (filename == NULL)
    {
       return(OS_INVALID_POINTER);
    }

    /*
     ** Translate the filename to the Host System
     */
    return_code = OS_TranslatePath(filename, translated_path);
    if ( return_code != OS_SUCCESS )
    {
        return(return_code);
    }


    /*
     * Locking the global ensures only one symbol table dump
     * can be executing.  It also prevents module loading/unloading
     * while the dump is occurring.
     *
     * Because calls to this function are serialized, the
     * underlying implementation may safely use globals for
     * state storage.
     */
    OS_Lock_Global_Impl(LOCAL_OBJID_TYPE);

    return_code = OS_SymbolTableDump_Impl(translated_path, SizeLimit);

    OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);

    return(return_code);

} /* end OS_SymbolTableDump */



