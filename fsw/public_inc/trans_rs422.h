/*******************************************************************************
* File: trans_rs422.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
* Purpose:
*   Provides the functionality to communicate over an RS-422 serial port. 
*   Suports VXWorks and POSIX OS.
*
* Notes:
*   1. The serial port is set to non-blocking to prevent lock-up if link is 
*   interrupted and/or not all of the expected message is received. 
*   2. The VTIME and VMIN take presendence over NON_BLOCKING. If VMIN and 
*   VTIME > 0 (recommended), timer will only start if at least one byte is 
*   received. Read will return after at least VMIN bytes are received,
*   or VTIME timeout. VTIME is an intercharacter timout in this case. Use the 
*   ReadTimeout function or use select with timeout at a higher level before 
*   calling the TransRS422Read function to prevent indefinite blocking. 
*
* History:
*   Apr 07, 2015  Guy de Carufel
*    * Created
*
******************************************************************************/

#ifndef _IO_TRANS_RS422_
#define _IO_TRANS_RS422_

#include "io_lib.h"

/* Possible timeout + minBytes settings:
 * min = 0, timeout = 0: return immediately with available bytes. 
 *                       (Polling. Not recommended.)
 * min = 0, timeout > 0: Timer starts on read(). return on timeout or any #bytes
 * min > 0, timeout = 0: block until less of min or requested size
 * min > 0, timeout > 0: Timer starts on first byte. return on timeout or 
 *                       lesser of min or requested size. (Recommended.)
 * Recommended setting:
 * Set min = hdrSize (primary header size).
 * Set timeout > 0 (eg: 100ms) to prevent excessive polling. */

/* The following cflag are always set: CS8, CLOCAL, CREAD.
 * RTS/CTS (CRTSCTS) Hardware flow control is not enabled by default. 
 * Add other flags in cFlags or set to 0x0 */

/* The IGNPAR (ignore parity) is always used for iflag */

/* Device is open as O_NONBLOCK to prevent locking on smaller than expected
 * messages. */

#define PORT_NAME_SIZE 20

#define IO_TRANS_RS422_ERROR            -1
#define IO_TRANS_RS422_BADINPUT_ERR     -2
#define IO_TRANS_RS422_BADDEVICE_ERR    -3
#define IO_TRANS_RS422_BAUDRATE_ERR     -4
#define IO_TRANS_RS422_OPEN_ERR         -5
#define IO_TRANS_RS422_SETATTR_ERR      -6


#ifdef _VXWORKS_OS_
/* Open the serial port as read / write (default to non-blocking */
#define IO_TRANS_RS422_OPEN_FLAGS   O_RDWR

#else
/* Open the serial port as read / write non-blocking. 
 * O_NOCTTY is No controlling TTY (eg: can't send CTRL-C) */
#define IO_TRANS_RS422_OPEN_FLAGS  (O_RDWR | O_NOCTTY | O_NONBLOCK)
#endif



/* Structures */
typedef struct {
    char   device[PORT_NAME_SIZE]; /**< Device name (eg: "/dev/ttyS1") */
    int32  baudRate;               /**< Baud Rate in bytes per second. */
    int32  cFlags;                 /**< Any serial port flags to set. */        
    int32  timeout;                /**< Timeout on read (in ms). */
    int32  minBytes;               /**< Minimum number of bytes to return 
                                        on a single read. */
} IO_TransRS422Config_t;

/* Prototypes */
int32 IO_TransRS422Init(IO_TransRS422Config_t * configIn);
int32 IO_TransRS422Close(int32 fd);
int32 IO_TransRS422ReadTimeout(int32 fd, uint8 *buffer, int32 numBytes, 
                               int32 timeout);
int32 IO_TransRS422Read(int32 fd, uint8 *buffer, int32 numBytes);
int32 IO_TransRS422Write(int32 fd, uint8 *msg, int32 size);

#endif
