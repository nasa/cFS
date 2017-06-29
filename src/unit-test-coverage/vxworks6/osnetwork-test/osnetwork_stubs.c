/*
 * File: osnetwork_stubs.c
 *
 * Purpose:
 *  Stub out various functions not stubbed out by the UT-Assert code
 *
 * Modification History:
 *   06/23/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 *
 */

#include <string.h>

#include "osapi.h"              /* cfe.h not available from within osal. */

#include "osnetwork_stubs.h"


Osnetwork_HookTable_t           Osnetwork_HookTable;
Osnetwork_ReturnCodeTable_t     Osnetwork_ReturnCodeTable[OSNETWORK_MAX_INDEX];

char testname[] = "testname";

void Osnetwork_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < OSNETWORK_MAX_INDEX) {
        Osnetwork_ReturnCodeTable[Index].Value = RtnVal;
        Osnetwork_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", (unsigned)Index);
    }
}


boolean Osnetwork_UseReturnCode(uint32 Index)
{
    if (Osnetwork_ReturnCodeTable[Index].Count > 0) {
        Osnetwork_ReturnCodeTable[Index].Count--;
        if (Osnetwork_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}


void Osnetwork_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if (Index == OSNETWORK_HOSTGETBYNAME_INDEX) {
        Osnetwork_HookTable.hostGetByName = FunPtr; 
    }
    else
    {
        printf("Unsupported Osnetwork Index In SetFunctionHook Call %u\n", (unsigned)Index);
    }
}


void Osnetwork_Reset(void)
{
    memset(&Osnetwork_HookTable, 0, sizeof(Osnetwork_HookTable));
    memset(&Osnetwork_ReturnCodeTable, 0, sizeof(Osnetwork_ReturnCodeTable));
}

int VCS_gethostname(char *name, int name_len)
{
  if (Osnetwork_UseReturnCode(OSNETWORK_GETHOSTNAME_INDEX)) {
      return (int)Osnetwork_ReturnCodeTable[OSNETWORK_GETHOSTNAME_INDEX].Value;
  }

  strcpy(name, testname);
  return 0;

}

int VCS_hostGetByName(char *name)
{
  if (Osnetwork_UseReturnCode(OSNETWORK_HOSTGETBYNAME_INDEX)) {
    return (int)Osnetwork_ReturnCodeTable[OSNETWORK_HOSTGETBYNAME_INDEX].Value;
  }

  strcpy(name, testname);
  return 0;

}
