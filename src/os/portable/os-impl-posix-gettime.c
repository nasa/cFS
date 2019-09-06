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
 * \file   os-impl-posix-gettime.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains implementation for OS_GetTime() and OS_SetTime()
 *      that map to the C library clock_gettime() and clock_settime() calls.
 *      This should be usable on any OS that supports those standard calls.
 *      The OS-specific code must #include the correct headers that define the
 *      prototypes for these functions before including this implementation file.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/* Handled by includer */


/****************************************************************************************
                                FUNCTIONS
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
 * Name: OS_GetLocalTime
 *
 * Purpose: This functions get the local time of the machine its on
 * ------------------------------------------------------------------------------------*/

int32 OS_GetLocalTime_Impl(OS_time_t *time_struct)
{
    int             Status;
    int32           ReturnCode;
    struct timespec time;

    Status = clock_gettime(CLOCK_REALTIME, &time);

    if (Status == 0)
    {
        time_struct -> seconds = time.tv_sec;
        time_struct -> microsecs = time.tv_nsec / 1000;
        ReturnCode = OS_SUCCESS;
    }
    else
    {
        OS_DEBUG("Error calling clock_gettime: %s\n",strerror(errno));
        ReturnCode = OS_ERROR;
    }

    return ReturnCode;
}/* end OS_GetLocalTime */

/*---------------------------------------------------------------------------------------
 * Name: OS_SetLocalTime
 *
 * Purpose: This functions set the local time of the machine its on
 * ------------------------------------------------------------------------------------*/

int32 OS_SetLocalTime_Impl(const OS_time_t *time_struct)
{
    int             Status;
    int32           ReturnCode;
    struct timespec time;

    time.tv_sec = time_struct -> seconds;
    time.tv_nsec = (time_struct -> microsecs * 1000);

    Status = clock_settime(CLOCK_REALTIME, &time);

    if (Status == 0)
    {
        ReturnCode = OS_SUCCESS;
    }
    else
    {
        ReturnCode = OS_ERROR;
    }

    return ReturnCode;

} /*end OS_SetLocalTime */

