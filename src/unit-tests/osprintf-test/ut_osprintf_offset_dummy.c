#ifdef UT_DO_OFFSET

#include "ut_osprintf.h"
#include "ut_osprintf_offset.h"

void CFE_TIME_PrintDummy(char *PrintBuffer, CFE_TIME_SysTime_t TimeToPrint)
{
}

unsigned char EVS_IsFilteredDummy(uint32 AppID,
                                  uint16 EventID, uint16 EventType)
{
    return 0;
}

void CFE_SB_InitMsgDummy(void *MsgPtr,
                         uint16 MsgId,
                         uint16 Length,
                         unsigned char Clear)
{
}

CFE_TIME_SysTime_t CFE_TIME_GetTimeDummy(void)
{
    CFE_TIME_SysTime_t Time = {0, 0};

    return Time;
}

void EVS_SendPacketDummy(uint32 AppID, CFE_TIME_SysTime_t Time,
                         CFE_EVS_Packet_t *EVS_PktPtr)
{
}

long int EVS_GetAppIDDummy(uint32 *AppIdPtr)
{
    return 0;
}

long int EVS_NotRegisteredDummy(uint32 AppID)
{
    return 0;
}

int OS_vsnprintfDummy(char *out_buffer, int max_len,
                      const char *format, ...)
{
    return 0;
}

void OS_printfDummy(const char *format, ...)
{
}

int OS_snprintfDummy(char *out_buffer, unsigned max_len, const char *format, ...)
{
    return 0;
}

char *strcatDummy(char *out, const char *in)
{
    return out;
}

char *strncatDummy(char *out, const char *in, int len)
{
    return out;
}

char *strncpyDummy(char *out, const char *in, int len)
{
    return out;
}

int strlenDummy(const char *in)
{
    return 0;
}

void UT_AddEventToHistoryDummy(uint16 EventID)
{
}
#endif
