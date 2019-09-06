/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osapi-utstub-sockets.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"

UT_DEFAULT_STUB(OS_SocketAPI_Init,(void))

/*****************************************************************************
 *
 * Stub function for OS_SocketOpen()
 *
 *****************************************************************************/
int32 OS_SocketOpen(uint32 *sock_id, OS_SocketDomain_t Domain, OS_SocketType_t Type)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketOpen);

    if (status == OS_SUCCESS)
    {
        status = UT_AllocStubObjId(UT_OBJTYPE_SOCKET);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketBind()
 *
 *****************************************************************************/
int32 OS_SocketBind(uint32 sock_id, const OS_SockAddr_t *Addr)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketBind);

    return status;
}
/*****************************************************************************
 *
 * Stub function for OS_SocketAccept()
 *
 *****************************************************************************/
int32 OS_SocketAccept(uint32 sock_id, uint32 *connsock_id, OS_SockAddr_t *Addr, int32 timeout)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAccept);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketConnect()
 *
 *****************************************************************************/
int32 OS_SocketConnect(uint32 sock_id, const OS_SockAddr_t *Addr, int32 Timeout)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketConnect);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketRecvFrom()
 *
 *****************************************************************************/
int32 OS_SocketRecvFrom(uint32 sock_id, void *buffer, uint32 buflen, OS_SockAddr_t *RemoteAddr, int32 timeout)
{
    int32 status;
    uint32 CopySize;

    UT_Stub_RegisterContext(UT_KEY(OS_SocketRecvFrom), RemoteAddr);

    status = UT_DEFAULT_IMPL(OS_SocketRecvFrom);

    if (status == 0x7FFFFFFF)
    {
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_SocketRecvFrom), buffer, buflen);

        /* If CopyToLocal returns zero, this probably means no buffer was supplied,
         * in which case just generate fill data and pretend it was read.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            memset(buffer, 0, buflen);
            status = buflen;
        }
    }
    else if (status > 0)
    {
        /* generate fill data for requested size */
        memset(buffer, 0, status);
    }


    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketSendTo()
 *
 *****************************************************************************/
int32 OS_SocketSendTo(uint32 sock_id, const void *buffer, uint32 buflen, const OS_SockAddr_t *RemoteAddr)
{
    int32 status;
    uint32 CopySize;

    UT_Stub_RegisterContext(UT_KEY(OS_SocketSendTo), RemoteAddr);

    status = UT_DEFAULT_IMPL_RC(OS_SocketSendTo, 0x7FFFFFFF);

    if (status == 0x7FFFFFFF)
    {
        CopySize = UT_Stub_CopyFromLocal(UT_KEY(OS_SocketSendTo), buffer, buflen);

        /* If CopyFromLocal returns zero, this probably means no buffer was supplied,
         * in which case just throw out the data and pretend it was written.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            status = buflen;
        }
    }

    return status;
}


/*****************************************************************************
 *
 * Stub function for OS_SocketGetIdByName()
 *
 *****************************************************************************/
int32 OS_SocketGetIdByName (uint32 *sock_id, const char *sock_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketGetIdByName);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_SocketGetIdByName), sock_id, sizeof(*sock_id)) < sizeof(*sock_id))
    {
        *sock_id =  1;
        UT_FIXUP_ID(*sock_id, UT_OBJTYPE_SOCKET);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketGetInfo()
 *
 *****************************************************************************/
int32 OS_SocketGetInfo (uint32 sock_id, OS_socket_prop_t *sock_prop)
{
    int32 status;
    uint32 CopySize;

    status = UT_DEFAULT_IMPL(OS_SocketGetInfo);

    if (status == OS_SUCCESS)
    {
        /* The user may supply specific entries to return */
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_SocketGetInfo), sock_prop, sizeof(*sock_prop));
        if (CopySize < sizeof(*sock_prop))
        {
            sock_prop->creator = 1;
            UT_FIXUP_ID(sock_prop->creator, UT_OBJTYPE_TASK);
            strncpy(sock_prop->name, "ut", sizeof(sock_prop->name));
        }
    }

    return status;
}


int32 OS_SocketAddrInit(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrInit);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrInit), Addr, sizeof(*Addr)) < sizeof(*Addr))
    {
        memset(Addr, 0, sizeof(*Addr));
    }

    return status;
}

int32 OS_SocketAddrToString(char *buffer, uint32 buflen, const OS_SockAddr_t *Addr)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrToString);

    if (status == OS_SUCCESS && buflen > 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrToString), buffer, buflen) == 0)
    {
        strncpy(buffer, "UT-addr", buflen-1);
        buffer[buflen-1] = 0;
    }

    return status;
}

int32 OS_SocketAddrFromString(OS_SockAddr_t *Addr, const char *string)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrFromString);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrFromString), Addr, sizeof(*Addr)) < sizeof(*Addr))
    {
        memset(Addr, 0, sizeof(*Addr));
    }

    return status;
}

int32 OS_SocketAddrGetPort(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrGetPort);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrGetPort), PortNum, sizeof(*PortNum)) < sizeof(*PortNum))
    {
        *PortNum = 0;
    }

    return status;
}

int32 OS_SocketAddrSetPort(OS_SockAddr_t *Addr, uint16 PortNum)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrSetPort);

    return status;
}

