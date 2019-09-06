/*
** File: osapi-os-timer.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author:  Alan Cudmore - Code 582
**
** Purpose: Contains functions prototype definitions and variable declarations
**          for the OS Abstraction Layer, Timer API
**
*/

#ifndef _osapi_timer_
#define _osapi_timer_

/*
** Typedefs
*/
typedef void (*OS_TimerCallback_t)(uint32 timer_id);
typedef uint32 (*OS_TimerSync_t)(uint32 timer_id);

typedef struct 
{
   char                name[OS_MAX_API_NAME];
   uint32              creator;
   uint32              start_time;
   uint32              interval_time;
   uint32              accuracy;

} OS_timer_prop_t;

typedef struct
{
    char                name[OS_MAX_API_NAME];
    uint32              creator;
    uint32              nominal_interval_time;
    uint32              freerun_time;
    uint32              accuracy;
} OS_timebase_prop_t;

/*
** Timer API
*/
int32  OS_TimerAPIInit          (void);

int32 OS_TimeBaseCreate         (uint32 *timebase_id, const char *timebase_name, OS_TimerSync_t external_sync);
int32 OS_TimeBaseSet            (uint32 timebase_id, uint32 start_time, uint32 interval_time);
int32 OS_TimeBaseDelete         (uint32 timebase_id);
int32 OS_TimeBaseGetIdByName    (uint32 *timebase_id, const char *timebase_name);
int32 OS_TimeBaseGetInfo        (uint32 timebase_id, OS_timebase_prop_t *timebase_prop);
int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val);

int32 OS_TimerCreate            (uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t callback_ptr);
int32 OS_TimerAdd               (uint32 *timer_id, const char *timer_name, uint32 timebase_id, OS_ArgCallback_t  callback_ptr, void *callback_arg);
int32 OS_TimerSet               (uint32 timer_id, uint32 start_time, uint32 interval_time);
int32 OS_TimerDelete            (uint32 timer_id);

int32 OS_TimerGetIdByName       (uint32 *timer_id, const char *timer_name);
int32 OS_TimerGetInfo           (uint32  timer_id, OS_timer_prop_t *timer_prop);

#endif
