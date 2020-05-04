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
 * \file     os-rtems.h
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains definitions that are shared across the RTEMS
 *          OSAL implementation.  This file is private to the RTEMS port and it
 *          may contain RTEMS-specific definitions.
 */

#ifndef INCLUDE_OS_RTEMS_H_
#define INCLUDE_OS_RTEMS_H_


/****************************************************************************************
                                    COMMON INCLUDE FILES
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <rtems.h>
#include <rtems/malloc.h>
#include <rtems/rtems/intr.h>

#include <os-shared-globaldefs.h>

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
 ***************************************************************************************/

typedef struct
{
   uint32        ClockAccuracyNsec;
   rtems_id      IdleTaskId;
} RTEMS_GlobalVars_t;


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern RTEMS_GlobalVars_t RTEMS_GlobalVars;


/****************************************************************************************
                       RTEMS IMPLEMENTATION FUNCTION PROTOTYPES
 ***************************************************************************************/

int32 OS_Rtems_TaskAPI_Impl_Init(void);
int32 OS_Rtems_QueueAPI_Impl_Init(void);
int32 OS_Rtems_BinSemAPI_Impl_Init(void);
int32 OS_Rtems_CountSemAPI_Impl_Init(void);
int32 OS_Rtems_MutexAPI_Impl_Init(void);
int32 OS_Rtems_TimeBaseAPI_Impl_Init(void);
int32 OS_Rtems_ModuleAPI_Impl_Init(void);
int32 OS_Rtems_StreamAPI_Impl_Init(void);
int32 OS_Rtems_DirAPI_Impl_Init(void);
int32 OS_Rtems_FileSysAPI_Impl_Init(void);


int32 OS_Rtems_TableMutex_Init(uint32 idtype);


#endif  /* INCLUDE_OS_RTEMS_H_ */

