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
 * \file   os-posix.h
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains definitions that are shared across the POSIX
 *          OSAL implementation.  This file is private to the POSIX port and it
 *          may contain POSIX-specific definitions.
 *
 */

/****************************************************************************************
                                    COMMON INCLUDE FILES
 ***************************************************************************************/
#ifndef _os_posix_h_
#define _os_posix_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/signal.h>

#include "common_types.h"
#include "osapi.h"
#include "os-impl.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
 ***************************************************************************************/

typedef void * (*PthreadFuncPtr_t)(void *arg);

typedef struct
{
    int           PriorityMax;
    int           PriorityMin;
} POSIX_PriorityLimits_t;

typedef struct
{
   bool          EnableTaskPriorities;
   uint32        TruncateQueueDepth;
   uint32        ClockAccuracyNsec;
   pthread_key_t ThreadKey;
   sigset_t      MaximumSigMask;
   sigset_t      NormalSigMask;
   POSIX_PriorityLimits_t PriLimits;
   int           SelectedRtScheduler;
} POSIX_GlobalVars_t;

typedef struct
{
    int fd;
    bool selectable;
} OS_Posix_filehandle_entry_t;


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern POSIX_GlobalVars_t POSIX_GlobalVars;

/*
 * The global file handle table.
 *
 * This table is shared across multiple units (files, sockets, etc) and they will share
 * the same file handle table from the basic file I/O.
 */
extern OS_Posix_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];



/****************************************************************************************
                       POSIX IMPLEMENTATION FUNCTION PROTOTYPES
 ***************************************************************************************/

int32 OS_Posix_TaskAPI_Impl_Init(void);
int32 OS_Posix_QueueAPI_Impl_Init(void);
int32 OS_Posix_BinSemAPI_Impl_Init(void);
int32 OS_Posix_CountSemAPI_Impl_Init(void);
int32 OS_Posix_MutexAPI_Impl_Init(void);
int32 OS_Posix_ModuleAPI_Impl_Init(void);
int32 OS_Posix_TimeBaseAPI_Impl_Init(void);
int32 OS_Posix_StreamAPI_Impl_Init(void);
int32 OS_Posix_DirAPI_Impl_Init(void);
int32 OS_Posix_FileSysAPI_Impl_Init(void);

int32 OS_Posix_InternalTaskCreate_Impl (pthread_t *thr, uint32 priority, size_t stacksz, PthreadFuncPtr_t Entry, void *entry_arg);


#endif /* End of os-posix*/
