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
 * \file   os-impl-posix-dl.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains a module loader implementation for systems
 *      that implement a POSIX-style dynamic module loader.  This includes
 *      RTEMS even if built without its native POSIX API.
 *
 * NOTE: This is a "template" file and not a directly usable source file.
 *       It must be adapted/instantiated from within the OS-specific
 *       implementation on platforms that wish to use this template.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/* Handled by includer -- typically this must include dlfcn.h and whatever else is required
 * to get the prototypes of these functions:
 *
 *   dlopen()
 *   dlclose()
 *   dlsym()
 */

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/***************************************************************************************
                                 FUNCTION PROTOTYPES
 **************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/*
 * A local lookup table for posix-specific information.
 * This is not directly visible to the outside world.
 */
typedef struct
{
    void *dl_handle;
} OS_impl_module_internal_record_t;

/*
 * The storage table is only instantiated when OS_MAX_MODULES is nonzero.
 * It is allowed to be zero to save memory in statically linked apps.
 * However even in that case it is still relevant to include the
 * OS_SymbolLookup_Impl() function for symbol lookups.
 *
 * If neither loading nor symbol lookups are desired then this file
 * shouldn't be used at all -- a no-op version should be used instead.
 */
#if (OS_MAX_MODULES > 0)
OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];
#endif

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                     FUNCTION PROTOTYPES
 ***************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/



/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/
                        
/*----------------------------------------------------------------
 *
 * Function: OS_Posix_ModuleAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Posix_ModuleAPI_Impl_Init(void)
{
#if (OS_MAX_MODULES > 0)
   memset(OS_impl_module_table, 0, sizeof(OS_impl_module_table));
#endif
   return(OS_SUCCESS);
} /* end OS_Posix_ModuleAPI_Impl_Init */

/****************************************************************************************
                                    Symbol table API
 ***************************************************************************************/
                        
/*----------------------------------------------------------------
 *
 * Function: OS_SymbolLookup_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolLookup_Impl( cpuaddr *SymbolAddress, const char *SymbolName )
{
    int32        status = OS_ERROR;
    const char   *dlError;           /*  Pointer to error string   */
    void         *Function;

    /*
     * call dlerror() to clear any prior error that might have occured.
     */
    dlerror();
    Function = dlsym((void *)0, SymbolName);
    dlError = dlerror();
    if( dlError == NULL )
    {
        /* considered successful if no error is present -
         * note the address itself is allowed to be NULL */
        *SymbolAddress = (cpuaddr)Function;
        status = OS_SUCCESS;
    }

    return status;

} /* end OS_SymbolLookup_Impl */


/****************************************************************************************
                                    Module Loader API
 ***************************************************************************************/

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl ( uint32 module_id, char *translated_path )
{
    int32 status = OS_ERROR;

#if (OS_MAX_MODULES > 0)
   dlerror();
   OS_impl_module_table[module_id].dl_handle = dlopen(translated_path, RTLD_NOW | RTLD_GLOBAL);
   if (OS_impl_module_table[module_id].dl_handle != NULL)
   {
       status = OS_SUCCESS;
   }
   else
   {
       OS_DEBUG("Error loading shared library: %s\n", dlerror());
   }
#endif

   return status;

} /* end OS_ModuleLoad_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleUnload_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl ( uint32 module_id )
{
    int32 status = OS_ERROR;

#if (OS_MAX_MODULES > 0)
    /*
    ** Attempt to close/unload the module
    */
    dlerror();
    if (dlclose(OS_impl_module_table[module_id].dl_handle) == 0)
    {
        OS_impl_module_table[module_id].dl_handle = NULL;
        status = OS_SUCCESS;
    }
    else
    {
        OS_DEBUG("Error unloading shared library: %s\n", dlerror());
    }
#endif

    return status;

} /* end OS_ModuleUnload_Impl */




