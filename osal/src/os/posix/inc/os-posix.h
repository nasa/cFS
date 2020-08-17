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
 * \file     os-posix.h
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains definitions that are shared across the POSIX
 *          OSAL implementation.  This file is private to the POSIX port and it
 *          may contain POSIX-specific definitions.
 */

#ifndef INCLUDE_OS_POSIX_H_
#define INCLUDE_OS_POSIX_H_


/****************************************************************************************
                                    COMMON INCLUDE FILES
 ***************************************************************************************/

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

/*
 * Use the global definitions from the shared layer
 */
#include <os-shared-globaldefs.h>

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
   size_t        PageSize;
   POSIX_PriorityLimits_t PriLimits;
   int           SelectedRtScheduler;
} POSIX_GlobalVars_t;


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern POSIX_GlobalVars_t POSIX_GlobalVars;


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

int32 OS_Posix_TableMutex_Init(uint32 idtype);

int32 OS_Posix_InternalTaskCreate_Impl (pthread_t *pthr, uint32 priority, size_t stacksz, PthreadFuncPtr_t entry, void *entry_arg);
void  OS_Posix_CompAbsDelayTime( uint32 msecs , struct timespec * tm);


#endif  /* INCLUDE_OS_POSIX_H_ */

