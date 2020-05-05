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
 * \file     ut-adaptor-timebase.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_TIMEBASE_H_
#define INCLUDE_UT_ADAPTOR_TIMEBASE_H_

#include <common_types.h>
#include <OCS_symLib.h>
#include <OCS_time.h>

extern void*  const UT_Ref_OS_impl_timebase_table;
extern size_t const UT_Ref_OS_impl_timebase_table_SIZE;


int32 UT_Call_OS_VxWorks_TimeBaseAPI_Impl_Init(void);

void UT_TimeBaseTest_Setup(uint32 local_id, int signo, bool reset_flag);

/**
 * Invokes OS_VxWorks_SigWait() with the given arguments.
 * This is normally a static function but exposed via a non-static wrapper for UT purposes.
 */
int32 UT_TimeBaseTest_CallSigWaitFunc(uint32 local_id);

/* Invokes the static OS_VxWorks_TimeBaseTask() function with given argument */
int UT_TimeBaseTest_CallHelperTaskFunc(int arg);

/* Invokes the static OS_VxWorks_RegisterTimer() function with given argument */
void UT_TimeBaseTest_CallRegisterTimer(uint32 local_id);

/* Hook functions which set the timer registration state */
void UT_TimeBaseTest_SetTimeBaseRegState(uint32 local_id, bool is_success);
void UT_TimeBaseTest_ClearTimeBaseRegState(uint32 local_id);

/* Hook functions which test the timer registration state */
bool UT_TimeBaseTest_CheckTimeBaseRegisteredState(uint32 local_id);
bool UT_TimeBaseTest_CheckTimeBaseErrorState(uint32 local_id);

/* Invoke the internal UsecToTimespec API */
void UT_TimeBaseTest_UsecToTimespec(uint32 usecs, struct OCS_timespec *time_spec);

#endif  /* INCLUDE_UT_ADAPTOR_TIMEBASE_H_ */

