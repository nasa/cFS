/************************************************************************
** File:
**   $Id: utf_test_sch.c 1.3 2015/03/01 14:01:26EST sstrege Exp  $
**
**  Copyright © 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: 
**   This is a test driver used to unit test the CFS Scheduler (SCH)
**   Application.
**
**   The SCH Application provides onboard Time Division Multiplexing for
**   sending messages to other applications.
** 
**   Output can be directed either to screen or to file:
**   To direct output to screen, 
**      comment in '#define UTF_USE_STDOUT' statement in the
**      utf_custom.h file.
**
**   To direct output to file, 
**      comment out '#define UTF_USE_STDOUT' statement in 
**      utf_custom.h file.
** 
**   $Log: utf_test_sch.c  $
**   Revision 1.3 2015/03/01 14:01:26EST sstrege 
**   Added copyright information
**   Revision 1.2 2011/06/30 21:03:42EDT aschoeni 
**   Updated unit tests for new timers and customization interfaces
**   Revision 1.1 2008/10/16 15:10:23EDT dkobe 
**   Initial revision
**   Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/unit_test/project.pj
** 
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "sch_platform_cfg.h"
#include "sch_msgids.h"

#include "sch_app.h"           /* Application headers */
#include "sch_cmds.h"
#include "sch_custom.h"
#include "sch_api.h"

#include "cfe_es_cds.h"        /* cFE headers         */
#include "cfe_tbl_internal.h"
#include "cfe_time_msg.h"

#include "utf_custom.h"        /* UTF headers         */
#include "utf_types.h"
#include "utf_cfe_sb.h"
#include "utf_osapi.h"
#include "utf_cfe.h"
#include "utf_ostimer.h"

#include <sys/fcntl.h>         /* System headers      */
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

/************************************************************************
** Macro Definitions
*************************************************************************/
#define MESSAGE_FORMAT_IS_CCSDS

#define SCH_CMD_PIPE	  1

#define SIM_MEMORY_ADDR   0xB0000000  /* UTF Simulated Memory Address */   
#define SIM_MEMORY_SIZE   1024        /* UTF Simulated Memory Size    */

/************************************************************************
** SCH global data
*************************************************************************/
extern  SCH_AppData_t SCH_AppData;
extern  void SCH_MajorFrameCallback(void);
extern  void SCH_MinorFrameCallback(void);

/************************************************************************
** SCH test data set
*************************************************************************/
extern uint32           os_clock_accuracy;

boolean LittleEndian=FALSE;
SCH_ScheduleEntry_t Schedule[SCH_TABLE_ENTRIES];
SCH_MessageEntry_t  Messages[SCH_MAX_MESSAGES];
char MostRecentDiagPkt[SCH_TABLE_ENTRIES];
SCH_HkPacket_t MostRecentHkPkt;
char Filenames[10][OS_MAX_PATH_LEN];

int32  MyTimeInSecs = 0;
int32  MyTimeInMillisecs = 0;
uint32 MyTimeInMicrosecs = 0;


boolean PoorMinorFrameTimer=FALSE;
uint32  MajorFrameMETOffset=0;
uint32  MySemaphore=0;

boolean utf_time_registersynchcallback_failure=FALSE;

char DefSDTFilename[] = SCH_SCHEDULE_FILENAME;
char DefMDTFilename[] = SCH_MESSAGE_FILENAME;

typedef struct
{
    int32   Secs;
    int32   Subsecs;
} FrameSignalTimeRec_t;

FrameSignalTimeRec_t MajorFrameSignals[] =
{
        { 0,   0}, { 1,   0}, { 2,   0}, { 3,   0}, { 4,   0}, { 5,   0}, { 6,   0}, { 7,   0}, { 8,   0}, { 9,   0}, 
        {10,   0}, {11,   0}, {12,   0}, {13,   0}, {14,   0}, {15,   0}, {16,   0}, {16, 490}, {18,   0}, {19,   0}, 
        {19, 750}, {20, 250}, {21,   0}, {22,   0}, {23,   0}, {24,   0}, {25,   0}, {26,   0}, 
                                         {32, 500}, {34,   0}, {35,   0}, {36,   0}, {37,   0}, {38,   0}, {39,   0}, 
        {40,   0}, {41,   0}, {42,   0}, {43,   0}, {44,   0}, {45,   0}, {46,   0}, {47,   0}, {48,   0}, {49,   0},
        {-1,  -1}
};

/* The following need to be sent because the Major Frame signal above is ignored by FSW */
/* Note that the Minor Frame Signal is always sent when the Major Frame signal is not   */
FrameSignalTimeRec_t AdditionalMinorFrameSignals[] =
{
        {20, 250}, {21,   0},
        {-1,  -1}
};

/************************************************************************
** Local function prototypes
*************************************************************************/
boolean CreateScheduleDefTblFile(uint32 BadOption, char *Filename); 

boolean CreateMessageDefTblFile(uint32 BadOption, char *Filename);


void    PrintHKPacket(void);

void    PrintDiagPacket(void);

void    PrintLocalHKVars(void);

int32   CFE_SB_SubscribeHook(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId);

void    SwapCfeFileHdr(CFE_FS_Header_t *FileHdrToSwap);

void    SwapCfeTblHdr(CFE_TBL_File_Hdr_t *TblHdrToSwap);

void    ByteSwapUint32(uint32 *Uint32ToSwapPtr);

int32   BinSemTakeHook (uint32 sem_id);
int32   BinSemGiveHook (uint32 sem_id);

uint32  CFE_TIME_GetMETsubsecsHook(void);

int32   CFE_TBL_RegisterHook( CFE_TBL_Handle_t *TblHandlePtr,
                        const char *Name,
                        uint32  Size,
                        uint16  TblOptionFlags,
                        CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr );

int32   CFE_TBL_LoadHook( CFE_TBL_Handle_t TblHandle,
                          CFE_TBL_SrcEnum_t SrcType,
                          const void *SrcDataPtr );

int32   CFE_TBL_GetAddressHook( void **TblPtr,CFE_TBL_Handle_t TblHandle );

int32   CFE_TIME_RegisterSynchCallback( CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr );

void    UTF_SCRIPT_CorruptTableEntry(int argc, char *argv[]);

void    UTF_SCRIPT_LoadCorruptTable(int argc, char *argv[]);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Program main                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main(void)
{
   char AppName[10];
   uint32 EndianCheck=0x01020304;
   int  i=0;
   int  j=1;
   
   if ((*(char *)(&EndianCheck)) == 0x04)
   {
       LittleEndian = TRUE;
   }
   
   strcpy(AppName, "SCH");
   
   /*
   ** Set up to read in script
   */
   SCH_AppData.CmdPipe = SCH_CMD_PIPE;  /* Hook for application code */
   UTF_add_input_file(SCH_AppData.CmdPipe, "sch_utf_cmds.in");
   
   /*
   ** Set up output file and HK and Diag packet handlers           
   */
   UTF_set_output_filename("sch_utf_test.out");
   UTF_set_packet_handler(SCH_HK_TLM_MID, (utf_packet_handler)PrintHKPacket);
   UTF_set_packet_handler(SCH_DIAG_TLM_MID, (utf_packet_handler)PrintDiagPacket);
    
   /*
   ** Initialize time data structures
   */
   UTF_init_sim_time(-0.01);
   MyTimeInSecs = 0;
   MyTimeInMillisecs = -10;

   /*
   ** Set up Hook function to simulate time steps for SCH App
   */
   UTF_OSAPI_set_function_hook(OS_BINSEMTAKE_HOOK, BinSemTakeHook);

   /*
   ** Set up Hook function to simulate time steps for SCH App
   */
   UTF_OSAPI_set_function_hook(OS_BINSEMGIVE_HOOK, BinSemGiveHook);
   
   /*
   ** Register app SCH with executive services.                         
   */
   UTF_ES_InitAppRecords();
   UTF_ES_AddAppRecord(AppName,0);  
   CFE_ES_RegisterApp();
   CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);
   
   /*
   ** Initialize Timer API
   */
   OS_TimerAPIInit();
   
   /*
   ** Initialize table services data structures
   */
   CFE_ES_CDS_EarlyInit();
   CFE_TBL_EarlyInit();

   /*
   ** Add an entry to the volume table
   */
   UTF_add_volume("/", "cf", FS_BASED, FALSE, FALSE, TRUE, "CF", "/cf", 0);

   /*
   ** Add this hook so we can force a software bus read error
   ** in our command input file that will make the application exit
   */
   UTF_add_special_command("SET_SB_RETURN_CODE", UTF_SCRIPT_SB_Set_Api_Return_Code);
   /*
   ** Add this hook so we can force a software bus read error
   ** in our command input file that will make the application exit
   */
   UTF_add_special_command("SET_SB_RETURN_CODE_TO_DEFAULT", UTF_SCRIPT_SB_Use_Default_Api_Return_Code);

   /*
   ** Add this hook so we can force a software bus read error
   ** in our command input file that will make the application exit
   */
   UTF_add_special_command("CORRUPT_TABLE", UTF_SCRIPT_CorruptTableEntry);

   /*
   ** Add this hook so we can force a software bus read error
   ** in our command input file that will make the application exit
   */
   UTF_add_special_command("LOAD_CORRUPT_TABLE", UTF_SCRIPT_LoadCorruptTable);

   /*
   ** Add this hook so we can force a software bus read error
   ** in our command input file that will make the application exit
   */
   UTF_add_special_command("SET_TIME_RETURN_CODE", UTF_SCRIPT_TIME_Set_Api_Return_Code);

   /* Create all Corrupt Table Images */
   /* Locate and remove file extension from default filenames */
   i = strlen(DefSDTFilename);
   while (i > 0)
   {
       i--;
       if (DefSDTFilename[i] == '.')
       {
           DefSDTFilename[i] = '\0';
           i = 0;
       }
   }
   i = strlen(DefMDTFilename);
   while (i > 0)
   {
       i--;
       if (DefMDTFilename[i] == '.')
       {
           DefMDTFilename[i] = '\0';
           i = 0;
       }
   }
   for (i=1; i<7; i++)
   {
       sprintf(&Filenames[j-1][0], "%s%01d.tbl", &DefSDTFilename[0], j);
       j++;
   }
   for (i=1; i<4; i++)
   {
       sprintf(&Filenames[j-1][0], "%s%01d.tbl", &DefMDTFilename[0], j);
       j++;
   }

   /* Initialize the scheduler library*/
#if SCH_LIB_PRESENCE == 1
   SCH_LibInit();
#endif

   /* 
   ** Call Application Main procedure that will test command 
   ** processing through the software bus command pipe via
   ** the sch_utf_cmds.in command script
   */
   UTF_put_text("***UTF SCH CMD PIPE TESTS START***\n\n");
   
   SCH_AppMain();
    
   UTF_put_text("***UTF SCH CMD PIPE TESTS END***\n\n");

   UTF_put_text("***UTF SCH APP POOR TIMER TESTS BEGIN***\n\n");
   /* Simulate situation with a poor minor frame timer */
   SCH_AppData.CmdPipe++;  /* Hook for application code */
   UTF_add_input_file(SCH_AppData.CmdPipe, "sch_utf_hk.in");

   UTF_CFE_SB_Use_Default_Api_Return_Code(CFE_SB_RCVMSG_PROC); 
   
   os_clock_accuracy = SCH_WORST_CLOCK_ACCURACY + 1;
   PoorMinorFrameTimer = TRUE;
   
   /*
   ** Configure simulated time
   */
   UTF_init_sim_time(0.560);
   MyTimeInSecs = 0;
   MyTimeInMillisecs = 560;
   
   /*
   ** Register app SCH with executive services.                         
   */
   UTF_ES_InitAppRecords();
   UTF_ES_AddAppRecord(AppName,0);  
   CFE_ES_RegisterApp();
   CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);
   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Initialize table services data structures
   */
   CFE_ES_CDS_EarlyInit();
   CFE_TBL_EarlyInit();

   UTF_TIME_set_function_hook(CFE_TIME_GETMETSUBSECS_HOOK, (void *)&CFE_TIME_GetMETsubsecsHook);   
   
   UTF_put_text("\n\nFlywheeling Major Frame and Poor Minor Frame Timer \n");
   UTF_put_text("-------------------------------------------------- \n");
   UTF_CFE_TIME_Set_Api_Return_Code(CFE_TIME_GETCLOCKINFO_PROC, CFE_TIME_FLAG_FLYING);
   SCH_AppMain();
   
   UTF_CFE_TIME_Use_Default_Api_Return_Code(CFE_TIME_GETCLOCKINFO_PROC);
   UTF_TIME_set_function_hook(CFE_TIME_GETMETSUBSECS_HOOK, (void *)NULL);
   
   PoorMinorFrameTimer = FALSE;
   os_clock_accuracy = 0;

   
   /* Simulate situation with a poor minor frame timer and flywheeling starts in middle */
   SCH_AppData.CmdPipe++;  /* Hook for application code */
   UTF_add_input_file(SCH_AppData.CmdPipe, "sch_utf_hk2.in");

   UTF_CFE_SB_Use_Default_Api_Return_Code(CFE_SB_RCVMSG_PROC); 
   
   os_clock_accuracy = SCH_WORST_CLOCK_ACCURACY + 1;
   PoorMinorFrameTimer = TRUE;
   
   /*
   ** Configure simulated time
   */
   UTF_init_sim_time(-0.01);
   MyTimeInSecs = 0;
   MyTimeInMillisecs = -10;
   
   /*
   ** Register app SCH with executive services.                         
   */
   UTF_ES_InitAppRecords();
   UTF_ES_AddAppRecord(AppName,0);  
   CFE_ES_RegisterApp();
   CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);
   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Initialize table services data structures
   */
   CFE_ES_CDS_EarlyInit();
   CFE_TBL_EarlyInit();

   UTF_TIME_set_function_hook(CFE_TIME_GETMETSUBSECS_HOOK, (void *)&CFE_TIME_GetMETsubsecsHook);   
   
   UTF_put_text("\n\nFlywheeling Major Frame After startup and Poor Minor Frame Timer \n");
   UTF_put_text("---------------------------------------------------------------- \n");
   MajorFrameMETOffset = 430;
   SCH_AppMain();
   
   UTF_CFE_TIME_Use_Default_Api_Return_Code(CFE_TIME_GETCLOCKINFO_PROC);
   UTF_TIME_set_function_hook(CFE_TIME_GETMETSUBSECS_HOOK, (void *)NULL);
   
   PoorMinorFrameTimer = FALSE;
   os_clock_accuracy = 0;
   UTF_put_text("***UTF SCH APP POOR TIMER TESTS END***\n\n");
   
   /*
   ** Remove Hook function to simulate time steps for SCH App
   */
   UTF_OSAPI_set_function_hook(OS_BINSEMTAKE_HOOK, (void *)NULL);
   
   /*
   ** Remove Hook function to simulate time steps for SCH App
   */
   UTF_OSAPI_set_function_hook(OS_BINSEMGIVE_HOOK, (void *)NULL);
   
   /*
   ** These tests force some CFE api error returns
   ** during SCH initialization. This increases
   ** the gcov coverage metrics for the app startup
   ** code.
   */
   UTF_put_text("***UTF SCH APP INIT TESTS START***\n\n");
 
   UTF_put_text("\n");
   UTF_put_text("Test App Init Error conditions \n");
   UTF_put_text("-------------------------------\n");
   
   /*
   ** Set trigger so CFE_ES_GetAppID returns something
   ** other than CFE_SUCCESS (0). Then call app main, this
   ** should make the app init fail.
   */

   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   UTF_CFE_ES_Set_Api_Return_Code(CFE_ES_GETAPPID_PROC, 0xc4000001L);

   UTF_put_text("Test Init with GetAppID Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   SysLog:1980-001-00:00:06.42000 SCH App: Unable to obtain own AppID, RC=0xC4000001\n");
   UTF_put_text("!   SysLog:1980-001-00:00:06.42000 CFE_ES_RunLoop Error: Cannot get AppID for the caller: RC = C4000001\n");
   UTF_put_text("------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");


   /* Go back to "normal" behavior */
   UTF_CFE_ES_Use_Default_Api_Return_Code(CFE_ES_GETAPPID_PROC);
   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Use hook function because second call needs to fail for the runtime 
   ** table registration. Could have used 'set return code' for first call
   ** (SDT Reg), then hook for second call (MDT reg), but wanted
   ** to be consistent
   */
   UTF_TBL_set_function_hook(CFE_TBL_REGISTER_HOOK, (void *)&CFE_TBL_RegisterHook);   
   
   /*
   ** Test path that terminates SCH due to failure during Schedule table register 
   */
   UTF_put_text("Test Init with SDT Registration Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=7\n");
   UTF_put_text("--------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Test path that terminates HK due to failure during Message Def table register 
   */
   UTF_put_text("Test Init with MDT Registration Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=8\n");
   UTF_put_text("--------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");
       
   /*
   ** Done using hook function, set back to original utf stub 
   */
   UTF_TBL_set_function_hook(CFE_TBL_REGISTER_HOOK, (void *)NULL);
   
   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Use hook function because second call needs to fail for the runtime 
   ** table load. Could have used 'set return code' for first call
   ** (SDT load), then hook for second call (MDT load), but wanted
   ** to be consistent
   */
   UTF_TBL_set_function_hook(CFE_TBL_LOAD_HOOK, (void *)&CFE_TBL_LoadHook);   
   
   /*
   ** Test path that terminates SCH due to failure during Schedule table load 
   */
   UTF_put_text("Test Init with SDT Load Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=9\n");
   UTF_put_text("------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Test path that terminates SCH due to failure during Message Def table load 
   */
   UTF_put_text("Test Init with MDT Load Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=10\n");
   UTF_put_text("------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");
       
   /*
   ** Done using hook function, set back to original utf stub 
   */
   UTF_TBL_set_function_hook(CFE_TBL_LOAD_HOOK, (void *)NULL);
   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Use hook function because second call needs to fail for the runtime 
   ** table GetAddress. Could have used 'set return code' for first call
   ** (SDT GetAddress), then hook for second call (MDT GetAddress), but wanted
   ** to be consistent
   */
   UTF_TBL_set_function_hook(CFE_TBL_GETADDRESS_HOOK, (void *)&CFE_TBL_GetAddressHook);   
   
   /*
   ** Test path that terminates SCH due to failure during Schedule table GetAddress 
   */
   UTF_put_text("Test Init with SDT GetAddress Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=11\n");
   UTF_put_text("------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Test path that terminates SCH due to failure during Message Def table GetAddress
   */
   UTF_put_text("Test Init with MDT GetAddress Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=11\n");
   UTF_put_text("------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");
       
   /*
   ** Done using hook function, set back to original utf stub 
   */
   UTF_TBL_set_function_hook(CFE_TBL_GETADDRESS_HOOK, (void *)NULL);
   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Set trigger so CFE_EVS_Register returns something
   ** other than CFE_SUCCESS (0). Then call app main, this
   ** should make the app init fail.
   */
   UTF_CFE_EVS_Set_Api_Return_Code(CFE_EVS_REGISTER_PROC, 0xc2000003L);
   
   UTF_put_text("Test Init with EVS Registration Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   SysLog: SCH App: Error Registering For Event Services, RC=0xC2000003\n");
   UTF_put_text("--------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   /* Go back to "normal" behavior */
   UTF_CFE_EVS_Use_Default_Api_Return_Code(CFE_EVS_REGISTER_PROC);
   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Set trigger so CFE_SB_CreatePipe returns an error code
   */
   UTF_CFE_SB_Set_Api_Return_Code(CFE_SB_CREATEPIPE_PROC, 0xca000004L);
   
   UTF_put_text("Test Init with Create Pipe Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=3\n");
   UTF_put_text("---------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   UTF_CFE_SB_Use_Default_Api_Return_Code(CFE_SB_CREATEPIPE_PROC); 

   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Hook our own custom function to CFE_SB_Subscribe so we can
   ** trigger an error return on the SECOND call in SCH_AppInit
   */
   UTF_SB_set_function_hook(CFE_SB_SUBSCRIBE_HOOK, (void *)&CFE_SB_SubscribeHook);
   
   UTF_put_text("Test Init with HK Request Subscribe Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=4\n");
   UTF_put_text("------------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   UTF_put_text("Test Init with SCH Cmd Subscribe Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=5\n");
   UTF_put_text("---------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   UTF_SB_set_function_hook(CFE_SB_SUBSCRIBE_HOOK, (void *)NULL);
   
   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Test path that terminates SCH due to an OS_TimerCreate failure
   */
   UTF_OSTIMER_Set_Api_Return_Code(OS_TIMER_CREATE_PROC, OS_ERROR);
   
   UTF_put_text("Test Init with OS_TimerCreate Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=12\n");
   UTF_put_text("------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   UTF_OSTIMER_Use_Default_Api_Return_Code(OS_TIMER_CREATE_PROC);

   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Test path that terminates SCH due to an OS_BinSemCreate failure
   */
   SCH_AppData.TimeSemaphore = OS_MAX_BIN_SEMAPHORES + 1;
   
   UTF_put_text("Test Init with OS_BinSemCreate Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=15\n");
   UTF_put_text("-------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   SCH_AppData.TimeSemaphore = 0; 

   
   /*
   ** Initialize the Timer API
   */
   OS_TimerAPIInit();

   /*
   ** Test path that terminates SCH due to a CFE_TIME_RegisterSynchCallback failure
   */
   UTF_CFE_TIME_Set_Api_Return_Code(CFE_TIME_REGISTERSYNCHCALLBACK_PROC, 0xce000003L);
   
   UTF_put_text("Test Init with CFE_TIME_RegisterSynchCallback Failure \n");
   UTF_put_text("! Expect to see:\n");
   UTF_put_text("!   EVENT ID=14\n");
   UTF_put_text("----------------------------------------------------- \n");    
   SCH_AppMain();
   UTF_put_text("\n");

   UTF_CFE_TIME_Use_Default_Api_Return_Code(CFE_TIME_REGISTERSYNCHCALLBACK_PROC); 

   UTF_put_text("***UTF SCH APP INIT TESTS END***\n\n");

   return 0;
   
} /* end main */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Prints out the current values in the SCH housekeeping packet    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PrintHKPacket (void)
{
    static boolean FirstHkPkt=TRUE;
    boolean Changed=FALSE;

   /* Output the CFS SCH housekeeping data */
   UTF_put_text("SCH HOUSEKEEPING PKT:\n");
   
   if (FirstHkPkt)
   {
       UTF_put_text("   Command Counter                   = %d\n", SCH_AppData.HkPacket.CmdCounter);
       UTF_put_text("   Command Error Counter             = %d\n", SCH_AppData.HkPacket.ErrCounter);
    
       UTF_put_text("   Schedule Activity Success Counter = %d\n", SCH_AppData.HkPacket.ScheduleActivitySuccessCount);
       UTF_put_text("   Schedule Activity Failure Counter = %d\n", SCH_AppData.HkPacket.ScheduleActivityFailureCount);
       UTF_put_text("   Schedule Slots Processed Counter  = %d\n", SCH_AppData.HkPacket.SlotsProcessedCount);
       UTF_put_text("   Schedule Slots Skipped Counter    = %d\n", SCH_AppData.HkPacket.SkippedSlotsCount);
       UTF_put_text("   Multiple Schedule Slots Processed = %d\n", SCH_AppData.HkPacket.MultipleSlotsCount);
       UTF_put_text("   Same Slot Processed Counter       = %d\n", SCH_AppData.HkPacket.SameSlotCount);
       UTF_put_text("   Bad Schedule Table Data Counter   = %d\n", SCH_AppData.HkPacket.BadTableDataCount);
       UTF_put_text("   Table Verification Success Ctr    = %d\n", SCH_AppData.HkPacket.TableVerifySuccessCount);
       UTF_put_text("   Table Verification Failure Ctr    = %d\n", SCH_AppData.HkPacket.TableVerifyFailureCount);
       UTF_put_text("   Table Pass Counter                = %d\n", SCH_AppData.HkPacket.TablePassCount);
       UTF_put_text("   Valid Major Frame Counter         = %d\n", SCH_AppData.HkPacket.ValidMajorFrameCount);
       UTF_put_text("   Missed Major Frame Counter        = %d\n", SCH_AppData.HkPacket.MissedMajorFrameCount);
       UTF_put_text("   Unexpected Major Frame Counter    = %d\n", SCH_AppData.HkPacket.UnexpectedMajorFrameCount);
       UTF_put_text("   Minor Frames Since Tone Counter   = %d\n", SCH_AppData.HkPacket.MinorFramesSinceTone);
       UTF_put_text("   Next Minor Frame Number           = %d\n", SCH_AppData.HkPacket.NextSlotNumber);
       UTF_put_text("   Last Sync MET Slot Number         = %d\n", SCH_AppData.HkPacket.LastSyncMETSlot);
       UTF_put_text("   Major Frame is Ignored            = %s\n", SCH_AppData.HkPacket.IgnoreMajorFrame ? "TRUE" : "FALSE");
       UTF_put_text("   Unexpected Major Frame Detected   = %s\n", SCH_AppData.HkPacket.UnexpectedMajorFrame ? "TRUE" : "FALSE");
       UTF_put_text("   Sync To MET                (0x%02X) = Minor Frame: ", SCH_AppData.HkPacket.SyncToMET);
       if (SCH_AppData.HkPacket.SyncToMET & SCH_MINOR_SYNCHRONIZED)
       {
           UTF_put_text("YES, Major Frame: ");
       }
       else
       {
           UTF_put_text("NO,  Major Frame: ");
       }
       if (SCH_AppData.HkPacket.SyncToMET & SCH_PENDING_MAJOR_SYNCH)
       {
           UTF_put_text("PENDING\n");
       }
       else
       {
           if (SCH_AppData.HkPacket.SyncToMET & SCH_MAJOR_SYNCHRONIZED)
           {
               UTF_put_text("YES\n");
           }
           else
           {
               UTF_put_text("NO\n");
           }
       }
       FirstHkPkt = FALSE;
   }
   else
   {
       UTF_put_text("Changes in HK Pkt since last print out:\n");
       if (MostRecentHkPkt.CmdCounter != SCH_AppData.HkPacket.CmdCounter) 
       {
           UTF_put_text("   Command Counter                   = %d\n", SCH_AppData.HkPacket.CmdCounter);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.ErrCounter != SCH_AppData.HkPacket.ErrCounter) 
       {
           UTF_put_text("   Command Error Counter             = %d\n", SCH_AppData.HkPacket.ErrCounter);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.ScheduleActivitySuccessCount != SCH_AppData.HkPacket.ScheduleActivitySuccessCount) 
       {
           UTF_put_text("   Schedule Activity Success Counter = %d\n", SCH_AppData.HkPacket.ScheduleActivitySuccessCount);
           Changed = TRUE;
       }

       if (MostRecentHkPkt.ScheduleActivitySuccessCount != SCH_AppData.HkPacket.ScheduleActivitySuccessCount) 
       {
           UTF_put_text("   Schedule Activity Failure Counter = %d\n", SCH_AppData.HkPacket.ScheduleActivityFailureCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.SlotsProcessedCount != SCH_AppData.HkPacket.SlotsProcessedCount) 
       {
           UTF_put_text("   Schedule Slots Processed Counter  = %d\n", SCH_AppData.HkPacket.SlotsProcessedCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.SkippedSlotsCount != SCH_AppData.HkPacket.SkippedSlotsCount) 
       {
           UTF_put_text("   Schedule Slots Skipped Counter    = %d\n", SCH_AppData.HkPacket.SkippedSlotsCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.MultipleSlotsCount != SCH_AppData.HkPacket.MultipleSlotsCount) 
       {
           UTF_put_text("   Multiple Schedule Slots Processed = %d\n", SCH_AppData.HkPacket.MultipleSlotsCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.SameSlotCount != SCH_AppData.HkPacket.SameSlotCount) 
       {
           UTF_put_text("   Same Slot Processed Counter       = %d\n", SCH_AppData.HkPacket.SameSlotCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.BadTableDataCount != SCH_AppData.HkPacket.BadTableDataCount) 
       {
           UTF_put_text("   Bad Schedule Table Data Counter   = %d\n", SCH_AppData.HkPacket.BadTableDataCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.TableVerifySuccessCount != SCH_AppData.HkPacket.TableVerifySuccessCount) 
       {
           UTF_put_text("   Table Verification Success Ctr    = %d\n", SCH_AppData.HkPacket.TableVerifySuccessCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.TableVerifyFailureCount != SCH_AppData.HkPacket.TableVerifyFailureCount) 
       {
           UTF_put_text("   Table Verification Failure Ctr    = %d\n", SCH_AppData.HkPacket.TableVerifyFailureCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.TablePassCount != SCH_AppData.HkPacket.TablePassCount) 
       {
           UTF_put_text("   Table Pass Counter                = %d\n", SCH_AppData.HkPacket.TablePassCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.ValidMajorFrameCount != SCH_AppData.HkPacket.ValidMajorFrameCount) 
       {
           UTF_put_text("   Valid Major Frame Counter         = %d\n", SCH_AppData.HkPacket.ValidMajorFrameCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.MissedMajorFrameCount != SCH_AppData.HkPacket.MissedMajorFrameCount) 
       {
           UTF_put_text("   Missed Major Frame Counter        = %d\n", SCH_AppData.HkPacket.MissedMajorFrameCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.UnexpectedMajorFrameCount != SCH_AppData.HkPacket.UnexpectedMajorFrameCount) 
       {
           UTF_put_text("   Unexpected Major Frame Counter    = %d\n", SCH_AppData.HkPacket.UnexpectedMajorFrameCount);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.MinorFramesSinceTone != SCH_AppData.HkPacket.MinorFramesSinceTone) 
       {
           UTF_put_text("   Minor Frames Since Tone Counter   = %d\n", SCH_AppData.HkPacket.MinorFramesSinceTone);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.NextSlotNumber != SCH_AppData.HkPacket.NextSlotNumber) 
       {
           UTF_put_text("   Next Minor Frame Number           = %d\n", SCH_AppData.HkPacket.NextSlotNumber);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.LastSyncMETSlot != SCH_AppData.HkPacket.LastSyncMETSlot) 
       {
           UTF_put_text("   Last Sync MET Slot Number         = %d\n", SCH_AppData.HkPacket.LastSyncMETSlot);
           Changed = TRUE;
       }
       if (MostRecentHkPkt.IgnoreMajorFrame != SCH_AppData.HkPacket.IgnoreMajorFrame) 
       {
           UTF_put_text("   Major Frame is Ignored            = %s\n", SCH_AppData.HkPacket.IgnoreMajorFrame ? "TRUE" : "FALSE");
           Changed = TRUE;
       }
       if (MostRecentHkPkt.UnexpectedMajorFrame != SCH_AppData.HkPacket.UnexpectedMajorFrame) 
       {
           UTF_put_text("   Unexpected Major Frame Detected   = %s\n", SCH_AppData.HkPacket.UnexpectedMajorFrame ? "TRUE" : "FALSE");
           Changed = TRUE;
       }
       if (MostRecentHkPkt.SyncToMET != SCH_AppData.HkPacket.SyncToMET) 
       {
           UTF_put_text("   Sync To MET                (0x%02X) = Minor Frame: ", SCH_AppData.HkPacket.SyncToMET);

           if (SCH_AppData.HkPacket.SyncToMET & SCH_MINOR_SYNCHRONIZED)
           {
               UTF_put_text("YES, Major Frame: ");
           }
           else
           {
               UTF_put_text("NO,  Major Frame: ");
           }
           if (SCH_AppData.HkPacket.SyncToMET & SCH_PENDING_MAJOR_SYNCH)
           {
               UTF_put_text("PENDING\n");
           }
           else
           {
               if (SCH_AppData.HkPacket.SyncToMET & SCH_MAJOR_SYNCHRONIZED)
               {
                   UTF_put_text("YES\n");
               }
               else
               {
                   UTF_put_text("NO\n");
               }
           }
           Changed = TRUE;
       }
       if (!Changed)
       {
           UTF_put_text("   NO CHANGES\n");
       }
   }
   
   memcpy(&MostRecentHkPkt, &SCH_AppData.HkPacket, sizeof(SCH_HkPacket_t));

} /* end PrintHKPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Prints out the current values in the local SCH housekeeping     */
/* variables that don't get copied into the housekeeping packet    */
/* structure until a housekeeping request is received              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PrintLocalHKVars (void)
{
    
   /* Output the CFS SCH housekeeping data */
   UTF_put_text("SCH HOUSEKEEPING DATA:\n");


   UTF_put_text("   Command Counter                   = %d\n", SCH_AppData.CmdCounter);
   UTF_put_text("   Command Error Counter             = %d\n", SCH_AppData.ErrCounter);

   UTF_put_text("   Schedule Activity Success Counter = %d\n", SCH_AppData.ScheduleActivitySuccessCount);
   UTF_put_text("   Schedule Activity Failure Counter = %d\n", SCH_AppData.ScheduleActivityFailureCount);
   UTF_put_text("   Schedule Slots Processed Counter  = %d\n", SCH_AppData.SlotsProcessedCount);
   UTF_put_text("   Schedule Slots Skipped Counter    = %d\n", SCH_AppData.SkippedSlotsCount);
   UTF_put_text("   Multiple Schedule Slots Processed = %d\n", SCH_AppData.MultipleSlotsCount);
   UTF_put_text("   Same Slot Processed Counter       = %d\n", SCH_AppData.SameSlotCount);
   UTF_put_text("   Bad Schedule Table Data Counter   = %d\n", SCH_AppData.BadTableDataCount);
   UTF_put_text("   Table Verification Success Ctr    = %d\n", SCH_AppData.TableVerifySuccessCount);
   UTF_put_text("   Table Verification Failure Ctr    = %d\n", SCH_AppData.TableVerifyFailureCount);
   UTF_put_text("   Table Pass Counter                = %d\n", SCH_AppData.TablePassCount);
   UTF_put_text("   Valid Major Frame Counter         = %d\n", SCH_AppData.ValidMajorFrameCount);
   UTF_put_text("   Missed Major Frame Counter        = %d\n", SCH_AppData.MissedMajorFrameCount);
   UTF_put_text("   Unexpected Major Frame Counter    = %d\n", SCH_AppData.UnexpectedMajorFrameCount);
   UTF_put_text("   Minor Frames Since Tone Counter   = %d\n", SCH_AppData.MinorFramesSinceTone);
   UTF_put_text("   Next Minor Frame Number           = %d\n", SCH_AppData.NextSlotNumber);
   UTF_put_text("   Last Sync MET Slot Number         = %d\n", SCH_AppData.LastSyncMETSlot);
   UTF_put_text("   Major Frame is Ignored            = %s\n", SCH_AppData.IgnoreMajorFrame ? "TRUE" : "FALSE");
   UTF_put_text("   Unexpected Major Frame Detected   = %s\n", SCH_AppData.UnexpectedMajorFrame ? "TRUE" : "FALSE");
   UTF_put_text("   Sync To MET                (0x%02X) = Minor Frame: ", SCH_AppData.SyncToMET);
   if (SCH_AppData.SyncToMET & SCH_MINOR_SYNCHRONIZED)
   {
       UTF_put_text("YES, Major Frame: ");
   }
   else
   {
       UTF_put_text("NO,  Major Frame: ");
   }
   if (SCH_AppData.SyncToMET & SCH_PENDING_MAJOR_SYNCH)
   {
       UTF_put_text("PENDING\n");
   }
   else
   {
       if (SCH_AppData.SyncToMET & SCH_MAJOR_SYNCHRONIZED)
       {
           UTF_put_text("YES\n");
       }
       else
       {
           UTF_put_text("NO\n");
       }
   }

} /* end PrintLocalHKVars */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Prints out the current values in the SCH Diagnostic packet      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PrintDiagPacket (void)
{
   static boolean FirstDiagPkt=TRUE;
   char States[8];
   uint32 EntryWordIndex = 0;
   uint32 EntryIndex = 0;
   uint32 SlotIndex = 0;
   uint32 TblIndex = 0;
   uint32 StateIndex = 0;
   boolean DetectedChange = FALSE;
   
   /* Output the CFS SCH Diagnostic data */
   UTF_put_text("SCH DIAGNOSTIC PKT:\n");

   if (FirstDiagPkt)
   {
       UTF_put_text("  Slot #       Entries\n   ");
       for (EntryIndex=0; EntryIndex<SCH_ENTRIES_PER_SLOT; EntryIndex++)
       {
           UTF_put_text("%14d", EntryIndex);
       }
       UTF_put_text("\n         ");
       for (EntryIndex=0; EntryIndex<SCH_ENTRIES_PER_SLOT; EntryIndex++)
       {
           UTF_put_text("   State MsgID");
       }
       UTF_put_text("\n");
   }
   
   EntryIndex = 0;
   
   while (EntryWordIndex < (SCH_NUM_STATUS_BYTES_REQD/2))
   {
       switch ((SCH_AppData.DiagPacket.EntryStates[EntryWordIndex] & 0xC000))
       {
           case 0x8000:
               States[0] = 'D';
               break;
        
           case 0x4000:
               States[0] = 'E';
               break;
        
           case 0x0000:
               States[0] = 'U';
               break;
        
           default:
               States[0] = 'X';
               break;
       }

       switch ((SCH_AppData.DiagPacket.EntryStates[EntryWordIndex] & 0x3000))
       {
           case 0x2000:
               States[1] = 'D';
               break;
        
           case 0x1000:
               States[1] = 'E';
               break;
        
           case 0x0000:
               States[1] = 'U';
               break;
        
           default:
               States[1] = 'X';
               break;
       }

       switch ((SCH_AppData.DiagPacket.EntryStates[EntryWordIndex] & 0x0C00))
       {
           case 0x0800:
               States[2] = 'D';
               break;
        
           case 0x0400:
               States[2] = 'E';
               break;
        
           case 0x0000:
               States[2] = 'U';
               break;
        
           default:
               States[2] = 'X';
               break;
       }

       switch ((SCH_AppData.DiagPacket.EntryStates[EntryWordIndex] & 0x0300))
       {
           case 0x0200:
               States[3] = 'D';
               break;
        
           case 0x0100:
               States[3] = 'E';
               break;
        
           case 0x0000:
               States[3] = 'U';
               break;
        
           default:
               States[3] = 'X';
               break;
       }

       switch ((SCH_AppData.DiagPacket.EntryStates[EntryWordIndex] & 0x00C0))
       {
           case 0x0080:
               States[4] = 'D';
               break;
        
           case 0x0040:
               States[4] = 'E';
               break;
        
           case 0x0000:
               States[4] = 'U';
               break;
        
           default:
               States[4] = 'X';
               break;
       }

       switch ((SCH_AppData.DiagPacket.EntryStates[EntryWordIndex] & 0x0030))
       {
           case 0x0020:
               States[5] = 'D';
               break;
        
           case 0x0010:
               States[5] = 'E';
               break;
        
           case 0x0000:
               States[5] = 'U';
               break;
        
           default:
               States[5] = 'X';
               break;
       }

       switch ((SCH_AppData.DiagPacket.EntryStates[EntryWordIndex] & 0x000C))
       {
           case 0x0008:
               States[6] = 'D';
               break;
        
           case 0x0004:
               States[6] = 'E';
               break;
        
           case 0x0000:
               States[6] = 'U';
               break;
        
           default:
               States[6] = 'X';
               break;
       }

       switch ((SCH_AppData.DiagPacket.EntryStates[EntryWordIndex] & 0x0003))
       {
           case 0x0002:
               States[7] = 'D';
               break;
        
           case 0x0001:
               States[7] = 'E';
               break;
        
           case 0x0000:
               States[7] = 'U';
               break;
        
           default:
               States[7] = 'X';
               break;
       }
       
       while ((StateIndex<8) && (TblIndex < SCH_TABLE_ENTRIES))
       {
           if (EntryIndex == 0)
           {
               if (FirstDiagPkt)
               {
                   UTF_put_text("   %4d  ", SlotIndex);
               }
           }
           while ((EntryIndex<SCH_ENTRIES_PER_SLOT) && (StateIndex<8) && (TblIndex < SCH_TABLE_ENTRIES))
           {
               if (FirstDiagPkt)
               {
                   UTF_put_text("     %c   x%04X", States[StateIndex], SCH_AppData.DiagPacket.MsgIDs[TblIndex]);
                   MostRecentDiagPkt[TblIndex] = States[StateIndex];
               }
               else
               {
                   if (MostRecentDiagPkt[TblIndex] != States[StateIndex])
                   {
                       UTF_put_text("   Diff from Previous: Slot: %d, Entry: %d, Old: %c -> New: %c, MsgID: 0x%04X\n",
                                    SlotIndex, EntryIndex, MostRecentDiagPkt[TblIndex], States[StateIndex], 
                                    SCH_AppData.DiagPacket.MsgIDs[TblIndex]);
                       
                       MostRecentDiagPkt[TblIndex] = States[StateIndex];
                       
                       DetectedChange = TRUE;
                   }
               }
               TblIndex++;
               EntryIndex++;
               StateIndex++;
           }
           if ((EntryIndex == SCH_ENTRIES_PER_SLOT) || (TblIndex == SCH_TABLE_ENTRIES))
           {
               if (FirstDiagPkt)
               {
                   UTF_put_text("\n");
               }
               EntryIndex = 0;
               SlotIndex++;
           }
       }
       StateIndex = 0;
       EntryWordIndex++;
   }
   
   if ((!FirstDiagPkt) && (!DetectedChange))
   {
       UTF_put_text("   No Differences from Previous SCH Diagnostic Packet\n");
   }
   UTF_put_text("\n");
   
   FirstDiagPkt = FALSE;

} /* end PrintDiagPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Functions for swapping file headers to Big Endian format        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void    SwapCfeFileHdr(CFE_FS_Header_t *FileHdrToSwap)
{
    ByteSwapUint32(&FileHdrToSwap->ContentType);
    ByteSwapUint32(&FileHdrToSwap->SubType);
    ByteSwapUint32(&FileHdrToSwap->Length);
    ByteSwapUint32(&FileHdrToSwap->SpacecraftID);
    ByteSwapUint32(&FileHdrToSwap->ProcessorID);
    ByteSwapUint32(&FileHdrToSwap->ApplicationID);
    ByteSwapUint32(&FileHdrToSwap->TimeSeconds);
    ByteSwapUint32(&FileHdrToSwap->TimeSubSeconds);
} /* End of SwapCfeFileHdr() */

void    SwapCfeTblHdr(CFE_TBL_File_Hdr_t *TblHdrToSwap)
{
    ByteSwapUint32(&TblHdrToSwap->Reserved);
    ByteSwapUint32(&TblHdrToSwap->Offset);
    ByteSwapUint32(&TblHdrToSwap->NumBytes);
} /* End of SwapCfeTblHdr() */

void ByteSwapUint32(uint32 *Uint32ToSwapPtr)
{
    int32 Temp = *Uint32ToSwapPtr;
    char *InPtr = (char *)&Temp;
    char *OutPtr = (char *)Uint32ToSwapPtr;
    
    OutPtr[0] = InPtr[3];
    OutPtr[1] = InPtr[2];
    OutPtr[2] = InPtr[1];
    OutPtr[3] = InPtr[0];    
} /* End of ByteSwapUint32() */

int32   BinSemGiveHook (uint32 sem_id)
{
    MySemaphore++;
    
    return OS_SUCCESS;
}

int32   BinSemTakeHook (uint32 sem_id)
{
    float CurrentTime;
    boolean SendMajorFrameSignal = FALSE;
    boolean SendAdditionalMinorFrameSignal = FALSE;
    int32 i=0;

    if (SCH_AppData.TimeSemaphore == sem_id)
    {
        if (PoorMinorFrameTimer)
        {
            if (MySemaphore > 0) MySemaphore = 0;
            
            while (MySemaphore == 0)
            {
                /* Simulate a scatter-brained minor frame timer that triggers a little early and/or late */
                MyTimeInMillisecs += 10;
                
                switch (MyTimeInMillisecs)
                {
                    case 30: /* Wake up a little too early */
                        MyTimeInMicrosecs = 29000;
                        break;
                     
                    case 40: /* Wake up a little too late */
                        MyTimeInMicrosecs = 41000;
                        break;

                    case 520: /* Wake up a cycle too late */
                        MyTimeInMicrosecs = 530000;
                        break;
                        
                    case 530: /* Wake up shortly to catch up */
                        MyTimeInMicrosecs = 537000;
                        break;
                        
                    case 560:
                        if (MyTimeInSecs == 1)
                        {
                            MyTimeInMicrosecs = 559000;  /* Running a little fast */
                        }
                        else
                        {
                            MyTimeInMicrosecs = 560000;
                        }
                        break;
                        
                    case 570:
                        if (MyTimeInSecs == 1)
                        {
                            MyTimeInMicrosecs = 569000;  /* Continuing to run a little fast */
                        }
                        else
                        {
                            MyTimeInMicrosecs = 570000;
                        }
                        break;
#if SCH_LIB_PRESENCE == 1
                    case 620: /* Disable the schedule */
                        SCH_DisableProcessing();
                        MyTimeInMicrosecs = 620000;
                        break;

                    case 650: /* ReEnable the schedule */
                        SCH_EnableProcessing();
                        MyTimeInMicrosecs = 650000;
                        break;
#endif            
                    case 750:  /* Wake up too early but test round-up */
                        MyTimeInMicrosecs = 749999;
                        break;
                        
                    case 1000:
                        /* Check what happens when we need to round up last/first slot */
                        if (MyTimeInSecs == 2)
                        {
                            MyTimeInMicrosecs = 999999;
                            MyTimeInSecs += 1;
                            MyTimeInMillisecs = 0;
                        }
                        else
                        {
                            MyTimeInMicrosecs = 0;
                        }
                        break;
                        
                    default:
                        MyTimeInMicrosecs = MyTimeInMillisecs * 1000;
                        break;
                }
                
                if (MyTimeInMillisecs == 1000)
                {
                    MyTimeInSecs += 1;
                    MyTimeInMillisecs = 0;
                }
                
                CurrentTime = MyTimeInMillisecs/1000.0;
                CurrentTime += MyTimeInSecs;
                
                /* Update the simulated time by 10 milliseconds */
                UTF_set_sim_time(CurrentTime);
                
                /* Let's assume the Major Frame Timer is reliable except at T=2secs */
                if ((MyTimeInMillisecs == MajorFrameMETOffset) && (MyTimeInSecs != 2))
                {
                    UTF_put_text("****Current Sim Time: %f **** Major Frame Signal\n", UTF_get_sim_time());
                    SCH_MajorFrameCallback();
                    
                    /* If the major frame did not release the semaphore, then allow the minor frame timer to do so */
                    if (MySemaphore == 0)
                    {
                        SCH_MinorFrameCallback();
                    }
                }
                else
                {
                    if ((MyTimeInMillisecs >= MajorFrameMETOffset) || (MyTimeInSecs > 0))
                    {
                        SCH_MinorFrameCallback();
                    }
                }
            }
        }
        else
        {
            MyTimeInMillisecs += 10;
            
            if (MyTimeInMillisecs == 1000)
            {
                MyTimeInSecs += 1;
                MyTimeInMillisecs = 0;
            }
            
            CurrentTime = MyTimeInMillisecs/1000.0;
            CurrentTime += MyTimeInSecs;
            
            /* Update the simulated time by 10 milliseconds */
            UTF_set_sim_time(CurrentTime);
            
            /* Determine if we are to issue a major frame signal */
            i = 0;
            while ((MajorFrameSignals[i].Secs != -1) && (!SendMajorFrameSignal))
            {
                if ((MyTimeInSecs == MajorFrameSignals[i].Secs) && (MyTimeInMillisecs == MajorFrameSignals[i].Subsecs))
                {
                    SendMajorFrameSignal = TRUE;
                }
                i++;
            }
            
            /* If we are sending a Major Frame signal, see if we need to follow up with a MinorFrame signal as well */
            if (SendMajorFrameSignal)
            {
                i = 0;
                while ((AdditionalMinorFrameSignals[i].Secs != -1) && (!SendAdditionalMinorFrameSignal))
                {
                    if ((MyTimeInSecs == AdditionalMinorFrameSignals[i].Secs) && 
                        (MyTimeInMillisecs == AdditionalMinorFrameSignals[i].Subsecs))
                    {
                        SendAdditionalMinorFrameSignal = TRUE;
                    }
                    i++;
                }
            }
            
            if (SendMajorFrameSignal)
            {
                UTF_put_text("****Current Sim Time: %f **** Major Frame Signal\n", UTF_get_sim_time());
                
                if (!SendAdditionalMinorFrameSignal)
                {
                    /* If we are not also sending a Minor Frame Signal, then jump simulated time ahead */
                    if (MyTimeInMillisecs != 0)
                    {
                        MyTimeInMillisecs = 0;
                        MyTimeInSecs += 1;
                        
                        CurrentTime = MyTimeInSecs;
                        
                        UTF_set_sim_time(CurrentTime);
                    }
                }
                
                SCH_MajorFrameCallback();
                
                if (SendAdditionalMinorFrameSignal)
                {
                    SCH_MinorFrameCallback();
                }
            }
            else
            {
                SCH_MinorFrameCallback();
            }
        }
    }
    
    return OS_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Hook function for CFE_TBL_Register that will return an error    */
/* on some calls                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CFE_TBL_RegisterHook( CFE_TBL_Handle_t *TblHandlePtr,
                        const char *Name,
                        uint32  Size,
                        uint16  TblOptionFlags,
                        CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr ) 
{
    static uint32 Count = 0;

    Count++;
    
    if ((Count == 1)||/* used to fail the first register call in SCH_TblInit */
        (Count == 3))/* used to fail the second register call in SCH_TblInit */
    {
        return(CFE_TBL_ERR_INVALID_SIZE);
    }else{
       return(CFE_SUCCESS);
    }
       

}/* end CFE_TBL_RegisterHook */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Hook function for CFE_TBL_Load that will return an error        */
/* on some calls                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32   CFE_TBL_LoadHook( CFE_TBL_Handle_t TblHandle,
                          CFE_TBL_SrcEnum_t SrcType,
                          const void *SrcDataPtr )
{
    static uint32 Count = 0;

    Count++;
    
    if ((Count == 1)||/* used to fail the first register call in SCH_TblInit */
        (Count == 3))/* used to fail the second register call in SCH_TblInit */
    {
        return(CFE_TBL_ERR_NO_ACCESS);
    }else{
       return(CFE_SUCCESS);
    }
       

}/* end CFE_TBL_LoadHook */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Hook function for CFE_TBL_GetAddress that will return an error  */
/* on some calls                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CFE_TBL_GetAddressHook( void **TblPtr,
                              CFE_TBL_Handle_t TblHandle )
{
    static uint32 Count = 0;

    Count++;
    
    if ((Count == 1)||/* used to fail the first GetAddress call SCH_TblInit */
        (Count == 3))/* used to fail the second GetAddress call SCH_TblInit */
    {
        return(CFE_TBL_ERR_UNREGISTERED);
    }else if(Count == 2){
        return CFE_TBL_INFO_UPDATED;
    }else{
       return(CFE_SUCCESS);
    }
       
}/* end CFE_TBL_GetAddressHook */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Hook function for CFE_SB_Subscribe that will return an error    */
/* on every call except the first one                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CFE_SB_SubscribeHook(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId) 
{
    static uint32 Count = 0;

    Count++;
    
    if ((Count == 1)||/* used to fail the first SB_Subscribe call SCH_TblInit */
        (Count == 3))/* used to fail the second SB_Subscribe call SCH_TblInit */
    {
        return(CFE_SB_MAX_MSGS_MET);
    }else{
       return(CFE_SUCCESS);
    }
}/* end CFE_SB_SubscribeHook */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Hook function for CFE_TIME_GetMETsubsecs that will return an    */
/* simulated sub seconds field                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32  CFE_TIME_GetMETsubsecsHook(void)
{
    return CFE_TIME_Micro2SubSecs(MyTimeInMicrosecs);
}/* end CFE_TIME_GetMETsubsecsHook */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Function for inserting a table entry corruption                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void    UTF_SCRIPT_CorruptTableEntry(int argc, char *argv[])
{
    uint32  CorruptionType;
    
    CorruptionType = UTF_arg2uint(argv[1]);
    
    switch(CorruptionType)
    {
        /* Msg Index out of range */
        case 0:
            SCH_AppData.ScheduleTable[450].EnableState = SCH_ENABLED;
            SCH_AppData.ScheduleTable[450].Type = SCH_ACTIVITY_SEND_MSG;
            SCH_AppData.ScheduleTable[450].Frequency = 3;
            SCH_AppData.ScheduleTable[450].Remainder = 0;
            SCH_AppData.ScheduleTable[450].MessageIndex = SCH_MAX_MESSAGES;
            SCH_AppData.ScheduleTable[450].GroupData = 0x12345678;
            break;
        
        /* Frequency = 0 */
        case 1:
            SCH_AppData.ScheduleTable[451].EnableState = SCH_ENABLED;
            SCH_AppData.ScheduleTable[451].Type = SCH_ACTIVITY_SEND_MSG;
            SCH_AppData.ScheduleTable[451].Frequency = 0;
            SCH_AppData.ScheduleTable[451].Remainder = 0;
            SCH_AppData.ScheduleTable[451].MessageIndex = 1;
            SCH_AppData.ScheduleTable[451].GroupData = 0x12345678;
            break;
            
        /* Type != Send Message */
        case 2:
            SCH_AppData.ScheduleTable[452].EnableState = SCH_ENABLED;
            SCH_AppData.ScheduleTable[452].Type = 2;
            SCH_AppData.ScheduleTable[452].Frequency = 3;
            SCH_AppData.ScheduleTable[452].Remainder = 0;
            SCH_AppData.ScheduleTable[452].MessageIndex = 1;
            SCH_AppData.ScheduleTable[452].GroupData = 0x12345678;
            break;
            
        /* Remainder >= Frequency */
        case 3:
            SCH_AppData.ScheduleTable[453].EnableState = SCH_ENABLED;
            SCH_AppData.ScheduleTable[453].Type = SCH_ACTIVITY_SEND_MSG;
            SCH_AppData.ScheduleTable[453].Frequency = 3;
            SCH_AppData.ScheduleTable[453].Remainder = 3;
            SCH_AppData.ScheduleTable[453].MessageIndex = 1;
            SCH_AppData.ScheduleTable[453].GroupData = 0x12345678;
             break;
            
        default:
            UTF_put_text("!!ERROR CORRUPT_TABLE command parameter unrecognized\n");
            break;
    }
}

void    UTF_SCRIPT_LoadCorruptTable(int argc, char *argv[])
{
    uint32  CorruptTblType;
    
    CorruptTblType = UTF_arg2uint(argv[1]);
    
    if (CorruptTblType < 7)
    {
        UTF_put_text("Loading Corrupt Table Image: %s\n", &Filenames[CorruptTblType-1][0]);
        UTF_TBL_LoadTableFromGround("SCH.SCHED_DEF", &Filenames[CorruptTblType-1][0]);
    }
    else if (CorruptTblType < 10)
    {
        UTF_put_text("Loading Corrupt Table Image: %s\n", &Filenames[CorruptTblType-1][0]);
        UTF_TBL_LoadTableFromGround("SCH.MSG_DEFS", &Filenames[CorruptTblType-1][0]);
    }
    else
    {
        UTF_put_text("!!ERROR LOAD_CORRUPT_TABLE Parameter out of range!\n");
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Functions added until UTF is up to date with latest cFE         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
void CFE_ES_WaitForStartupSync(uint32 TimeOutMilliseconds)
{
    UTF_put_text("SCH App waits up to %d millisecs for all apps to synchronize\n", TimeOutMilliseconds);
}
*/
/*
int32 OS_TimerCreate(uint32 *timer_id, uint32 *clockaccuracy, OS_TimerSignal_t signal)
{
    if (utf_os_timercreate_failure)
        return OS_ERROR;
    else if (utf_os_timercreate_accuracy != 0)
    {
        *clockaccuracy = utf_os_timercreate_accuracy;
        return OS_SUCCESS;
    }
    else
        return OS_SUCCESS;
}

int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 time_interval)
{
    return OS_SUCCESS;
}

int32 OS_TimerDelete(uint32 timer_id)
{
    return OS_SUCCESS;
}
*/

