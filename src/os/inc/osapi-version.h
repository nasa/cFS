/************************************************************************
** File: osapi-version.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Purpose: 
**  The OSAL version numbers 
**
** Notes:
**
*************************************************************************/
#ifndef _osapi_version_h_
#define _osapi_version_h_

#define OS_MAJOR_VERSION 5
#define OS_MINOR_VERSION 0
#define OS_REVISION      3
#define OS_MISSION_REV   0

/**
 * Combine the revision components into a single value that application code can check against
 * e.g. "#if OSAL_API_VERSION >= 40100" would check if some feature added in OSAL 4.1 is present.
 */
#define OSAL_API_VERSION ((OS_MAJOR_VERSION * 10000) + (OS_MINOR_VERSION * 100) + OS_REVISION)
      
#endif /* _osapi_version_h_ */

/************************/
/*  End of File Comment */
/************************/
