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

