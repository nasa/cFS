/*******************************************************************************
* File: trans_udp.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
* Purpose:
*   Provides the functionality to communicate over a UDP socket. 
*   Supports POSIX.
*
* Reference:
*
* Notes:
*   1. Socket set to blocking with timeouts.
*   2. Set timeouts to IO_TRANS_PEND_FOREVER or 0 to block forever.
*   3. Timeouts for socket do not affect behavior of select if used.
*   4. Use the IO_TransUdpRcv function if used with trans_select library.
*
* History:
*   Apr 07, 2015  Guy de Carufel * Created
*   June 2, 2015  Guy de Carufel * Revised API
*
******************************************************************************/

#include <errno.h>
#include <string.h>

#include "trans_udp.h"

/** Initialize (create, configure and bind) a UDP Socket */
int32 IO_TransUdpInit(IO_TransUdpConfig_t * config, IO_TransUdp_t * udp)
{
    /* Create socket */
    if (IO_TransUdpCreateSocket(udp) < 0)
    {
        return IO_TRANS_UDP_SOCKETCREATE_ERROR;
    }

    /* Configure socket */
    if (IO_TransUdpConfigSocket(config, udp) < 0)
    {
        return IO_TRANS_UDP_SOCKETOPT_ERROR;
    }

    /* Bind socket */
    if (IO_TransUdpBindSocket(udp) < 0)
    {
        return IO_TRANS_UDP_SOCKETBIND_ERROR;
    }

    return udp->sockId;
}


/** Create a IPv4 Datagram UDP Socket */
int32 IO_TransUdpCreateSocket(IO_TransUdp_t *udp)
{
    if (udp == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: Null input argument. ");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }
    
    /* Create socket */
    /* AF_INET: IPv4 */
    /* SOCK_DGRAM: Datagram socket */
    /* IPPROTO_UDP:  UDP socket */
    udp->sockId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        
    if (udp->sockId < 0)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: create socket failed. " 
                          "errno:%d", errno);
    }

    return udp->sockId;
}


/** Set the UDP Socket sockAddr structure */
int32 IO_TransUdpConfigSocket(IO_TransUdpConfig_t *config, IO_TransUdp_t *udp)
{
    int32  status = IO_TRANS_UDP_NO_ERROR;
    uint32 uiAddr = INADDR_ANY; 
    struct timeval timeout;
    
    if (udp == NULL || config == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: Null input argument. ");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }

    if (udp->sockId < 0)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                          "IO_TransUDP Error: Socket not created. "
                          "Can't config.");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }

    if ((config->timeoutRcv < 0 && config->timeoutRcv != IO_TRANS_PEND_FOREVER) ||
        (config->timeoutSnd < 0 && config->timeoutSnd != IO_TRANS_PEND_FOREVER))
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                          "IO_TransUDP Error: Bad config timeout input.");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }
    
    /* Get IP address from cAddr */
    /* NOTE: inet_aton errors out if cAddr = "0.0.0.0", the value of
     * IO_TRANS_UDP_INADDR_ANY.  So if this is the case, set uiAddr to
     * the system defined value of INADDR_ANY
     * */
    if(strcmp(config->cAddr, IO_TRANS_UDP_INADDR_ANY) == 0)
    {
        uiAddr = INADDR_ANY;
    }
    else
    {
        status = inet_aton(&config->cAddr[0], (struct in_addr *) &uiAddr);
        if (status == INET_ATON_ERROR)
        {
            CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                              "IO_TransUDP Error: Bad config addr input:%s",
                              &config->cAddr[0]);
            return IO_TRANS_UDP_BAD_INPUT_ERROR;
        }
    }

    /* Initialize socket address structures */
    CFE_PSP_MemSet((void *) &udp->sockAddr, 0x0, sizeof(struct sockaddr_in));
    CFE_PSP_MemSet((void *) &udp->srcAddr, 0x0, sizeof(struct sockaddr_in));
    CFE_PSP_MemSet((void *) &udp->destAddr, 0x0, sizeof(struct sockaddr_in));

    /* Save UDP Socket Addr structure */
    udp->sockAddr.sin_family        = AF_INET;
    udp->sockAddr.sin_addr.s_addr   = uiAddr;
    udp->sockAddr.sin_port          = htons(config->usPort);

    /* Set Receive Timeout */
    if (config->timeoutRcv != 0)
    {
        timeout.tv_sec = (long)(config->timeoutRcv / 1000);
        timeout.tv_usec = (long)((config->timeoutRcv % 1000) * 1000);
        
        if (setsockopt(udp->sockId, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, 
            sizeof(timeout)) < 0)
        {
            CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                              "IO_TransUDP Error: Set option SO_RCVTIMEO failed. "
                              "Timeout input:%d", config->timeoutRcv);
            
            return IO_TRANS_UDP_SOCKETOPT_ERROR;
        }
    }

    /* Set Send Timeout */
    if (config->timeoutSnd != 0)
    {
        timeout.tv_sec = (long)(config->timeoutSnd / 1000);
        timeout.tv_usec = (long)((config->timeoutSnd % 1000) * 1000);
        
        if (setsockopt(udp->sockId, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, 
            sizeof(timeout)) < 0)
        {
            CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                              "IO_TransUDP Error: Set option SO_SNDTIMEO failed. "
                              "Timeout input:%d", config->timeoutSnd);
            
            return IO_TRANS_UDP_SOCKETOPT_ERROR;
        }
    }

    return IO_TRANS_UDP_NO_ERROR;
}


/** Bind socket to srcAddr for receiving socket */
int32 IO_TransUdpBindSocket(IO_TransUdp_t * udp)
{
    if (udp == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: Null input argument. ");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }
    
    /* Bind socket to port */
    if ((bind(udp->sockId, (struct sockaddr*)&udp->sockAddr, 
              sizeof(struct sockaddr)) < 0))
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                          "IO_TransUDP Error: bind socket failed. errno:%d", 
                          errno);
        return IO_TRANS_UDP_SOCKETBIND_ERROR; 
    }

    return IO_TRANS_UDP_NO_ERROR;
}


/** Close a UDP Socket */
int32 IO_TransUdpCloseSocket(IO_TransUdp_t *udp)
{
    int32 status;

    if (udp == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: Null input argument. ");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }
    
    status = close(udp->sockId);

    if (status < 0)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                          "IO_TransUDP Error: Failed to close socket ID:%d, "
                          "errno:%d", udp->sockId, errno);
    }
    
    return status;
}


/** Set the destination address structure */
int32 IO_TransUdpSetDestAddr(IO_TransUdp_t *udp, char * destAddr, 
                             uint16 usPort)
{
    int32 status;
    uint32 uiAddr = INADDR_ANY; 
    
    if (udp == NULL || destAddr == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: Null input argument. ");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }
    
    /* Get IP address from cAddr */
    status = inet_aton(destAddr, (struct in_addr *) &uiAddr);
    if (status == INET_ATON_ERROR)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                          "IO_TransUDP Error: Bad destAddr input: %s 0x%08X",
                          destAddr, uiAddr);
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }

    /* Initialize destination socket structure */
    CFE_PSP_MemSet((void *) &udp->destAddr, 0x0, sizeof(struct sockaddr_in));

    /* Save UDP Socket Destination Addr structure */
    udp->destAddr.sin_family        = AF_INET;
    udp->destAddr.sin_addr.s_addr   = uiAddr;
    udp->destAddr.sin_port          = htons(usPort);

    CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_INFORMATION,
                      "IO_TransUDP: Destination IP set to %s:%u", 
                      destAddr, usPort);


    return IO_TRANS_UDP_NO_ERROR;
}



/** Receive message on blocking socket with select. 
 *  Will timeout after selectTimout based on input argument. 
 *  Do not use this function if you are using the IO_Trans_Select library. */
int32 IO_TransUdpRcvTimeout(IO_TransUdp_t * udp, uint8 * buffer, int32 bufSize,
                            int32 selectTimeout)
{
    struct timeval timeout;
    fd_set fdSet;
    int32 size = 0;
    
    if (udp == NULL || buffer == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: Null input argument. ");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }
    
    FD_ZERO(&fdSet);
    FD_SET(udp->sockId, &fdSet);

    /* Wait on socket for timeout time until some data
     * is available. */
    if (selectTimeout == IO_TRANS_PEND_FOREVER)
    {
        size = select(udp->sockId + 1, &fdSet, NULL, NULL, NULL);
    }
    else
    {
        timeout.tv_sec  = selectTimeout / 1000000;
        timeout.tv_usec = selectTimeout % 1000000; 
        size = select(udp->sockId + 1, &fdSet, NULL, NULL, &timeout);
    }

    /* Read the Socket if some data is available. */
    if (size > 0)
    {
        size = IO_TransUdpRcv(udp, buffer, bufSize);
    }
    
    return size;
}


/** Receive message on blocking socket. 
 *  Will block for timoutRcv msec based on udp configuration. */
int32 IO_TransUdpRcv(IO_TransUdp_t *udp, uint8 *buffer, int32 bufSize)
{
    socklen_t addrLen = sizeof(struct sockaddr_in);
    int32 msgSize;
    
    if (udp == NULL || buffer == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: Null input argument. ");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }
    
    msgSize = recvfrom(udp->sockId, (void *) buffer, (size_t) bufSize, 0, 
                       (struct sockaddr *)&udp->srcAddr, &addrLen);
    
    /* Return size of zero if timed out. */
    if(msgSize == -1 && errno == EWOULDBLOCK)
    {
        msgSize = 0;
    }
    
    return msgSize;
}


/** Send message on outbound socket */
int32 IO_TransUdpSnd(IO_TransUdp_t *udp, uint8 * msgPtr, int32 size)
{
    int32 sizeOut;

    if (udp == NULL || msgPtr == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID,CFE_EVS_ERROR,
                          "IO_TransUDP Error: Null input argument. ");
        return IO_TRANS_UDP_BAD_INPUT_ERROR;
    }
    
    sizeOut = sendto(udp->sockId, (void *) msgPtr, (size_t) size, 0, 
                       (struct sockaddr *) &udp->destAddr, 
                       sizeof(struct sockaddr_in));

    if (sizeOut < 0)
    {
        CFE_EVS_SendEvent(IO_LIB_TRANS_UDP_EID, CFE_EVS_ERROR,
                          "IO_TransUDP Error: errno:%d on Send.", errno);
        return sizeOut;
    }

    return sizeOut;
}
