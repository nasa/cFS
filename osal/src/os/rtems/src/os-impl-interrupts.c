/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     os-impl-interrupts.c
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-shared-interrupts.h"

/****************************************************************************************
                                 INT API (deprecated)
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_IntAttachHandler_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntAttachHandler_Impl  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
#if (CPU_SIMPLE_VECTORED_INTERRUPTS == false)
  return OS_ERR_NOT_IMPLEMENTED;
#else
   rtems_status_code ret_status;
   uint32 status ;
   rtems_isr_entry old_handler;

   ret_status = rtems_interrupt_catch(
		(rtems_isr_entry)InterruptHandler,
		(rtems_vector_number)InterruptNumber,
		&old_handler);

   switch (ret_status)
   {
       case RTEMS_SUCCESSFUL :
          status = OS_SUCCESS;
	  break ;

       case RTEMS_INVALID_NUMBER :
          status = OS_INVALID_INT_NUM;
          break ;

       case RTEMS_INVALID_ADDRESS :
          status = OS_INVALID_POINTER;
          break ;

    default :
          status = OS_ERROR;
          break ;
    }
    return(status) ;
#endif
} /* end OS_IntAttachHandler_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_IntUnlock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntUnlock_Impl (int32 IntLevel)
{
    rtems_interrupt_level rtems_int_level = IntLevel;
    rtems_interrupt_local_enable ( rtems_int_level );
    return (OS_SUCCESS);

} /* end OS_IntUnlock_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntLock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntLock_Impl (void)
{
   rtems_interrupt_level rtems_int_level;

   /*
    * NOTE: rtems_interrupt_local_disable() is a macro
    * that sets the rtems_int_level value.
    *
    * This code assumes that the value is also storable
    * in an int32.
    *
    * This uses the "local" version which operates on
    * the current CPU in case of a multi-processor environment.
    *
    * This should be identical to rtems_interrupt_disable in
    * a single-processor config, but that call is not
    * implemented in multi-processor configs.
    */
   rtems_interrupt_local_disable(rtems_int_level) ;
   return ( (int32) rtems_int_level) ;

} /* end OS_IntLock_Impl */



/*----------------------------------------------------------------
 *
 * Function: OS_IntEnable_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntEnable_Impl (int32 Level)
{
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntEnable_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntDisable_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntDisable_Impl (int32 Level)
{
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntDisable_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntSetMask_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntSetMask_Impl ( uint32 MaskSetting )
{
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntSetMask_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntGetMask_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntGetMask_Impl ( uint32 * MaskSettingPtr )
{
    *MaskSettingPtr = 0;
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntGetMask_Impl */

