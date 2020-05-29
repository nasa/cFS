/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/******************************************************************************
** File:  cfe_psp_start.c
**
** Purpose:
**   cFE BSP main entry point.
**
** History:
**   2005/07/26  A. Cudmore      | Initial version for OS X/Linux 
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>

/*
** cFE includes 
*/
#include "common_types.h"
#include "osapi.h"

#include "cfe_psp.h"
#include "cfe_psp_memory.h"

/*
 * The preferred way to obtain the CFE tunable values at runtime is via
 * the dynamically generated configuration object.  This allows a single build
 * of the PSP to be completely CFE-independent.
 */
#include <target_config.h>
#include "cfe_psp_module.h"

#define CFE_PSP_MAIN_FUNCTION        (*GLOBAL_CONFIGDATA.CfeConfig->SystemMain)
#define CFE_PSP_1HZ_FUNCTION         (*GLOBAL_CONFIGDATA.CfeConfig->System1HzISR)
#define CFE_PSP_NONVOL_STARTUP_FILE  (GLOBAL_CONFIGDATA.CfeConfig->NonvolStartupFile)
#define CFE_PSP_CPU_ID               (GLOBAL_CONFIGDATA.Default_CpuId)
#define CFE_PSP_CPU_NAME             (GLOBAL_CONFIGDATA.Default_CpuName)
#define CFE_PSP_SPACECRAFT_ID        (GLOBAL_CONFIGDATA.Default_SpacecraftId)

/*
** Defines
*/

#define CFE_PSP_CPU_NAME_LENGTH  32
#define CFE_PSP_RESET_NAME_LENGTH 10

/*
** Typedefs for this module
*/

/*
** Structure for the Command line parameters
*/
typedef struct
{   
   char     ResetType[CFE_PSP_RESET_NAME_LENGTH];   /* Reset type can be "PO" for Power on or "PR" for Processor Reset */
   uint32   GotResetType;    /* did we get the ResetType parameter ? */

   uint32   SubType;         /* Reset Sub Type ( 1 - 5 )  */
   uint32   GotSubType;      /* did we get the ResetSubType parameter ? */
   
   char     CpuName[CFE_PSP_CPU_NAME_LENGTH];     /* CPU Name */
   uint32   GotCpuName;      /* Did we get a CPU Name ? */

   uint32   CpuId;            /* CPU ID */
   uint32   GotCpuId;         /* Did we get a CPU Id ?*/

   uint32   SpacecraftId;     /* Spacecraft ID */ 
   uint32   GotSpacecraftId;  /* Did we get a Spacecraft ID */
   
} CFE_PSP_CommandData_t;

/*
** Prototypes for this module
*/
void CFE_PSP_TimerHandler (int signum);
void CFE_PSP_DisplayUsage(char *Name );
void CFE_PSP_ProcessArgumentDefaults(CFE_PSP_CommandData_t *CommandDataDefault);
void CFE_PSP_SetupLocal1Hz(void);

/*
** Global variables
*/
uint32              TimerCounter;
CFE_PSP_CommandData_t CommandData;
uint32              CFE_PSP_SpacecraftId;
uint32              CFE_PSP_CpuId;
char                CFE_PSP_CpuName[CFE_PSP_CPU_NAME_LENGTH];

CFE_PSP_IdleTaskState_t  CFE_PSP_IdleTaskState;

/*
** getopts parameter passing options string
*/
static const char *optString = "R:S:C:I:N:h";

/*
** getopts_long long form argument table
*/
static const struct option longOpts[] = {
   { "reset",     required_argument, NULL, 'R' },
   { "subtype",   required_argument, NULL, 'S' },
   { "cpuid",     required_argument, NULL, 'C' },
   { "scid",      required_argument, NULL, 'I'},
   { "cpuname",   required_argument, NULL, 'N'},
   { "help",      no_argument,       NULL, 'h' },
   { NULL,        no_argument,       NULL,  0 }
};


/******************************************************************************
**  Function:  main()
**
**  Purpose:
**    BSP Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
void OS_Application_Startup(void)
{
   uint32             reset_type;
   uint32             reset_subtype;
   int32              time_status;
   uint32             sys_timebase_id;
   uint32             fs_id;
   int                opt = 0;
   int                longIndex = 0;
   int32              Status;
   char * const *     argv;
   int                argc;

   /*
   ** Initialize the CommandData struct 
   */
   memset(&(CommandData), 0, sizeof(CFE_PSP_CommandData_t));

   /* 
   ** Process the arguments with getopt_long(), then 
   ** start the cFE
   */
   argc = OS_BSP_GetArgC();
   argv = OS_BSP_GetArgV();
   opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
   while( opt != -1 ) 
   {
      switch( opt ) 
      {
         case 'R':
            strncpy(CommandData.ResetType, optarg, CFE_PSP_RESET_NAME_LENGTH-1);
            CommandData.ResetType[CFE_PSP_RESET_NAME_LENGTH-1] = 0;

            if ((strncmp(CommandData.ResetType, "PO", CFE_PSP_RESET_NAME_LENGTH ) != 0 ) &&
                (strncmp(CommandData.ResetType, "PR", CFE_PSP_RESET_NAME_LENGTH ) != 0 ))
            {
               printf("\nERROR: Invalid Reset Type: %s\n\n",CommandData.ResetType);
               CommandData.GotResetType = 0;
               CFE_PSP_DisplayUsage(argv[0]);
               break;
            }
            printf("CFE_PSP: Reset Type: %s\n",(char *)optarg);
            CommandData.GotResetType = 1;
            break;
				
         case 'S':
            CommandData.SubType = strtol(optarg, NULL, 0 );
            if ( CommandData.SubType < 1 || CommandData.SubType > 5 )
            {
               printf("\nERROR: Invalid Reset SubType: %s\n\n",optarg);
               CommandData.SubType = 0;
               CommandData.GotSubType = 0;
               CFE_PSP_DisplayUsage(argv[0]);
               break;
            }
            printf("CFE_PSP: Reset SubType: %d\n",(int)CommandData.SubType);
            CommandData.GotSubType = 1;
            break;

         case 'N':
            strncpy(CommandData.CpuName, optarg, CFE_PSP_CPU_NAME_LENGTH-1 );
            CommandData.CpuName[CFE_PSP_CPU_NAME_LENGTH-1] = 0;
            printf("CFE_PSP: CPU Name: %s\n",CommandData.CpuName);
            CommandData.GotCpuName = 1;
            break;

         case 'C':
            CommandData.CpuId = strtol(optarg, NULL, 0 );
            printf("CFE_PSP: CPU ID: %d\n",(int)CommandData.CpuId);
            CommandData.GotCpuId = 1;
            break;

         case 'I':
            CommandData.SpacecraftId = strtol(optarg, NULL, 0 );
            printf("CFE_PSP: Spacecraft ID: %d\n",(int)CommandData.SpacecraftId);
            CommandData.GotSpacecraftId = 1;
            break;

         case 'h':
            CFE_PSP_DisplayUsage(argv[0]);
            break;
	
         default:
            CFE_PSP_DisplayUsage(argv[0]);
            break;
       }
		
       opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
   } /* end while */
   
   /*
   ** Set the defaults for values that were not given for the 
   ** optional arguments, and check for arguments that are required.
   */
   CFE_PSP_ProcessArgumentDefaults(&CommandData);

   /*
   ** Assign the Spacecraft ID, CPU ID, and CPU Name
   */
   CFE_PSP_SpacecraftId = CommandData.SpacecraftId;
   CFE_PSP_CpuId = CommandData.CpuId;
   strncpy(CFE_PSP_CpuName, CommandData.CpuName, sizeof(CFE_PSP_CpuName)-1);
   CFE_PSP_CpuName[sizeof(CFE_PSP_CpuName)-1] = 0;

   /*
   ** Set the reset subtype
   */
   reset_subtype = CommandData.SubType;


   /*
   ** Initialize the OS API data structures
   */
   Status = OS_API_Init();
   if (Status != OS_SUCCESS)
   {
       /* irrecoverable error if OS_API_Init() fails. */
       /* note: use printf here, as OS_printf may not work */
       printf("CFE_PSP: OS_API_Init() failure\n");
       CFE_PSP_Panic(Status);
   }

   /*
    * Map the PSP shared memory segments
    */
   CFE_PSP_SetupReservedMemoryMap();

   /*
    * Prepare for exception handling in the idle task
    */
   memset(&CFE_PSP_IdleTaskState, 0, sizeof(CFE_PSP_IdleTaskState));
   CFE_PSP_IdleTaskState.ThreadID = pthread_self();

   /*
   ** Set up the timebase, if OSAL supports it
   ** Done here so that the modules can also use it, if desired
   **
   ** This is a clock named "cFS-Master" that will serve to drive
   ** all time-related CFE functions including the 1Hz signal.
   **
   ** Note the timebase is only prepared here; the application is
   ** not ready to receive a callback yet, as it hasn't been started.
   ** CFE TIME registers its own callback when it is ready to do so.
   */
   time_status = OS_TimeBaseCreate(&sys_timebase_id, "cFS-Master", NULL);
   if (time_status == OS_SUCCESS)
   {
       /*
        * Set the clock to trigger with 50ms resolution - slow enough that
        * it will not hog CPU resources but fast enough to have sufficient resolution
        * for most general timing purposes.
        * (It may be better to move this to the mission config file)
        */
       time_status = OS_TimeBaseSet(sys_timebase_id, 50000, 50000);
   }
   else
   {
       /*
        * Cannot create a timebase in OSAL.
        *
        * Note: Most likely this is due to building with
        * the old/classic POSIX OSAL which does not support this.
        *
        * See below for workaround.
        */
       sys_timebase_id = 0;
   }

   /*
   ** Set up the virtual FS mapping for the "/cf" directory
   ** On this platform it is just a local/relative dir of the same name.
   */
   Status = OS_FileSysAddFixedMap(&fs_id, "./cf", "/cf");
   if (Status != OS_SUCCESS)
   {
       /* Print for informational purposes --
        * startup can continue, but loads may fail later, depending on config. */
       OS_printf("CFE_PSP: OS_FileSysAddFixedMap() failure: %d\n", (int)Status);
   }

   /*
   ** Initialize the statically linked modules (if any)
   ** This is only applicable to CMake build - classic build
   ** does not have the logic to selectively include/exclude modules
   */
   CFE_PSP_ModuleInit();
     
   sleep(1);

   /*
    * For informational purposes, show the state of the last exit
    */
   if (CFE_PSP_ReservedMemoryMap.BootPtr->ValidityFlag == CFE_PSP_BOOTRECORD_VALID)
   {
       OS_printf("CFE_PSP: Normal exit from previous cFE instance\n");
   }
   else if (CFE_PSP_ReservedMemoryMap.BootPtr->ValidityFlag == CFE_PSP_BOOTRECORD_INVALID)
   {
       OS_printf("CFE_PSP: Abnormal exit from previous cFE instance\n");
   }

   /*
    * determine reset type...
    * If not specified at the command line, then check the "boot record"
    */
   reset_type = 0;
   if (!CommandData.GotResetType)
   {
       if (CFE_PSP_ReservedMemoryMap.BootPtr->ValidityFlag == CFE_PSP_BOOTRECORD_VALID ||
               CFE_PSP_ReservedMemoryMap.BootPtr->ValidityFlag == CFE_PSP_BOOTRECORD_INVALID)
       {
           reset_type = CFE_PSP_ReservedMemoryMap.BootPtr->NextResetType;
       }
   }
   else if (strncmp("PR", CommandData.ResetType, 2 ) == 0 )
   {
       reset_type = CFE_PSP_RST_TYPE_PROCESSOR;
   }

   if (reset_type == CFE_PSP_RST_TYPE_PROCESSOR)
   {
       OS_printf("CFE_PSP: Starting the cFE with a PROCESSOR reset.\n");
   }
   else
   {
       /* catch-all for anything else */
       reset_type = CFE_PSP_RST_TYPE_POWERON;
       OS_printf("CFE_PSP: Starting the cFE with a POWER ON reset.\n");
   }


   /*
   ** Initialize the reserved memory 
   */
   Status = CFE_PSP_InitProcessorReservedMemory(reset_type);
   if (Status != CFE_PSP_SUCCESS)
   {
       OS_printf("CFE_PSP: CFE_PSP_InitProcessorReservedMemory() Failure");
       CFE_PSP_Panic(Status);
   }

   /*
   ** Call cFE entry point.
   */
   CFE_PSP_MAIN_FUNCTION(reset_type, reset_subtype, 1, CFE_PSP_NONVOL_STARTUP_FILE);

   /*
    * Backward compatibility for old OSAL.
    */
   if (sys_timebase_id == 0 || time_status != OS_SUCCESS)
   {
       OS_printf("CFE_PSP: WARNING - Compatibility mode - using local 1Hz Interrupt\n");
       CFE_PSP_SetupLocal1Hz();
   }

}

void OS_Application_Run(void)
{
    int sig;
    int ret;
    sigset_t sigset;


    /*
     * Now that all main tasks are created,
     * this original thread will exist just to service signals
     * that aren't directed to a specific task.
     *
     * OSAL sets a very conservative signal mask that
     * blocks most signals. Start by unblocking the
     * ones that should be handled.
     *
     * Unblock SIGQUIT so the user can force exit the CFE
     * by pressing CTRL+\ (default handler).  Also allow
     * SIGTERM for which a handler was installed in CFE_PSP_AttachExceptions()
     */
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGQUIT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);

    /*
     * Reset to the signal for background events (SIGUSR1)
     */
    sigemptyset(&sigset);
    sigaddset(&sigset, CFE_PSP_EXCEPTION_EVENT_SIGNAL);

    /*
    ** just wait for events to occur and notify CFE
    **
    ** "shutdownreq" will become true if CFE calls CFE_PSP_Restart(),
    ** indicating a request to gracefully exit and restart CFE.
    */
    while (!CFE_PSP_IdleTaskState.ShutdownReq)
    {
        /* go idle and wait for an event */
        ret = sigwait(&sigset, &sig);

        if (ret == 0 && !CFE_PSP_IdleTaskState.ShutdownReq &&
                sig == CFE_PSP_EXCEPTION_EVENT_SIGNAL &&
                GLOBAL_CFE_CONFIGDATA.SystemNotify != NULL)
        {
            /* notify the CFE of the event */
            GLOBAL_CFE_CONFIGDATA.SystemNotify();
        }
    }

   /*
    * This happens if an unhandled exception occurs, or if the user presses CTRL+C
    */
   OS_printf("\nCFE_PSP: Shutdown initiated - Exiting cFE\n");
   OS_TaskDelay(100);

   OS_DeleteAllObjects();
}

/******************************************************************************
**  Function:  CFE_PSP_TimerHandler()
**
**  Purpose:
**    1hz "isr" routine for linux/OSX
**    This timer handler will execute 4 times a second.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
void CFE_PSP_TimerHandler (int signum)
{
      /*
      ** call the CFE_TIME 1hz ISR
      */
      if((TimerCounter % 4) == 0) CFE_PSP_1HZ_FUNCTION();

	  /* update timer counter */
	  TimerCounter++;
}

/******************************************************************************
**  Function:  CFE_PSP_DisplayUsage
**
**  Purpose:
**    Display program usage, and exit.
**
**  Arguments:
**    Name -- the name of the binary.
**
**  Return:
**    (none)
*/
void CFE_PSP_DisplayUsage(char *Name )
{

   printf("usage : %s [-R <value>] [-S <value>] [-C <value] [-N <value] [-I <value] [-h] \n", Name);
   printf("\n");
   printf("        All parameters are optional and can be used in any order\n");
   printf("\n");
   printf("        Parameters include:\n");
   printf("        -R [ --reset ] Reset Type is one of:\n");
   printf("             PO   for Power On reset ( default )\n");
   printf("             PR   for Processor Reset\n");
   printf("        -S [ --subtype ] Reset Sub Type is one of\n");
   printf("             1   for  Power on ( default )\n");
   printf("             2   for  Push Button Reset\n");
   printf("             3   for  Hardware Special Command Reset\n");
   printf("             4   for  Watchdog Reset\n");
   printf("             5   for  Reset Command\n");
   printf("        -C [ --cpuid ]   CPU ID is an integer CPU identifier.\n");
   printf("             The default  CPU ID is from the platform configuration file: %d\n",CFE_PSP_CPU_ID);
   printf("        -N [ --cpuname ] CPU Name is a string to identify the CPU.\n");
   printf("             The default  CPU Name is from the platform configuration file: %s\n",CFE_PSP_CPU_NAME);
   printf("        -I [ --scid ]    Spacecraft ID is an integer Spacecraft identifier.\n");
   printf("             The default Spacecraft ID is from the mission configuration file: %d\n",CFE_PSP_SPACECRAFT_ID);
   printf("        -h [ --help ]    This message.\n");
   printf("\n");
   printf("       Example invocation:\n");
   printf(" \n");
   printf("       Short form:\n");
   printf("       %s -R PO -S 1 -C 1 -N CPU1 -I 32\n",Name);
   printf("       Long form:\n");
   printf("       %s --reset PO --subtype 1 --cpuid 1 --cpuname CPU1 --scid 32\n",Name);
   printf(" \n");

   exit( 1 );
}
/******************************************************************************
**  Function: CFE_PSP_ProcessArgumentDefaults
**
**  Purpose:
**    This function assigns defaults to parameters and checks to make sure
**    the user entered required parameters 
**
**  Arguments:
**    CFE_PSP_CommandData_t *CommandDataDefault -- A pointer to the command parameters.
**
**  Return:
**    (none)
*/
void CFE_PSP_ProcessArgumentDefaults(CFE_PSP_CommandData_t *CommandDataDefault)
{
   if ( CommandDataDefault->GotSubType == 0 )
   {
      CommandDataDefault->SubType = 1;
      printf("CFE_PSP: Default Reset SubType = 1\n");
      CommandDataDefault->GotSubType = 1;
   }
   
   if ( CommandDataDefault->GotCpuId == 0 )
   {
      CommandDataDefault->CpuId = CFE_PSP_CPU_ID;
      printf("CFE_PSP: Default CPU ID = %d\n",CFE_PSP_CPU_ID);
      CommandDataDefault->GotCpuId = 1;
   }
   
   if ( CommandDataDefault->GotSpacecraftId == 0 )
   {
      CommandDataDefault->SpacecraftId = CFE_PSP_SPACECRAFT_ID;
      printf("CFE_PSP: Default Spacecraft ID = %d\n",CFE_PSP_SPACECRAFT_ID);
      CommandDataDefault->GotSpacecraftId = 1;
   }
   
   if ( CommandDataDefault->GotCpuName == 0 )
   {
      strncpy(CommandDataDefault->CpuName, CFE_PSP_CPU_NAME, CFE_PSP_CPU_NAME_LENGTH-1 );
      CommandDataDefault->CpuName[CFE_PSP_CPU_NAME_LENGTH-1] = 0;
      printf("CFE_PSP: Default CPU Name: %s\n",CFE_PSP_CPU_NAME);
      CommandDataDefault->GotCpuName = 1;
   }

}

/******************************************************************************
**  Function:  CFE_PSP_SetupLocal1Hz
**
**  Purpose:
**    This is a backward-compatible timer setup that is invoked when
**    there is a failure to set up the timebase in OSAL.  It is basically
**    the old way of doing things.
**
**    IMPORTANT: Note this is technically incorrect as it gives the
**    callback directly in the context of the signal handler.  It is
**    against spec to use most OSAL functions within a signal.
**
**    This is included merely to mimic the previous system behavior. It
**    should be removed in a future version of the PSP.
**
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
**
*/

void CFE_PSP_SetupLocal1Hz(void)
{
    struct sigaction    sa;
    struct itimerval    timer;
    int ret;

    /*
    ** Init timer counter
    */
    TimerCounter = 0;

    /*
    ** Install timer_handler as the signal handler for SIGALRM.
    */
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &CFE_PSP_TimerHandler;

    /*
    ** Configure the timer to expire after 250ms
    **
    ** (this is historical; the actual callback is invoked
    ** only on every 4th timer tick.  previous versions of the
    ** PSP did it this way, so this is preserved here).
    */
    timer.it_value.tv_sec  = 0;
    timer.it_value.tv_usec = 250000;
    timer.it_interval.tv_sec  = 0;
    timer.it_interval.tv_usec = 250000;

    ret = sigaction (SIGALRM, &sa, NULL);

    if (ret < 0)
    {
        OS_printf("CFE_PSP: sigaction() error %d: %s \n", ret, strerror(errno));
    }
    else
    {
        ret = setitimer (ITIMER_REAL, &timer, NULL);
        if (ret < 0)
        {
            OS_printf("CFE_PSP: setitimer() error %d: %s \n", ret, strerror(errno));
        }
    }
}

