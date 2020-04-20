The include file inc/ut_os_support.h file contains a number of macros (that call static inline utility fn's) to make for concise functional unit test functions, namely:

* UT_IMPL(Fn) -> returns true if the fn is implemented, otherwise throws an assertion
* UT_RETVAL(Fn, Exp, Msg) -> asserts that Fn == Exp with the addl message Msg
* UT_SETUP(Fn) -> calls setup Fn and throws an assertion if it's not successful
* UT_TEARDOWN(Fn) -> calls teardown Fn and throws an assertion if it's not successful
* UT_NOMINAL(Fn) -> calls Fn and asserts that it is nominal

Use the following structure for your unit test cases:

```
/*--------------------------------------------------------------------------------*
** Syntax: OS_SomethingCreate
** Purpose: Creates something
** Parameters: uint32 *id, const char *name, int32 max, int32 counter
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there are no more free something ids
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_somethingcreate()
{
    int i;
    char  name[UT_OS_NAME_BUFF_SIZE];
    char  long_name[UT_OS_NAME_BUFF_SIZE];
    uint32  ids[OS_MAX_SOMETHING_IDS+1];

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_SomethingCreate(&ids[0], "Something", 1, 0))) return;

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_SomethingCreate(NULL, "Something", 1, 0), OS_INVALID_POINTER, "null pointer arg 1");

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_SomethingCreate(&ids[0], NULL, 1, 0), OS_INVALID_POINTER, "null pointer arg 2");

    /*-----------------------------------------------------*/
    memset(long_name, 'X', sizeof(long_name));
    long_name[sizeof(long_name)-1] = '\0';
    UT_RETVAL(OS_SomethingCreate(&ids[0], long_name, 1, 0), OS_ERR_NAME_TOO_LONG, "name too long");

    /*-----------------------------------------------------*/
    /* Setup */
    for ( i = 0; i< OS_MAX_SOMETHING_IDS; i++ )
    {
       memset(name, '\0', sizeof(name));
       UT_os_sprintf(name, "Something%d", i);

       if(!UT_SETUP(OS_SomethingCreate(&ids[i], name, 1, 0)))
       {
          break;
       }
    }

    if ( i == OS_MAX_SOMETHING_IDS ) /* setup was successful */
    {
       UT_RETVAL(OS_SomethingCreate(&ids[OS_MAX_BIN_SEMAPHORES], "OneTooMany", 1, 0), OS_ERR_NO_FREE_IDS, "no free ids");
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    if(UT_SETUP(OS_SomethingCreate(&ids[0], "Something", 1, 0)))
    {
       UT_RETVAL(OS_SomethingCreate(&ids[0], "Something", 1, 0), OS_ERR_NAME_TAKEN, "duplicate name");

       /* Reset test environment */
       UT_TEARDOWN(OS_SomethingDelete(ids[0]));
    }

    /*-----------------------------------------------------*/
    UT_NOMINAL(OS_SomethingCreate(&ids[0], "Something", 1, 0));

    /* Reset test environment */
    UT_TEARDOWN(OS_SomethingDelete(ids[0]));
}
```
