/*================================================================================*
** File:  ut_osloader_test_platforms.h
** Owner: Tam Ngo
** Date:  November 2014
**================================================================================*/

#ifndef _UT_OSLOADER_TEST_PLATFORMS_H_
#define _UT_OSLOADER_TEST_PLATFORMS_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*
 * The actual module files that the loader tests attempt to load need
 * to be consistent with the system type that is being compiled for.
 *
 * It can be assumed that the BSP will provide some sort of virtual
 * filesystem mapping for the /cf directory, but the file extension
 * for a loadable module still differs.
 */

/*--------------------------------------------*/
#if defined(_VXWORKS_OS_) || defined(OSP_ARINC653)
/*--------------------------------------------*/

#define UT_OS_GENERIC_MODULE_NAME1   "/cf/apps/MODULE.o"
#define UT_OS_GENERIC_MODULE_NAME2   "/cf/apps/MODULE1.o"
#define UT_OS_SPECIFIC_MODULE_NAME   "/cf/apps/MODULE%d.o"

/*--------------------------------------------*/
#elif defined(_RTEMS_OS_)
/*--------------------------------------------*/

#define UT_OS_GENERIC_MODULE_NAME1   "/cf/MODULE.obj"
#define UT_OS_GENERIC_MODULE_NAME2   "/cf/MODULE1.obj"
#define UT_OS_SPECIFIC_MODULE_NAME   "/cf/MODULE%d.obj"

/*--------------------------------------------*/
#else /* For any other OS assume Linux/POSIX style .so files */
/*--------------------------------------------*/

#define UT_OS_GENERIC_MODULE_NAME1   "/cf/MODULE.so"
#define UT_OS_GENERIC_MODULE_NAME2   "/cf/MODULE1.so"
#define UT_OS_SPECIFIC_MODULE_NAME   "/cf/MODULE%d.so"

#endif

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OSLOADER_TEST_PLATFORMS_H_ */

/*================================================================================*
** End of File: ut_osloader_test_platforms.h
**================================================================================*/
