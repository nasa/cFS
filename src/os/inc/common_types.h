/*---------------------------------------------------------------------------
**
**  Filename:
**    $Id: common_types.h 1.9 2014/01/14 16:28:32GMT-05:00 acudmore Exp  $
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
**  $Date: 2014/01/14 16:28:32GMT-05:00 $
**  $Revision: 1.9 $
**  $Log: common_types.h  $
**  Revision 1.9 2014/01/14 16:28:32GMT-05:00 acudmore 
**  Fixed typo in macro for x86-64
**  Revision 1.8 2013/08/09 13:58:04GMT-05:00 acudmore 
**  Added int64 type, added support for ARM arch, added 64 bit x86 arch, added arch check for GCC arch macros, added check for proper data type sizes
**  Revision 1.7 2013/07/25 10:01:29GMT-05:00 acudmore 
**  Added C++ support
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

#ifdef __cplusplus
   extern "C" {
#endif

/*
** Includes
*/

/*
** Macro Definitions
*/

/* 
** Condition = TRUE is ok, Condition = FALSE is error 
*/
#define CompileTimeAssert(Condition, Message) typedef char Message[(Condition) ? 1 : -1]


/*
** Define compiler specific macros
** The __extension__ compiler pragma is required
** for the uint64 type using GCC with the ANSI C90 standard.
** Other macros can go in here as needed, for example alignment 
** pragmas.
**
** NOTE: The white-box (coverage) unit testing may need to disable
** these extra attributes.  These test builds define the OSAPI_NO_SPECIAL_ATTRIBS
** macro to disable this.
*/
#if defined (__GNUC__) && !defined(OSAPI_NO_SPECIAL_ATTRIBS)
   #define _EXTENSION_     __extension__
   #define OS_PACK         __attribute__ ((packed))
   #define OS_ALIGN(n)     __attribute__((aligned(n)))
   #define OS_USED         __attribute__((used))
   #define OS_PRINTF(n,m)  __attribute__ ((format (printf, n, m)))
#else
   #define _EXTENSION_ 
   #define OS_PACK
   #define OS_ALIGN(n) 
   #define OS_USED 
   #define OS_PRINTF(n,m)
#endif

/*
 * If the host system has a ISO C99 standard stdint header file, prefer it.
 * This ensures that fixed-width types are correct including on 64-bit systems.
 */
#if defined(_HAVE_STDINT_)

#include <stdint.h>
#include <stddef.h>
/*
 * NOTE - NOT DEFINING STRUCT_LOW_BIT_FIRST or STRUCT_HIGH_BIT_FIRST
 * We should not make assumptions about the bit order here
 */

  typedef uint8_t                               osalbool;
  typedef int8_t                                int8;
  typedef int16_t                               int16;
  typedef int32_t                               int32;
  typedef int64_t                               int64;
  typedef uint8_t                               uint8;
  typedef uint16_t                              uint16;
  typedef uint32_t                              uint32;
  typedef uint64_t                              uint64;
  typedef intptr_t                              intptr;
  typedef uintptr_t                             cpuaddr;
  typedef size_t                                cpusize;
  typedef ptrdiff_t                             cpudiff;

/*
 * Fall back to default integer type maps -
 * These definitions assume a 32-bit processor
 */
#elif defined(_ix86_) || defined (__i386__)
/* ----------------------- Intel x86 processor family -------------------------*/
  /* Little endian */
  #undef   _STRUCT_HIGH_BIT_FIRST_
  #define  _STRUCT_LOW_BIT_FIRST_

  typedef unsigned char                         osalbool;
  typedef signed char                           int8;
  typedef short int                             int16;
  typedef long int                              int32;
 _EXTENSION_ typedef long long int              int64; 
  typedef unsigned char                         uint8;
  typedef unsigned short int                    uint16;
  typedef unsigned long int                     uint32;
  _EXTENSION_ typedef unsigned long long int    uint64;

  typedef unsigned long int                     cpuaddr;
  typedef unsigned long int                     cpusize;
  typedef long int                              cpudiff;

#elif defined (_ix64_) || defined (__x86_64__) 
/* ----------------------- Intel/AMD x64 processor family -------------------------*/
  /* Little endian */
  #undef   _STRUCT_HIGH_BIT_FIRST_
  #define  _STRUCT_LOW_BIT_FIRST_

  typedef unsigned char                         osalbool;
  typedef signed char                           int8;
  typedef short int                             int16;
  typedef int                                   int32;
  typedef long int                              int64;
  typedef unsigned char                         uint8;
  typedef unsigned short int                    uint16;
  typedef unsigned int                          uint32;
  typedef unsigned long int                     uint64;

  typedef unsigned long int                     cpuaddr;
  typedef unsigned long int                     cpusize;
  typedef long int                              cpudiff;

#elif defined(__PPC__) || defined (__ppc__)
   /* ----------------------- Motorola Power PC family ---------------------------*/
   /* The PPC can be programmed to be big or little endian, we assume native */
   /* Big endian */
   #define _STRUCT_HIGH_BIT_FIRST_
   #undef  _STRUCT_LOW_BIT_FIRST_

   typedef unsigned char                        osalbool;
   typedef signed char                          int8;
   typedef short int                            int16;
   typedef long int                             int32;
   _EXTENSION_ typedef long long int            int64;
   typedef unsigned char                        uint8;
   typedef unsigned short int                   uint16;
   typedef unsigned long int                    uint32;
   _EXTENSION_ typedef unsigned long long int   uint64;

   typedef unsigned long int                     cpuaddr;
   typedef unsigned long int                     cpusize;
   typedef long int                              cpudiff;

#elif defined(_m68k_) || defined(__m68k__)
   /* ----------------------- Motorola m68k/Coldfire family ---------------------------*/
   /* Big endian */
   #define _STRUCT_HIGH_BIT_FIRST_
   #undef  _STRUCT_LOW_BIT_FIRST_

   typedef unsigned char                        osalbool;
   typedef signed char                          int8;
   typedef short int                            int16;
   typedef long int                             int32;
   _EXTENSION_ typedef long long int            int64;
   typedef unsigned char                        uint8;
   typedef unsigned short int                   uint16;
   typedef unsigned long int                    uint32;
   _EXTENSION_ typedef unsigned long long int   uint64;

   typedef unsigned long int                     cpuaddr;
   typedef unsigned long int                     cpusize;
   typedef long int                              cpudiff;

#elif defined (__ARM__) || defined(__arm__)
/* ----------------------- ARM processor family -------------------------*/
  /* Little endian */
  #undef   _STRUCT_HIGH_BIT_FIRST_
  #define  _STRUCT_LOW_BIT_FIRST_

  typedef unsigned char                         osalbool;
  typedef signed char                           int8;
  typedef short int                             int16;
  typedef long int                              int32;
  _EXTENSION_ typedef long long int             int64;
  typedef unsigned char                         uint8;
  typedef unsigned short int                    uint16;
  typedef unsigned long int                     uint32;
  _EXTENSION_ typedef unsigned long long int    uint64;

  typedef unsigned long int                     cpuaddr;
  typedef unsigned long int                     cpusize;
  typedef long int                              cpudiff;

#elif defined(__SPARC__) || defined (_sparc_)
   /* ----------------------- SPARC/LEON family ---------------------------*/
   /* SPARC Big endian */
   #define _STRUCT_HIGH_BIT_FIRST_
   #undef  _STRUCT_LOW_BIT_FIRST_

   typedef unsigned char                        osalbool;
   typedef signed char                          int8;
   typedef short int                            int16;
   typedef long int                             int32;
   _EXTENSION_ typedef long long int            int64;
   typedef unsigned char                        uint8;
   typedef unsigned short int                   uint16;
   typedef unsigned long int                    uint32;
   _EXTENSION_ typedef unsigned long long int   uint64;

   typedef unsigned long int                     cpuaddr;
   typedef unsigned long int                     cpusize;
   typedef long int                              cpudiff;

#else  /* not any of the above */
   #error undefined processor
#endif  /* processor types */

/*
 * Boolean type for compatibility --
 *
 * Note it is a bad idea to typedef "bool" or "boolean" -- MANY other projects
 * and libraries also define a boolean type due to the lack of a standard bool in C89.
 * But calling it simply "bool" or "boolean" almost guarantees a namespace conflict
 * if trying to use OSAL with one of those other existing projects.
 *
 * RTEMS 4.11 no longer defines boolean type by default (deprecated) probably also
 * due to the high likelihood of name conflicts.
 *
 * In order to preserve compatibility for apps written against prior versions of
 * OSAL, the name "boolean" is typedefed as well, but this may be turned off
 * in a future version whenever appropriate.
 */

#if (!defined(_USING_RTEMS_INCLUDES_) || !defined(RTEMS_DEPRECATED_TYPES))
  typedef osalbool boolean;
#endif


#ifndef NULL              /* pointer to nothing */
   #define NULL ((void *) 0)
#endif

#ifndef TRUE              /* Boolean true */
   #define TRUE (1)
#endif

#ifndef FALSE              /* Boolean false */
   #define FALSE (0)
#endif

/* 
** Check Sizes 
*/
CompileTimeAssert(sizeof(uint8)==1,  TypeUint8WrongSize);
CompileTimeAssert(sizeof(uint16)==2, TypeUint16WrongSize);
CompileTimeAssert(sizeof(uint32)==4, TypeUint32WrongSize);
CompileTimeAssert(sizeof(uint64)==8, TypeUint64WrongSize);
CompileTimeAssert(sizeof(int8)==1,   Typeint8WrongSize);
CompileTimeAssert(sizeof(int16)==2,  Typeint16WrongSize);
CompileTimeAssert(sizeof(int32)==4,  Typeint32WrongSize);
CompileTimeAssert(sizeof(int64)==8,  Typeint64WrongSize);
CompileTimeAssert(sizeof(cpuaddr) >= sizeof(void *),  TypePtrWrongSize);

/*
 * TEMPORARY COMPATIBILITY MACRO
 *
 * Any code that depends on this macro should be fixed so as to not need it.
 * The value for this had been set by the BSP makefiles but this is not reliable,
 * especially on processors that support both big- and little- endian modes e.g.
 * ARM and MIPS.
 *
 * This is deprecated and only here to bridge the gap until code that depends
 * on this can be fixed.  Do not write any new code that uses this macro.
 *
 * If using an older makefile that defines one of the BIT_ORDER macros already,
 * then this entire section is skipped and the macro is used as-is.
 */
#if !defined(SOFTWARE_BIG_BIT_ORDER) && !defined(SOFTWARE_LITTLE_BIT_ORDER)

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
/* It is a big-endian target architecture */
#define SOFTWARE_BIG_BIT_ORDER
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
/* It is a little-endian target architecture */
#define SOFTWARE_LITTLE_BIT_ORDER
#endif

#endif /* !defined(SOFTWARE_BIG_BIT_ORDER) && !defined(SOFTWARE_LITTLE_BIT_ORDER) */

#ifdef __cplusplus
   }
#endif

#endif  /* _common_types_ */
