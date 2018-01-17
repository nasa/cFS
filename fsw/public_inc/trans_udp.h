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
#ifndef _IO_TRANS_UDP_
#define _IO_TRANS_UDP_

#include "io_lib.h"

#define IO_TRANS_UDP_NO_ERROR             0
#define IO_TRANS_UDP_BAD_INPUT_ERROR     -1
#define IO_TRANS_UDP_SOCKETCREATE_ERROR  -2
#define IO_TRANS_UDP_SOCKETOPT_ERROR     -3
#define IO_TRANS_UDP_SOCKETBIND_ERROR    -4

/* INADDR_NONE: 0xffffffff */
#define IO_TRANS_UDP_INADDR_NONE     "255.255.255.255"
/* INADDR_ANY:  0x0 */
#define IO_TRANS_UDP_INADDR_ANY      "0.0.0.0"      
/* INADDR_LOOPBACK */
#define IO_TRANS_UDP_INADDR_LOOPBACK "127.0.0.1"
#define IO_TRANS_UDP_INPORT_ANY INPORT_ANY

/* Structures */
/** Config structure of UDP */
typedef struct {
    char           cAddr[16];    /**< -- Address IP (V4)             */
    uint16         usPort;       /**< -- Port number                 */
    int32          timeoutRcv;   /**< -- Timeout on receive (in ms). */
    int32          timeoutSnd;   /**< -- Timeout on send    (in ms). */
} IO_TransUdpConfig_t;

/** The Working structure for the UDP */
typedef struct {
    int32  sockId;                  /**< -- Socket Id                   */
    struct sockaddr_in sockAddr;    /**< -- Socket Addr (Working)       */
    struct sockaddr_in srcAddr;     /**< -- Source Socket Address (In)  */
    struct sockaddr_in destAddr;    /**< -- Dest. Socket Address (Out)  */
} IO_TransUdp_t;

/* Prototypes */
int32 IO_TransUdpInit(IO_TransUdpConfig_t *config, IO_TransUdp_t * udp);
int32 IO_TransUdpCreateSocket(IO_TransUdp_t *udp);
int32 IO_TransUdpConfigSocket(IO_TransUdpConfig_t * config, IO_TransUdp_t *udp);
int32 IO_TransUdpBindSocket(IO_TransUdp_t *udp);
int32 IO_TransUdpCloseSocket(IO_TransUdp_t *udp);
int32 IO_TransUdpSetDestAddr(IO_TransUdp_t *udp, char * ipAddr, uint16 port);
int32 IO_TransUdpRcvTimeout(IO_TransUdp_t * udp, uint8 * buffer, int32 size, 
                            int32 selectTimeout);
int32 IO_TransUdpRcv(IO_TransUdp_t * udp, uint8 * buffer, int32 size);
int32 IO_TransUdpSnd(IO_TransUdp_t * udp, uint8 * msgPtr, int32 size);

#endif 
