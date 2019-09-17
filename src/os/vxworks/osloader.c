/*
** File   : osloader.c
**
**      Copyright (c) 2004-2015, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved. This software was created at NASA Glenn
**      Research Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Joe Hickey based on original RTEMS implementation by Alan Cudmore
**
** Purpose: This file contains the module loader and symbol lookup functions for the OSAL.
**          Currently this relies on the CEXP external module loader.  A "static" loader
**          is not yet implemented as that code is not open source yet.
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include <stdio.h>
#include <string.h> /* memset() */
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <errnoLib.h>
#include <sysLib.h>
#include <moduleLib.h>
#include <symLib.h>
#include <loadLib.h>
#include <unldLib.h>

/*
 * Vxworks 6.9 has stricter function pointer types
 * These are for the symEach() callback
 * they are supplied in 6.9 but not defined in 6.4
 */
#if defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR > 6 || _WRS_VXWORKS_MINOR >= 9)
/* no cast to FUNCPTR - use real type checking */
#define SYMEACH_FUNC
#else /* before 6.9 */
/* in prior versions the symEach `callback is cast to FUNCPTR */
#define SYMEACH_FUNC  (FUNCPTR)
typedef int         _Vx_usr_arg_t;
typedef UINT16      SYM_GROUP;
typedef char *      SYM_VALUE;
#endif

typedef struct
{
    char     SymbolName[OS_MAX_SYM_LEN];
    cpuaddr  SymbolAddress;
} SymbolRecord_t;

typedef struct
{
    uint32  Sizelimit;
    uint32  CurrSize;
    int32   StatusCode;
    int     fd;
} SymbolDumpState_t;


/* A global for storing the state in a SymbolDump call */
static struct
{
    SymbolDumpState_t sym_dump;
#if (OS_MAX_MODULES > 0)
    MODULE_ID vxid_table[OS_MAX_MODULES];
#endif
} OS_impl_module_global;

/* the system symbol table */
extern SYMTAB_ID sysSymTbl;

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/
                        
/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_ModuleAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_ModuleAPI_Impl_Init(void)
{
   memset(&OS_impl_module_global, 0, sizeof(OS_impl_module_global));
   return(OS_SUCCESS);
} /* end OS_VxWorks_ModuleAPI_Impl_Init */


                        
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
   STATUS    vxStatus;

   /*
   ** Check parameters
   */
   if (( SymbolAddress == NULL ) || (SymbolName == NULL ))
   {
      return(OS_INVALID_POINTER);
   }

   /*
   ** Lookup the entry point
   **
   ** VxWorks 6.9 has deprecated the "symFindByName" API and it is replaced
   ** with a "symFind" API instead. 
   */

#if defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR > 6 || _WRS_VXWORKS_MINOR >= 9)
   {
      SYMBOL_DESC SymDesc;

      memset (&SymDesc, 0, sizeof (SYMBOL_DESC));
      SymDesc.mask = SYM_FIND_BY_NAME;
      SymDesc.name = (char*)SymbolName;

      vxStatus = symFind(sysSymTbl,&SymDesc);
      *SymbolAddress = (cpuaddr)SymDesc.value;
   }
#else /* Before VxWorks 6.9 */
   {
      SYM_TYPE  SymType;
      SYM_VALUE SymValue;

      vxStatus = symFindByName (sysSymTbl, (char *)SymbolName, &SymValue, &SymType);
      *SymbolAddress = (cpuaddr)SymValue;
   }
#endif

   if (vxStatus == ERROR)
   {
      return(OS_ERROR);
   }


   return(OS_SUCCESS);

} /* end OS_SymbolLookup_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_SymTableIterator_Impl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Function called by vxWorks to iterate the vxworks symbol table
 *
 * Parameters:
 *           name - The symbol name
 *           val  - The symbol address value
 *           type - The vxWorks symbol type ( not used )
 *           max_size - The maximum size of the file that is written to.
 *           group - The vxWorks symbol group ( not used )
 *
 *  Returns: true to tell vxWorks to continue to iterate the symbol table
 *           false to tell vxWorks to stop iterating the symbol table
 *
 *           The address of the symbol will be stored in the pointer that is passed in.
 *
 *-----------------------------------------------------------------*/
static BOOL  OS_SymTableIterator_Impl ( char *name, SYM_VALUE val,  SYM_TYPE type,  _Vx_usr_arg_t arg, SYM_GROUP group )
{
   SymbolRecord_t symRecord;
   uint32         NextSize;
   int            status;
   SymbolDumpState_t *state;

   /*
    * Rather than passing the state pointer through the generic "int" arg,
    * use a global.  This is OK because dumps are serialized externally.
    */
   state = &OS_impl_module_global.sym_dump;

   if (strlen(name) >= OS_MAX_SYM_LEN)
   {
       OS_DEBUG("%s(): symbol name too long\n", __func__);
       state->StatusCode = OS_ERROR;
       return(false);
   }

   /*
   ** Check to see if the maximum size of the file has been reached
   */
   NextSize = state->CurrSize + sizeof(symRecord);
   if ( NextSize > state->Sizelimit )
   {
       /*
       ** We exceeded the maximum size, so tell vxWorks to stop
       ** However this is not considered an error, just a stop condition.
       */
       OS_DEBUG("%s(): symbol table size exceeded\n", __func__);
       return(false);
   }

   /*
   ** Copy symbol name
   */
   strncpy(symRecord.SymbolName, name, OS_MAX_SYM_LEN);

   /*
   ** Save symbol address
   */
   symRecord.SymbolAddress = (cpuaddr)val;

   /*
   ** Write entry in file
   */
   status = write(state->fd, (char *)&symRecord, sizeof(symRecord));
   /* There is a problem if not all bytes were written OR if we get an error
    * value, < 0. */
   if ( status < (int)sizeof(symRecord) )
   {
       state->StatusCode = OS_ERROR;
       return(false);
   }

   state->CurrSize = NextSize;

   /*
   ** It's OK to continue
   */
   return(true);
} /* end OS_SymTableIterator_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl ( const char *local_filename, uint32 SizeLimit )
{
    SymbolDumpState_t *state;

    /*
     * Rather than passing the state pointer through the generic "int" arg,
     * use a global.  This is OK because dumps are serialized externally.
     */
    state = &OS_impl_module_global.sym_dump;

    memset(state, 0, sizeof(*state));
    state->Sizelimit = SizeLimit;

    /*
    ** Open file
    */
    state->fd = open(local_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if ( state->fd  < 0 )
    {
        OS_DEBUG("open(%s): error: %s\n", local_filename, strerror(errno));
        state->StatusCode = OS_ERROR;
    }
    else
    {
        /*
        ** Iterate the symbol table
        */
        (void) symEach( sysSymTbl, SYMEACH_FUNC OS_SymTableIterator_Impl, 0 );

        close(state->fd);
    }

   return(state->StatusCode);

} /* end OS_SymbolTableDump_Impl */

/****************************************************************************************
                                    Module Loader API
****************************************************************************************/

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl ( uint32 local_id, char *translated_path )
{
   int32       return_code;
   int         fd;
   MODULE_ID   vxModuleId;

   /*
   ** File is ready to load
   */

   /*
   ** Open the file
   */
   fd = open (translated_path, O_RDONLY, 0);
   if( fd < 0 )
   {
       OS_DEBUG("OSAL: Error, cannot open application file: %s\n",translated_path);
       return_code = OS_ERROR;
   }
   else
   {
       /*
       ** Load the module
       */
       vxModuleId = loadModule (fd, LOAD_ALL_SYMBOLS);

       if( vxModuleId == (MODULE_ID)0 )
       {
           OS_DEBUG("OSAL: Error, cannot load module: %s\n",translated_path);
           return_code = OS_ERROR;
       }
       else
       {
           OS_impl_module_global.vxid_table[local_id] = vxModuleId;
           return_code = OS_SUCCESS;
       }

       /*
       ** Close the file
       */
       close(fd);
   }

   return(return_code);

} /* end OS_ModuleLoad_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleUnload_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl ( uint32 local_id )
{
   STATUS vxStatus;

   /*
   ** Attempt to close/unload the module
   */
   vxStatus = unldByModuleId((MODULE_ID )OS_impl_module_global.vxid_table[local_id], 0);
   if ( vxStatus == ERROR )
   {
      OS_DEBUG("OSAL: Error, Cannot Close/Unload application file: %d\n",vxStatus);
      return(OS_ERROR);
   }

   return(OS_SUCCESS);

} /* end OS_ModuleUnload_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl ( uint32 local_id, OS_module_prop_t *module_prop )
{
   MODULE_INFO  vxModuleInfo;
   STATUS       vxStatus;


   module_prop->host_module_id = (cpuaddr)OS_impl_module_global.vxid_table[local_id];

   /*
   ** Get the module info from vxWorks
   */
   vxStatus = moduleInfoGet(OS_impl_module_global.vxid_table[local_id], &vxModuleInfo);
   if ( vxStatus == ERROR )
   {
      OS_DEBUG("OSAL: OS_ModuleInfoGet Error from vxWorks: %d\n",vxStatus);
   }
   else
   {
       module_prop->addr.valid = true;
       module_prop->addr.code_address = (cpuaddr)vxModuleInfo.segInfo.textAddr;
       module_prop->addr.code_size = vxModuleInfo.segInfo.textSize;
       module_prop->addr.data_address = (cpuaddr)vxModuleInfo.segInfo.dataAddr;
       module_prop->addr.data_size = vxModuleInfo.segInfo.dataSize;
       module_prop->addr.bss_address = (cpuaddr)vxModuleInfo.segInfo.bssAddr;
       module_prop->addr.bss_size = vxModuleInfo.segInfo.bssSize;
   }

   return(OS_SUCCESS);

} /* end OS_ModuleGetInfo_Impl */

