/* OSAL coverage stub replacement for string.h */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utstubs.h"

#include <overrides/string.h>

void* OCS_memset(void * s, int c, OCS_size_t n)
{
    int32 Status;
    void *Result;

    Status = UT_DEFAULT_IMPL(OCS_memset);

    if (Status == 0)
    {
        Result = memset(s, c, n);
    }
    else
    {
        Result = NULL;
    }

    return Result;
}

void *OCS_memcpy (void * dest, const void * src, OCS_size_t n)
{
    int32 Status;
    void *Result;

    Status = UT_DEFAULT_IMPL(OCS_memcpy);

    if (Status == 0)
    {
        Result = memcpy(dest, src, n);
    }
    else
    {
        Result = NULL;
    }

    return Result;
}

char *OCS_strchr(const char *s, int c)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_strchr);

    if (Status == 0)
    {
        /* "nominal" response */
        return strchr(s,c);
    }
    if (Status < 0)
    {
        return (char*)0;
    }

    return (char*)&s[Status-1];
}

char *OCS_strrchr(const char *s, int c)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_strrchr);

    if (Status == 0)
    {
        /* "nominal" response */
        return strrchr(s,c);
    }
    if (Status < 0)
    {
        return (char*)0;
    }

    return (char*)&s[Status-1];
}

size_t OCS_strlen(const char *s)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_strlen, strlen(s));

    return Status;
}

char *OCS_strcat(char *dest, const char *src)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_strcat);

    if (Status == 0)
    {
        /* "nominal" response */
        return strcat(dest,src);
    }

    return (char*)0;
}

char *OCS_strncat(char *dest, const char *src, OCS_size_t size)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_strncat);

    if (Status == 0)
    {
        /* "nominal" response */
        return strncat(dest,src,size);
    }

    return (char*)0;
}

int OCS_strncmp(const char *s1, const char *s2, OCS_size_t size)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_strncmp, strncmp(s1,s2,size));

    return Status;
}

int OCS_strcmp(const char *s1, const char *s2)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_strcmp, strcmp(s1,s2));

    return Status;
}

char *OCS_strcpy(char *dst, const char *src)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_strcpy);

    if (Status == 0)
    {
        /* "nominal" response */
        return strcpy(dst,src);
    }

    return (char*)0;
}

char *OCS_strncpy(char *dst, const char *src, OCS_size_t size)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_strncpy);

    if (Status == 0)
    {
        /* "nominal" response */
        return strncpy(dst,src,size);
    }

    return (char*)0;
}

char *OCS_strerror(int errnum)
{
    static char str[16];
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_strerror);

    if (Status != 0)
    {
        return NULL;
    }

    /* "nominal" response */
    snprintf(str,sizeof(str),"UT_ERR_%d", errnum);
    return str;
}




