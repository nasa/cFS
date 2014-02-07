/*================================================================================*
** File:  ut_osfile_fileio.h
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

#ifndef _UT_OSFILE_FILEIO_H_
#define _UT_OSFILE_FILEIO_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_stubs.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

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

void UT_os_initfs_test(void);

void UT_os_createfile_test(void);
void UT_os_openfile_test(void);
void UT_os_closefile_test(void);

void UT_os_readfile_test(void);
void UT_os_writefile_test(void);
void UT_os_lseekfile_test(void);

void UT_os_chmodfile_test(void);
void UT_os_statfile_test(void);

void UT_os_removefile_test(void);
void UT_os_renamefile_test(void);
void UT_os_copyfile_test(void);
void UT_os_movefile_test(void);

void UT_os_outputtofile_test(void);
void UT_os_getfdinfo_test(void);

void UT_os_checkfileopen_test(void);
void UT_os_closeallfiles_test(void);
void UT_os_closefilebyname_test(void);

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OSFILE_FILEIO_H_ */

/*================================================================================*
** End of File: ut_osfile_fileio.h
**================================================================================*/
