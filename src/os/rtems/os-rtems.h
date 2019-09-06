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
 * \file   os-rtems.h
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains definitions that are shared across the RTEMS
 *          OSAL implementation.  This file is private to the RTEMS port and it
 *          may contain RTEMS-specific definitions.
 *
 */

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

#include "common_types.h"
#include "osapi.h"
#include "os-impl.h"

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

typedef struct
{
    int fd;
    bool selectable;
} OS_Rtems_filehandle_entry_t;


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern RTEMS_GlobalVars_t RTEMS_GlobalVars;

/*
 * The global file handle table.
 *
 * This table is shared across multiple units (files, sockets, etc) and they will share
 * the same file handle table from the basic file I/O.
 */
extern OS_Rtems_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];


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



