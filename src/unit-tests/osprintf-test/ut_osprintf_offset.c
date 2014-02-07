#ifdef UT_DO_OFFSET

#include "ut_osprintf.h"
#include "ut_osprintf_offset.h"

extern char cMsg[];
extern char cNum[];

unsigned long testPattern = 0x12ab34cd;

void UT_osprintf_CalcOffsets(void)
{
    CFE_TIME_SysTime_t Time = {0, 0};

    /* Determine variadic offsets */
    CalcOffset_CFE_ES_WriteToSysLog("CFE_ES_WriteToSysLog test %ld",
                                    testPattern);
    CalcOffset_EVS_SendEvent(111, 222,
                             "EVS_SendEvent test %ld",
                             testPattern);
    CalcOffset_CFE_EVS_SendEvent(333, 444,
                                 "CFE_EVS_SendEvent test %ld",
                                 testPattern);
    CalcOffset_CFE_EVS_SendEventWithAppID(555, 666, 5,
                                         "CFE_EVS_SendEventWithAppID test %ld",
                                          testPattern);
    CalcOffset_CFE_EVS_SendTimedEvent(Time, 777, 888,
                                      "CFE_EVS_SendTimedEvent test %ld",
                                      testPattern);
}

void UT_ShowHex(unsigned char *ptr, int numBytes)
{
    int i;
    unsigned char *bytePtr = (unsigned char *) ptr;

    strcpy(cMsg, "args = ");

    for (i = 0; i < numBytes; i++)
    {
        OS_sprintf(cNum, "%02x ", *bytePtr);
        strcat(cMsg, cNum);
        bytePtr++;
    }

    UT_Text(cMsg);
}

void UT_CheckArgumentOffset(VA_LIST ptr)
{
    int i = 0;
    int offset = -1;
    int max_allowed_offset = 20;
    unsigned char *bytePtr = (unsigned char *) ptr;
    unsigned char *testPtr = (void *) &testPattern;

    while (offset < max_allowed_offset && i < sizeof(unsigned long))
    {
        offset++;

        for (i = 0; i < sizeof(unsigned long); i++)
        {
            if (*(bytePtr + (offset * sizeof(VA_LIST)) + i) != *(testPtr + i))
            {
                break;
            }
        }
    }

    if (offset == max_allowed_offset)
    {
        offset = -1;
    }

    OS_sprintf(cMsg, "  Offset = %d\n", offset);
    UT_Text(cMsg);
}

/* Mimic CFE_ES_WriteToSysLog() */
long int CalcOffset_CFE_ES_WriteToSysLog(const char *SpecStringPtr, ...)
{
    char TmpString[CFE_ES_MAX_SYSLOG_MSG_SIZE];
    char MsgWithoutTime[CFE_EVS_MAX_MESSAGE_LENGTH];
    CFE_TIME_SysTime_t time = {0, 0};
    VA_LIST ArgPtr;

    VA_START(ArgPtr, SpecStringPtr, 0);
    UT_Text("\nCFE_ES_WriteToSysLog Argument Offset Calculation:\n");
    UT_CheckArgumentOffset(ArgPtr);

    /* These function calls from the original function are needed in
     *  order for the correct offset to be calculated */
    OS_vsnprintfDummy(MsgWithoutTime, 0, SpecStringPtr, ArgPtr);
    CFE_TIME_PrintDummy(TmpString, time);
    strcatDummy(TmpString, " ");
    strncatDummy(TmpString, MsgWithoutTime, 0);
    OS_printfDummy("%s", TmpString);
    strlenDummy(TmpString);
    strncpyDummy(TmpString, TmpString, 0);
    strncpyDummy(TmpString, "\0", 1);
    OS_printfDummy("Warning: Last System Log Message Truncated.\n");
    strlenDummy(TmpString);
    strncpyDummy(TmpString, TmpString, 0);
    return 0;
}

/* Mimic EVS_SendEvent() */
long int CalcOffset_EVS_SendEvent(unsigned short int EventID,
                                  unsigned short int EventType,
                                  const char *Spec,
                                  ...)
{
    CFE_EVS_Packet_t EVS_Packet;
    CFE_TIME_SysTime_t Time = {0, 0};
    VA_LIST Ptr;

    VA_START(Ptr, Spec, 0);
    UT_Text("\nEVS_SendEvent Argument Offset Calculation:\n");
    UT_CheckArgumentOffset(Ptr);

    /* These function calls from the original function are needed in
     *  order for the correct offset to be calculated */
    EVS_IsFilteredDummy(0, EventID, EventType);
    CFE_SB_InitMsgDummy(&EVS_Packet, 0, sizeof(CFE_EVS_Packet_t), 0);
    OS_vsnprintfDummy(EVS_Packet.Message, 0, Spec, Ptr);
    CFE_TIME_GetTimeDummy();
    EVS_SendPacketDummy(0, Time, &EVS_Packet);
    return 0;
}

/* Mimic CFE_EVS_SendEvent() */
long int CalcOffset_CFE_EVS_SendEvent(unsigned short int EventID,
                                      unsigned short int EventType,
                                      const char *Spec,
                                      ...)
{
    CFE_EVS_Packet_t EVS_Packet;
    unsigned long int AppID = 0;
    CFE_TIME_SysTime_t Time = {0, 0};
    VA_LIST Ptr;

    VA_START(Ptr, Spec, 0);
    UT_Text("\nCFE_EVS_SendEvent Argument Offset Calculation:\n");
    UT_CheckArgumentOffset(Ptr);

    /* These function calls from the original function are needed in
     *  order for the correct offset to be calculated */
    EVS_GetAppIDDummy(&AppID);
    EVS_NotRegisteredDummy(AppID);
    EVS_IsFilteredDummy(AppID, EventID, EventType);
    CFE_SB_InitMsgDummy(&EVS_Packet, 0, sizeof(CFE_EVS_Packet_t), 0);
    OS_vsnprintfDummy(EVS_Packet.Message, 0, Spec, Ptr);
    CFE_TIME_GetTimeDummy();
    EVS_SendPacketDummy(AppID, Time, &EVS_Packet);
    return 0;
}

/* Mimic CFE_EVS_SendEventWithAppID() */
long int CalcOffset_CFE_EVS_SendEventWithAppID(unsigned short int EventID,
                                               unsigned short int EventType,
                                               unsigned long int AppID,
                                               const char *Spec,
                                               ...)
{
    CFE_EVS_Packet_t EVS_Packet;
    CFE_TIME_SysTime_t Time = {0, 0};
    VA_LIST Ptr;

    VA_START(Ptr, Spec, 0);
    UT_Text("\nCFE_EVS_SendEventWithAppID Argument Offset Calculation:\n");
    UT_CheckArgumentOffset(Ptr);

    /* These function calls from the original function are needed in
     *  order for the correct offset to be calculated */
    EVS_NotRegisteredDummy(AppID);
    EVS_IsFilteredDummy(AppID, EventID, EventType);
    CFE_SB_InitMsgDummy(&EVS_Packet, 0, sizeof(CFE_EVS_Packet_t), 0);
    OS_vsnprintfDummy(EVS_Packet.Message, 0, Spec, Ptr);
    CFE_TIME_GetTimeDummy();
    EVS_SendPacketDummy(AppID, Time, &EVS_Packet);
    return 0;
}

/* Mimic CFE_EVS_SendTimedEvent() */
long int CalcOffset_CFE_EVS_SendTimedEvent(CFE_TIME_SysTime_t Time,
                                           unsigned short int EventID,
                                           unsigned short int EventType,
                                           const char *Spec,
                                           ...)
{
    CFE_EVS_Packet_t EVS_Packet;
    unsigned long int AppID = 0;
    VA_LIST Ptr;

    VA_START(Ptr, Spec, 0);
    UT_Text("\nCFE_EVS_SendTimedEvent Argument Offset Calculation:\n");
    UT_CheckArgumentOffset(Ptr);

    /* These function calls from the original function are needed in
     *  order for the correct offset to be calculated */
    EVS_GetAppIDDummy(&AppID);
    EVS_NotRegisteredDummy(AppID);
    EVS_IsFilteredDummy(AppID, EventID, EventType);
    CFE_SB_InitMsgDummy(&EVS_Packet, 0, sizeof(CFE_EVS_Packet_t), 0);
    OS_vsnprintfDummy(EVS_Packet.Message, 0, Spec, Ptr);
    EVS_SendPacketDummy(AppID, Time, &EVS_Packet);
    return 0;
}

#endif
