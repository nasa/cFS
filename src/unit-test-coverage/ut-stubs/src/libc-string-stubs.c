/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for string.h */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utstubs.h"

#include <OCS_string.h>

void* OCS_memset(void * s, int c, size_t n)
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

void *OCS_memcpy (void * dest, const void * src, size_t n)
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

char *OCS_strncat(char *dest, const char *src, size_t size)
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

int OCS_strncmp(const char *s1, const char *s2, size_t size)
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

char *OCS_strncpy(char *dst, const char *src, size_t size)
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




