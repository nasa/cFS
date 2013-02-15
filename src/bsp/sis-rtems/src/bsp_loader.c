/******************************************************************************
** File:  bsp_loader.c
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
**   Include the Static loader. This is not part of the OSAL, so it cannot be 
**   included in the source. This file allows the static loader to be included
**   if it's available. If not, simply undef the OS_STATIC_LOADER define
**   in osconfig.h
**
** History:
**
******************************************************************************/
#include "osapi.h"

#ifdef OS_STATIC_LOADER
   #include "LzmaDec.c"
   #include "loadstaticloadfile.c"
#endif

