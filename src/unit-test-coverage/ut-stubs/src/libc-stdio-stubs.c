/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for stdio.h */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utstubs.h"

#include <OCS_stdio.h>

struct OCS_FILE
{
    int f;
};

#define OCS_STDIO_MAX_SIZE      0x01000000

int OCS_fclose (OCS_FILE * stream)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_fclose);

    return Status;
}

char *OCS_fgets (char * s, int n, OCS_FILE * stream)
{
    int32 Status;
    uint32 CopySize;

    Status = UT_DEFAULT_IMPL_RC(OCS_fgets, OCS_STDIO_MAX_SIZE);

    if (Status > 0)
    {
        if (Status > n)
        {
            CopySize = n;
        }
        else
        {
            CopySize = Status;
        }

        CopySize = UT_Stub_CopyToLocal(UT_KEY(OCS_fgets), s, CopySize);

        if (CopySize != 0)
        {
            Status = CopySize;
        }
        else if (Status <= n)
        {
            memset(s, 'x', Status);
        }
        else if (UT_GetStubCount(UT_KEY(OCS_fgets) < 4))
        {
            memset(s, 'x', n);
            Status = n;
        }
        else
        {
            Status = 0;
        }
    }

    if (Status <= 0)
    {
        return NULL;
    }

    return s;
}

OCS_FILE *OCS_fopen (const char * filename, const char * modes)
{
    int32 Status;
    OCS_FILE *retval;
    static OCS_FILE FOPEN_FP = { 0 };

    Status = UT_DEFAULT_IMPL(OCS_fopen);

    if (Status == 0)
    {
        retval = &FOPEN_FP;
    }
    else
    {
        retval = NULL;
    }

    return retval;
}

int OCS_fputs (const char * s, OCS_FILE * stream)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_fputs);

    return Status;
}

int OCS_putchar (int c)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_putchar);

    return Status;
}


int OCS_remove (const char * filename)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_remove);

    return Status;
}

int OCS_rename (const char * old, const char * nw)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_rename);

    return Status;
}

int OCS_snprintf (char * s, size_t maxlen, const char * format, ...)
{
    int32 Status;
    int actual = 0;
    va_list ap;

    Status = UT_DEFAULT_IMPL(OCS_snprintf);

    /* need to actually _do_ the snprintf */
    if (Status >= 0)
    {
        va_start(ap, format);
        actual = vsnprintf(s,maxlen,format,ap);
        va_end(ap);
    }

    if (Status != 0)
    {
        actual = Status;
    }

    return actual;
}

int OCS_vsnprintf (char * s, size_t maxlen, const char * format, OCS_va_list arg)
{
    int32 Status;
    int actual = 0;

    Status = UT_DEFAULT_IMPL(OCS_vsnprintf);

    /* need to actually _do_ something here -
     * cannot do the real vsnprintf because we lost the args. */
    if (Status >= 0)
    {
        actual = snprintf(s,maxlen,"%s",format);
    }

    if (Status != 0)
    {
        actual = Status;
    }

    return actual;
}

int OCS_printf (const char * format, ...)
{
    return UT_DEFAULT_IMPL(OCS_printf);
}


static OCS_FILE LOCAL_FP[3] = { { 10 }, { 11 }, { 12 } };

OCS_FILE*    OCS_stdin = &LOCAL_FP[0];
OCS_FILE*    OCS_stdout = &LOCAL_FP[1];
OCS_FILE*    OCS_stderr = &LOCAL_FP[2];


