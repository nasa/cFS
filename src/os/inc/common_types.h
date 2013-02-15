/*---------------------------------------------------------------------------
**
**  Filename:
**    $Id: common_types.h 1.6 2012/04/11 09:19:03GMT-05:00 acudmore Exp  $
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement. 
**
**  Purpose:
**	    Unit specification for common types.
**
**  Design Notes:
**         Assumes make file has defined processor family
**
**  References:
**     Flight Software Branch C Coding Standard Version 1.0a
**
**
**	Notes:
**
**
**  $Date: 2012/04/11 09:19:03GMT-05:00 $
**  $Revision: 1.6 $
**  $Log: common_types.h  $
**  Revision 1.6 2012/04/11 09:19:03GMT-05:00 acudmore 
**  added OS_USED attribute
**  Revision 1.5 2010/02/18 16:43:29EST acudmore 
**  Added SPARC processor section
**  Removed special characters from comments that cause problems with some tools.
**  Revision 1.4 2010/02/18 16:41:39EST acudmore 
**  Added a block of defines for GCC specific pragmas and extensions.
**  Removed RTEMS boolean related ifdefs
**  moved OS_PACK into the GCC specific block
**  Revision 1.3 2010/02/01 12:31:17EST acudmore 
**  Added uint64 type
**  Revision 1.2 2009/07/07 16:30:05EDT acudmore 
**  Removed conditinal comp. around boolean for m68k.
**  This will need to be done for all RTEMS targets
**  Revision 1.1 2009/06/02 10:04:58EDT acudmore 
**  Initial revision
**  Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/os/inc/project.pj
**  Revision 1.1 2008/04/20 22:35:58EDT ruperera 
**  Initial revision
**  Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/inc/project.pj
**  Revision 1.1 2007/10/16 16:14:49EDT apcudmore 
**  Initial revision
**  Member added to project d:/mksdata/MKS-OSAL-REPOSITORY/src/inc/project.pj
**  Revision 1.2 2006/06/08 14:28:32EDT David Kobe (dlkobe) 
**  Added NASA Open Source Legal Statement
**  Revision 1.1 2005/06/09 09:57:51GMT-05:00 rperera 
**  Initial revision
**  Member added to project d:/mksdata/MKS-CFE-REPOSITORY/cfe-core/inc/project.pj
**  Revision 1.6  2005/03/24 19:20:52  rmcgraw
**  Wrapped the boolean defintion for all three processors with #ifndef _USING_RTEMS_INCLUDES_
**
**  Revision 1.5  2005/03/10 16:59:08  acudmore
**  removed boolean prefix to TRUE and FALSE defintion to avoid vxWorks conflict.
**
**  Revision 1.4  2005/03/07 20:23:34  acudmore
**  removed duplicate boolean definition
**
**  Revision 1.3  2005/03/07 20:05:17  acudmore
**  updated with __PPC__ macro that gnu compiler uses
**
**  Revision 1.2  2005/03/04 16:02:44  acudmore
**  added coldfire architecture
**
**  Revision 1.1  2005/03/04 15:58:45  acudmore
**  Added common_types.h
**
**
**
**-------------------------------------------------------------------------*/

#ifndef _common_types_
#define _common_types_

/*
** Includes
*/

/*
** Macro Definitions
*/

/*
** Define compiler specific macros
** The __extension__ compiler pragma is required
** for the uint64 type using GCC with the ANSI C90 standard.
** Other macros can go in here as needed, for example alignment 
** pragmas.
*/
#if defined (__GNUC__)
   #define _EXTENSION_  __extension__
   #define OS_PACK      __attribute__ ((packed))
   #define OS_ALIGN(n)  __attribute__((aligned(n)))
   #define OS_USED      __attribute__((used))
#else
   #define _EXTENSION_ 
   #define OS_PACK
   #define OS_ALIGN(n) 
   #define OS_USED 
#endif

#if defined(_ix86_)
/* ----------------------- Intel x86 processor family -------------------------*/
  /* Little endian */
  #undef   _STRUCT_HIGH_BIT_FIRST_
  #define  _STRUCT_LOW_BIT_FIRST_

#ifndef _USING_RTEMS_INCLUDES_
  typedef unsigned char                         boolean;
#endif

  typedef signed char                           int8;
  typedef short int                             int16;
  typedef long int                              int32;
  typedef unsigned char                         uint8;
  typedef unsigned short int                    uint16;
  typedef unsigned long int                     uint32;
  _EXTENSION_ typedef unsigned long long int    uint64;

#elif defined(__PPC__)
   /* ----------------------- Motorola Power PC family ---------------------------*/
   /* The PPC can be programmed to be big or little endian, we assume native */
   /* Big endian */
   #define _STRUCT_HIGH_BIT_FIRST_
   #undef  _STRUCT_LOW_BIT_FIRST_

   typedef unsigned char                        boolean;
   typedef signed char                          int8;
   typedef short int                            int16;
   typedef long int                             int32;
   typedef unsigned char                        uint8;
   typedef unsigned short int                   uint16;
   typedef unsigned long int                    uint32;
   _EXTENSION_ typedef unsigned long long int   uint64;

#elif defined(_m68k_)
   /* ----------------------- Motorola m68k/Coldfire family ---------------------------*/
   /* Big endian */
   #define _STRUCT_HIGH_BIT_FIRST_
   #undef  _STRUCT_LOW_BIT_FIRST_

   typedef unsigned char                        boolean;
   typedef signed char                          int8;
   typedef short int                            int16;
   typedef long int                             int32;
   typedef unsigned char                        uint8;
   typedef unsigned short int                   uint16;
   typedef unsigned long int                    uint32;
   _EXTENSION_ typedef unsigned long long int   uint64;

#elif defined(__SPARC__)
   /* ----------------------- SPARC/LEON family ---------------------------*/
   /* SPARC Big endian */
   #define _STRUCT_HIGH_BIT_FIRST_
   #undef  _STRUCT_LOW_BIT_FIRST_

   typedef unsigned char                        boolean;
   typedef signed char                          int8;
   typedef short int                            int16;
   typedef long int                             int32;
   typedef unsigned char                        uint8;
   typedef unsigned short int                   uint16;
   typedef unsigned long int                    uint32;
   _EXTENSION_ typedef unsigned long long int   uint64;

#else  /* not any of the above */
   #error undefined processor
#endif  /* processor types */

#ifndef NULL              /* pointer to nothing */
   #define NULL ((void *) 0)
#endif

#ifndef TRUE              /* Boolean true */
   #define TRUE (1)
#endif

#ifndef FALSE              /* Boolean false */
   #define FALSE (0)
#endif

#endif  /* _common_types_ */
