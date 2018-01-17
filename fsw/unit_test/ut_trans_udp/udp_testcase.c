/*
 * Filename: udp_testcase.c
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

#include "udp_stubs.h"

#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include "trans_udp.h"



extern speed_t IO_TransUDPGetBaudRateMacro(int32 bps);

/* -------------------- Special Test Case Variables ------------------------- */


/* ---------------------  Begin test cases  --------------------------------- */


void Test_TransUdpInit_CreateSocket(void)
{
    int32 fd;
    int32 expected = 0;
    IO_TransUdpConfig_t config;
    IO_TransUdp_t udp;

    memset(&config, 0x0, sizeof(config));

    /*************************/
    fd = IO_TransUdpInit(&config, NULL);
    expected = IO_TRANS_UDP_SOCKETCREATE_ERROR; 
    UtAssert_True(fd == expected, "NULL udp ptr");
    
    /*************************/
    UDP_SetReturnCode(UDP_SOCKET_INDEX, -1, 1);

    fd = IO_TransUdpInit(&config, &udp);
    expected = IO_TRANS_UDP_SOCKETCREATE_ERROR;
    UtAssert_True(fd == expected, "Create Socket Failed.");
    
    /*************************/
    fd = IO_TransUdpCreateSocket(&udp);
    expected = 1;
    UtAssert_True(fd == expected, "Create Socket Nominal.");
}


void Test_TransUdpInit_ConfigSocket(void)
{
    int32 actual;
    int32 expected = 0;
    IO_TransUdpConfig_t config;
    IO_TransUdp_t udp;

    memset(&config, 0x0, sizeof(config));

    /*************************/
    actual = IO_TransUdpInit(NULL, &udp);
    expected = IO_TRANS_UDP_SOCKETOPT_ERROR;
    UtAssert_True(actual == expected, "NULL Config");
    
    /*************************/
    udp.sockId = -1;
    
    actual = IO_TransUdpConfigSocket(&config, &udp);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "Socket not created");
    
    /*************************/
    udp.sockId = 1;
    config.timeoutRcv = -2;
    
    actual = IO_TransUdpConfigSocket(&config, &udp);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "Bad timeoutRcv");
    
    /*************************/
    config.timeoutRcv = 100;
    config.timeoutSnd = -2;
    
    actual = IO_TransUdpConfigSocket(&config, &udp);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "Bad timeoutSnd");
    
    /*************************/
    config.timeoutRcv = 100;
    config.timeoutSnd = 100;
    strncpy(config.cAddr,"",1);

    actual = IO_TransUdpConfigSocket(&config, &udp);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "Bad cAddr Setting");
    
    /*************************/
    strncpy(config.cAddr, IO_TRANS_UDP_INADDR_ANY, 16);

    UDP_SetReturnCode(UDP_SETSOCKOPT_INDEX, -1, 1);

    actual = IO_TransUdpConfigSocket(&config, &udp);
    expected = IO_TRANS_UDP_SOCKETOPT_ERROR;
    UtAssert_True(actual == expected, "Set SO_RCVTIMEO failed");

    /*************************/
    UDP_SetReturnCode(UDP_SETSOCKOPT_INDEX, -1, 2);

    actual = IO_TransUdpConfigSocket(&config, &udp);
    expected = IO_TRANS_UDP_SOCKETOPT_ERROR;
    UtAssert_True(actual == expected, "Set SO_SNDTIMEO failed");
    
    /*************************/
    actual = IO_TransUdpConfigSocket(&config, &udp);
    expected = IO_TRANS_UDP_NO_ERROR;
    UtAssert_True(actual == expected, "Config Socket Nominal");
}

void Test_TransUdpInit_BindSocket(void)
{
    int32 actual;
    int32 expected = 0;
    IO_TransUdpConfig_t config;
    IO_TransUdp_t udp;

    memset(&config, 0x0, sizeof(config));
    
    udp.sockId = 1;
    config.timeoutRcv = 100;
    config.timeoutSnd = 100;
    strncpy(config.cAddr, IO_TRANS_UDP_INADDR_ANY, 16);

    /*************************/
    actual = IO_TransUdpBindSocket(NULL);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "NULL Input");

    /*************************/
    UDP_SetReturnCode(UDP_BIND_INDEX, -1, 1);
    
    actual = IO_TransUdpInit(&config, &udp);
    expected = IO_TRANS_UDP_SOCKETBIND_ERROR;
    UtAssert_True(actual == expected, "Socket Bind Fail");
    
    /*************************/
    actual = IO_TransUdpInit(&config, &udp);
    expected = 1;
    UtAssert_True(actual == expected, "Socket Bind Nominal");
}
    
void Test_TransUdpCloseSocket(void)
{
    int32 actual;
    int32 expected = 0;
    IO_TransUdp_t udp;

    /*************************/
    actual = IO_TransUdpCloseSocket(NULL);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "NULL Input");

    /*************************/
    UDP_SetReturnCode(UDP_CLOSE_INDEX, -1, 1);
    
    actual = IO_TransUdpCloseSocket(&udp);
    expected = -1;
    UtAssert_True(actual == expected, "Socket Close Fail");
    
    /*************************/
    actual = IO_TransUdpCloseSocket(&udp);
    expected = IO_TRANS_UDP_NO_ERROR;
    UtAssert_True(actual == expected, "Socket Close Nominal");
}


void Test_TransUdpSetDestAddr(void)
{
    int32 actual;
    int32 expected = 0;
    IO_TransUdp_t udp;
    
    /*************************/
    actual = IO_TransUdpSetDestAddr(NULL, "", 0);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "NULL Udp Input");
    
    /*************************/
    actual = IO_TransUdpSetDestAddr(&udp, NULL, 0);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "NULL destAddr Input");
    
    /*************************/
    actual = IO_TransUdpSetDestAddr(&udp, "", 0);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "Invalid Dest Address");
    
    /*************************/
    actual = IO_TransUdpSetDestAddr(&udp, "111.111.111.111", 0);
    expected = IO_TRANS_UDP_NO_ERROR;
    UtAssert_True(actual == expected, "Set Dest Address - Nominal");
}


void Test_TransUdpRcv(void)
{
    int32 actual;
    int32 expected = 0;
    IO_TransUdp_t udp;
    uint8 buffer[10];

    /*************************/
    actual = IO_TransUdpRcv(NULL, buffer, 10);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "NULL Udp Input");

    /*************************/
    actual = IO_TransUdpRcv(&udp, buffer, 10);
    expected = 10;
    UtAssert_True(actual == expected, "Nominal");
}


void Test_TransUdpRcvTimeout(void)
{
    int32 actual;
    int32 expected = 0;
    IO_TransUdp_t udp;
    uint8 buffer[10];

    /*************************/
    actual = IO_TransUdpRcvTimeout(NULL, buffer, 10, IO_TRANS_PEND_FOREVER);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "NULL Udp Input");
    
    /*************************/
    udp.sockId = 1;
    UDP_SetReturnCode(UDP_SELECT_INDEX, -1, 1);
    
    actual = IO_TransUdpRcvTimeout(&udp, buffer, 10, IO_TRANS_PEND_FOREVER);
    expected = -1;
    UtAssert_True(actual == expected, "Select Pend Forever Fail");
    
    /*************************/
    UDP_SetReturnCode(UDP_SELECT_INDEX, -1, 1);
    
    actual = IO_TransUdpRcvTimeout(&udp, buffer, 10, 100);
    expected = -1;
    UtAssert_True(actual == expected, "Select Pend Fail");
    
    /*************************/
    actual = IO_TransUdpRcvTimeout(&udp, buffer, 10, 100);
    expected = 10;
    UtAssert_True(actual == expected, "Nominal");
}


void Test_TransUdpSnd(void)
{
    int32 actual;
    int32 expected = 0;
    IO_TransUdp_t udp;
    uint8 msg[10] = "Haha. Not real.";
    
    /*************************/
    actual = IO_TransUdpSnd(NULL, msg, 10);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "NULL Udp Input");
    
    /*************************/
    actual = IO_TransUdpSnd(&udp, NULL, 10);
    expected = IO_TRANS_UDP_BAD_INPUT_ERROR;
    UtAssert_True(actual == expected, "NULL Msg Input");
    
    /*************************/
    UDP_SetReturnCode(UDP_SENDTO_INDEX, -1, 1);
    
    actual = IO_TransUdpSnd(&udp, msg, 10);
    expected = -1;
    UtAssert_True(actual == expected, "sendto Fail");
    
    /*************************/
    actual = IO_TransUdpSnd(&udp, msg, 10);
    expected = 10;
    UtAssert_True(actual == expected, "Nominal");
}



/* ------------------- End of test cases --------------------------------------*/


/*
 * UDP_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void UDP_Setup(void)
{
    Ut_OSAPI_Reset();
    Ut_CFE_EVS_Reset();
}

/*
 * UDP_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void UDP_TearDown(void)
{

}


#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* UDP_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UDP_AddTestCase(void)
{
    /* UDP_GetVersion Tests */
    ADD_TEST(Test_TransUdpInit_CreateSocket, UDP_Setup, UDP_TearDown);
    ADD_TEST(Test_TransUdpInit_ConfigSocket, UDP_Setup, UDP_TearDown);
    ADD_TEST(Test_TransUdpInit_BindSocket, UDP_Setup, UDP_TearDown);
    ADD_TEST(Test_TransUdpCloseSocket, UDP_Setup, UDP_TearDown);
    ADD_TEST(Test_TransUdpSetDestAddr, UDP_Setup, UDP_TearDown);
    ADD_TEST(Test_TransUdpRcv, UDP_Setup, UDP_TearDown);
    ADD_TEST(Test_TransUdpRcvTimeout, UDP_Setup, UDP_TearDown);
    ADD_TEST(Test_TransUdpSnd, UDP_Setup, UDP_TearDown);
}
