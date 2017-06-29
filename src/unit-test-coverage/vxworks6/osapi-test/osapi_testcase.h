/*
 * Filename: osapi_testcase.h
 *
 * Purpose: This file declares common variables and functions used among the
 *          unit test case source files
 *
 * Notes:
 *
 * Modification History:
 *   06/25/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "osapi.h"              /* cfe.h not available from within osal. */

#include "utassert.h"
#include "uttest.h"
#include "utlist.h"

#include "osapi_stubs.h"
#include "osapi_adaptor.h"


#define OS_INCLUDE_MODULE_LOADER
#define OS_UTILITY_TASK_ON



/* Variables defined in osapi.c */
extern const uint32 MAX_PRIORITY;
extern const uint32 UNINITIALIZED;


/* Utility functions */
void deleteFnc(void);
void setTaskTableEntry(uint32 idx, int free, int id, char *name, int creator,
                       uint32 stack_size, uint32 priority, void *delete_hook_pointer);



/* -------------------------------------------------------------------------- */
#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

void Osapi_Setup(void);
void Osapi_TearDown(void);
void Osapi_AddTestCase(void);


