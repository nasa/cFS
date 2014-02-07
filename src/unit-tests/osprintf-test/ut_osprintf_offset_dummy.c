#ifdef UT_DO_OFFSET

#include "ut_osprintf.h"
#include "ut_osprintf_offset.h"

void CFE_TIME_PrintDummy(char *PrintBuffer, CFE_TIME_SysTime_t TimeToPrint)
{
}

unsigned char EVS_IsFilteredDummy(unsigned long int AppID,
                                  unsigned int EventID, unsigned int EventType)
{
    return 0;
}

void CFE_SB_InitMsgDummy(void *MsgPtr,
                         unsigned int MsgId,
                         unsigned int Length,
                         unsigned char Clear)
{
}

CFE_TIME_SysTime_t CFE_TIME_GetTimeDummy(void)
{
    CFE_TIME_SysTime_t Time = {0, 0};

    return Time;
}

void EVS_SendPacketDummy(unsigned long int AppID, CFE_TIME_SysTime_t Time,
                         CFE_EVS_Packet_t *EVS_PktPtr)
{
}

long int EVS_GetAppIDDummy(unsigned long int *AppIdPtr)
{
    return 0;
}

long int EVS_NotRegisteredDummy(unsigned long int AppID)
{
    return 0;
}

int OS_vsnprintfDummy(char *out_buffer, int max_len,
                      const char *format, VA_LIST varg)
{
    return 0;
}

void OS_printfDummy(const char *format, ...)
{
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

#endif
