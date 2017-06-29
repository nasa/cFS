/*
 * File: osloader_stubs.c
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

#include "vxworks6-coverage-stubs.h"

#include "common_types.h"
#include "osapi.h"

#include "osloader_stubs.h"

VCS_SYMTAB stubSymbolTable;
VCS_SYMTAB_ID sysSymTbl = &stubSymbolTable;

Osloader_HookTable_t Osloader_HookTable;
Osloader_ReturnCodeTable_t Osloader_ReturnCodeTable[OSLOADER_MAX_INDEX];

boolean semCreated;
uint32 nSemTaken;
uint32 nSemGiven;

char StubSymbolName[] = "StubSymbolName";

void Osloader_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < OSLOADER_MAX_INDEX)
    {
        Osloader_ReturnCodeTable[Index].Value = RtnVal;
        Osloader_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported Index In SetReturnCode Call %lu\n", (unsigned long)Index);
    }
}

boolean Osloader_UseReturnCode(uint32 Index)
{
    if (Osloader_ReturnCodeTable[Index].Count > 0)
    {
        Osloader_ReturnCodeTable[Index].Count--;
        return (TRUE);
    }

    return (FALSE);
}

void Osloader_SetFunctionHook(uint32 Index, void *FunPtr)
{
    switch (Index)
    {
        case (OSLOADER_SEMMCREATE_INDEX):
            Osloader_HookTable.semMCreate = FunPtr;
            break;
        case (OSLOADER_OS_TANSLATE_PATH_INDEX):
            Osloader_HookTable.OS_TranslatePath = FunPtr;
            break;
        case (OSLOADER_SYMFINDBYNAME_INDEX):
            Osloader_HookTable.symFindByName = FunPtr;
            break;
        case (OSLOADER_SYMEACH_INDEX):
            Osloader_HookTable.symEach = FunPtr;
            break;
        case (OSLOADER_MODULEINFOGET_INDEX):
            Osloader_HookTable.moduleInfoGet = FunPtr;
            break;
        case (OSLOADER_LOADMODULE_INDEX):
            Osloader_HookTable.loadModule = FunPtr;
            break;
        case (OSLOADER_UNLDBYMODULEID_INDEX):
            Osloader_HookTable.unldByModuleId = FunPtr;
            break;
        case (OSLOADER_WRITE_INDEX):
            Osloader_HookTable.write = FunPtr;
            break;
        case (OSLOADER_OPEN_INDEX):
            Osloader_HookTable.open = FunPtr;
            break;
        case (OSLOADER_CLOSE_INDEX):
            Osloader_HookTable.close = FunPtr;
            break;
        default:
            printf("Unsupported Osloader Index In SetFunctionHook Call %lu\n",
                    (unsigned long)Index);
            break;
    }

}

void Osloader_Reset(void)
{
    memset(&Osloader_HookTable, 0, sizeof(Osloader_HookTable));
    memset(&Osloader_ReturnCodeTable, 0, sizeof(Osloader_ReturnCodeTable));

    semCreated = FALSE;
    nSemTaken = 0;
    nSemGiven = 0;
}

VCS_SEM_ID VCS_semMCreate(int options)
{
    semCreated = FALSE;
    if (Osloader_UseReturnCode(OSLOADER_SEMMCREATE_INDEX))
    {
        VCS_SEM_ID retval =
                (VCS_SEM_ID) Osloader_ReturnCodeTable[OSLOADER_SEMMCREATE_INDEX].Value;
        if (retval > 0)
        {
            semCreated = TRUE;
        }
        return retval;
    }
    return 0;
}

boolean isSemCreated(void)
{
    return semCreated;
}

VCS_STATUS VCS_semTake(VCS_SEM_ID semId, int timeout)
{
    nSemTaken++;
    return 0;
}

VCS_STATUS VCS_semGive(VCS_SEM_ID semId)
{
    nSemGiven++;
    return 0;
}

int32 getNSemTake(void)
{
    return nSemTaken;
}

int32 getNSemGive(void)
{
    return nSemGiven;
}

int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
{
    if (Osloader_UseReturnCode(OSLOADER_OS_TANSLATE_PATH_INDEX))
    {
        return (int32) Osloader_ReturnCodeTable[OSLOADER_OS_TANSLATE_PATH_INDEX].Value;
    }
    return 0;
}

VCS_STATUS VCS_symFindByName(VCS_SYMTAB_ID symTblId, char * name, char ** pValue,
        VCS_SYM_TYPE * pType)
{
    if (Osloader_UseReturnCode(OSLOADER_SYMFINDBYNAME_INDEX))
    {
        *pValue = (char*) 4;
        return (VCS_STATUS) Osloader_ReturnCodeTable[OSLOADER_SYMFINDBYNAME_INDEX].Value;
    }
    return 0;
}

VCS_SYMBOL * VCS_symEach(VCS_SYMTAB_ID symTblId, VCS_FUNCPTR routine, int routineArg)
{
    if (Osloader_UseReturnCode(OSLOADER_SYMEACH_INDEX))
    {
        return (VCS_SYMBOL *) Osloader_ReturnCodeTable[OSLOADER_SYMEACH_INDEX].Value;
    }
    return 0;
}

VCS_STATUS VCS_moduleInfoGet(VCS_MODULE_ID moduleId, VCS_MODULE_INFO * pModuleInfo)
{
    if (Osloader_UseReturnCode(OSLOADER_MODULEINFOGET_INDEX))
    {
        pModuleInfo->format = 100;
        pModuleInfo->group = 102;
        strncpy(pModuleInfo->name, "testmodname", sizeof(pModuleInfo->name));
        pModuleInfo->segInfo.textAddr = (void*)103;
        pModuleInfo->segInfo.dataAddr = (void*)104;
        pModuleInfo->segInfo.bssAddr = (void*)105;
        pModuleInfo->segInfo.textSize = 106;
        pModuleInfo->segInfo.dataSize = 107;
        pModuleInfo->segInfo.bssSize = 108;
        pModuleInfo->segInfo.textFlags = 109;
        pModuleInfo->segInfo.dataFlags = 110;
        pModuleInfo->segInfo.bssFlags = 111;
        return (VCS_STATUS) Osloader_ReturnCodeTable[OSLOADER_MODULEINFOGET_INDEX].Value;
    }
    return VCS_ERROR;
}

VCS_MODULE_ID VCS_loadModule(int fd, unsigned int symFlag)
{
    if (Osloader_UseReturnCode(OSLOADER_LOADMODULE_INDEX))
    {
        return (VCS_MODULE_ID) Osloader_ReturnCodeTable[OSLOADER_LOADMODULE_INDEX].Value;
    }
    return 0;
}

VCS_STATUS VCS_unldByModuleId(VCS_MODULE_ID moduleId, int options)
{
    if (Osloader_UseReturnCode(OSLOADER_UNLDBYMODULEID_INDEX))
    {
        return (VCS_STATUS) Osloader_ReturnCodeTable[OSLOADER_UNLDBYMODULEID_INDEX].Value;
    }
    return VCS_OK;
}

int VCS_write(int fd, char *buffer, int nbytes)
{
    if (Osloader_UseReturnCode(OSLOADER_WRITE_INDEX))
    {
        return (size_t)Osloader_ReturnCodeTable[OSLOADER_WRITE_INDEX].Value;
    }
    return 0; //write(fd, buffer, nbytes);
}


int VCS_open (const char *filename, int flags, ...)
/*int open (const char *name, int dummy, ...)*/
{
    if (Osloader_UseReturnCode(OSLOADER_OPEN_INDEX))
    {
        return (ssize_t) Osloader_ReturnCodeTable[OSLOADER_OPEN_INDEX].Value;
    }
    return 0; //open(filename, flags, mode);
}

VCS_STATUS VCS_close (int fd)
{
    if (Osloader_UseReturnCode(OSLOADER_CLOSE_INDEX))
    {
        return (size_t)Osloader_ReturnCodeTable[OSLOADER_CLOSE_INDEX].Value;
    }
    return 0; //close(fd);
}

/* The osloader implementation uses strcpy, strlen, etc but we do not implement stubs for them */
size_t VCS_strlen(const char *str)
{
    return strlen(str);
}

int VCS_strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}

int VCS_strncmp(const char *str1, const char *str2, size_t n)
{
    return strncmp(str1, str2, n);
}

char *VCS_strcpy(char *str1, const char *str2)
{
    return strcpy(str1, str2);
}

char *VCS_strncpy(char *str1, const char *str2, size_t n)
{
    return strncpy(str1, str2, n);
}


