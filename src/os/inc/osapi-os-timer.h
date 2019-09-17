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


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Create an abstract Time Base resource
 *
 * An OSAL time base is an abstraction of a "timer tick" that can, in turn, be
 * used for measurement of elapsed time between events.
 *
 * Time bases can be simulated by the operating system using the OS kernel-provided
 * timing facilities, or based on a hardware timing source if provided by the BSP.
 *
 * A time base object has a servicing task associated with it, that runs at elevated
 * priority and will thereby interrupt user-level tasks when timing ticks occur.
 *
 * If the external_sync function is passed as NULL, the operating system kernel
 * timing resources will be utilized for a simulated timer tick.
 *
 * If the external_sync function is not NULL, this should point to a BSP-provided
 * function that will block the calling task until the next tick occurs.  This
 * can be used for synchronizing with hardware events.
 *
 * @note When provisioning a tunable RTOS kernel, such as RTEMS, the kernel should
 * be configured to support at least (OS_MAX_TASKS + OS_MAX_TIMEBASES) threads,
 * to account for the helper threads associated with time base objects.
 *
 * @param[out]  timebase_id     An identifier corresponding to the timebase resource
 * @param[in]   timebase_name   The name of the time base
 * @param[in]   external_sync   A synchronization function for BSP hardware-based timer ticks
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_TimeBaseCreate         (uint32 *timebase_id, const char *timebase_name, OS_TimerSync_t external_sync);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Sets the tick period for simulated time base objects
 *
 * This sets the actual tick period for timing ticks that are
 * simulated by the RTOS kernel (i.e. the "external_sync" parameter
 * on the call to OS_TimeBaseCreate() is NULL).
 *
 * The RTOS will be configured to wake up the helper thread at the
 * requested interval.
 *
 * This function has no effect for time bases that are using
 * a BSP-provided external_sync function.
 *
 * @param[in]   timebase_id     The timebase resource to configure
 * @param[in]   start_time      The amount of delay for the first tick, in microseconds.
 * @param[in]   interval_time   The amount of delay between ticks, in microseconds.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_TimeBaseSet            (uint32 timebase_id, uint32 start_time, uint32 interval_time);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes a time base object
 *
 * The helper task and any other resources associated with the time base
 * abstraction will be freed.
 *
 * @param[in]   timebase_id     The timebase resource to delete
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_TimeBaseDelete         (uint32 timebase_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find the ID of an existing time base resource
 *
 * Given a time base name, find and output the ID associated with it.
 *
 * @param[out]  timebase_id     The timebase resource ID
 * @param[in]   timebase_name   The name of the timebase resource to find
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if timebase_id or timebase_name are NULL pointers
 * OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
 * OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_TimeBaseGetIdByName    (uint32 *timebase_id, const char *timebase_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about a timebase resource
 *
 * Fills the buffer referred to by the timebase_prop parameter with
 * relevant information about the time base resource.
 *
 * This function will pass back a pointer to structure that contains
 *             all of the relevant info( name and creator) about the specified timebase.
 *
 * @param[in]   timebase_id     The timebase resource ID
 * @param[out]  timebase_prop   Buffer to store timebase properties
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid timebase
 * OS_INVALID_POINTER if the timebase_prop pointer is null
 */
int32 OS_TimeBaseGetInfo        (uint32 timebase_id, OS_timebase_prop_t *timebase_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief  Read the value of the timebase free run counter
 *
 * Poll the timer free-running time counter in a lightweight fashion.
 *
 * The free run count is a monotonically increasing value reflecting the
 * total time elapsed since the timebase inception.  Units are the
 * same as the timebase itself, usually microseconds.
 *
 * Applications may quickly and efficiently calculate relative time
 * differences by polling this value and subtracting the previous
 * counter value.
 *
 * The absolute value of this counter is not relevant, because it
 * will "roll over" after 2^32 units of time.  For a timebase with
 * microsecond units, this occurs approximately every 4294 seconds,
 * or about 1.2 hours.
 *
 * @note To ensure consistency of results, the application should
 * sample the value at a minimum of two times the roll over frequency,
 * and calculate the difference between the consecutive samples.
 *
 * @param[in]   timebase_id The timebase to operate on
 * @param[out]  freerun_val Buffer to store the free run counter
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid timebase
 */
int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Create a timer object
 *
 * A timer object is a resource that invokes the specified application-provided function
 * upon timer expiration.  Timers may be one-shot or periodic in nature.
 *
 * This function creates a dedicated (hidden) time base object to service this timer,
 * which is created and deleted with the timer object itself.  The internal time base
 * is configured for an OS simulated timer tick at the same interval as the timer.
 *
 * @param[out]  timer_id        The resource ID of the timer object
 * @param[in]   timer_name      Name of the timer object
 * @param[out]  clock_accuracy  Expected precision of the timer, in microseconds.
 * @param[in]   callback_ptr    Application-provided function to invoke
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if any parameters are NULL
 * OS_TIMER_ERR_INVALID_ARGS if the callback function is not valid
 */
int32 OS_TimerCreate            (uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t callback_ptr);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Add a timer object based on an existing TimeBase resource
 *
 * A timer object is a resource that invokes the specified application-provided function
 * upon timer expiration.  Timers may be one-shot or periodic in nature.
 *
 * This function uses an existing time base object to service this timer, which must
 * exist prior to adding the timer.  The precision of the timer is the same
 * as that of the underlying time base object. Multiple timer objects can be
 * created referring to a single time base object.
 *
 * This routine also uses a different callback function prototype from OS_TimerCreate(),
 * allowing a single opaque argument to be passed to the callback routine.
 * The OSAL implementation does not use this parameter, and may be set NULL.
 *
 * @param[out]  timer_id        The resource ID of the timer object
 * @param[in]   timer_name      Name of the timer object
 * @param[in]   timebase_id     The time base resource to use as a reference
 * @param[in]   callback_ptr    Application-provided function to invoke
 * @param[in]   callback_arg    Opaque argument to pass to callback function
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_TimerAdd               (uint32 *timer_id, const char *timer_name, uint32 timebase_id, OS_ArgCallback_t  callback_ptr, void *callback_arg);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Configures the expiration time of the timer object
 *
 * Sets a timer to expire at the given start_time and
 * interval_time.  Units are the same as the underlying
 * time base object.  This is generally microseconds for
 * RTOS-provided (simulated) time base objects, but may be
 * different for BSP-provided time base objects.
 *
 * For a "one-shot" timer, the start_time configures the
 * expiration time, and the interval_time should be passed as
 * zero to indicate the timer is not to be automatically reset.
 *
 * For a periodic timer, the interval_time indicates the
 * desired period between callbacks.
 *
 * The start_time and interval_time should not both be zero.
 *
 * @param[in] timer_id      The timer ID to operate on
 * @param[in] start_time    Time to the first expiration
 * @param[in] interval_time Time between subsequent intervals
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_TimerSet               (uint32 timer_id, uint32 start_time, uint32 interval_time);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes a timer resource
 *
 * The application callback associated with the timer will be stopped,
 * and the resources freed for future use.
 *
 * @param[in] timer_id      The timer ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_TimerDelete            (uint32 timer_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Locate an existing timer resource by name
 *
 * Outputs the ID associated with the given timer, if it exists.
 *
 * @param[out] timer_id      The timer ID corresponding to the name
 * @param[in]  timer_name    The timer name to find
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if timer_id or timer_name are NULL pointers
 * OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
 * OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_TimerGetIdByName       (uint32 *timer_id, const char *timer_name);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Gets information about an existing timer
 *
 * This function will populate structure with
 * the relevant info (name and creator) about the specified timer.
 *
 * @param[in]  timer_id      The timer ID to operate on
 * @param[out] timer_prop    Buffer containing timer properties
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid timer
 * OS_INVALID_POINTER if the timer_prop pointer is null
 */
int32 OS_TimerGetInfo           (uint32  timer_id, OS_timer_prop_t *timer_prop);

#endif
