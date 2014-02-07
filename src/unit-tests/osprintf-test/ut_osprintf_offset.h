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

#include "cfe_sb.h"
#include "cfe_evs_task.h"
#include "cfe_time.h"
#include "osprintf.h"

#define OS_MAX_API_NAME 20

void UT_ShowHex(unsigned char *, int);
void UT_CheckArgumentOffset(VA_LIST);
void UT_osprintf_CalcOffsets(void);
long int CalcOffset_CFE_ES_WriteToSysLog(const char *, ...);
long int CalcOffset_EVS_SendEvent(unsigned short int,
                                  unsigned short int,
                                  const char *,
                                  ...);
long int CalcOffset_CFE_EVS_SendEvent(unsigned short int,
                                      unsigned short int,
                                      const char *,
                                      ...);
long int CalcOffset_CFE_EVS_SendEventWithAppID(unsigned short int,
                                               unsigned short int,
                                               unsigned long int,
                                               const char *,
                                               ...);
long int CalcOffset_CFE_EVS_SendTimedEvent(CFE_TIME_SysTime_t,
                                           unsigned short int,
                                           unsigned short int,
                                           const char *,
                                           ...);
void CFE_TIME_PrintDummy(char *, CFE_TIME_SysTime_t);
unsigned char EVS_IsFilteredDummy(unsigned long int, unsigned int,
                                  unsigned int);
void CFE_SB_InitMsgDummy(void *, unsigned int, unsigned int, unsigned char);
CFE_TIME_SysTime_t CFE_TIME_GetTimeDummy(void);
void EVS_SendPacketDummy(unsigned long int, CFE_TIME_SysTime_t,
                         CFE_EVS_Packet_t *);
long int EVS_GetAppIDDummy(unsigned long int *);
long int EVS_NotRegisteredDummy(unsigned long int);
int OS_vsnprintfDummy(char *, int, const char *, VA_LIST);
void OS_printfDummy(const char *, ...);
char *strcatDummy(char *, const char *);
char *strncatDummy(char *, const char *, int);
char *strncpyDummy(char *, const char *, int);
int strlenDummy(const char *);

#endif /* UT_OSPRINTF_OFFSET_H_ */

#endif
