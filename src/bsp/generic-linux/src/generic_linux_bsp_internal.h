/******************************************************************************
** File:  generic_linux_bsp_internal.h
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
**   Header file for internal data to the LINUX BSP
**
******************************************************************************/

#ifndef GENERIC_LINUX_BSP_INTERNAL_H_
#define GENERIC_LINUX_BSP_INTERNAL_H_

#include "osapi.h"
#include "bsp-impl.h"

/*
** BSP types
*/
typedef struct
{
    bool    EnableTermControl;    /**< Will be set "true" when invoked from a TTY device, false otherwise */
} OS_BSP_GenericLinuxGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_GenericLinuxGlobalData_t OS_BSP_GenericLinuxGlobal;

#endif /* GENERIC_LINUX_BSP_INTERNAL_H_ */
