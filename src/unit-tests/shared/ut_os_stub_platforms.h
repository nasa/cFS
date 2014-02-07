/*================================================================================*
** File:  ut_os_stub_platforms.h
** Owner: Tam Ngo
** Date:  September 2013
**================================================================================*/

#ifndef _UT_OS_STUB_PLATFORMS_H_
#define _UT_OS_STUB_PLATFORMS_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#ifndef ORIONSCP_BUILD
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#endif  /* !ORIONSCP_BUILD */

#ifdef _LINUX_OS_
#endif  /* _LINUX_OS_ */

#ifdef OSP_ARINC653
#include "apex_api.h"

#include "osfilesys.h"
#endif  /* OSP_ARINC653 */

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------*/
#ifdef _LINUX_OS_
/*--------------------------------------------*/

#define UT_OS_PRINT_FDTABLE_CONTENT_MACRO                                 \
	UT_os_sprintf(sVal, "[%d]", (int)j);                                  \
	memcpy(&aLine[col0], sVal, strlen(sVal));                             \
	UT_os_sprintf(sVal, "%d", (int)OS_FDTable[j].OSfd);                   \
	memcpy(&aLine[col1], sVal, strlen(sVal));                             \
	memcpy(&aLine[col2], OS_FDTable[j].Path, strlen(OS_FDTable[j].Path)); \
	UT_os_sprintf(sVal, "%d", (int)OS_FDTable[j].User);                   \
	memcpy(&aLine[col3], sVal, strlen(sVal));                             \
	UT_os_sprintf(sVal, "%d", (int)OS_FDTable[j].IsValid);                \
	memcpy(&aLine[col4], sVal, strlen(sVal));

/*--------------------------------------------*/
#endif  /* _LINUX_OS_ */
/*--------------------------------------------*/

/*--------------------------------------------*/
#ifdef OSP_ARINC653
/*--------------------------------------------*/

#define UT_OS_PRINT_FDTABLE_CONTENT_MACRO                           \
	UT_os_sprintf(sVal, "[%d]", (int)j);                            \
	memcpy(&aLine[col0], sVal, strlen(sVal));                       \
	UT_os_sprintf(sVal, "%d", (int)OS_FDTable[j].fd_entry.OSfd);    \
	memcpy(&aLine[col1], sVal, strlen(sVal));                       \
	memcpy(&aLine[col2], OS_FDTable[j].fd_entry.Path,               \
		   strlen(OS_FDTable[j].fd_entry.Path));                    \
	UT_os_sprintf(sVal, "%d", (int)OS_FDTable[j].fd_entry.User);    \
	memcpy(&aLine[col3], sVal, strlen(sVal));                       \
	UT_os_sprintf(sVal, "%d", (int)OS_FDTable[j].fd_entry.IsValid); \
	memcpy(&aLine[col4], sVal, strlen(sVal));

/*--------------------------------------------*/
#endif  /* OSP_ARINC653 */
/*--------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------*/
#ifdef _LINUX_OS_
/*--------------------------------------------*/

/* As defined in osfileapi.c */
extern OS_FDTableEntry OS_FDTable[OS_MAX_NUM_OPEN_FILES];


/*--------------------------------------------*/
#endif  /* _LINUX_OS_ */
/*--------------------------------------------*/

/*--------------------------------------------*/
#ifdef OSP_ARINC653
/*--------------------------------------------*/

/* As defined in osfileapi.c */
extern OS_FDTableEntry_Wrapper OS_FDTable[OS_MAX_NUM_OPEN_FILES];

/*--------------------------------------------*/
#endif  /* OSP_ARINC653 */
/*--------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OS_STUB_PLATFORMS_H_ */

/*================================================================================*
** End of File: ut_os_stub_platforms.h
**================================================================================*/
