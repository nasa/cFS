/*
**  GSC-18128-1, "Core Flight Executive Version 6.6"
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
void CFE_PSP_SigintHandler (int signal);
void CFE_PSP_TimerHandler (int signum);
void CFE_PSP_DisplayUsage(char *Name );
void CFE_PSP_ProcessArgumentDefaults(CFE_PSP_CommandData_t *CommandData);
void CFE_PSP_SetupLocal1Hz(void);

/*
**  External Declarations
*/
extern void CFE_PSP_DeleteProcessorReservedMemory(void);

/*
** Global variables
*/
uint32              TimerCounter;
CFE_PSP_CommandData_t CommandData;
uint32              CFE_PSP_SpacecraftId;
uint32              CFE_PSP_CpuId;
char                CFE_PSP_CpuName[CFE_PSP_CPU_NAME_LENGTH];

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
int main(int argc, char *argv[])
{
   uint32             reset_type;
   uint32             reset_subtype;
   int32              time_status;
   uint32             sys_timebase_id;
   int                opt = 0;
   int                longIndex = 0;
   int32              Status;

   /*
   ** Initialize the CommandData struct 
   */
   memset(&(CommandData), 0, sizeof(CFE_PSP_CommandData_t));
      
   /* 
   ** Process the arguments with getopt_long(), then 
   ** start the cFE
   */
   opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
   while( opt != -1 ) 
   {
      switch( opt ) 
      {
         case 'R':
            strncpy(CommandData.ResetType, optarg, CFE_PSP_RESET_NAME_LENGTH);
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
            strncpy(CommandData.CpuName, optarg, CFE_PSP_CPU_NAME_LENGTH );
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
   ** Set the reset type
   */
   if (strncmp("PR", CommandData.ResetType, 2 ) == 0 )
   {
      reset_type = CFE_PSP_RST_TYPE_PROCESSOR;
      printf("CFE_PSP: Starting the cFE with a PROCESSOR reset.\n");
   }
   else
   {
      reset_type = CFE_PSP_RST_TYPE_POWERON;
      printf("CFE_PSP: Starting the cFE with a POWER ON reset.\n");
   }

   /*
   ** Assign the Spacecraft ID, CPU ID, and CPU Name
   */
   CFE_PSP_SpacecraftId = CommandData.SpacecraftId;
   CFE_PSP_CpuId = CommandData.CpuId;
   strncpy(CFE_PSP_CpuName, CommandData.CpuName, CFE_PSP_CPU_NAME_LENGTH);

   /*
   ** Set the reset subtype
   */
   reset_subtype = CommandData.SubType;

   /*
   ** Install sigint_handler as the signal handler for SIGINT.
   */
   signal(SIGINT, CFE_PSP_SigintHandler);

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
   ** Initialize the statically linked modules (if any)
   ** This is only applicable to CMake build - classic build
   ** does not have the logic to selectively include/exclude modules
   */
   CFE_PSP_ModuleInit();
     
   sleep(1);

   /*
   ** Initialize the reserved memory 
   */
   CFE_PSP_InitProcessorReservedMemory(reset_type);


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

   /*
   ** Let the main thread sleep.
   **
   ** OS_IdleLoop() will wait forever and return if
   ** someone calls OS_ApplicationShutdown(true)
   */
   OS_IdleLoop();

   /*
    * The only way OS_IdleLoop() will return is if SIGINT is captured
    * Handle cleanup duties.
    */
   OS_printf("\nCFE_PSP: Control-C Captured - Exiting cFE\n");

   /* Deleting these memories will unlink them, but active references should still work */
   CFE_PSP_DeleteProcessorReservedMemory();

   OS_printf("CFE_PSP: NOTE: After quitting the cFE with a Control-C signal, it MUST be started next time\n");
   OS_printf("     with a Poweron Reset ( --reset PO ). \n");

   OS_DeleteAllObjects();


   return(0);
}

/******************************************************************************
**  Function:  CFE_PSP_SigintHandler()
**
**  Purpose:
**    SIGINT routine for linux/OSX
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

void CFE_PSP_SigintHandler (int signal)
{
    OS_ApplicationShutdown(true);
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
**    CFE_PSP_CommandData_t *CommandData -- A pointer to the command parameters.
**
**  Return:
**    (none)
*/
void CFE_PSP_ProcessArgumentDefaults(CFE_PSP_CommandData_t *CommandData)
{
   if ( CommandData->GotResetType == 0 )
   {
      strncpy(CommandData->ResetType, "PO", sizeof(CommandData->ResetType) );
      printf("CFE_PSP: Default Reset Type = PO\n");
      CommandData->GotResetType = 1;
   }
   
   if ( CommandData->GotSubType == 0 )
   {
      CommandData->SubType = 1;
      printf("CFE_PSP: Default Reset SubType = 1\n");
      CommandData->GotSubType = 1;
   }
   
   if ( CommandData->GotCpuId == 0 )
   {
      CommandData->CpuId = CFE_PSP_CPU_ID;
      printf("CFE_PSP: Default CPU ID = %d\n",CFE_PSP_CPU_ID);
      CommandData->GotCpuId = 1;
   }
   
   if ( CommandData->GotSpacecraftId == 0 )
   {
      CommandData->SpacecraftId = CFE_PSP_SPACECRAFT_ID;
      printf("CFE_PSP: Default Spacecraft ID = %d\n",CFE_PSP_SPACECRAFT_ID);
      CommandData->GotSpacecraftId = 1;
   }
   
   if ( CommandData->GotCpuName == 0 )
   {
      strncpy(CommandData->CpuName, CFE_PSP_CPU_NAME, CFE_PSP_CPU_NAME_LENGTH );
      printf("CFE_PSP: Default CPU Name: %s\n",CFE_PSP_CPU_NAME);
      CommandData->GotCpuName = 1;
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

