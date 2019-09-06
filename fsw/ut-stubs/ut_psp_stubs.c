/*
**  GSC-18128-1, "Core Flight Executive Version 6.6"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
** File: ut_bsp_stubs.c
**
** Purpose:
** Unit test stubs for BSP routines
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
*/

/*
** Includes
*/

#include "cfe_psp.h"
#include "utstubs.h"
#include <string.h>

#include <target_config.h>

Target_PspConfigData GLOBAL_PSP_CONFIGDATA = { 0 };
Target_CfeConfigData GLOBAL_CFE_CONFIGDATA = { 0 };

/**
 * Instantiation of global system-wide configuration struct
 * This contains build info plus pointers to the PSP and CFE
 * configuration structures.  Everything will be linked together
 * in the final executable.
 */
Target_ConfigData GLOBAL_CONFIGDATA =
{
        .MissionVersion = "MissionUnitTest",
        .CfeVersion = "CfeUnitTest",
        .OsalVersion = "OsalUnitTest",
        .Config = "MissionConfig",
        .Date = "MissionBuildDate",
        .User = "MissionBuildUser",
        .Default_CpuName = "UnitTestCpu",
        .Default_CpuId = 1,
        .Default_SpacecraftId = 42,
        .CfeConfig = &GLOBAL_CFE_CONFIGDATA,
        .PspConfig = &GLOBAL_PSP_CONFIGDATA
};


/*
** Functions
*/

/*****************************************************************************/
/**
** \brief CFE_PSP_Panic stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        CFE_PSP_Panic.  The variable PSPPanicRtn.value is set equal to the
**        input variable ErrorCode and the variable PSPPanicRtn.count is
**        incremented each time this function is called.  The unit tests
**        compare these values to expected results to verify proper system
**        response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        This function does not return a value.
**
******************************************************************************/
void CFE_PSP_Panic(int32 ErrorCode)
{
    UT_DEFAULT_IMPL(CFE_PSP_Panic);
    UT_Stub_CopyFromLocal(UT_KEY(CFE_PSP_Panic), (uint8*)&ErrorCode, sizeof(ErrorCode));
}

/*****************************************************************************/
/**
** \brief CFE_PSP_GetProcessorId stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_GetProcessorId.  It is set to return a fixed value for the
**        unit tests.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns 0.
**
******************************************************************************/
uint32 CFE_PSP_GetProcessorId(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_GetProcessorId);

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_GetSpacecraftId stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_GetSpacecraftId.  It is set to return a fixed value for the
**        unit tests.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns 0.
**
******************************************************************************/
uint32 CFE_PSP_GetSpacecraftId(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_GetSpacecraftId);

    return status;
}

/*****************************************************************************/
/**
v** \brief CFE_PSP_GetTime stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_GetTime.  The LocalTime structure is set to the user-defined
**        values in BSP_Time.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        This function does not return a value.
**
******************************************************************************/
void CFE_PSP_GetTime(OS_time_t *LocalTime)
{
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_GetTime);

    if (status >= 0)
    {
        if (UT_Stub_CopyToLocal(UT_KEY(CFE_PSP_GetTime), (uint8*)LocalTime, sizeof(*LocalTime)) < sizeof(*LocalTime))
        {
            LocalTime->seconds = 100;
            LocalTime->microsecs = 200;
        }
    }
}

/*****************************************************************************/
/**
** \brief CFE_PSP_WriteToCDS stub function
**
** \par Description
**        This function is used to mimic the response of the PSP function
**        CFE_PSP_WriteToCDS.  The user can adjust the response by setting
**        the values in the BSPWriteCDSRtn structure prior to this function
**        being called.  If the value BSPWriteCDSRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value BSPWriteCDSRtn.value.
**        Otherwise, the value of the user-defined variable UT_BSP_Fail
**        determines the status returned by the function.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS, OS_ERROR, or a user-defined value.
**
******************************************************************************/
int32 CFE_PSP_WriteToCDS(const void *PtrToDataToWrite,
                         uint32 CDSOffset,
                         uint32 NumBytes)
{
    uint8 *BufPtr;
    uint32 CdsSize;
    uint32 Position;
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_WriteToCDS);

    if (status >= 0)
    {
        UT_GetDataBuffer(UT_KEY(CFE_PSP_WriteToCDS), (void**)&BufPtr, &CdsSize, &Position);
        if (BufPtr != NULL && (CDSOffset + NumBytes) <= CdsSize)
        {
            memcpy(BufPtr + CDSOffset, PtrToDataToWrite, NumBytes);
        }
    }

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_ReadFromCDS stub function
**
** \par Description
**        This function is used to mimic the response of the PSP function
**        CFE_PSP_ReadFromCDS.  The user can adjust the response by setting
**        the values in the BSPReadCDSRtn structure prior to this function
**        being called.  If the value BSPReadCDSRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value BSPReadCDSRtn.value.
**        Otherwise, the value of the user-defined variable UT_BSP_Fail
**        determines the status returned by the function.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS, OS_ERROR, or a user-defined value.
**
******************************************************************************/
int32 CFE_PSP_ReadFromCDS(void *PtrToDataToRead,
                          uint32 CDSOffset,
                          uint32 NumBytes)
{
    uint8 *BufPtr;
    uint32 CdsSize;
    uint32 Position;
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_ReadFromCDS);

    if (status >= 0)
    {
        UT_GetDataBuffer(UT_KEY(CFE_PSP_ReadFromCDS), (void**)&BufPtr, &CdsSize, &Position);
        if (BufPtr != NULL && (CDSOffset + NumBytes) <= CdsSize)
        {
            memcpy(PtrToDataToRead, BufPtr + CDSOffset, NumBytes);
        }
    }

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_GetCDSSize stub function
**
** \par Description
**        This function is used to mimic the response of the PSP function
**        CFE_PSP_GetCDSSize.  The user can adjust the response by
**        setting the value of UT_BSP_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 CFE_PSP_GetCDSSize(uint32 *SizeOfCDS)
{
    int32 status;
    void *BufPtr;
    uint32 Position;

    status = UT_DEFAULT_IMPL(CFE_PSP_GetCDSSize);

    if (status >= 0)
    {
        UT_GetDataBuffer(UT_KEY(CFE_PSP_GetCDSSize), &BufPtr, SizeOfCDS, &Position);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_GetVolatileDiskMem stub function
**
** \par Description
**        This function is used to mimic the response of the PSP function
**        CFE_PSP_GetVolatileDiskMem.  The user can adjust the response by
**        setting the value of UT_BSP_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 CFE_PSP_GetVolatileDiskMem(cpuaddr *PtrToVolDisk, uint32 *SizeOfVolDisk)
{
    int32 status;
    uint32 Position;

    status = UT_DEFAULT_IMPL(CFE_PSP_GetVolatileDiskMem);

    if (status >= 0)
    {
        UT_GetDataBuffer(UT_KEY(CFE_PSP_GetVolatileDiskMem), (void**)PtrToVolDisk, SizeOfVolDisk, &Position);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_Restart stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_Restart.  The variable PSPRestartRtn.value is set to the
**        value passed to the function, reset_type, and the variable
**        PSPRestartRtn.count is incremented each time this function is called.
**        The unit tests compare these values to expected results to verify
**        proper system response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        This function does not return a value.
**
******************************************************************************/
void CFE_PSP_Restart(uint32 reset_type)
{
    UT_DEFAULT_IMPL(CFE_PSP_Restart);
    UT_Stub_CopyFromLocal(UT_KEY(CFE_PSP_Restart), (uint8*)&reset_type, sizeof(reset_type));
}

/*****************************************************************************/
/**
** \brief CFE_PSP_Get_Timebase stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_Get_Timebase.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        This function does not return a value.
**
******************************************************************************/
void CFE_PSP_Get_Timebase(uint32 *Tbu, uint32* Tbl)
{
    *Tbu = 0;
    *Tbl = 0;

    UT_Stub_RegisterContext(UT_KEY(CFE_PSP_Get_Timebase), Tbu);
    UT_Stub_RegisterContext(UT_KEY(CFE_PSP_Get_Timebase), Tbl);
    UT_DEFAULT_IMPL(CFE_PSP_Get_Timebase);
}

/*****************************************************************************/
/**
** \brief CFE_PSP_GetResetArea stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_GetResetArea.  It returns the user-defined value,
**        UT_StatusBSP.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns a user-defined status value, UT_StatusBSP.
**
******************************************************************************/
int32 CFE_PSP_GetResetArea(cpuaddr *PtrToResetArea, uint32 *SizeOfResetArea)
{
    int32 status;
    uint32 Position;

    status = UT_DEFAULT_IMPL(CFE_PSP_GetResetArea);

    if (status >= 0)
    {
        UT_GetDataBuffer(UT_KEY(CFE_PSP_GetResetArea), (void**)PtrToResetArea, SizeOfResetArea, &Position);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_AttachExceptions stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_AttachExceptions.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        This function does not return a value.
**
******************************************************************************/
void  CFE_PSP_AttachExceptions(void)
{
    UT_DEFAULT_IMPL(CFE_PSP_AttachExceptions);
}

/*****************************************************************************/
/**
** \brief CFE_PSP_SetDefaultExceptionEnvironment stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_SetDefaultExceptionEnvironment.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        This function does not return a value.
**
******************************************************************************/
void CFE_PSP_SetDefaultExceptionEnvironment(void)
{
    UT_DEFAULT_IMPL(CFE_PSP_SetDefaultExceptionEnvironment);
}

/*****************************************************************************/
/**
** \brief CFE_PSP_GetTimerTicksPerSecond stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_GetTimerTicksPerSecond.  It is set to return a fixed value
**        for the unit tests.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns 2000.
**
******************************************************************************/
uint32 CFE_PSP_GetTimerTicksPerSecond(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL_RC(CFE_PSP_GetTimerTicksPerSecond, 2000);

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_GetTimerLow32Rollover stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_GetTimerLow32Rollover.  It is set to return a fixed value
**        for the unit tests.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns 1000000.
**
******************************************************************************/
uint32 CFE_PSP_GetTimerLow32Rollover(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL_RC(CFE_PSP_GetTimerLow32Rollover, 100000);

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_GetCFETextSegmentInfo stub function
**
** \par Description
**        This function is used to mimic the response of the PSP function
**        CFE_PSP_GetCFETextSegmentInfo.  The user can adjust the response by
**        setting the values in the BSPGetCFETextRtn structure prior to this
**        function being called.  If the value BSPGetCFETextRtn.count is
**        greater than zero then the counter is decremented; if it then equals
**        zero the return value is set to the user-defined value
**        BSPGetCFETextRtn.value.  OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 CFE_PSP_GetCFETextSegmentInfo(cpuaddr *PtrToCFESegment,
                                    uint32 *SizeOfCFESegment)
{
    static uint32 LocalTextSegment;
    int32 status;
    uint32 Position;

    status = UT_DEFAULT_IMPL(CFE_PSP_GetCFETextSegmentInfo);

    if (status >= 0)
    {
        UT_GetDataBuffer(UT_KEY(CFE_PSP_GetCFETextSegmentInfo), (void**)PtrToCFESegment, SizeOfCFESegment, &Position);
        if (*PtrToCFESegment == 0)
        {
            /* Backup -- Set the pointer and size to anything */
            *PtrToCFESegment = (cpuaddr)&LocalTextSegment;
            *SizeOfCFESegment = sizeof(LocalTextSegment);
        }
    }

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_MemRead8 stub function
**
** \par Description
**        This function is used as a placeholder for the PSP function
**        CFE_PSP_MemRead8.  It sets the Data variable to a fixed value and
**        always returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 CFE_PSP_MemRead8(cpuaddr Address, uint8 *Data)
{
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_MemRead8);

    if (status >= 0)
    {
        *Data = 0x01;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_MemValidateRange stub function
**
** \par Description
**        This function is used to mimic the response of the PSP function
**        CFE_PSP_MemValidateRange.  The user can adjust the response by
**        setting the values in the PSPMemValRangeRtn structure prior to this
**        function being called.  If the value PSPMemValRangeRtn.count is
**        greater than zero then the counter is decremented; if it then equals
**        zero the return value is set to the user-defined value
**        PSPMemValRangeRtn.value.  OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 CFE_PSP_MemValidateRange(cpuaddr Address, uint32 Size, uint32 MemoryType)
{
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_MemValidateRange);

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_MemCpy stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        CFE_PSP_MemCpy.  It always returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 CFE_PSP_MemCpy(void *dst, const void *src, uint32 size)
{
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_MemCpy);

    if (status >= 0)
    {
        /* this is not actually a stub; it actually has to _do_ the intended function */
        memcpy(dst, src, size);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief CFE_PSP_MemSet stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        CFE_PSP_MemSet.  It always returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 CFE_PSP_MemSet(void *dst, uint8 value , uint32 size)
{
    int32 status;

    status = UT_DEFAULT_IMPL(CFE_PSP_MemSet);

    if (status >= 0)
    {
        /* this is not actually a stub; it actually has to _do_ the intended function */
        memset(dst, (int)value, (size_t)size);
    }

    return status;
}

