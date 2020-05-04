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
 * \file     os-shared-shell.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_SHELL_H_
#define INCLUDE_OS_SHARED_SHELL_H_

#include <os-shared-globaldefs.h>




/****************************************************************************************
                 SHELL API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/


/*----------------------------------------------------------------
   Function: OS_ShellOutputToFile_Impl

    Purpose: Takes a shell command in and writes the output of that command to the specified file

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ShellOutputToFile_Impl(uint32 stream_id, const char* Cmd);

#endif  /* INCLUDE_OS_SHARED_SHELL_H_ */

