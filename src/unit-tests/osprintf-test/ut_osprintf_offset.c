/* Calculate offset, breakpoint, and skip values for the cFE and cFE unit
 * test variadic functions.  Note that some "dummy" function calls from
 * the original function are needed in order for the correct offset,
 * breakpoint, and skip values to be calculated.  The ordering of these
 * dummy functions must match that in the actual function
 */
#ifdef UT_DO_OFFSET

#include "ut_osprintf.h"
#include "ut_osprintf_offset.h"

extern char cMsg[];
extern char cNum[];

extern int OS_printf_break;
extern int OS_printf_skip;
extern int OS_printf_enabled;


unsigned long testPattern[12][10] =
{
    {
        0x12ab34cd,
        0x19999991,
        0x18888881,
        0x17777771,
        0x16666661,
        0x15555551,
        0x14444441,
        0x13333331,
        0x12222221,
        0x11111111
    },
    {
        0x23bc45de,
        0x29999992,
        0x28888882,
        0x27777772,
        0x26666662,
        0x25555552,
        0x24444442,
        0x23333332,
        0x22222222,
        0x21111112
    },
    {
        0x34cd56ef,
        0x39999993,
        0x38888883,
        0x37777773,
        0x36666663,
        0x35555553,
        0x34444443,
        0x33333333,
        0x32222223,
        0x31111113
    },
    {
        0x45ab67cd,
        0x49999994,
        0x48888884,
        0x47777774,
        0x46666664,
        0x45555554,
        0x44444444,
        0x43333334,
        0x42222224,
        0x41111114
    },
    {
        0x56bc78de,
        0x59999995,
        0x58888885,
        0x57777775,
        0x56666665,
        0x55555555,
        0x54444445,
        0x53333335,
        0x52222225,
        0x51111115
    },
    {
        0x67cd78ef,
        0x69999996,
        0x68888886,
        0x67777776,
        0x66666666,
        0x65555556,
        0x64444446,
        0x63333336,
        0x62222226,
        0x61111116
    },
    {
        0x78ab9acd,
        0x79999997,
        0x78888887,
        0x77777777,
        0x76666667,
        0x75555557,
        0x74444447,
        0x73333337,
        0x72222227,
        0x71111117
    },
    {
        0x89bcabde,
        0x89999998,
        0x88888888,
        0x87777778,
        0x86666668,
        0x85555558,
        0x84444448,
        0x83333338,
        0x82222228,
        0x81111118
    },
    {
        0x9acdbcef,
        0x99999999,
        0x98888889,
        0x97777779,
        0x96666669,
        0x95555559,
        0x94444449,
        0x93333339,
        0x92222229,
        0x91111119
    },
    {
        0xababcdcd,
        0xa999999a,
        0xa888888a,
        0xa777777a,
        0xa666666a,
        0xa555555a,
        0xa444444a,
        0xa333333a,
        0xa222222a,
        0xa111111a
    },
    {
        0xbcbcdede,
        0xb999999b,
        0xb888888b,
        0xb777777b,
        0xb666666b,
        0xb555555b,
        0xb444444b,
        0xb333333b,
        0xb222222b,
        0xb111111b
    },
    {
        0xcdcdefef,
        0xc999999c,
        0xc888888c,
        0xc777777c,
        0xc666666c,
        0xc555555c,
        0xc444444c,
        0xc333333c,
        0xc222222c,
        0xc111111c
    }
};

void UT_osprintf_CalcOffsets(void)
{
    CFE_TIME_SysTime_t Time = {0, 0};

    /* Determine variadic offsets */
    CalcOffset_CFE_ES_WriteToSysLog("CFE_ES_WriteToSysLog test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                                    testPattern[0][0], testPattern[0][1],
                                    testPattern[0][2], testPattern[0][3],
                                    testPattern[0][4], testPattern[0][5],
                                    testPattern[0][6], testPattern[0][7],
                                    testPattern[0][8], testPattern[0][9]);
    CalcOffset_EVS_SendEvent(111, 222,
                             "EVS_SendEvent test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                             testPattern[1][0], testPattern[1][1],
                             testPattern[1][2], testPattern[1][3],
                             testPattern[1][4], testPattern[1][5],
                             testPattern[1][6], testPattern[1][7],
                             testPattern[1][8], testPattern[1][9]);
    CalcOffset_CFE_EVS_SendEvent(333, 444,
                                 "CFE_EVS_SendEvent test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                                 testPattern[2][0], testPattern[2][1],
                                 testPattern[2][2], testPattern[2][3],
                                 testPattern[2][4], testPattern[2][5],
                                 testPattern[2][6], testPattern[2][7],
                                 testPattern[2][8], testPattern[2][9]);
    CalcOffset_CFE_EVS_SendEventWithAppID(555, 666, 7,
                                         "CFE_EVS_SendEventWithAppID test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                                         testPattern[3][0], testPattern[3][1],
                                         testPattern[3][2], testPattern[3][3],
                                         testPattern[3][4], testPattern[3][5],
                                         testPattern[3][6], testPattern[3][7],
                                         testPattern[3][8], testPattern[3][9]);
    CalcOffset_CFE_EVS_SendTimedEvent(Time, 888, 999,
                                      "CFE_EVS_SendTimedEvent test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                                      testPattern[4][0], testPattern[4][1],
                                      testPattern[4][2], testPattern[4][3],
                                      testPattern[4][4], testPattern[4][5],
                                      testPattern[4][6], testPattern[4][7],
                                      testPattern[4][8], testPattern[4][9]);
    CalcOffset_OS_printf("OS_printf test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                         testPattern[5][0], testPattern[5][1],
                         testPattern[5][2], testPattern[5][3],
                         testPattern[5][4], testPattern[5][5],
                         testPattern[5][6], testPattern[5][7],
                         testPattern[5][8], testPattern[5][9]);
    CalcOffset_OS_sprintf(cMsg, "OS_sprintf test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                          testPattern[6][0], testPattern[6][1],
                          testPattern[6][2], testPattern[6][3],
                          testPattern[6][4], testPattern[6][5],
                          testPattern[6][6], testPattern[6][7],
                          testPattern[6][8], testPattern[6][9]);
    CalcOffset_OS_snprintf(cMsg, 100, "OS_snprintf test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                           testPattern[7][0], testPattern[7][1],
                           testPattern[7][2], testPattern[7][3],
                           testPattern[7][4], testPattern[7][5],
                           testPattern[7][6], testPattern[7][7],
                           testPattern[7][8], testPattern[7][9]);
    CalcOffset_OS_printf_stub("OS_printf stub test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                         testPattern[8][0], testPattern[8][1],
                         testPattern[8][2], testPattern[8][3],
                         testPattern[8][4], testPattern[8][5],
                         testPattern[8][6], testPattern[8][7],
                         testPattern[8][8], testPattern[8][9]);
    CalcOffset_CFE_ES_WriteToSysLog_stub("CFE_ES_WriteToSysLog stub test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                                         testPattern[9][0], testPattern[9][1],
                                         testPattern[9][2], testPattern[9][3],
                                         testPattern[9][4], testPattern[9][5],
                                         testPattern[9][6], testPattern[9][7],
                                         testPattern[9][8], testPattern[9][9]);
    CalcOffset_CFE_EVS_SendEvent_stub(333, 444,
                                      "CFE_EVS_SendEvent stub test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                                      testPattern[10][0], testPattern[10][1],
                                      testPattern[10][2], testPattern[10][3],
                                      testPattern[10][4], testPattern[10][5],
                                      testPattern[10][6], testPattern[10][7],
                                      testPattern[10][8], testPattern[10][9]);
    CalcOffset_CFE_EVS_SendEventWithAppID_stub(555, 666, 5,
                                               "CFE_EVS_SendEventWithAppID stub test %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                                               testPattern[11][0], testPattern[11][1],
                                               testPattern[11][2], testPattern[11][3],
                                               testPattern[11][4], testPattern[11][5],
                                               testPattern[11][6], testPattern[11][7],
                                               testPattern[11][8], testPattern[11][9]);
}

void UT_ShowHex(unsigned char *ptr, int numBytes)
{
    int i, j;
    unsigned char *bytePtr = (unsigned char *) ptr;
    char hMsg[3000];

    strcpy(hMsg, "args = ");

    for (i = 0, j = 1; i < numBytes; i++, j++)
    {
        UT_itoa(*bytePtr, cNum, 16, 2);
        strcat(hMsg, cNum);
        strcat(hMsg, " ");
        bytePtr++;

        if (j == 4)
        {
            strcat(hMsg, "\n       ");
            j = 0;
        }
    }

    UT_Text(hMsg);
}

/* Determine the offset, breakpoint, and skip values for a variadic function */
void UT_CheckArgumentOffset(va_list ptr, int tpIndex)
{
    int i = 0;
    int offset = -1;
    int breakpoint = 0;
    int skip = 0;
    int max_allowed_offset = 20;
    int max_allowed_skip = 100;
    int num_parms = 10;
    unsigned char *bytePtr = (unsigned char *) ptr;
    unsigned char *testPtr = (void *) &testPattern[tpIndex][0];

/*
    UT_ShowHex(ptr, 440);
*/

    while (offset < max_allowed_offset && i < sizeof(unsigned long))
    {
        offset++;

        for (i = 0; i < sizeof(unsigned long); i++)
        {
            if (*(bytePtr + (offset * sizeof(va_list)) + i) != *(testPtr + i))
            {
                break;
            }
        }
    }

    if (offset == max_allowed_offset)
    {
        UT_Text("  Offset could not be determined\n");
    }
    else
    {
        sprintf(cMsg, "  Offset = %d\n", offset);
        UT_Text(cMsg);
        i = sizeof(unsigned long);

        /* Find breakpoint value */
        while (offset < max_allowed_offset + num_parms && i == sizeof(unsigned long))
        {
            offset++;
            breakpoint++;
            testPtr += sizeof(unsigned long);

            for (i = 0; i < sizeof(unsigned long); i++)
            {
                if (*(bytePtr + (offset * sizeof(va_list)) + i) != *(testPtr + i))
                {
                    break;
                }
            }
        }

        if (breakpoint == num_parms)
        {
            UT_Text("  No breakpoint found\n");
        }
        else
        {
            sprintf(cMsg, "  Breakpoint = %d\n", breakpoint);
            UT_Text(cMsg);

            /* Find skip value */
            while (offset < max_allowed_offset + num_parms + max_allowed_skip && i < sizeof(unsigned long))
            {
                offset++;
                skip++;

                for (i = 0; i < sizeof(unsigned long); i++)
                {
                    if (*(bytePtr + (offset * sizeof(va_list)) + i) != *(testPtr + i))
                    {
                        break;
                    }
                }
            }

            if (skip == max_allowed_skip)
            {
                UT_Text("  Skip could not be determined\n");
            }
            else
            {
                sprintf(cMsg, "  Skip = %d\n", skip);
                UT_Text(cMsg);
            }
        }
    }
}

/* Mimic actual CFE_ES_WriteToSysLog() */
int32 CalcOffset_CFE_ES_WriteToSysLog(const char *SpecStringPtr, ...)
{
    char TmpString[CFE_ES_MAX_SYSLOG_MSG_SIZE];
    char MsgWithoutTime[CFE_EVS_MAX_MESSAGE_LENGTH];
    CFE_TIME_SysTime_t time = {0, 0};
    va_list Ptr;

    va_start(Ptr, SpecStringPtr, 0, 0, 0);
    OS_vsnprintfDummy(MsgWithoutTime, 1, SpecStringPtr, Ptr);
    va_end(Ptr);
    CFE_TIME_PrintDummy(TmpString, time);
    strcatDummy(TmpString, " ");
    strncatDummy(TmpString, MsgWithoutTime, 1);
    OS_printfDummy("%s", TmpString);
    strlenDummy(TmpString);
    strncpyDummy(TmpString, TmpString, 1);
    strncpyDummy(TmpString, "\0", 1);
    OS_printfDummy("Warning: Last System Log Message Truncated.\n");
    strlenDummy(TmpString);
    strncpyDummy(TmpString, TmpString, 1);

    UT_Text("\nCFE_ES_WriteToSysLog Argument Calculation:\n");
    UT_CheckArgumentOffset(Ptr, 0);
    return 0;
}

/* Mimic actual EVS_SendEvent() */
int32 CalcOffset_EVS_SendEvent(uint16 EventID,
                               uint16 EventType,
                               const char *Spec,
                               ...)
{
    CFE_EVS_Packet_t EVS_Packet;
    CFE_TIME_SysTime_t Time = {0, 0};
    va_list Ptr;

    EVS_IsFilteredDummy(0, EventID, EventType);
    CFE_SB_InitMsgDummy(&EVS_Packet, 1, sizeof(CFE_EVS_Packet_t), 1);
    va_start(Ptr, Spec, 0, 0, 0);
    OS_vsnprintfDummy(EVS_Packet.Message, 1, Spec, Ptr);
    va_end(Ptr);
    CFE_TIME_GetTimeDummy();
    EVS_SendPacketDummy(0, Time, &EVS_Packet);

    UT_Text("\nEVS_SendEvent Argument Calculation:\n");
    UT_CheckArgumentOffset(Ptr, 1);
    return 0;
}

/* Mimic actual CFE_EVS_SendEvent() */
int32 CalcOffset_CFE_EVS_SendEvent(uint16 EventID,
                                   uint16 EventType,
                                   const char *Spec,
                                   ...)
{
    CFE_EVS_Packet_t EVS_Packet;
    uint32 AppID = 0;
    CFE_TIME_SysTime_t Time = {0, 0};
    va_list Ptr;

    EVS_GetAppIDDummy(&AppID);
    EVS_NotRegisteredDummy(AppID);
    EVS_IsFilteredDummy(AppID, EventID, EventType);
    CFE_SB_InitMsgDummy(&EVS_Packet, 1, sizeof(CFE_EVS_Packet_t), 1);
    va_start(Ptr, Spec, 0, 0, 0);
    OS_vsnprintfDummy(EVS_Packet.Message, 1, Spec, Ptr);
    va_end(Ptr);
    CFE_TIME_GetTimeDummy();
    EVS_SendPacketDummy(AppID, Time, &EVS_Packet);

    UT_Text("\nCFE_EVS_SendEvent Argument Calculation:\n");
    UT_CheckArgumentOffset(Ptr, 2);
    return 0;
}

/* Mimic actual CFE_EVS_SendEventWithAppID() */
/* THIS IS RETURNING THE WRONG SKIP VALUE!!!  (off by -2) */
int32 CalcOffset_CFE_EVS_SendEventWithAppID(uint16 EventID,
                                            uint16 EventType,
                                            uint32 AppID,
                                            const char *Spec,
                                            ...)
{
    CFE_EVS_Packet_t EVS_Packet;
    CFE_TIME_SysTime_t Time = {0, 0};
    va_list Ptr;

    EVS_NotRegisteredDummy(AppID);
    EVS_IsFilteredDummy(AppID, EventID, EventType);
    CFE_SB_InitMsgDummy(&EVS_Packet, 1, sizeof(CFE_EVS_Packet_t), 1);
    va_start(Ptr, Spec, 0, 0, 0);
    OS_vsnprintfDummy(EVS_Packet.Message, 1, Spec, Ptr);
    va_end(Ptr);
    CFE_TIME_GetTimeDummy();
    EVS_SendPacketDummy(AppID, Time, &EVS_Packet);

    UT_Text("\nCFE_EVS_SendEventWithAppID Argument Calculation:\n");
    UT_CheckArgumentOffset(Ptr, 3);
    return 0;
}

/* Mimic actual CFE_EVS_SendTimedEvent() */
int32 CalcOffset_CFE_EVS_SendTimedEvent(CFE_TIME_SysTime_t Time,
                                        uint16 EventID,
                                        uint16 EventType,
                                        const char *Spec,
                                        ...)
{
    CFE_EVS_Packet_t EVS_Packet;
    uint32 AppID = 0;
    va_list Ptr;

    EVS_GetAppIDDummy(&AppID);
    EVS_NotRegisteredDummy(AppID);
    EVS_IsFilteredDummy(AppID, EventID, EventType);
    CFE_SB_InitMsgDummy(&EVS_Packet, 0, sizeof(CFE_EVS_Packet_t), 0);
    va_start(Ptr, Spec, 0, 0, 0);
    OS_vsnprintfDummy(EVS_Packet.Message, 0, Spec, Ptr);
    va_end(Ptr);
    EVS_SendPacketDummy(AppID, Time, &EVS_Packet);

    UT_Text("\nCFE_EVS_SendTimedEvent Argument Calculation:\n");
    UT_CheckArgumentOffset(Ptr, 4);
    return 0;
}

/* Mimic actual OS_printf() */
void CalcOffset_OS_printf(const char *format, ...)
{
    va_list varg;

    va_start(varg, format, 0, 0, 0);
    OS_vsnprintfDummy(0, -1, format, varg);
    va_end(varg);

    UT_Text("\nOS_printf Argument Calculation:\n");
    UT_CheckArgumentOffset(varg, 5);
}

/* Mimic actual and stub OS_sprintf() */
int CalcOffset_OS_sprintf(char *out, const char *format, ...)
{
    va_list varg;
    int length;

    va_start(varg, format, 0, 0, 0);
    length = OS_vsnprintfDummy(out, -1, format, varg);
    va_end(varg);

    UT_Text("\nOS_sprintf Argument Calculation:\n");
    UT_CheckArgumentOffset(varg, 6);
    return(length);
}

/* Mimic actual and stub OS_snprintf() */
int CalcOffset_OS_snprintf(char *out, unsigned max_len, const char *format, ...)
{
    va_list varg;
    int length;

    va_start(varg, format, 0, 0, 0);
    length = OS_vsnprintfDummy(out, (int) max_len - 1, format, varg);
    va_end(varg);

    UT_Text("\nOS_snprintf Argument Calculation:\n");
    UT_CheckArgumentOffset(varg, 7);
    return(length);
}

/* Mimic stub OS_printf() */
void CalcOffset_OS_printf_stub(const char *string, ...)
{
    char    tmpString[CFE_ES_MAX_SYSLOG_MSG_SIZE * 2];
    va_list ptr;

    va_start(ptr, string, 0, 0, 0);
    OS_vsnprintfDummy(tmpString, CFE_ES_MAX_SYSLOG_MSG_SIZE * 2, string, ptr);
    va_end(ptr);

    UT_Text("\nOS_printf Stub Argument Calculation:\n");
    UT_CheckArgumentOffset(ptr, 8);
}

int32 CalcOffset_CFE_ES_WriteToSysLog_stub(const char *pSpecString, ...)
{
    char    tmpString[CFE_ES_MAX_SYSLOG_MSG_SIZE];
    va_list ap;

    va_start(ap, pSpecString, 0, 0, 0);
    OS_vsnprintfDummy(tmpString, CFE_ES_MAX_SYSLOG_MSG_SIZE, pSpecString, ap);
    va_end(ap);

    UT_Text("\nCFE_ES_WriteToSysLog Stub Argument Calculation:\n");
    UT_CheckArgumentOffset(ap, 9);
    return 0;
}

/* Mimic stub CFE_EVS_SendEvent() */
int32 CalcOffset_CFE_EVS_SendEvent_stub(uint16 EventID,
                                        uint16 EventType,
                                        const char *Spec,
                                        ...)
{
    char    BigBuf[CFE_EVS_MAX_MESSAGE_LENGTH];
    va_list Ptr;

    va_start(Ptr, Spec, 0, 0, 0);
    OS_vsnprintfDummy(BigBuf, CFE_EVS_MAX_MESSAGE_LENGTH, Spec, Ptr);
    va_end(Ptr);
    UT_AddEventToHistoryDummy(EventID);

    UT_Text("\nCFE_EVS_SendEvent Stub Argument Calculation:\n");
    UT_CheckArgumentOffset(Ptr, 10);
    return 0;
}

/* Mimic stub CFE_EVS_SendEventWithAppID() */
int32 CalcOffset_CFE_EVS_SendEventWithAppID_stub(uint16 EventID,
                                                 uint16 EventType,
                                                 uint32 AppID,
                                                 const char *Spec,
                                                 ...)
{
    char    BigBuf[CFE_EVS_MAX_MESSAGE_LENGTH];
    va_list Ptr;

    va_start(Ptr, Spec, 0, 0, 0);
    OS_vsnprintfDummy(BigBuf, CFE_EVS_MAX_MESSAGE_LENGTH, Spec, Ptr);
    va_end(Ptr);
    UT_AddEventToHistoryDummy(EventID);
    OS_snprintfDummy(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "  CFE_EVS_SendEvent from app %lu: %u, %u - %s",
                     AppID, EventID, EventType, BigBuf);

    UT_Text("\nCFE_EVS_SendEventWithAppID Stub Argument Calculation:\n");
    UT_CheckArgumentOffset(Ptr, 11);
    return 0;
}

#endif
