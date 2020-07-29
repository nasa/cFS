/************************************************************************
** File: io_lib.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** Purpose: 
**  The IO Lib header file 
**
** Notes:
**
** History:
**   Apr, 2015  Guy de Carufel
**    * Created
**
*************************************************************************/
#ifndef _IO_LIB_H_
#define _IO_LIB_H_

#include "cfe.h"
#include "common_types.h"
#include "network_includes.h"

#ifdef _VXWORKS_OS_
/* For vxworks, include ioLib for ioctl suport */
#include <ioLib.h>

/* In VxWorks inet_aton() returns OK and ERORR
 * in VxWorks ERROR = -1, OK = 0 
 * */
#define INET_ATON_ERROR ERROR

#else
/* If not vxworks, include termios and select.h */
#include <termios.h>
#include <sys/select.h>

/* In Linux inet_aton() returns 0 for error and non-zero for ok */
#define INET_ATON_ERROR 0

#endif

#include "io_lib_events.h"


#define IO_LIB_MAJOR_VERSION    1
#define IO_LIB_MINOR_VERSION    0
#define IO_LIB_REVISION         0
#define IO_LIB_MISSION_REV      0

#define IO_LIB_SUCCESS          0
#define IO_LIB_ERROR           -1

#define IO_TRANS_PEND_FOREVER    -1

typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[IO_LIB_EVT_CNT];
} IO_LIB_LibData_t;


int32 IO_LibInit(void);

#endif /* _io_lib_h_ */

/************************/
/*  End of File Comment */
/************************/
