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
 * \file     os-vxworks.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_VXWORKS_H_
#define INCLUDE_OS_VXWORKS_H_


/****************************************************************************************
                                    COMMON INCLUDE FILES
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <semLib.h>
#include <errnoLib.h>

#include <os-shared-globaldefs.h>


/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
****************************************************************************************/

typedef struct
{
    void * const mem;
    SEM_ID vxid;
} VxWorks_GlobalMutex_t;

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

extern VxWorks_GlobalMutex_t VX_MUTEX_TABLE[];


/****************************************************************************************
                       VXWORKS IMPLEMENTATION FUNCTION PROTOTYPES
****************************************************************************************/

int32 OS_VxWorks_TaskAPI_Impl_Init(void);
int32 OS_VxWorks_QueueAPI_Impl_Init(void);
int32 OS_VxWorks_BinSemAPI_Impl_Init(void);
int32 OS_VxWorks_CountSemAPI_Impl_Init(void);
int32 OS_VxWorks_MutexAPI_Impl_Init(void);
int32 OS_VxWorks_TimeBaseAPI_Impl_Init(void);
int32 OS_VxWorks_ModuleAPI_Impl_Init(void);
int32 OS_VxWorks_StreamAPI_Impl_Init(void);
int32 OS_VxWorks_DirAPI_Impl_Init(void);

int OS_VxWorks_TaskEntry(int arg);
int OS_VxWorks_ConsoleTask_Entry(int arg);

uint32 OS_VxWorks_SigWait(uint32 local_id);
int OS_VxWorks_TimeBaseTask(int arg);
void OS_VxWorks_RegisterTimer(uint32 local_id);
void OS_VxWorks_UsecToTimespec(uint32 usecs, struct timespec *time_spec);

int32 OS_VxWorks_GenericSemTake(SEM_ID vxid, int sys_ticks);
int32 OS_VxWorks_GenericSemGive(SEM_ID vxid);


int32 OS_VxWorks_TableMutex_Init(uint32 idtype);

#endif  /* INCLUDE_OS_VXWORKS_H_ */

