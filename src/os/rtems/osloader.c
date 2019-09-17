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
 * \file   osloader.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains the module loader and symbol lookup functions for the OSAL.
 *          RTEMS uses the POSIX-style "dl" implementation (even if the rest of the POSIX API
 *          is disabled).
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#define _USING_RTEMS_INCLUDES_

#include "os-rtems.h"
#include <dlfcn.h>

#include <rtems/rtl/rtl.h>
#include <rtems/rtl/rtl-unresolved.h>


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
                                INITIALIZATION FUNCTION
 ***************************************************************************************/
                        
/*----------------------------------------------------------------
 *
 * Function: OS_Rtems_ModuleAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_ModuleAPI_Impl_Init(void)
{
#if (OS_MAX_MODULES > 0)
   memset(OS_impl_module_table, 0, sizeof(OS_impl_module_table));
#endif
   return(OS_SUCCESS);
} /* end OS_Rtems_ModuleAPI_Impl_Init */

/****************************************************************************************
                                    Symbol table API
 ***************************************************************************************/
                        
/*----------------------------------------------------------------
 *
 * Function: OS_SymbolLookup_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolLookup_Impl( cpuaddr *SymbolAddress, const char *SymbolName )
{
    int32        status;
    const char   *dlError;
    void         *Function;

    /*
     * call dlerror() to clear any prior error that might have occured.
     */
    dlerror();
    Function = dlsym(RTLD_DEFAULT, SymbolName);
    dlError = dlerror();

    /*
     * For the POSIX DL implementation, if the symbol does not exist
     * then dlerror() is supposed to return non-null.  This is intended
     * to differentiate between valid symbols which are actually 0/NULL
     * and invalid symbols that do not exist.
     *
     * RTEMS does _not_ seem to implement this detail, and dlerror()
     * still returns NULL after looking up an invalid symbol name.
     *
     * In practice, all valid symbols should be non-NULL anyway,
     * so we check both here.
     */

    if( dlError == NULL && Function != NULL )
    {
        /* considered successful if no error is present -
         * note the address itself is allowed to be NULL */
        *SymbolAddress = (cpuaddr)Function;
        status = OS_SUCCESS;
    }
    else
    {
        *SymbolAddress = 0;
        status = OS_ERROR;
    }

    return status;

} /* end OS_SymbolLookup_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl ( const char *filename, uint32 SizeLimit )
{

   return(OS_ERR_NOT_IMPLEMENTED);

} /* end OS_SymbolTableDump_Impl */



/****************************************************************************************
                                    Module Loader API
 ***************************************************************************************/

/*
 * NOTE: The module loading functions are only compiled of OS_MAX_MODULES is >0
 * Otherwise stubs will be compiled in place of these (see below)
 */

#if (OS_MAX_MODULES > 0) && defined(OS_INCLUDE_MODULE_LOADER)

/****************************************************************************************
                                HELPER ROUTINES
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_rtems_rtl_check_unresolved
 *
 *  Purpose: Local helper routine, not part of OSAL API.

 * This callback checks symbols in the RTL unresolved record list
 * NOTE: Right now this considers any unresolved items to be a failure.
 * This could be fine-tuned later.
 *
 *-----------------------------------------------------------------*/
static bool OS_rtems_rtl_check_unresolved (rtems_rtl_unresolv_rec_t* rec,
        void*                     data)
{
    int32 *status = data;

    switch(rec->type)
    {
    case rtems_rtl_unresolved_name:
        OS_DEBUG("unresolved name: %s\n", rec->rec.name.name);
        *status = OS_ERROR;
        break;
    case rtems_rtl_unresolved_reloc:
        *status = OS_ERROR;
        break;
    default:
        break;
    }
    return false;
} /* end OS_rtems_rtl_check_unresolved */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl ( uint32 module_id, char *translated_path )
{
    int32 status = OS_ERROR;
    int unresolved;
    void *dl_handle;

    dlerror();
    dl_handle = dlopen(translated_path, RTLD_NOW | RTLD_GLOBAL);
    if (dl_handle == NULL)
    {
        OS_DEBUG("Error loading shared library: %s\n", dlerror());
        status = OS_ERROR;
    }
    else if (dlinfo (dl_handle, RTLD_DI_UNRESOLVED, &unresolved) < 0)
    {
        /* should never happen */
        OS_DEBUG("dlinfo error checking unresolved status\n");
        status = OS_ERROR;
    }
    else if (unresolved)
    {
        /*
         * Note that RTEMS is a little different than traditional POSIX
         * in that even with RTLD_NOW specified, dlopen() will still return
         * success if there are unresolved symbols.  RTEMS has implemented
         * it this way to allow cross/circular dependencies.
         *
         * However, OSAL applications are not anticipated to have circular
         * dependencies such as this.  So this explicitly checks if unresolved
         * symbols are present after loading, and if so, do additional checks.
         *
         * It is possible that not every unresolved situation indicates failure.
         * The "check_unresolved" helper should verify if the condition
         * is acceptable.  If not acceptable, it sets the status back to an error.
         */

        OS_DEBUG("module has has unresolved externals\n");
        status = OS_SUCCESS; /* note - not final, probably overridden */
        rtems_rtl_unresolved_interate (OS_rtems_rtl_check_unresolved, &status);
    }
    else
    {
        status = OS_SUCCESS;
    }


    if (status == OS_SUCCESS)
    {
        /* success: save for future use */
        OS_impl_module_table[module_id].dl_handle = dl_handle;
    }
    else if (dl_handle != NULL)
    {
        /*
         * When returning non-success based on one of the
         * subsequent verifications, also dlclose() the handle
         * to avoid leaking resources.
         */
        dlclose(dl_handle);
        dl_handle = NULL;
    }

    return status;

} /* end OS_ModuleLoad_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleUnload_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl ( uint32 module_id )
{
    int32 status = OS_ERROR;

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

    return status;

} /* end OS_ModuleUnload_Impl */




#else

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl ( uint32 module_id, char *translated_path )
{
    return OS_SUCCESS;
} /* end OS_ModuleLoad_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleUnload_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl ( uint32 module_id )
{
    return OS_SUCCESS;
} /* end OS_ModuleUnload_Impl */

#endif

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl ( uint32 module_id, OS_module_prop_t *module_prop )
{
   /*
   ** RTEMS does not specify a way to get these values
   ** Everything left at zero
   */
   return(OS_SUCCESS);

} /* end OS_ModuleGetInfo_Impl */


