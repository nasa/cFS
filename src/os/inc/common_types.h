/*---------------------------------------------------------------------------
**
**  Filename: common_types.h
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
**  Notes:
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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * NOTE - NOT DEFINING STRUCT_LOW_BIT_FIRST or STRUCT_HIGH_BIT_FIRST
 * We should not make assumptions about the bit order here
 */

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

#ifndef OSAL_OMIT_DEPRECATED

/*
 * Define the "osalbool" type for backward compatibility.
 * This will be removed in a future release.
 */
typedef bool                                  osalbool;

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

#ifndef TRUE              /* Boolean true */
#define TRUE     true
#endif

#ifndef FALSE              /* Boolean false */
#define FALSE    false
#endif


#endif /* OSAL_OMIT_DEPRECATED */


#ifndef NULL              /* pointer to nothing */
   #define NULL ((void *) 0)
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
