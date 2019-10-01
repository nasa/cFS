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
 * \file   os-impl-no-network.c
 * \author joseph.p.hickey@nasa.gov
 *
 *
 * Purpose: This file contains the network functionality for for
 *      systems which do not implement any networking (OS_INCLUDE_NETWORK is false).
 *
 * It implements the required calls and returns OS_ERR_NOT_IMPLEMENTED for all of them.
 *
 * NOTE: This is a "template" file and not a directly usable source file.
 *       It must be adapted/instantiated from within the OS-specific
 *       implementation on platforms that wish to use this template.
 */


/****************************************************************************************
                                    Network API
 ***************************************************************************************/


                        
/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetID_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl             (int32 *IdBuf)
{
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_NetworkGetID_Impl */



                        
/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetHostName_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl       (char *host_name, uint32 name_len)
{
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_NetworkGetHostName_Impl */


