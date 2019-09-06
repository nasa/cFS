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
 */


/****************************************************************************************
                                    Network API
 ***************************************************************************************/


/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetID

    Purpose: Gets the ID of the current Network
---------------------------------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl             (int32 *IdBuf)
{
    return OS_ERR_NOT_IMPLEMENTED;
}/* end OS_NetworkGetID */



/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetHostName

    Purpose: Gets the name of the current host

    Returns: OS_ERROR if the  host name could not be found
             OS_SUCCESS if the name was copied to host_name successfully
---------------------------------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl       (char *host_name, uint32 name_len)
{
    return OS_ERR_NOT_IMPLEMENTED;
}/* end OS_NetworkGetHostName */


