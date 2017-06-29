/*
 * File: osfilesys_stubs.h
 *
 * Purpose:
 *   Provide stubs for unit testing
 *
 * History:
 *   05/28/2015  A. Asp, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSFILESYS_STUBS_H_
#define _OSFILESYS_STUBS_H_

#include "osapi.h"              /* cfe.h not available from within osal. */
#include "uttools.h"
#include "vxworks6-coverage-stubs.h"


/* Global table from osfileapi.c */
OS_FDTableEntry OS_FDTable[OS_MAX_NUM_OPEN_FILES];
/* Global table from cfe_psp_voltab.c */
OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES];


/* Define missing types */
typedef enum
{
    OSFILESYS_STRLEN_INDEX,
    OSFILESYS_STRCMP_INDEX,
    OSFILESYS_STRNCMP_INDEX,
    OSFILESYS_STRCPY_INDEX,
    OSFILESYS_STRNCPY_INDEX,
    OSFILESYS_SNPRINTF_INDEX,
    OSFILESYS_STATFS_INDEX,
    OSFILESYS_CLOSE_INDEX,
    OSFILESYS_IOCTL_INDEX,
    OSFILESYS_OPEN_INDEX,
    OSFILESYS_ATADEVCREATE_INDEX,
    OSFILESYS_DOSFSVOLFORMAT_INDEX,
    OSFILESYS_RAMDEVCREATE_INDEX,
    OSFILESYS_XBDBLKDEVCREATESYNC_INDEX,
    OSFILESYS_MAX_INDEX
} OsFileSys_Index_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} OsFileSys_ReturnCodeTable_t;

typedef struct
{
    int     (*open) (const char *filename, int flags, int mode);

} OsFileSys_HookTable_t;


void OsFileSys_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void OsFileSys_SetFunctionHook(uint32 Index, void *FunPtr);
void OsFileSys_Reset(void);

/* Test utilities */
uint32 getNSemTake(void);
uint32 getNSemGive(void);
uint32 getNCloseCalls(void);
uint32 getNOpenCalls(void);

#endif
