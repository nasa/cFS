/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-impl-loader.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-loader.h"
#include "os-shared-module.h"

#include <errnoLib.h>
#include <loadLib.h>
#include <symLib.h>
#include <unldLib.h>
#include <unistd.h>
#include <fcntl.h>


OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];

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
   memset(&OS_impl_module_table, 0, sizeof(OS_impl_module_table));
   return(OS_SUCCESS);
} /* end OS_VxWorks_ModuleAPI_Impl_Init */




/****************************************************************************************
                                    Module Loader API
****************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl ( uint32 local_id, const char *translated_path )
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
           OS_impl_module_table[local_id].moduleID = vxModuleId;
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
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl ( uint32 local_id )
{
   STATUS vxStatus;

   /*
   ** Attempt to close/unload the module
   */
   vxStatus = unldByModuleId(OS_impl_module_table[local_id].moduleID, 0);
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
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl ( uint32 local_id, OS_module_prop_t *module_prop )
{
   MODULE_INFO  vxModuleInfo;
   STATUS       vxStatus;


   module_prop->host_module_id = (cpuaddr)OS_impl_module_table[local_id].moduleID;

   /*
   ** Get the module info from vxWorks
   */
   vxStatus = moduleInfoGet(OS_impl_module_table[local_id].moduleID, &vxModuleInfo);
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

