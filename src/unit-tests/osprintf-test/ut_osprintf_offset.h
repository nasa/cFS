/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * ut_osprintf_offset.h
 *
 * Variadic argument offset calculation
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */
#ifdef UT_DO_OFFSET

#ifndef UT_OSPRINTF_OFFSET_H_
#define UT_OSPRINTF_OFFSET_H_

#include "common_types.h"
#include "osapi.h"
#include "cfe_es.h"
#include "cfe_sb.h"
#include "cfe_evs_task.h"
#include "cfe_time.h"
#include "cfe_mission_cfg.h"
#include "osprintf.h"

void UT_ShowHex(unsigned char *, int);
void UT_CheckArgumentOffset(va_list, int);
void UT_osprintf_CalcOffsets(void);
int32 CalcOffset_CFE_ES_WriteToSysLog(const char *, ...);
int32 CalcOffset_EVS_SendEvent(uint16,
                               uint16,
                               const char *,
                               ...);
int32 CalcOffset_CFE_EVS_SendEvent(uint16,
                                   uint16,
                                   const char *,
                                   ...);
int32 CalcOffset_CFE_EVS_SendEventWithAppID(uint16,
                                            uint16,
                                            uint32,
                                            const char *,
                                            ...);
int32 CalcOffset_CFE_EVS_SendTimedEvent(CFE_TIME_SysTime_t,
                                        uint16,
                                        uint16,
                                        const char *,
                                        ...);
void CalcOffset_OS_printf(const char *, ...);
int CalcOffset_OS_sprintf(char *, const char *, ...);
int CalcOffset_OS_snprintf(char *, unsigned, const char *, ...);
void CalcOffset_OS_printf_stub(const char *, ...);
int32 CalcOffset_CFE_ES_WriteToSysLog_stub(const char *, ...);
int32 CalcOffset_CFE_EVS_SendEvent_stub(uint16,
                                        uint16,
                                        const char *,
                                        ...);
int32 CalcOffset_CFE_EVS_SendEventWithAppID_stub(uint16,
                                                 uint16,
                                                 uint32,
                                                 const char *,
                                                 ...);
void CFE_TIME_PrintDummy(char *, CFE_TIME_SysTime_t);
unsigned char EVS_IsFilteredDummy(uint32, uint16,
                                  uint16);
void CFE_SB_InitMsgDummy(void *, uint16, uint16, unsigned char);
CFE_TIME_SysTime_t CFE_TIME_GetTimeDummy(void);
void EVS_SendPacketDummy(uint32, CFE_TIME_SysTime_t,
                         CFE_EVS_Packet_t *);
long int EVS_GetAppIDDummy(uint32 *);
long int EVS_NotRegisteredDummy(uint32);
int OS_vsnprintfDummy(char *, int, const char *, ...);
void OS_printfDummy(const char *, ...);
int OS_snprintfDummy(char *, unsigned, const char *, ...);
char *strcatDummy(char *, const char *);
char *strncatDummy(char *, const char *, int);
char *strncpyDummy(char *, const char *, int);
int strlenDummy(const char *);
void UT_AddEventToHistoryDummy(uint16 EventID);

#endif /* UT_OSPRINTF_OFFSET_H_ */

#endif
