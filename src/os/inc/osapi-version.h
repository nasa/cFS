/************************************************************************
** File:
**   $Id: osapi-version.h 1.10 2014/01/23 16:33:31GMT-05:00 acudmore Exp  $
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
** $Log: osapi-version.h  $
** Revision 1.10 2014/01/23 16:33:31GMT-05:00 acudmore 
** Update for 4.1 release
** Revision 1.9 2013/01/16 14:35:18GMT-05:00 acudmore 
** updated version label
** Revision 1.8 2012/04/16 14:57:04GMT-05:00 acudmore 
** Updated version label to 3.5.0.0
** Revision 1.7 2012/01/17 16:04:29EST acudmore 
** Updated version to 3.4.1
** Revision 1.6 2011/12/05 15:45:16EST acudmore 
** Updated version label to 3.4.0
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/os/inc/project.pj
**
*************************************************************************/
#ifndef _osapi_version_h_
#define _osapi_version_h_

/*
 * Note about the OS_REVISION value:
 * During development of the "next" version of OSAL (whatever number it might be), the
 * OS_REVISION value should be something high (>90) to indicate the fact that this is a development
 * version and not an official release version.  For instance, at the time of this writing
 * the official version is 4.1.1 and the next version would likely be 4.2.0.  So anyone
 * who uses the "bleeding edge" development branch before 4.2.0 is officially released will
 * get a version that reads e.g. "4.1.91" to show that this is a pre-4.2 build.
 *
 * This still leaves room for patches to be applied to 4.1.1 to produce 4.1.2 if needed.
 *
 * During development, if an API-affecting change is introduced then the OS_REVISION value should
 * be incremented with it to indicate this.  Client code that depends on the new API can do a "#if"
 * against the development version number e.g. "#if OSAL_API_VERSION >= 40191" and this check
 * will still be valid after the version becomes 40200 (although at some point it can be cleaned
 * up to reflect the official version number, it is not going to break the build in the meantime).
 */

#define OS_MAJOR_VERSION 4
#define OS_MINOR_VERSION 2
#define OS_REVISION      1
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
