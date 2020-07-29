/*
**
** File: ut_cfe_tbl_hooks.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_tbl_hooks.c 1.1 2011/05/04 11:20:57EDT rmcgraw Exp  $
**
** Purpose: Unit test hooks for cFE Table Services routines
**
** $Log: ut_cfe_tbl_hooks.c  $
** Revision 1.1 2011/05/04 11:20:57EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:42EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.5 2011/03/04 14:59:12EST sslegel 
** Added a define for the maximum number of tables
** Fixed a memory leak in Ut_CFE_TBL_ClearTables
** Fixed a bug in Ut_CFE_TBL_RegisterTable where the table buffer was not cleared after the memory was allocated
** Revision 1.4 2011/02/21 16:15:32EST sslegel 
** Renamed global tables
** Revision 1.3 2011/02/18 15:57:42EST sslegel 
** Added new hooks and return codes
** Changed Ut_CFE_TBL_LoadHook to automatically call the table validate function
** Revision 1.2 2011/02/17 16:34:26EST rmcgraw 
** Tbl GetAdr Hook change to return TBL_UPDATED after a TBL load
** Revision 1.1 2011/02/15 11:13:04EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

#include "cfe.h"
#include "utlist.h"
#include <string.h>

#define UT_CFE_TBL_MAX_TABLES   32

typedef struct {
    boolean                     InUse;
    boolean                     TblUpdatedFlag;
    void                       *Buffer;
    char                        Name[CFE_TBL_MAX_FULL_NAME_LEN];
    uint32                      Size;
    uint16                      TblOptionFlags;
    CFE_TBL_CallbackFuncPtr_t   TblValidationFuncPtr;    
} Ut_CFE_TBL_Registry_t;

typedef struct {
    boolean                     InUse;
    char                        Filename[OS_MAX_PATH_LEN];
    void                       *TablePtr;
} Ut_CFE_TBL_Images_t;

Ut_CFE_TBL_Images_t     Ut_CFE_TBL_Images[UT_CFE_TBL_MAX_TABLES];
Ut_CFE_TBL_Registry_t   Ut_CFE_TBL_Registry[UT_CFE_TBL_MAX_TABLES];

int32 Ut_CFE_TBL_RegisterTable(const char *Name, uint32 Size, uint16 TblOptionFlags, CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr)
{
    uint32          i;

    for (i=0; i < UT_CFE_TBL_MAX_TABLES; i++) {
        if (Ut_CFE_TBL_Registry[i].InUse == FALSE) {
            Ut_CFE_TBL_Registry[i].InUse = TRUE;
            Ut_CFE_TBL_Registry[i].TblUpdatedFlag = FALSE;
            Ut_CFE_TBL_Registry[i].Buffer = malloc(Size);
            memset(Ut_CFE_TBL_Registry[i].Buffer, 0, Size);
            strncpy(Ut_CFE_TBL_Registry[i].Name, Name, CFE_TBL_MAX_FULL_NAME_LEN);
            Ut_CFE_TBL_Registry[i].Size = Size;
            Ut_CFE_TBL_Registry[i].TblOptionFlags = TblOptionFlags;
            Ut_CFE_TBL_Registry[i].TblValidationFuncPtr = TblValidationFuncPtr;
            return(i);
        }
    }

    return(-1);    
}

int32 Ut_CFE_TBL_AddTable(char *Filename, void *TablePtr)
{
    uint32          i;

    for (i=0; i < UT_CFE_TBL_MAX_TABLES; i++) {
        if (Ut_CFE_TBL_Images[i].InUse == FALSE) {
            Ut_CFE_TBL_Images[i].InUse = TRUE;
            strncpy(Ut_CFE_TBL_Images[i].Filename, Filename, OS_MAX_PATH_LEN);
            Ut_CFE_TBL_Images[i].TablePtr = TablePtr;
            return(i);
        }
    }

    return(-1);
}

int32 Ut_CFE_TBL_LoadTable(CFE_TBL_Handle_t TblHandle, void *SrcDataPtr)
{
    if (Ut_CFE_TBL_Registry[TblHandle].InUse) {
        memcpy(Ut_CFE_TBL_Registry[TblHandle].Buffer, SrcDataPtr, Ut_CFE_TBL_Registry[TblHandle].Size);
        Ut_CFE_TBL_Registry[TblHandle].TblUpdatedFlag = TRUE;
        return(CFE_SUCCESS);
    }
    else {
        return(CFE_TBL_ERR_INVALID_HANDLE);
    }
}

void Ut_CFE_TBL_ClearTables(void)
{
    uint32          i;

    for (i=0; i < UT_CFE_TBL_MAX_TABLES; i++) {
        if (Ut_CFE_TBL_Registry[i].InUse == TRUE &&
            Ut_CFE_TBL_Registry[i].Buffer != NULL) {
            free(Ut_CFE_TBL_Registry[i].Buffer);
        }
    }
    memset(&Ut_CFE_TBL_Registry, 0, sizeof(Ut_CFE_TBL_Registry));
    memset(&Ut_CFE_TBL_Images, 0, sizeof(Ut_CFE_TBL_Images));
}

int32 Ut_CFE_TBL_FindTable(char *Filename)
{
    uint32          i;

    for (i=0; i < UT_CFE_TBL_MAX_TABLES; i++) {
        if ((Ut_CFE_TBL_Images[i].InUse == TRUE) &&
            (strncmp(Ut_CFE_TBL_Images[i].Filename, Filename, strlen(Filename)) == 0)) {
            return(i);
        }
    }
    return(-1);
}

void *Ut_CFE_TBL_GetAddress(CFE_TBL_Handle_t TblHandle)
{
    if (Ut_CFE_TBL_Registry[TblHandle].InUse) {
        return(Ut_CFE_TBL_Registry[TblHandle].Buffer);
    }
    else {
        return(NULL);
    }
}

int32 Ut_CFE_TBL_RegisterHook(CFE_TBL_Handle_t *TblHandlePtr, const char *Name, uint32 Size, uint16 TblOptionFlags, CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr)
{
    *TblHandlePtr = Ut_CFE_TBL_RegisterTable(Name, Size, TblOptionFlags, TblValidationFuncPtr);
    return(CFE_SUCCESS);
}

int32 Ut_CFE_TBL_LoadHook(CFE_TBL_Handle_t TblHandle, CFE_TBL_SrcEnum_t SrcType, const void *SrcDataPtr)
{
    int32   TableIndex;
    int32   Status;

    TableIndex = Ut_CFE_TBL_FindTable((char *)SrcDataPtr);
    if (TableIndex >= 0) {
        if (Ut_CFE_TBL_Registry[TblHandle].TblValidationFuncPtr != NULL) {
            Status = Ut_CFE_TBL_Registry[TblHandle].TblValidationFuncPtr(Ut_CFE_TBL_Images[TableIndex].TablePtr);
            if (Status != CFE_SUCCESS)
                return(Status);
        }
        return(Ut_CFE_TBL_LoadTable(TblHandle, Ut_CFE_TBL_Images[TableIndex].TablePtr));
    }
    else {
        return(CFE_TBL_ERR_FILE_NOT_FOUND);
    }
}

int32 Ut_CFE_TBL_GetAddressHook(void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
    if ((*TblPtr = Ut_CFE_TBL_GetAddress(TblHandle)) != NULL) {

        if(Ut_CFE_TBL_Registry[TblHandle].TblUpdatedFlag == TRUE) {
            Ut_CFE_TBL_Registry[TblHandle].TblUpdatedFlag = FALSE;
            return (CFE_TBL_INFO_UPDATED);
        }
        
        return(CFE_SUCCESS);
    }
    else {
        
        return(CFE_TBL_ERR_INVALID_HANDLE);
    }

}
