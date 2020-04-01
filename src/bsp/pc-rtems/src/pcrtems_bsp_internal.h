/******************************************************************************
** File:  pcrtems_bsp_internal.h
**
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
**      Copyright (c) 2004-2006, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved.
**
**
** Purpose:
**   Header file for internal data to the PC-RTEMS BSP
**
******************************************************************************/

#ifndef _PCRTEMS_BSP_INTERNAL_H_
#define _PCRTEMS_BSP_INTERNAL_H_

/*
** OSAL includes
*/
#include "osapi.h"
#include "bsp-impl.h"

/*
 * BSP compile-time tuning
 */
#define RTEMS_MAX_USER_OPTIONS   4
#define RTEMS_NUMBER_OF_RAMDISKS 1
#define RTEMS_MAX_CMDLINE        256

/*
 * For debugging, it is helpful to keep the shell
 * priority relatively high.  But this may cause the
 * shell activity to preempt the actual realtime tasks.
 */
#ifdef NDEBUG
#define RTEMS_SHELL_PRIORITY 100
#else
#define RTEMS_SHELL_PRIORITY 5
#endif

/*
** BSP types
*/
typedef struct
{
    char UserArgBuffer[RTEMS_MAX_CMDLINE];
    bool BatchMode;
} OS_BSP_PcRtemsGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_PcRtemsGlobalData_t OS_BSP_PcRtemsGlobal;

#endif /* _PCRTEMS_BSP_INTERNAL_H_ */
