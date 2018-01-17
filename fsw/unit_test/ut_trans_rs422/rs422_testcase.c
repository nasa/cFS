/*
 * Filename: rs422_testcase.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains a unit test cases for transfer frames
 *
 */


/*
 * Includes
 */
#include "cfe.h"
#include "utassert.h"
#include "uttest.h"
#include "utlist.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_osapi_stubs.h"

#include "rs422_stubs.h"

#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include "trans_rs422.h"

/* Global File descriptor */
int32 fd;

/* Prototypes for non-exported functions */
void Test_TransRS422GetBaudRateMacro(void);


extern speed_t IO_TransRS422GetBaudRateMacro(int32 bps);

/* -------------------- Special Test Case Variables ------------------------- */


/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  RS422_TransRS422GetBaudRateMacro Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_TransRS422GetBaudRateMacro(void)
{
    speed_t baud;

    /* Execute Test */
    baud = IO_TransRS422GetBaudRateMacro(19200);
    UtAssert_True(baud == B19200, "Test B19200");
    baud = IO_TransRS422GetBaudRateMacro(38400);
    UtAssert_True(baud == B38400, "Test B38400");
    baud = IO_TransRS422GetBaudRateMacro(57600);
    UtAssert_True(baud == B57600, "Test B57600");
    baud = IO_TransRS422GetBaudRateMacro(115200);
    UtAssert_True(baud == B115200, "Test B115200");
    baud = IO_TransRS422GetBaudRateMacro(230400);
    UtAssert_True(baud == B230400, "Test B230400");
    baud = IO_TransRS422GetBaudRateMacro(460800);
    UtAssert_True(baud == B460800, "Test B460800");
    baud = IO_TransRS422GetBaudRateMacro(921600);
    UtAssert_True(baud == B921600, "Test B921600");
    baud = IO_TransRS422GetBaudRateMacro(0);
    UtAssert_True(baud == -1, "Test Wrong Input");
}


void Test_TransRS422Init(void)
{
    int32 expected = 0;
    IO_TransRS422Config_t config;

    memset(&config, 0x0, sizeof(config));

    /*************************/
    fd = IO_TransRS422Init(NULL);
    expected = IO_TRANS_RS422_BADINPUT_ERR;
    UtAssert_True(fd == expected, "Test Null Input");

    /*************************/
    config.baudRate = 19200;
    strcpy(config.device, "");

    fd = IO_TransRS422Init(&config);
    expected = IO_TRANS_RS422_BADDEVICE_ERR;
    UtAssert_True(fd == expected, "Test Bad Device Name");
    
    /*************************/
    strcpy(config.device, "junk");
    config.baudRate = 0;

    fd = IO_TransRS422Init(&config);
    expected = IO_TRANS_RS422_BAUDRATE_ERR;
    UtAssert_True(fd == expected, "Test Bad BaudRate");

    /*************************/
    strcpy(config.device, "junk");
    config.baudRate = 19200;

    fd = IO_TransRS422Init(&config);
    expected = IO_TRANS_RS422_OPEN_ERR;
    UtAssert_True(fd == expected, "Test Open Error");
    
    /*************************/
    strcpy(config.device, "fakedevice");
    config.baudRate = 19200;
    config.timeout = 0;

    RS422_SetReturnCode(RS422_GETATTR_INDEX, -1, 2);

    fd = IO_TransRS422Init(&config);
    expected = IO_TRANS_RS422_SETATTR_ERR;
    UtAssert_True(fd == expected, "Test SetAttr Error");
    
    /*************************/
    strcpy(config.device, "fakedevice");
    config.baudRate = 19200;
    config.timeout = 0;

    RS422_SetReturnCode(RS422_GETATTR_INDEX, 0, 1);
    
    fd = IO_TransRS422Init(&config);
    expected = 0;
    UtAssert_True(fd > expected, "Test Succesful Init");
}


void Test_TransRS422Close(void)
{
    UtAssert_True(IO_TransRS422Close(fd) == 0, "Test close");
}

void Test_TransRS422ReadTimeout(void)
{
    int32 expected = 0;
    int32 size = 0;
    uint8 buf[2];
    
    size = IO_TransRS422ReadTimeout(-3, NULL, 0, IO_TRANS_PEND_FOREVER);
    expected = -1;
    UtAssert_True(size == expected, "Test Select Error");

    fd = open("fakedevice", O_RDWR);
    write(fd, ".", 1);

    size = IO_TransRS422ReadTimeout(fd, &buf[0], 1, IO_TRANS_PEND_FOREVER);
    expected = 1;
    UtAssert_True(size == expected, "Test Pend Forever");
    
    size = IO_TransRS422ReadTimeout(fd, &buf[0], 1, 0);
    expected = 0;
    UtAssert_True(size == expected, "Test No pend");
}


void Test_TransRS422Write(void)
{
    int32 expected = 0;
    int32 size = 0;
    uint16 buf = 0x01;
    
    size = IO_TransRS422Write(-3, NULL, 0);
    expected = IO_TRANS_RS422_BADDEVICE_ERR;
    UtAssert_True(size == expected, "Test Bad Device");

    fd = open("fakedevice", O_RDWR);
    
    size = IO_TransRS422Write(fd, NULL, 1);
    expected = IO_TRANS_RS422_ERROR;
    UtAssert_True(size == expected, "Test null data");
    
    size = IO_TransRS422Write(fd, (uint8 *) &buf, 2);
    expected = 2;
    UtAssert_True(size == expected, "Test Write");
}

/* ------------------- End of test cases --------------------------------------*/


/*
 * RS422_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void RS422_Setup(void)
{
    Ut_OSAPI_Reset();
    Ut_CFE_EVS_Reset();
}

/*
 * RS422_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void RS422_TearDown(void)
{

}


#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* RS422_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void RS422_AddTestCase(void)
{
    /* RS422_GetVersion Tests */
    ADD_TEST(Test_TransRS422GetBaudRateMacro, RS422_Setup, RS422_TearDown);
    ADD_TEST(Test_TransRS422Init,             RS422_Setup, RS422_TearDown);
    ADD_TEST(Test_TransRS422Close,            RS422_Setup, RS422_TearDown);
    ADD_TEST(Test_TransRS422ReadTimeout,      RS422_Setup, RS422_TearDown);
    ADD_TEST(Test_TransRS422Write,            RS422_Setup, RS422_TearDown);
}
