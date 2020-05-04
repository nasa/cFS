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
 * \file     os-impl-no-module.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-vxworks.h"


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
    /* nothing to init, but needs to return SUCCESS to allow the rest of OSAL to work */
    return(OS_SUCCESS);
} /* end OS_VxWorks_ModuleAPI_Impl_Init */


