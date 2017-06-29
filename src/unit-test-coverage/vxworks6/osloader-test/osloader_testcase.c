/*
 * Filename: osloader_testcase.c
 *
 * Purpose: This file contains unit test cases
 *
 * Modification History:
 *   07/09/2015, A. Brown, Odyssey Space Research, LLC
 *    * Created
 *
 */

/*
 * Includes
 */
#include "osapi.h"

#include "utassert.h"
#include "uttest.h"
#include "utlist.h"

#include "osloader_stubs.h"

/* items in vxworks6/osloader.c */

typedef struct {
    /* NOTE: this has to match the INTERNAL structure layout
    ** in the specific "osloader.c" implementation that is
    ** being tested.
    **
    ** Currently, this means the one in os/vxworks6 ...
    */
   int                 free;
   cpuaddr             entry_point;
   uint32              host_module_id;
   char                filename[OS_MAX_PATH_LEN];
   char                name[OS_MAX_API_NAME];
   OS_module_address_t addr;  /* Note, this can probably be removed, just wasting memory */
} OS_module_internal_record_t;

extern OS_module_internal_record_t OS_module_table[OS_MAX_MODULES];
extern int OS_sym_table_file_fd;

/* Prototypes for non-exported functions, from osloader.c */
VCS_BOOL  OS_SymTableIterator ( char *name, int val,  VCS_SYM_TYPE type,  int32 max_size, VCS_UINT16 group );

/* Extern items from osloader.c */
extern int OS_symbol_table_size;

/* -------------------- Special Test Case Variables ------------------------- */

/* Many osloader.c functions have logic based on the sizeof(SymbolRecord_t),
 * so let's make our equivalent copy here for convenience, rather than hard-
 * coding assumptions everywhere. */
typedef struct
{
    char    SymbolName[OS_MAX_SYM_LEN];
    uint32  SymbolAddress;
} TESTCOPYSymbolRecord_t;

#define OS_SYMBOL_RECORD_SIZE sizeof(TESTCOPYSymbolRecord_t)

/* ------------------------Utility functions--------------------------------- */
/* set OS_module_table to unusable data */
void Osloader_ScrambleModuleTable(void)
{
    unsigned int i;
    for (i = 0; i < OS_MAX_MODULES; i++)
    {
        OS_module_table[i].free = FALSE;
        OS_module_table[i].entry_point = i+100;
        OS_module_table[i].host_module_id = i+100;
        OS_module_table[i].addr.valid = FALSE;
        strcpy(OS_module_table[i].name, "JUNK");
        strcpy(OS_module_table[i].filename, "JUNK");
    }
}

/* Returns the number of free entries in the OS_module_table. */
unsigned int numModuleTableFree(void) {

    unsigned int count = 0;
    unsigned int i;
    for (i = 0; i < OS_MAX_MODULES; i++)
    {
        if (OS_module_table[i].free == TRUE) {
            count++;
        }
    }
    return count;
}

/* Returns true if all entries in the OS_module_table are free, false otherwise */
VCS_BOOL isModuleTableFree(void) {
    return (numModuleTableFree() == OS_MAX_MODULES);
}


/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
 **
 **  OS_ModuleTableInit Tests
 **
 *******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleTableInit_SemFail(void)
{
    int32 retval = 0;
    unsigned int i;

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 0, 1);

    /* Scramble the memory */
    Osloader_ScrambleModuleTable();

    /* Execute Test */
    retval = OS_ModuleTableInit();

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");

    /* Check Side-Effects */
    UtAssert_True(isSemCreated() == FALSE, "isSemCreated() == F");

    for (i = 0; i < OS_MAX_MODULES; i++)
    {
        UtAssert_True(OS_module_table[i].free == TRUE, "module table check #1");
        UtAssert_True(OS_module_table[i].entry_point == 0, "module table check #2");
        UtAssert_True(OS_module_table[i].host_module_id == 0, "module table check #3");
        UtAssert_True(OS_module_table[i].addr.valid == FALSE, "module table check #4");
	/* Didn't bother to check names. */
    }

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleTableInit(void)
{
    int32 retval = 0;

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);

    /* Scramble the memory */
    Osloader_ScrambleModuleTable();

    /* Execute Test */
    retval = OS_ModuleTableInit();

    /* Verify Outputs*/
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");

    /* Check Side-Effects */
    UtAssert_True(isSemCreated(), "isSemCreated() == T");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}

/*******************************************************************************
 **
 **  OS_SymbolLookup Tests
 **
 *******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolLookup(void)
{
    int32 retval = 0;
    uint32 symaddr = 0;
    char symName[] = "SymName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_SYMFINDBYNAME_INDEX, VCS_OK, 1);

    /* Execute Test */
    retval = OS_SymbolLookup(&symaddr, &(symName[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(symaddr == 4, "symaddr == 4");

    /* Verify Semaphore Usage - should not be accessing the OS_module_table */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolLookup_LookupFail(void)
{
    int32 retval = 0;
    uint32 symaddr = 0;
    char symName[] = "SymName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_SYMFINDBYNAME_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    retval = OS_SymbolLookup(&symaddr, &(symName[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");

    /* Verify Semaphore Usage - should not be accessing the OS_module_table */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolLookup_NullArg1(void)
{
    int32 retval = 0;
    char symName[] = "SymName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_SYMFINDBYNAME_INDEX, VCS_OK, 1);

    /* Execute Test */
    retval = OS_SymbolLookup(NULL, &(symName[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");

    /* Verify Semaphore Usage - should not be accessing the OS_module_table */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolLookup_NullArg2(void)
{
    int32 retval = 0;
    uint32 symaddr = 0;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_SYMFINDBYNAME_INDEX, VCS_OK, 1);

    /* Execute Test */
    retval = OS_SymbolLookup(&symaddr, NULL);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");

    /* Verify Semaphore Usage - should not be accessing the OS_module_table */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}

/*******************************************************************************
 **
 **  OS_SymTableIterator Tests
 **
 *******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_SymTableIterator(void)
{
    VCS_BOOL retval = FALSE;
    char name[] = "TESTNAME";
    int val = 0;
    VCS_SYM_TYPE type = 0;
    int32 max_size = OS_SYMBOL_RECORD_SIZE+1;
    VCS_UINT16 group = 0;

    /* Initialize the osloader */
    OS_symbol_table_size = 0;

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_WRITE_INDEX, max_size, 1);

    /* Execute Test */
    retval = OS_SymTableIterator(&(name[0]), val, type, max_size, group);

    /* Verify Outputs*/
    UtAssert_True(retval == TRUE, "retval == TRUE");

    /* Verify Semaphore Usage - check and make sure a semaphore is NOT taken.
     * This is a support function called by OS_SymbolTableDump() and nowhere
     * else. */
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymTableIterator_HitMax(void)
{
    VCS_BOOL retval = FALSE;
    char name[] = "TESTNAME";
    int val = 0;
    VCS_SYM_TYPE type = 0;
    int32 max_size = 1;
    VCS_UINT16 group = 0;

    /* Initialize the osloader */
    OS_symbol_table_size = 0;

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_WRITE_INDEX, OS_SYMBOL_RECORD_SIZE, 1);

    /* Execute Test */
    retval = OS_SymTableIterator(&(name[0]), val, type, max_size, group);

    /* Verify Outputs*/
    UtAssert_True(retval == FALSE, "retval == FALSE");

    /* Verify Semaphore Usage - check and make sure a semaphore is NOT taken.
     * This is a support function called by OS_SymbolTableDump() and nowhere
     * else. */
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymTableIterator_FailedWrite(void)
{
    VCS_BOOL retval = FALSE;
    char name[] = "TESTNAME";
    int val = 0;
    VCS_SYM_TYPE type = 0;
    int32 max_size = OS_SYMBOL_RECORD_SIZE+1;
    VCS_UINT16 group = 0;

    /* Initialize the osloader */
    OS_symbol_table_size = 0;

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_WRITE_INDEX, -1, 1);

    /* Execute Test */
    retval = OS_SymTableIterator(&(name[0]), val, type, max_size, group);

    /* Verify Outputs*/
    UtAssert_True(retval == FALSE, "retval == FALSE");

    /* Verify Semaphore Usage - check and make sure a semaphore is NOT taken.
     * This is a support function called by OS_SymbolTableDump() and nowhere
     * else. */
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymTableIterator_UnfinishedWrite(void)
{
    VCS_BOOL retval = FALSE;
    char name[] = "TESTNAME";
    int val = 0;
    VCS_SYM_TYPE type = 0;
    int32 max_size = OS_SYMBOL_RECORD_SIZE+1;
    VCS_UINT16 group = 0;

    /* Initialize the osloader */
    OS_symbol_table_size = 0;

    /* Setup Inputs */
    /* the unexpected happened, write() didn't write everything, say
     * one last bit? */
    Osloader_SetReturnCode(OSLOADER_WRITE_INDEX, (OS_SYMBOL_RECORD_SIZE-1), 1);

    /* Execute Test */
    retval = OS_SymTableIterator(&(name[0]), val, type, max_size, group);

    /* Verify Outputs*/
    UtAssert_True(retval == FALSE, "retval == FALSE");

    /* Verify Semaphore Usage - check and make sure a semaphore is NOT taken.
     * This is a support function called by OS_SymbolTableDump() and nowhere
     * else. */
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymTableIterator_NameTooLong(void)
{
    VCS_BOOL retval = FALSE;
    char name[OS_MAX_SYM_LEN+1];
    int val = 0;
    VCS_SYM_TYPE type = 0;
    int32 max_size = OS_SYMBOL_RECORD_SIZE;
    VCS_UINT16 group = 0;

    memset(&(name[0]),(int)"A",OS_MAX_SYM_LEN);
    name[OS_MAX_SYM_LEN]=(char)0;

    /* Initialize the osloader */
    OS_symbol_table_size = 0;

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_SymTableIterator(&(name[0]), val, type, max_size, group);

    /* Verify Outputs*/
    UtAssert_True(retval == FALSE, "retval == FALSE");

    /* Verify Semaphore Usage - check and make sure a semaphore is NOT taken.
     * This is a support function called by OS_SymbolTableDump() and nowhere
     * else. */
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}

/*******************************************************************************
 **
 **  OS_SymbolTableDump Tests
 **
 *******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolTableDump(void)
{
    int32 retval = 0;
    char filename[] = "TESTFILENAME";
    int32 size = OS_SYMBOL_RECORD_SIZE;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_FS_SUCCESS, 1);
    Osloader_SetReturnCode(OSLOADER_OPEN_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_SYMEACH_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_CLOSE_INDEX, 0, 1);

    /* Execute Test */
    retval = OS_SymbolTableDump(&(filename[0]), size);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolTableDump_BadArg1(void)
{
    int32 retval = 0;
    int32 size = OS_SYMBOL_RECORD_SIZE;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_SymbolTableDump(NULL, size);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolTableDump_BadArg2(void)
{
    int32 retval = 0;
    char filename[] = "TESTFILENAME";
    int32 size = 1;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_SymbolTableDump(&(filename[0]), size);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolTableDump_TransPathFail(void)
{
    int32 retval = 0;
    char filename[] = "TESTFILENAME";
    int32 size = OS_SYMBOL_RECORD_SIZE;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_FS_ERR_PATH_INVALID, 1);

    /* Execute Test */
    retval = OS_SymbolTableDump(&(filename[0]), size);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_FS_ERR_PATH_INVALID, "retval == OS_FS_ERR_PATH_INVALID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_SymbolTableDump_OpenFail(void)
{
    int32 retval = 0;
    char filename[] = "TESTFILENAME";
    int32 size = OS_SYMBOL_RECORD_SIZE;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_FS_SUCCESS, 1);
    Osloader_SetReturnCode(OSLOADER_OPEN_INDEX, -1, 1);

    /* Execute Test */
    retval = OS_SymbolTableDump(&(filename[0]), size);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}

/*******************************************************************************
 **
 **  OS_ModuleLoad Tests
 **
 *******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";
    char filename[OS_MAX_PATH_LEN] = "TestFileName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_FS_SUCCESS, 1);
    Osloader_SetReturnCode(OSLOADER_OPEN_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_LOADMODULE_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_CLOSE_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(module_id == 0, "module_id set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_BadArg1(void)
{
    int32 retval = 0;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";
    char filename[OS_MAX_PATH_LEN] = "TestFileName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_ModuleLoad(NULL, &(modulename[0]), &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_BadArg2(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char filename[OS_MAX_PATH_LEN] = "TestFileName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, NULL, &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(module_id == -99, "module_id NOT set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_BadArg3(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), NULL);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(module_id == -99, "module_id NOT set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_BadArg3b_TooLong(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char filename[OS_MAX_PATH_LEN] = "TestFileName";
    char checkval = -1;

    /* Make the module name too long */
    char modulename[OS_MAX_API_NAME+1];
    memset(&modulename,(int)"A",OS_MAX_API_NAME+1);
    modulename[OS_MAX_API_NAME] = (char)0;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_FS_SUCCESS, 1);
    Osloader_SetReturnCode(OSLOADER_OPEN_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_LOADMODULE_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_CLOSE_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), &(filename[0]));

    /* Verify Outputs */
    UtAssert_True(retval != OS_SUCCESS, "retval != OS_SUCCESS");
    checkval = OS_module_table[0].name[OS_MAX_API_NAME-1];
    UtAssert_True(checkval == 0, "Terminating filename null in table.name");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_AllTaken(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";
    char filename[OS_MAX_PATH_LEN] = "TestFileName";
    unsigned int i;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    /* Fill up the OS_module_table */
    for (i = 0; i < OS_MAX_MODULES; i++) {
        OS_module_table[i].free = FALSE;
    }

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERR_NO_FREE_IDS, "retval == OS_ERR_NO_FREE_IDS");
    UtAssert_True(module_id == -99, "module_id NOT set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");

    /* Verify table assumption */
    UtAssert_True(numModuleTableFree() == 0, "all module slots loaded");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_AlreadyLoaded(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";
    char filename[OS_MAX_PATH_LEN] = "TestFileName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    /* set the name on one of them */
    OS_module_table[1].free = FALSE;
    strncpy(OS_module_table[1].name , modulename, OS_MAX_API_NAME);

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERR_NAME_TAKEN, "retval == OS_ERR_NAME_TAKEN");
    UtAssert_True(module_id == -99, "module_id NOT set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");

    /* Verify table assumption */
    UtAssert_True(numModuleTableFree() == (OS_MAX_MODULES-1), "Only 1 module loaded");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_OtherModsLoaded(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";
    char filename[OS_MAX_PATH_LEN] = "TestFileName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_FS_SUCCESS, 1);
    Osloader_SetReturnCode(OSLOADER_OPEN_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_LOADMODULE_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_CLOSE_INDEX, 1, 1);
    OS_module_table[1].free = FALSE;
    strncpy(OS_module_table[1].name , "SomeOtherName", OS_MAX_API_NAME);

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(module_id == 0, "module_id set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");

    /* Verify table assumption */
    UtAssert_True(numModuleTableFree() == (OS_MAX_MODULES-2), "Only 2 modules loaded");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_TransPathFail(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";
    char filename[OS_MAX_PATH_LEN] = "TestFileName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_ERROR, 1);

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");
    UtAssert_True(module_id == -99, "module_id NOT set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_OpenFail(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";
    char filename[OS_MAX_PATH_LEN] = "TestFileName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_FS_SUCCESS, 1);
    Osloader_SetReturnCode(OSLOADER_OPEN_INDEX, -1, 1);

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");
    UtAssert_True(module_id == -99, "module_id NOT set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleLoad_loadModuleFail(void)
{
    int32 retval = 0;
    uint32 module_id = -99;
    char modulename[OS_MAX_API_NAME] = "TestModuleName";
    char filename[OS_MAX_PATH_LEN] = "TestFileName";

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX, OS_FS_SUCCESS, 1);
    Osloader_SetReturnCode(OSLOADER_OPEN_INDEX, 1, 1);
    Osloader_SetReturnCode(OSLOADER_LOADMODULE_INDEX, (int32)NULL, 1);
    Osloader_SetReturnCode(OSLOADER_CLOSE_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_ModuleLoad(&module_id, &(modulename[0]), &(filename[0]));

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");
    UtAssert_True(module_id == -99, "module_id NOT set");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");

    /* Verify table assumption */
    UtAssert_True(isModuleTableFree() == TRUE, "Table is empty");
}

/*******************************************************************************
 **
 **  OS_ModuleUnload Tests
 **
 *******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleUnload(void)
{
    int32 retval = 0;
    uint32 module_id = 0;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_UNLDBYMODULEID_INDEX, VCS_OK, 1);
    OS_module_table[module_id].free = FALSE;

    /* Execute Test */
    retval = OS_ModuleUnload(module_id);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleUnload_BadArg(void)
{
    int32 retval = 0;
    uint32 module_id = OS_MAX_MODULES; /* out of range */

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_ModuleUnload(module_id);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleUnload_NoModule(void)
{
    int32 retval = 0;
    uint32 module_id = 0;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    OS_module_table[module_id].free = TRUE;

    /* Execute Test */
    retval = OS_ModuleUnload(module_id);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleUnload_unldByModuleIdFail(void)
{
    int32 retval = 0;
    uint32 module_id = 0;

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_UNLDBYMODULEID_INDEX, VCS_ERROR, 1);
    OS_module_table[module_id].free = FALSE;

    /* Execute Test */
    retval = OS_ModuleUnload(module_id);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERROR, "retval == OS_ERROR");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}

/*******************************************************************************
 **
 **  OS_ModuleInfo Tests
 **
 *******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleInfo(void)
{
    int32 retval = 0;
    uint32 module_id = 0;
    OS_module_record_t module_info = {0};

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_MODULEINFOGET_INDEX, VCS_OK, 1);
    OS_module_table[module_id].free = FALSE;

    /* Execute Test */
    retval = OS_ModuleInfo(module_id, &module_info);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(module_info.addr.valid == TRUE, "module_info.addr.valid == TRUE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleInfo_BadArg2(void)
{
    int32 retval = 0;
    uint32 module_id = 0;
    OS_module_record_t module_info = {0};

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_ModuleInfo(module_id, NULL);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(module_info.addr.valid == FALSE, "module_info.addr.valid == FALSE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleInfo_BadArg1(void)
{
    int32 retval = 0;
    uint32 module_id = OS_MAX_MODULES;
    OS_module_record_t module_info = {0};

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */

    /* Execute Test */
    retval = OS_ModuleInfo(module_id, &module_info);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");
    UtAssert_True(module_info.addr.valid == FALSE, "module_info.addr.valid == FALSE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() == 0, "getNSemTake() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleInfo_BadArg1Free(void)
{
    int32 retval = 0;
    uint32 module_id = 0;
    OS_module_record_t module_info = {0};

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_MODULEINFOGET_INDEX, VCS_OK, 1);
    OS_module_table[module_id].free = TRUE;

    /* Execute Test */
    retval = OS_ModuleInfo(module_id, &module_info);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");
    UtAssert_True(module_info.addr.valid == FALSE, "module_info.addr.valid == FALSE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_ModuleInfo_ModuleInfoGetFail(void)
{
    int32 retval = 0;
    uint32 module_id = 0;
    OS_module_record_t module_info = {0};

    /* Initialize the osloader */
    Osloader_SetReturnCode(OSLOADER_SEMMCREATE_INDEX, 1, 1);
    UtAssert_True(OS_ModuleTableInit() == OS_SUCCESS, "OS_ModuleTableInit() before executing test.");

    /* Setup Inputs */
    Osloader_SetReturnCode(OSLOADER_MODULEINFOGET_INDEX, VCS_ERROR, 1);
    OS_module_table[module_id].free = FALSE;

    /* Execute Test */
    retval = OS_ModuleInfo(module_id, &module_info);

    /* Verify Outputs*/
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(module_info.addr.valid == FALSE, "module_info.addr.valid == FALSE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake() == getNSemGive(), "getNSemTake() == getNSemGive()");
    UtAssert_True(getNSemTake() > 0, "getNSemTake() > 0");
}
/* ------------------- End of test cases --------------------------------------*/

/*
 * Osloader_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osloader_Setup(void)
{
    Osloader_Reset();
    /*Ut_OSAPI_Reset();*/

    OS_sym_table_file_fd = -1;
}

/*
 * Osloader_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osloader_TearDown(void)
{
    Osloader_Reset();
}

#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* Osloader_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(Test_OS_ModuleTableInit,                 Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleTableInit_SemFail,         Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolLookup,                    Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolLookup_LookupFail,         Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolLookup_NullArg1,           Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolLookup_NullArg2,           Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymTableIterator,                Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymTableIterator_HitMax,         Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymTableIterator_FailedWrite,    Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymTableIterator_UnfinishedWrite,Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymTableIterator_NameTooLong,    Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolTableDump,                 Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolTableDump_BadArg1,         Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolTableDump_BadArg2,         Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolTableDump_TransPathFail,   Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_SymbolTableDump_OpenFail,        Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad,                      Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_BadArg1,              Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_BadArg2,              Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_BadArg3,              Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_BadArg3b_TooLong,     Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_AllTaken,             Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_AlreadyLoaded,        Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_OtherModsLoaded,      Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_TransPathFail,        Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_OpenFail,             Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleLoad_loadModuleFail,       Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleUnload,                    Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleUnload_BadArg,             Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleUnload_NoModule,           Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleUnload_unldByModuleIdFail, Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleInfo,                      Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleInfo_BadArg2,              Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleInfo_BadArg1,              Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleInfo_BadArg1Free,          Osloader_Setup, Osloader_TearDown);
    ADD_TEST(Test_OS_ModuleInfo_ModuleInfoGetFail,    Osloader_Setup, Osloader_TearDown);
}

