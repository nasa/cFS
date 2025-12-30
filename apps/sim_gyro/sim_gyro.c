/*
** cFS Simulated Gyroscope Module
**
** This module provides a basic interface for a simulated gyroscope.
*/

#include "cfe.h"

void SIM_GYRO_Main(void)
{
    CFE_ES_PerfLogEntry(SIM_GYRO_MAIN_TASK_PERF_ID);
    
    /* Main application loop will go here */
    
    CFE_ES_PerfLogExit(SIM_GYRO_MAIN_TASK_PERF_ID);
}