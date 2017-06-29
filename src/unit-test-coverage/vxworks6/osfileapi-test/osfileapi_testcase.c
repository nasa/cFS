/*
 * Filename: osfileapi_testcase.c
 *
 * Purpose: This file contains unit test cases
 *
 * Notes:
 *  -Tests for path length assume OS_MAX_PATH_LEN includes the terminating null
 *   character, the same as the POSIX.1 standard for PATH_MAX
 *
 * Modification History:
 *   04/28/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include <fcntl.h>  /* for O_WRONLY etc. */

#include "osapi.h"              /* cfe.h not available from within osal. */

#include "utassert.h"
#include "uttest.h"
#include "utlist.h"

#include "osfileapi_stubs.h"



extern OS_FDTableEntry OS_FDTable[OS_MAX_NUM_OPEN_FILES];
extern const char testFileName[];

#define MAX_BUF 100

/* Prototypes for non-exported functions */


/* -------------------- Special Test Case Variables ------------------------- */
static const OS_FDTableEntry zero_OS_FDTableEntry = {0, "", 0, 0};
static const OS_FDTableEntry nonDefault_OS_FDTableEntry = {2, "/test", 5, TRUE};

static const __dirstream testDir = { 1 };

static char testCmd[] = "testCmd";


/* Utility functions */
/* -------------------------------------------------------------------------- */
/*
 * OsFileApi_Set_NonZeroFdTable_Values
 *
 * Purpose:
 *   Sets the entire OS_FDTable to non-zero values
 */
void OsFileApi_Set_NonZeroFdTable_Values(void)
{
    uint32 ii = 0;

    for (ii = 0; ii < OS_MAX_NUM_OPEN_FILES; ++ii)
    {
        OS_FDTable[ii] = nonDefault_OS_FDTableEntry;
    }
}


/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_FS_Init Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_FS_Init_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;
    uint32 ii      = 0;

    /* Setup Inputs */
    OsFileApi_Set_NonZeroFdTable_Values();

    OsFileApi_SetReturnCode(OSFILEAPI_SEMMCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_FS_Init();

    /* Verify Outputs */
    for (ii = 0; ii < OS_MAX_NUM_OPEN_FILES; ++ii)
    {
        UtAssert_True(OS_FDTable[ii].OSfd == -1, "OSFd == expected");
        UtAssert_MemCmp(OS_FDTable[ii].Path, "\0", 1, "Path == empty");
        UtAssert_True(OS_FDTable[ii].User == 0, "User == expected");
        UtAssert_True(OS_FDTable[ii].IsValid == FALSE, "IsValid == FALSE");
    }
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_Init_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;
    uint32 ii      = 0;

    /* Setup Inputs */
    OsFileApi_Set_NonZeroFdTable_Values();

    OsFileApi_SetReturnCode(OSFILEAPI_SEMMCREATE_INDEX, 1, 1);

    /* Execute Test */
    actual = OS_FS_Init();

    /* Verify Outputs */
    for (ii = 0; ii < OS_MAX_NUM_OPEN_FILES; ++ii)
    {
        UtAssert_True(OS_FDTable[ii].OSfd == -1, "OSFd == expected");
        UtAssert_MemCmp(OS_FDTable[ii].Path, "\0", 1, "Path == empty");
        UtAssert_True(OS_FDTable[ii].User == 0, "User == expected");
        UtAssert_True(OS_FDTable[ii].IsValid == FALSE, "IsValid == FALSE");
    }
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
}


/*******************************************************************************
**
**  OS_creat Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_creat_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = NULL;
    int32 access         = OS_WRITE_ONLY;

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_PathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    int32 access         = OS_WRITE_ONLY;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_PathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    int32 access         = OS_WRITE_ONLY;

    /* Verify max path length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);


    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_NameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    int32 access         = OS_WRITE_ONLY;

    /* Create early exit from OS_check_name_length */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 2);

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_NameMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    int32 access         = OS_WRITE_ONLY;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_TransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    int32 access         = OS_WRITE_ONLY;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_INVALID_POINTER, 1);

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_NoFreeFds(void)
{
    int32 expected = OS_FS_ERR_NO_FREE_FDS;
    int32 actual   = 99;
    uint32 ii      = 0;

    /* Setup Inputs */
    const char *fileName = testFileName;
    int32 access   = OS_WRITE_ONLY;
    for (ii = 0; ii < OS_MAX_NUM_OPEN_FILES; ++ii)
    {
        OS_FDTable[ii].IsValid = TRUE;
    }

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NO_FREE_FDS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_OpenWriteOnly(void)
{
    int32 expected = 0;
    int32 actual   = 99;
    int32 user     = 2;

    OS_FDTable[expected].OSfd = -1; /* set non-default values that should be overridden */
    OS_FDTable[expected].User = -1;

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access         = OS_WRITE_ONLY;

    OsFileApi_SetReturnCode(OSFILEAPI_OSFINDCREATOR_INDEX, user, 1);

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected index");
    UtAssert_True(OS_FDTable[expected].IsValid == TRUE, "IsValid == TRUE");
    UtAssert_True(OS_FDTable[expected].OSfd == (O_WRONLY | O_CREAT | O_TRUNC), "OSfd == expected");
    UtAssert_True(OS_FDTable[expected].User == user, "User == expected");
    UtAssert_MemCmp(OS_FDTable[expected].Path, fileName, strlen(fileName), "Path == input");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_OpenReadWrite(void)
{
    int32 expected = 5;
    int32 actual   = 99;
    int32 user     = 10;
    uint32 ii      = 0;

    OS_FDTable[expected].OSfd = -1; /* set non-default values that should be overridden */
    OS_FDTable[expected].User = -1;
    for (ii = 0; ii < expected; ++ii)
    {
        OS_FDTable[ii].IsValid = TRUE;
    }

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access   = OS_READ_WRITE;

    OsFileApi_SetReturnCode(OSFILEAPI_OSFINDCREATOR_INDEX, user, 1);

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected index");
    UtAssert_True(OS_FDTable[expected].IsValid == TRUE, "IsValid == TRUE");
    UtAssert_True(OS_FDTable[expected].OSfd == (O_RDWR | O_CREAT | O_TRUNC), "OSFd == expected");
    UtAssert_True(OS_FDTable[expected].User == user, "User == expected");
    UtAssert_MemCmp(OS_FDTable[expected].Path, fileName, strlen(fileName), "Path == input");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_BadAccess(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access   = -1;  /* invalid value */

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_creat_OpenError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access   = OS_READ_WRITE;

    OsFileApi_SetReturnCode(OSFILEAPI_OPEN_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_creat(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(OS_FDTable[0].IsValid == FALSE, "IsValid == FALSE");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_open Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_open_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = NULL;
    int32 access   = OS_READ_ONLY;
    uint32 mode    = 0;

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_PathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = "/test";
    int32 access   = OS_READ_ONLY;
    uint32 mode    = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_PathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = "/test";
    int32 access   = OS_READ_ONLY;
    uint32 mode    = 0;

    /* Verify max name length passes, if not, exit with non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_NameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = "/test";
    int32 access   = OS_READ_ONLY;
    uint32 mode    = 0;

    /* Create early exit from OS_check_name_length */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 2);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_NameMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = "/test";
    int32 access   = OS_READ_ONLY;
    uint32 mode    = 0;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_TransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = "/test";
    int32 access   = OS_READ_ONLY;
    uint32 mode    = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_INVALID_POINTER, 1);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_NoFreeFds(void)
{
    int32 expected = OS_FS_ERR_NO_FREE_FDS;
    int32 actual   = 99;
    uint32 ii      = 0;

    /* Setup Inputs */
    const char *fileName = testFileName;
    int32 access   = OS_READ_ONLY;
    uint32 mode    = 0;
    for (ii = 0; ii < OS_MAX_NUM_OPEN_FILES; ++ii)
    {
        OS_FDTable[ii].IsValid = TRUE;
    }

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NO_FREE_FDS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_OpenReadOnly(void)
{
    int32 expected = 0;
    int32 actual   = 99;
    int32 user     = 1;

    OS_FDTable[expected].OSfd = -1; /* set non-default values */
    OS_FDTable[expected].User = -1;

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access   = OS_READ_ONLY;
    uint32 mode    = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_OSFINDCREATOR_INDEX, user, 1);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected index");
    UtAssert_True(OS_FDTable[expected].IsValid == TRUE, "IsValid == TRUE");
    UtAssert_True(OS_FDTable[expected].OSfd == O_RDONLY, "OSFd == expected");
    UtAssert_True(OS_FDTable[expected].User == user, "User == expected");
    UtAssert_MemCmp(OS_FDTable[expected].Path, fileName, strlen(fileName), "Path == input");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_OpenWriteOnly(void)
{
    int32 expected = 0;
    int32 actual   = 99;
    int32 user     = 2;

    OS_FDTable[expected].OSfd = -1; /* set non-default values that should be overridden */
    OS_FDTable[expected].User = -1;

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access   = OS_WRITE_ONLY;
    uint32 mode    = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_OSFINDCREATOR_INDEX, user, 1);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected index");
    UtAssert_True(OS_FDTable[expected].IsValid == TRUE, "IsValid == TRUE");
    UtAssert_True(OS_FDTable[expected].OSfd == (O_WRONLY | O_CREAT), "OSFd == expected");
    UtAssert_True(OS_FDTable[expected].User == user, "User == expected");
    UtAssert_MemCmp(OS_FDTable[expected].Path, fileName, strlen(fileName), "Path == input");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_OpenReadWrite(void)
{
    int32 expected = 5;
    int32 actual   = 99;
    int32 user     = 10;
    uint32 ii      = 0;

    OS_FDTable[expected].OSfd = -1; /* set non-default values that should be overridden */
    OS_FDTable[expected].User = -1;
    for (ii = 0; ii < expected; ++ii)
    {
        OS_FDTable[ii].IsValid = TRUE;
    }

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access   = OS_READ_WRITE;
    uint32 mode    = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_OSFINDCREATOR_INDEX, user, 1);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected index");
    UtAssert_True(OS_FDTable[expected].IsValid == TRUE, "IsValid == TRUE");
    UtAssert_True(OS_FDTable[expected].OSfd == (O_RDWR | O_CREAT), "OSFd == expected");
    UtAssert_True(OS_FDTable[expected].User == user, "User == expected");
    UtAssert_MemCmp(OS_FDTable[expected].Path, fileName, strlen(fileName), "Path == input");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_BadAccess(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access   = -1;  /* invalid value */
    uint32 mode    = 0;

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_open_OpenError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName; /* value for open stub to return 'perm' value (set to OSFd) */
    int32 access   = OS_READ_WRITE;
    uint32 mode    = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_OPEN_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_open(fileName, access, mode);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(OS_FDTable[0].IsValid == FALSE, "IsValid == FALSE");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_close Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_close_FdNeg(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd = -1;

    /* Execute Test */
    actual = OS_close(fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_close_FdTooLarge(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd = OS_MAX_NUM_OPEN_FILES;

    /* Execute Test */
    actual = OS_close(fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_close_FdNotValid(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd = 0;
    OS_FDTable[fd].IsValid = FALSE;

    /* Execute Test */
    actual = OS_close(fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_close_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd = 9;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_CLOSE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_close(fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(OS_FDTable[fd].IsValid == FALSE, "IsValid == FALSE");
    UtAssert_True(OS_FDTable[fd].OSfd == -1, "OSFd == expected");
    UtAssert_True(OS_FDTable[fd].User == 0, "User == expected");
    UtAssert_MemCmp(OS_FDTable[fd].Path, "\0", 1, "Path == empty");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_close_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd = 3;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);

    /* Execute Test */
    actual = OS_close(fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(OS_FDTable[fd].IsValid == FALSE, "IsValid == FALSE");
    UtAssert_True(OS_FDTable[fd].OSfd == -1, "OSFd == expected");
    UtAssert_True(OS_FDTable[fd].User == 0, "User == expected");
    UtAssert_MemCmp(OS_FDTable[fd].Path, "\0", 1, "Path == empty");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_read Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_read_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 1;
    char *buffer  = NULL;
    uint32 nbytes = 10;

    /* Execute Test */
    actual = OS_read(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_read_FdNeg(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = -1;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;

    /* Execute Test */
    actual = OS_read(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_read_FdTooLarge(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = OS_MAX_NUM_OPEN_FILES;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;

    /* Execute Test */
    actual = OS_read(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_read_FdNotValid(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 1;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;

    /* Execute Test */
    actual = OS_read(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_read_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 1;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_READ_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_read(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_read_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 7;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_READ_INDEX, OS_FS_SUCCESS, 1);

    /* Execute Test */
    actual = OS_read(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_write Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_write_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 1;
    char *buffer  = NULL;
    uint32 nbytes = 10;

    /* Execute Test */
    actual = OS_write(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_write_FdNeg(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = -1;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;

    /* Execute Test */
    actual = OS_write(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_write_FdTooLarge(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = OS_MAX_NUM_OPEN_FILES;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;

    /* Execute Test */
    actual = OS_write(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_write_FdNotValid(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 1;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;

    /* Execute Test */
    actual = OS_write(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_write_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 1;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_WRITE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_write(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_write_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 7;
    char buffer[MAX_BUF] = {0};
    uint32 nbytes = MAX_BUF;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_WRITE_INDEX, OS_FS_SUCCESS, 1);

    /* Execute Test */
    actual = OS_write(fd, buffer, nbytes);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_chmod Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_chmod(void)
{
    int32 expected = OS_FS_UNIMPLEMENTED;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    uint32 access = O_RDONLY;

    /* Execute Test */
    actual = OS_chmod(fileName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_UNIMPLEMENTED");
}


/*******************************************************************************
**
**  OS_stat Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_stat_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = NULL;
    os_fstat_t fileStats;

    /* Execute Test */
    actual = OS_stat(fileName, &fileStats);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_stat_NullFileStats(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    os_fstat_t *fileStats = NULL;

    /* Execute Test */
    actual = OS_stat(fileName, fileStats);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_stat_PathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    os_fstat_t fileStats;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_stat(fileName, &fileStats);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_stat_PathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    os_fstat_t fileStats;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);


    /* Execute Test */
    actual = OS_stat(fileName, &fileStats);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_stat_TransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    os_fstat_t fileStats;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_INVALID_POINTER, 1);

    /* Execute Test */
    actual = OS_stat(fileName, &fileStats);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_stat_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    os_fstat_t fileStats;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_SUCCESS, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_STAT_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_stat(fileName, &fileStats);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_stat_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    os_fstat_t fileStats;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_SUCCESS, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_STAT_INDEX, OS_FS_SUCCESS, 1);

    /* Execute Test */
    actual = OS_stat(fileName, &fileStats);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_lseek Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_lseek_FdNeg(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = -1;
    int32 offset  = 0;
    uint32 whence = OS_SEEK_SET;

    /* Execute Test */
    actual = OS_lseek(fd, offset, whence);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_lseek_FdTooLarge(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = OS_MAX_NUM_OPEN_FILES;
    int32 offset  = 0;
    uint32 whence = OS_SEEK_SET;

    /* Execute Test */
    actual = OS_lseek(fd, offset, whence);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_lseek_FdNotValid(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 0;
    int32 offset  = 0;
    uint32 whence = OS_SEEK_SET;

    /* Execute Test */
    actual = OS_lseek(fd, offset, whence);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_lseek_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 0;
    int32 offset  = 0;
    uint32 whence = OS_SEEK_SET;
    OS_FDTable[fd].IsValid = TRUE;

    OsFileApi_SetReturnCode(OSFILEAPI_LSEEK_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_lseek(fd, offset, whence);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_lseek_SeekSet(void)
{
    int32 expected = SEEK_SET;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 8;
    int32 offset  = -1;   /* value for the lseek stub to return 'where' value */
    uint32 whence = OS_SEEK_SET;
    OS_FDTable[fd].IsValid = TRUE;

    /* Execute Test */
    actual = OS_lseek(fd, offset, whence);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == SEEK_SET");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_lseek_SeekCur(void)
{
    int32 expected = SEEK_CUR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 4;
    int32 offset  = -1;   /* value for the lseek stub to return 'where' value */
    uint32 whence = OS_SEEK_CUR;
    OS_FDTable[fd].IsValid = TRUE;

    /* Execute Test */
    actual = OS_lseek(fd, offset, whence);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == SEEK_CUR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_lseek_SeekEnd(void)
{
    int32 expected = SEEK_END;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 2;
    int32 offset  = -1;   /* value for the lseek stub to return 'where' value */
    uint32 whence = OS_SEEK_END;
    OS_FDTable[fd].IsValid = TRUE;

    /* Execute Test */
    actual = OS_lseek(fd, offset, whence);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == SEEK_END");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_lseek_SeekInvalid(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 fd      = 7;
    int32 offset  = 0;
    uint32 whence = -1;
    OS_FDTable[fd].IsValid = TRUE;

    /* Execute Test */
    actual = OS_lseek(fd, offset, whence);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_remove Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_remove_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = NULL;

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_remove_PathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_remove_PathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_remove_NameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;

    /* Create early exit from OS_check_name_length */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 2);

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_remove_NameMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_remove_FileOpenError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName  = testFileName;
    OS_FDTable[0] = nonDefault_OS_FDTableEntry;
    strcpy(OS_FDTable[0].Path, "/fakePath");     /* extra entry for MCDC coverage */
    OS_FDTable[1] = nonDefault_OS_FDTableEntry;

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_remove_TransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName  = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERROR, 1);

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_remove_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName  = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_REMOVE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_remove_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName  = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_REMOVE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_remove(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_rename Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_rename_NullOldPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = NULL;
    const char *newFileName = testFileName;

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_NullNewPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = NULL;

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_OldPathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_OldPathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_NewPathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 2);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_NewPathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 2);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_OldNameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    /* Create early exit from OS_check_name_length */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 3);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_OldNameMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_NewNameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 4);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_NewNameMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_OldTransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_INVALID_POINTER, 1);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_NewTransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_INVALID_POINTER, 2);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_RENAME_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rename_Success(void)
{ /* Probably should be a break after the valid file is found and renamed */
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *oldFileName = testFileName;
    const char *newFileName = testFileName;
    OS_FDTable[0] = nonDefault_OS_FDTableEntry;
    OS_FDTable[OS_MAX_NUM_OPEN_FILES-1] = nonDefault_OS_FDTableEntry;
    OS_FDTable[OS_MAX_NUM_OPEN_FILES-1].IsValid = FALSE;  /* for full MCDC coverage */

    OsFileApi_SetReturnCode(OSFILEAPI_RENAME_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_rename(oldFileName, newFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_cp Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_cp_NullSrcPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = NULL;
    const char *dst = testFileName;

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_NullDstPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = NULL;

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_SrcPathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_SrcPathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_DstPathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 2);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_DstPathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    /* Verify max name length passes, if so, exit with non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 2);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_SrcNameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    /* Create early exit from OS_check_name_length */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 3);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_SrcNameMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_DstNameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    /* Create early exit from OS_check_name_length */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 4);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_DstNameMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_SrcTransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERROR, 1);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_DstTransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERROR, 2);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_DstIsOpenError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;
    OS_FDTable[1].IsValid = TRUE;  /* for full MCDC testing */
    OS_FDTable[OS_MAX_NUM_OPEN_FILES-1] = nonDefault_OS_FDTableEntry;

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_CP_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_cp_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_CP_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_cp(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_mv Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_mv_NullSrcPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *srcFileName = NULL;
    const char *dstFileName = testFileName;

    /* Execute Test */
    actual = OS_mv(srcFileName, dstFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_NullDstPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *srcFileName = testFileName;
    const char *dstFileName = NULL;

    /* Execute Test */
    actual = OS_mv(srcFileName, dstFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_SrcPathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *srcFileName = testFileName;
    const char *dstFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_mv(srcFileName, dstFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_SrcPathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *srcFileName = testFileName;
    const char *dstFileName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_mv(srcFileName, dstFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_DstPathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *srcFileName = testFileName;
    const char *dstFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 2);

    /* Execute Test */
    actual = OS_mv(srcFileName, dstFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_DstPathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *srcFileName = testFileName;
    const char *dstFileName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 2);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_mv(srcFileName, dstFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_SrcNameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *srcFileName = testFileName;
    const char *dstFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 3);

    /* Execute Test */
    actual = OS_mv(srcFileName, dstFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_DstNameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *srcFileName = testFileName;
    const char *dstFileName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 4);

    /* Execute Test */
    actual = OS_mv(srcFileName, dstFileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_SrcIsOpenError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;
    OS_FDTable[1].IsValid = TRUE;  /* for full MCDC testing */
    OS_FDTable[OS_MAX_NUM_OPEN_FILES-1] = nonDefault_OS_FDTableEntry;

    /* Execute Test */
    actual = OS_mv(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_CP_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_mv(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mv_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *src = testFileName;
    const char *dst = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_CP_INDEX, VCS_OK, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_REMOVE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_mv(src, dst);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_mkdir Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_mkdir_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *pathName = NULL;
    uint32 access = 0;

    /* Execute Test */
    actual = OS_mkdir(pathName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkdir_PathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *pathName = testFileName;
    uint32 access = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_mkdir(pathName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkdir_PathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *pathName = testFileName;
    uint32 access = 0;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_mkdir(pathName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkdir_TransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *pathName = testFileName;
    uint32 access = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERROR, 1);

    /* Execute Test */
    actual = OS_mkdir(pathName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkdir_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *pathName = testFileName;
    uint32 access = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_MKDIR_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_mkdir(pathName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkdir_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *pathName = testFileName;
    uint32 access = 0;

    OsFileApi_SetReturnCode(OSFILEAPI_MKDIR_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_mkdir(pathName, access);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
}


/*******************************************************************************
**
**  OS_opendir Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_opendir_NullPath(void)
{
    DIR *expected = NULL;
    DIR *actual   = (DIR *)(&testDir);

    /* Setup Inputs */
    const char *pathName = NULL;

    /* Execute Test */
    actual = OS_opendir(pathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == NULL");
}

/*----------------------------------------------------------------------------*/
void Test_OS_opendir_PathTooLong(void)
{
    DIR *expected = NULL;
    DIR *actual   = (DIR *)(&testDir);

    /* Setup Inputs */
    const char *pathName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_opendir(pathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == NULL");
}

/*----------------------------------------------------------------------------*/
void Test_OS_opendir_PathMaxValid(void) /* Also covers a Success test */
{
    DIR *expected = (DIR *)(1);
    DIR *actual   = (DIR *)(&testDir);

    /* Setup Inputs */
    const char *pathName = testFileName;

    /* Verify max name length passes, if so, exit with next non-null return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_SUCCESS, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OPENDIR_INDEX, 1, 1);

    /* Execute Test */
    actual = OS_opendir(pathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_opendir_TransPathError(void)
{
    DIR *expected = NULL;
    DIR *actual   = (DIR *)(&testDir);

    /* Setup Inputs */
    const char *pathName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_opendir(pathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == NULL");
}

/*----------------------------------------------------------------------------*/
void Test_OS_opendir_Error(void)
{
    DIR *expected = NULL;
    DIR *actual   = (DIR *)(&testDir);

    /* Setup Inputs */
    const char *pathName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, 0, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_SUCCESS, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OPENDIR_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_opendir(pathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == NULL");
}


/*******************************************************************************
**
**  OS_closedir Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_closedir_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    os_dirp_t dirPtr = NULL;

    /* Execute Test */
    actual = OS_closedir(dirPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_closedir_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    os_dirp_t dir = (os_dirp_t)1;

    OsFileApi_SetReturnCode(OSFILEAPI_CLOSEDIR_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_closedir(dir);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_closedir_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    os_dirp_t dir = (os_dirp_t)1;

    OsFileApi_SetReturnCode(OSFILEAPI_CLOSEDIR_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_closedir(dir);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
}


/*******************************************************************************
**
**  OS_readdir Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_readdir_NullPath(void)
{
    struct dirent *expected = NULL;
    struct dirent *actual   = (struct dirent *)99;

    /* Setup Inputs */
    os_dirp_t dirPtr = NULL;

    /* Execute Test */
    actual = OS_readdir(dirPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == NULL");
}

/*----------------------------------------------------------------------------*/
void Test_OS_readdir_Error(void)
{
    struct dirent *expected = NULL;
    struct dirent *actual   = (struct dirent *)99;

    /* Setup Inputs */
    os_dirp_t dirPtr = (os_dirp_t)2;

    OsFileApi_SetReturnCode(OSFILEAPI_READDIR_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_readdir(dirPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == NULL");
}

/*----------------------------------------------------------------------------*/
void Test_OS_readdir_Success(void)
{
    struct dirent *expected = (struct dirent *)1;
    struct dirent *actual   = NULL;

    /* Setup Inputs */
    os_dirp_t dirPtr = (os_dirp_t)2;

    OsFileApi_SetReturnCode(OSFILEAPI_READDIR_INDEX, 1, 1);

    /* Execute Test */
    actual = OS_readdir(dirPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  OS_rewinddir Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_rewinddir_NullPath(void)
{
    int32 expected = 0;

    /* Setup Inputs */
    os_dirp_t dirPtr = NULL;

    /* Execute Test */
    OS_rewinddir(dirPtr);

    /* Verify Outputs */
    UtAssert_True(getRewinddirCalls() == expected, "actual == rewinddir not called");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rewinddir_Success(void)
{
    int32 expected = 1;

    /* Setup Inputs */
    os_dirp_t dirPtr = (os_dirp_t)1;

    /* Execute Test */
    OS_rewinddir(dirPtr);

    /* Verify Outputs */
    UtAssert_True(getRewinddirCalls() == expected, "actual == rewinddir called");
}


/*******************************************************************************
**
**  OS_rmdir Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_rmdir_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *dirName = NULL;

    /* Execute Test */
    actual = OS_rmdir(dirName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rmdir_PathTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *dirName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_rmdir(dirName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rmdir_PathMaxValid(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *dirName = testFileName;

    /* Verify max name length passes, if so, exit with next non-zero return */
    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN-1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    actual = OS_rmdir(dirName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rmdir_TransPathError(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *dirName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX, OS_FS_ERROR, 1);

    /* Execute Test */
    actual = OS_rmdir(dirName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rmdir_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *dirName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_RMDIR_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_rmdir(dirName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rmdir_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *dirName = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_RMDIR_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_rmdir(dirName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
}


/*******************************************************************************
**
**  OS_check_name_length Tests
**
*******************************************************************************/
extern int32 OS_check_name_length(const char *path);

/*----------------------------------------------------------------------------*/
void Test_OS_check_name_length_NullPath(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *path = NULL;

    /* Execute Test */
    actual = OS_check_name_length(path);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_check_name_length_PathTooLong(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *path = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_check_name_length(path);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_check_name_length_NoPathSep(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *path = testFileName;

    OsFileApi_SetReturnCode(OSFILEAPI_STRRCHR_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_check_name_length(path);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_check_name_length_NameTooLong(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *path = testFileName;

    /* Linker may replace call to strrchr(str, '\0') to call strchr directly */
    OsFileApi_SetReturnCode(OSFILEAPI_STRCHR_INDEX,
                            (uint32)(path + 1000), 1);
    OsFileApi_SetReturnCode(OSFILEAPI_STRRCHR_INDEX,
                            (uint32)(path + 1000), 2);

    /* Execute Test */
    actual = OS_check_name_length(path);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_check_name_length_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *path = testFileName;

    /* Execute Test */
    actual = OS_check_name_length(path);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
}


/*******************************************************************************
**
**  OS_ShellOutputToFile Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_ShellOutputToFile_NullCmd(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    char *cmd = NULL;
    int32 fd  = 0;

    /* Execute Test */
    actual = OS_ShellOutputToFile(cmd, fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_ShellOutputToFile_FdNeg(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    char *cmd = testCmd;
    int32 fd  = -1;

    /* Execute Test */
    actual = OS_ShellOutputToFile(cmd, fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_ShellOutputToFile_FdTooLarge(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    char *cmd = testCmd;
    int32 fd  = OS_MAX_NUM_OPEN_FILES;

    /* Execute Test */
    actual = OS_ShellOutputToFile(cmd, fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_ShellOutputToFile_FdInvalid(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    char *cmd = testCmd;
    int32 fd  = 0;

    /* Execute Test */
    actual = OS_ShellOutputToFile(cmd, fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_ShellOutputToFile_CmdFileCreateError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    char *cmd = testCmd;
    int32 fd  = 0;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, OS_MAX_PATH_LEN+1, 1);

    /* Execute Test */
    actual = OS_ShellOutputToFile(cmd, fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_ShellOutputToFile_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;
    int32 user     = 2;

    /* Setup Inputs */
    char *cmd = testCmd;
    int32 fd  = 5;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, 1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OPEN_INDEX, VCS_OK, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSFINDCREATOR_INDEX, user, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_WRITE_INDEX, OS_FS_SUCCESS, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_LSEEK_INDEX, VCS_OK, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_SHELLGENERICINIT_INDEX, VCS_ERROR, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_TASKNAMETOID_INDEX, VCS_OK, 3); /* cycle multiple times */
    OsFileApi_SetReturnCode(OSFILEAPI_CLOSE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_ShellOutputToFile(cmd, fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 1, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_ShellOutputToFile_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;
    int32 user     = 2;

    /* Setup Inputs */
    char *cmd = testCmd;
    int32 fd  = 9;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    OsFileApi_SetReturnCode(OSFILEAPI_STRLEN_INDEX, 1, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OPEN_INDEX, VCS_OK, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_OSFINDCREATOR_INDEX, user, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_WRITE_INDEX, OS_FS_SUCCESS, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_LSEEK_INDEX, VCS_OK, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_SHELLGENERICINIT_INDEX, VCS_OK, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_TASKNAMETOID_INDEX, VCS_OK, 1);
    OsFileApi_SetReturnCode(OSFILEAPI_CLOSE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_ShellOutputToFile(cmd, fd);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 1, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_FdGetInfo Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_FDGetInfo_NullFdEntry(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    OS_FDTableEntry *fdEntry = NULL;
    int32 fd  = 0;

    /* Execute Test */
    actual = OS_FDGetInfo(fd, fdEntry);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FDGetInfo_FdNeg(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    OS_FDTableEntry fdEntry = nonDefault_OS_FDTableEntry;
    int32 fd = -1;

    /* Execute Test */
    actual = OS_FDGetInfo(fd, &fdEntry);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(fdEntry.IsValid == FALSE, "entry IsValid == FALSE");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FDGetInfo_FdTooLarge(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    OS_FDTableEntry fdEntry = nonDefault_OS_FDTableEntry;
    int32 fd = OS_MAX_NUM_OPEN_FILES;

    /* Execute Test */
    actual = OS_FDGetInfo(fd, &fdEntry);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(fdEntry.IsValid == FALSE, "entry IsValid == FALSE");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FDGetInfo_FdInvalid(void)
{
    int32 expected = OS_FS_ERR_INVALID_FD;
    int32 actual   = 99;

    /* Setup Inputs */
    OS_FDTableEntry fdEntry = nonDefault_OS_FDTableEntry;
    int32 fd = 0;

    /* Execute Test */
    actual = OS_FDGetInfo(fd, &fdEntry);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_FD");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(fdEntry.IsValid == FALSE, "entry IsValid == FALSE");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FDGetInfo_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    OS_FDTableEntry fdEntry = zero_OS_FDTableEntry;
    int32 fd = OS_MAX_NUM_OPEN_FILES - 1;
    OS_FDTable[fd] = nonDefault_OS_FDTableEntry;

    /* Execute Test */
    actual = OS_FDGetInfo(fd, &fdEntry);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(fdEntry.IsValid == nonDefault_OS_FDTableEntry.IsValid, "Isvalid == expected");
    UtAssert_True(fdEntry.OSfd == nonDefault_OS_FDTableEntry.OSfd, "OSfd == expected");
    UtAssert_True(0 == strcmp(fdEntry.Path, nonDefault_OS_FDTableEntry.Path), "Path == expected");
    UtAssert_True(fdEntry.User == nonDefault_OS_FDTableEntry.User, "User == expected");
}


/*******************************************************************************
**
**  OS_FileOpenCheck Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_FileOpenCheck_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = NULL;

    /* Execute Test */
    actual = OS_FileOpenCheck(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FileOpenCheck_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = "/uniqueFilename";
    OS_FDTable[0] = nonDefault_OS_FDTableEntry;

    /* Execute Test */
    actual = OS_FileOpenCheck(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FileOpenCheck_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    OS_FDTable[OS_MAX_NUM_OPEN_FILES-1] = nonDefault_OS_FDTableEntry;

    /* Execute Test */
    actual = OS_FileOpenCheck((char *)fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_CloseFileByName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CloseFileByName_NullPath(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = NULL;

    /* Execute Test */
    actual = OS_CloseFileByName(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CloseFileByName_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    OS_FDTable[5].IsValid = TRUE;
    strcpy(OS_FDTable[5].Path, testFileName);

    OsFileApi_SetReturnCode(OSFILEAPI_CLOSE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_CloseFileByName((char *)fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(OS_FDTable[5].IsValid == FALSE, "IsValid == FALSE");
    UtAssert_True(OS_FDTable[5].OSfd == -1, "OSFd == expected");
    UtAssert_True(OS_FDTable[5].User == 0, "User == expected");
    UtAssert_MemCmp(OS_FDTable[5].Path, "\0", 1, "Path == empty");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CloseFileByName_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *fileName = testFileName;
    OS_FDTable[2].IsValid = TRUE;
    strcpy(OS_FDTable[2].Path, testFileName);

    OsFileApi_SetReturnCode(OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);

    /* Execute Test */
    actual = OS_CloseFileByName((char *)fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(OS_FDTable[2].IsValid == FALSE, "IsValid == FALSE");
    UtAssert_True(OS_FDTable[2].OSfd == -1, "OSFd == expected");
    UtAssert_True(OS_FDTable[2].User == 0, "User == expected");
    UtAssert_MemCmp(OS_FDTable[2].Path, "\0", 1, "Path == empty");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CloseFileByName_InvalidPath(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    char *fileName = "/falsePath";
    OS_FDTable[8] = nonDefault_OS_FDTableEntry;

    /* Execute Test */
    actual = OS_CloseFileByName(fileName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(OS_FDTable[8].IsValid == TRUE, "IsValid == TRUE");
    UtAssert_True(OS_FDTable[8].OSfd != -1, "OSFd != -1");
    UtAssert_True(OS_FDTable[8].User != 0, "User != 0");
    UtAssert_MemCmp(OS_FDTable[8].Path, nonDefault_OS_FDTableEntry.Path,
                    OS_MAX_PATH_LEN, "Path != empty");
    UtAssert_True(getNSemTake() == 0, "semTake NOT called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_CloseAllFiles Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CloseAllFiles_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;
    uint32 ii      = 0;

    /* Setup Inputs */
    for (ii = 0; ii < OS_MAX_NUM_OPEN_FILES; ++ii)
    {
        OS_FDTable[ii] = nonDefault_OS_FDTableEntry;
    }
    OsFileApi_SetReturnCode(OSFILEAPI_CLOSE_INDEX, VCS_ERROR, OS_MAX_NUM_OPEN_FILES);

    /* Execute Test */
    actual = OS_CloseAllFiles();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CloseAllFiles_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    /* Note, no files to close. */

    /* Execute Test */
    actual = OS_CloseAllFiles();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() == 0, "semTake NOT called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/* ------------------- End of test cases --------------------------------------*/





/*
 * OsFileApi_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void OsFileApi_Setup(void)
{
    uint32 ii = 0;

    OsFileApi_Reset();
    //Ut_OSAPI_Reset();

    for (ii = 0; ii < OS_MAX_NUM_OPEN_FILES; ++ii)
    {
        OS_FDTable[ii] = zero_OS_FDTableEntry;
    }
}


/*
 * OsFileApi_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void OsFileApi_TearDown(void)
{
    OsFileApi_Reset();
}


#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* OsFileApi_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    /* OS_FS_Init Tests */
    ADD_TEST(Test_OS_FS_Init_Error,                 OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_FS_Init_Success,               OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_creat Tests */
    ADD_TEST(Test_OS_creat_NullPath,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_PathTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_PathMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_NameTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_NameMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_TransPathError,          OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_NoFreeFds,               OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_OpenWriteOnly,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_OpenReadWrite,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_BadAccess,               OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_creat_OpenError,               OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_Open Tests */
    ADD_TEST(Test_OS_open_NullPath,                 OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_PathTooLong,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_PathMaxValid,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_NameTooLong,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_NameMaxValid,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_TransPathError,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_NoFreeFds,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_OpenReadOnly,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_OpenWriteOnly,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_OpenReadWrite,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_BadAccess,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_open_OpenError,                OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_close Tests */
    ADD_TEST(Test_OS_close_FdNeg,                   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_close_FdTooLarge,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_close_FdNotValid,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_close_Error,                   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_close_Success,                 OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_read Tests */
    ADD_TEST(Test_OS_read_NullPath,                 OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_read_FdNeg,                    OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_read_FdTooLarge,               OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_read_FdNotValid,               OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_read_Error,                    OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_read_Success,                  OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_write Tests */
    ADD_TEST(Test_OS_write_NullPath,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_write_FdNeg,                   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_write_FdTooLarge,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_write_FdNotValid,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_write_Error,                   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_write_Success,                 OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_chmod Tests */
    ADD_TEST(Test_OS_chmod,                         OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_stat Tests */
    ADD_TEST(Test_OS_stat_NullPath,                 OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_stat_NullFileStats,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_stat_PathTooLong,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_stat_PathMaxValid,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_stat_TransPathError,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_stat_Error,                    OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_stat_Success,                  OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_lseek Tests */
    ADD_TEST(Test_OS_lseek_FdNeg,                   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_lseek_FdTooLarge,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_lseek_FdNotValid,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_lseek_Error,                   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_lseek_SeekSet,                 OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_lseek_SeekCur,                 OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_lseek_SeekEnd,                 OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_lseek_SeekInvalid,             OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_remove Tests */
    ADD_TEST(Test_OS_remove_NullPath,               OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_remove_PathTooLong,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_remove_PathMaxValid,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_remove_NameTooLong,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_remove_NameMaxValid,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_remove_FileOpenError,          OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_remove_TransPathError,         OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_remove_Error,                  OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_remove_Success,                OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_rename Tests */
    ADD_TEST(Test_OS_rename_NullOldPath,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_NullNewPath,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_OldPathTooLong,         OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_OldPathMaxValid,        OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_NewPathTooLong,         OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_NewPathMaxValid,        OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_OldNameTooLong,         OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_OldNameMaxValid,        OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_NewNameTooLong,         OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_NewNameMaxValid,        OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_OldTransPathError,      OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_NewTransPathError,      OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_Error,                  OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rename_Success,                OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_cp Tests */
    ADD_TEST(Test_OS_cp_NullSrcPath,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_NullDstPath,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_SrcPathTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_SrcPathMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_DstPathTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_DstPathMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_SrcNameTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_SrcNameMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_DstNameTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_DstNameMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_SrcTransPathError,          OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_DstTransPathError,          OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_DstIsOpenError,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_Error,                      OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_cp_Success,                    OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_mv Tests */
    ADD_TEST(Test_OS_mv_NullSrcPath,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_NullDstPath,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_SrcPathTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_SrcPathMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_DstPathTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_DstPathMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_SrcNameTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_DstNameTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_SrcIsOpenError,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_Error,                      OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mv_Success,                    OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_mkdir Tests */
    ADD_TEST(Test_OS_mkdir_NullPath,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mkdir_PathTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mkdir_PathMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mkdir_TransPathError,          OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mkdir_Error,                   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_mkdir_Success,                 OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_opendir Tests */
    ADD_TEST(Test_OS_opendir_NullPath,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_opendir_PathTooLong,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_opendir_PathMaxValid,          OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_opendir_TransPathError,        OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_opendir_Error,                 OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_closedir Tests */
    ADD_TEST(Test_OS_closedir_NullPath,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_closedir_Error,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_closedir_Success,              OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_readdir Tests */
    ADD_TEST(Test_OS_readdir_NullPath,              OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_readdir_Error,                 OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_readdir_Success,               OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_rewinddir Tests */
    ADD_TEST(Test_OS_rewinddir_NullPath,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rewinddir_Success,             OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_rmdir Tests */
    ADD_TEST(Test_OS_rmdir_NullPath,                OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rmdir_PathTooLong,             OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rmdir_PathMaxValid,            OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rmdir_TransPathError,          OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rmdir_Error,                   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_rmdir_Success,                 OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_check_name_length Tests */
    ADD_TEST(Test_OS_check_name_length_NullPath,    OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_check_name_length_PathTooLong, OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_check_name_length_NoPathSep,   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_check_name_length_NameTooLong, OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_check_name_length_Success,     OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_ShellOutputToFile Tests */
    ADD_TEST(Test_OS_ShellOutputToFile_NullCmd,     OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_ShellOutputToFile_FdNeg,       OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_ShellOutputToFile_FdTooLarge,  OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_ShellOutputToFile_FdInvalid,   OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_ShellOutputToFile_CmdFileCreateError, OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_ShellOutputToFile_Error,       OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_ShellOutputToFile_Success,     OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_FDGetInfo Tests */
    ADD_TEST(Test_OS_FDGetInfo_NullFdEntry,         OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_FDGetInfo_FdNeg,               OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_FDGetInfo_FdTooLarge,          OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_FDGetInfo_FdInvalid,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_FDGetInfo_Success,             OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_FileOpenCheck Tests */
    ADD_TEST(Test_OS_FileOpenCheck_NullPath,        OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_FileOpenCheck_Error,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_FileOpenCheck_Success,         OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_CloseFileByName Tests */
    ADD_TEST(Test_OS_CloseFileByName_NullPath,      OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_CloseFileByName_Error,         OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_CloseFileByName_Success,       OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_CloseFileByName_InvalidPath,   OsFileApi_Setup, OsFileApi_TearDown);

    /* OS_CloseAllFiles Tests */
    ADD_TEST(Test_OS_CloseAllFiles_Error,           OsFileApi_Setup, OsFileApi_TearDown);
    ADD_TEST(Test_OS_CloseAllFiles_Success,         OsFileApi_Setup, OsFileApi_TearDown);
}




