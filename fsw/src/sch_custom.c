/*
** $Id: sch_custom.c 1.3 2015/03/01 14:01:44EST sstrege Exp  $
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
** Purpose: Scheduler (SCH) application custom component
**
** Author:
**
** Notes:
**
** $Log: sch_custom.c  $
** Revision 1.3 2015/03/01 14:01:44EST sstrege 
** Added copyright information
** Revision 1.2 2011/06/30 20:39:09EDT aschoeni 
** updated OS_SUCCESS to CFE_SUCCESS for custom earlyinit
** Revision 1.1 2011/06/30 15:30:03EDT aschoeni 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/src/project.pj
*/

/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "cfe.h"
#include "sch_platform_cfg.h"

#include "sch_app.h"

#include "cfe_time_msg.h"
#include "cfe_psp.h"


/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

/*
** Timer Characteristics
*/
#define SCH_TIMER_NAME   "SCH_MINOR_TIMER"

/*************************************************************************
** Local function prototypes
**************************************************************************/

/************************************************************************/
/** \brief Computes a minor slot number from a MET subseconds zero point
**  
**  \par Description
**       This function determines the current slot (minor frame) number if
**       one were to assume that slot zero started when the MET microseconds
**       are equal to zero. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retstmt Returns slot index from zero to (#SCH_TOTAL_SLOTS-1) \endcode
**  \endreturns
**
*************************************************************************/
uint32 SCH_GetMETSlotNumber(void);

/************************************************************************/
/** \brief Performs Major Frame Synchronization
**  
**  \par Description
**       This function is called by cFE TIME services when a Major Frame
**       synchronization signal is received.  It then synchronizes the
**       minor frame (slot) processing of the Schedule Definition Table. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void  SCH_MajorFrameCallback(void);

/************************************************************************/
/** \brief Performs Minor Frame time step
**  
**  \par Description
**       This function is called by an OSAL timer when the minor frame
**       timing reference sends a signal.  The Scheduler Application uses
**       this to drive the Application's processing of each minor frame. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void  SCH_MinorFrameCallback(uint32 TimerId);



/*************************************************************************
**
** Function definitions
**
**************************************************************************/


/*************************************************************************
**
** At some point the CFE_PSP should support customizable timers, which at
** minimum can fall through to the OSAL timers. Until this is implemented
** in CFE_PSP, this redefinition supplies the same functionality to allow
** Scheduler to work out of box.
**
**************************************************************************/
#ifndef CFE_PSP_MAX_TIMERS

int32 CFE_PSP_TimerCreate(uint32 *timer_id, const char *timer_name,
                     uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr);
int32 CFE_PSP_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time);


int32 CFE_PSP_TimerCreate(uint32 *timer_id,       const char         *timer_name,
                     uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr)
{
   int32 Status = CFE_SUCCESS;

   Status = OS_TimerCreate(timer_id, timer_name, clock_accuracy, callback_ptr);

   if(Status == OS_SUCCESS)
   {
      Status = CFE_SUCCESS;
   }

   return Status;
}
int32 CFE_PSP_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
   return OS_TimerSet(timer_id, start_time, interval_time);
}

#endif


/*******************************************************************
**
** SCH_CustomEarlyInit
**
** NOTE: For complete prolog information, see 'sch_custom.h'
**
** This function MUST update SCH_AppData.ClockAccuracy to the
** resolution of the minor frame timer.
********************************************************************/

int32 SCH_CustomEarlyInit(void)
{
    int32             Status = CFE_SUCCESS;
    
    Status = CFE_PSP_TimerCreate(&SCH_AppData.TimerId,
                                 SCH_TIMER_NAME,
                                 &SCH_AppData.ClockAccuracy,
                                 SCH_MinorFrameCallback);
    
    return Status;

} /* End of CustomEarlyInit() */


/*******************************************************************
**
** SCH_CustomLateInit
**
** NOTE: For complete prolog information, see 'sch_custom.h'
**
** This function MUST perform any startup synchronization required,
** and MUST finish setting up the major and minor frame timers. 
********************************************************************/

int32 SCH_CustomLateInit(void)
{
    int32  Status    = CFE_SUCCESS;
    
    CFE_ES_WaitForStartupSync(SCH_STARTUP_SYNC_TIMEOUT);

    /*
    ** Connect to cFE TIME's time reference marker (typically 1 Hz)
    ** to use it as the Major Frame synchronization source
    */
    Status = CFE_TIME_RegisterSynchCallback((CFE_TIME_SynchCallbackPtr_t)&SCH_MajorFrameCallback);
    if (Status == CFE_SUCCESS)
    {
        /*
        ** Start the Minor Frame Timer with an extended delay to allow a Major Frame Sync
        ** to start processing.  If the Major Frame Sync fails to arrive, then we will
        ** start when this timer expires and synch ourselves to the MET clock.
        */
        Status = CFE_PSP_TimerSet(SCH_AppData.TimerId, SCH_STARTUP_PERIOD, 0);
    }

    return Status;

} /* End of SH_CustomLateInit() */


/*******************************************************************
**
** SCH_CustomGetCurrentSlotNumber
**
** NOTE: For complete prolog information, see 'sch_custom.h'
********************************************************************/

uint32 SCH_CustomGetCurrentSlotNumber(void)
{
    uint32  CurrentSlot;
    
    if (SCH_AppData.SyncToMET != SCH_NOT_SYNCHRONIZED)
    {
        CurrentSlot = SCH_GetMETSlotNumber();
        
        /* 
        ** If we are only concerned with synchronizing the minor frames to an MET,
        ** then we need to adjust the current slot by whatever MET time is prevalent
        ** when the Major Frame Signal is received.
        ** If we are synchronizing the Major Frame, then, by definition, LastSyncMETSlot
        ** would be a zero and the current slot would be appropriate.
        */
        if (CurrentSlot < SCH_AppData.LastSyncMETSlot)
        {
            CurrentSlot = CurrentSlot + SCH_TOTAL_SLOTS - SCH_AppData.LastSyncMETSlot;
        }
        else
        {
            CurrentSlot = CurrentSlot - SCH_AppData.LastSyncMETSlot;
        }
    }
    else
    {
        CurrentSlot = SCH_AppData.MinorFramesSinceTone;
    }
    
    return CurrentSlot;
} /* End of SH_CustomGetCurrentSlotNumber() */

/*******************************************************************
**
** SCH_CustomCleanup
**
** NOTE: For complete prolog information, see 'sch_custom.h'
********************************************************************/

void SCH_CustomCleanup(void)
{
    /* unregister the TIME callback for the major frame */
    CFE_TIME_UnregisterSynchCallback((CFE_TIME_SynchCallbackPtr_t)&SCH_MajorFrameCallback);

} /* End of SH_CustomCleanup() */


/*******************************************************************
**
** SCH_GetMETSlotNumber
**
** NOTE: For complete prolog information, see above
********************************************************************/

uint32 SCH_GetMETSlotNumber(void)
{
    uint32 SubSeconds = 0;
    uint32 MicroSeconds;
    uint32 Remainder;
    uint32 METSlot;
    
    /*
    ** Use MET rather than current time to avoid time changes
    */
    SubSeconds = CFE_TIME_GetMETsubsecs();

    /*
    ** Convert sub-seconds to micro-seconds
    */
    MicroSeconds = CFE_TIME_Sub2MicroSecs(SubSeconds);

    /*
    ** Calculate schedule table slot number
    */
    METSlot = (MicroSeconds / SCH_NORMAL_SLOT_PERIOD);

    /*
    ** Check to see if close enough to round up to next slot
    */
    Remainder = MicroSeconds - (METSlot * SCH_NORMAL_SLOT_PERIOD);
    
    /*
    ** Add one more microsecond and see if it is sufficient to add another slot
    */
    Remainder += 1;
    METSlot += (Remainder / SCH_NORMAL_SLOT_PERIOD);
    
    /*
    ** Check to see if the Current Slot number needs to roll over
    */
    if (METSlot == SCH_TOTAL_SLOTS)
    {
        METSlot = 0;
    }
    
    return METSlot;
    
}


/*******************************************************************
**
** SCH_MajorFrameCallback
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_MajorFrameCallback(void)
{
    /*
    ** Synchronize slot zero to the external tone signal
    */
    uint16 StateFlags;

    /*
    ** If cFE TIME is in FLYWHEEL mode, then ignore all synchronization signals
    */
    StateFlags = CFE_TIME_GetClockInfo();
    
    if ((StateFlags & CFE_TIME_FLAG_FLYING) == 0)
    {
        /*
        ** Determine whether the major frame is noisy or not
        **
        ** Conditions below are as follows:
        **    If we are NOT synchronized to the MET (i.e. - the Minor Frame timer
        **    has an acceptable resolution), then the Major Frame signal should
        **    only occur in the last slot of the schedule table.
        **
        **    If we ARE synchronized to the MET (i.e. - the Minor Frame timer is
        **    not as good as we would like), then the Major Frame signal should
        **    occur within a window of slots at the end of the table.
        */
        if (((SCH_AppData.SyncToMET == SCH_NOT_SYNCHRONIZED) && 
             (SCH_AppData.MinorFramesSinceTone != SCH_TIME_SYNC_SLOT)) ||
            ((SCH_AppData.SyncToMET == SCH_MINOR_SYNCHRONIZED) && 
             (SCH_AppData.NextSlotNumber != 0) && 
             (SCH_AppData.NextSlotNumber < 
              (SCH_TOTAL_SLOTS - SCH_AppData.WorstCaseSlotsPerMinorFrame - 1))))
        {
            /*
            ** Count the number of consecutive noisy major frames and the Total number
            ** of noisy major frames.  Also, indicate in telemetry that this particular
            ** Major Frame signal is considered noisy.
            */
            SCH_AppData.UnexpectedMajorFrame = TRUE;
            SCH_AppData.UnexpectedMajorFrameCount++;

            /*
            ** If the Major Frame is not being ignored yet, then increment the consecutive noisy
            ** Major Frame counter.
            */
            if (!SCH_AppData.IgnoreMajorFrame)
            {
                SCH_AppData.ConsecutiveNoisyFrameCounter++;
                
                /*
                ** If the major frame is too "noisy", then send event message and ignore future signals
                */
                if (SCH_AppData.ConsecutiveNoisyFrameCounter >= SCH_MAX_NOISY_MAJORF)
                {
                    SCH_AppData.IgnoreMajorFrame = TRUE;
                }
            }
        }
        else /* Major Frame occurred when expected */
        {
            SCH_AppData.UnexpectedMajorFrame = FALSE;
            SCH_AppData.ConsecutiveNoisyFrameCounter = 0;
        }
        
        /*
        ** Ignore this callback if SCH has detected a noisy Major Frame Synch signal
        */
        if (SCH_AppData.IgnoreMajorFrame == FALSE)
        {
            /*
            ** Stop Minor Frame Timer (which should be waiting for an unusually long
            ** time to allow the Major Frame source to resynchronize timing) and start
            ** it again with nominal Minor Frame timing
            */
            CFE_PSP_TimerSet(SCH_AppData.TimerId, SCH_NORMAL_SLOT_PERIOD, SCH_NORMAL_SLOT_PERIOD);
    
            /*
            ** Increment Major Frame process counter
            */
            SCH_AppData.ValidMajorFrameCount++;
    
            /*
            ** Set current slot = zero to synchronize activities
            */
            SCH_AppData.MinorFramesSinceTone = 0;
            
            /*
            ** Major Frame Source is now from CFE TIME
            */
            SCH_AppData.MajorFrameSource = SCH_MAJOR_FS_CFE_TIME;
            
            /* Clear any Major Frame In Sync with MET flags */
            /* But keep the Minor Frame In Sync with MET flag if it is set */
            SCH_AppData.SyncToMET &= SCH_MINOR_SYNCHRONIZED;
            
            /*
            ** Give "wakeup SCH" semaphore
            */
            OS_BinSemGive(SCH_AppData.TimeSemaphore);
        }
    }

    /*
    ** We should assume that the next Major Frame will be in the same MET slot as this 
    */
    SCH_AppData.LastSyncMETSlot = SCH_GetMETSlotNumber();

    return;

} /* End of SCH_MajorFrameCallback() */


/*******************************************************************
**
** SCH_MinorFrameCallback
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_MinorFrameCallback(uint32 TimerId)
{
    uint32  CurrentSlot;
    
    /*
    ** If this is the very first timer interrupt, then the initial 
    ** Major Frame Synchronization timed out.  This can occur when
    ** either the signal is not arriving or the clock has gone into
    ** FLYWHEEL mode.  We should synchronize to the MET time instead.
    */
    if (SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_NONE)
    {
        SCH_AppData.MajorFrameSource = SCH_MAJOR_FS_MINOR_FRAME_TIMER;
        
        /* Synchronize timing to MET */
        SCH_AppData.SyncToMET |= SCH_PENDING_MAJOR_SYNCH;
        SCH_AppData.SyncAttemptsLeft = SCH_MAX_SYNC_ATTEMPTS;
        SCH_AppData.LastSyncMETSlot = 0;
    }
    
    /* If attempting to synchronize the Major Frame with MET, then wait for zero subsecs before starting */
    if (((SCH_AppData.SyncToMET & SCH_PENDING_MAJOR_SYNCH) != 0) &&
        (SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_MINOR_FRAME_TIMER))
    {
        /* Whether we have found the Major Frame Start or not, wait another slot */
        CFE_PSP_TimerSet(SCH_AppData.TimerId, SCH_NORMAL_SLOT_PERIOD, SCH_NORMAL_SLOT_PERIOD);

        /* Determine if this was the last attempt */
        SCH_AppData.SyncAttemptsLeft--;

        CurrentSlot = SCH_GetMETSlotNumber();
        if ((CurrentSlot != 0) && (SCH_AppData.SyncAttemptsLeft > 0))
        {
            return;
        }
        else  /* Synchronization achieved (or at least, aborted) */
        {
            /* Clear the pending synchronization flag and set the "Major In Sync" flag */
            SCH_AppData.SyncToMET &= ~SCH_PENDING_MAJOR_SYNCH;
            SCH_AppData.SyncToMET |= SCH_MAJOR_SYNCHRONIZED;
            
            /* CurrentSlot should be equal to zero.  If not, this is the best estimate we can use */
            SCH_AppData.MinorFramesSinceTone = CurrentSlot;
            SCH_AppData.LastSyncMETSlot = 0;
        }
    }
    else
    {
        /*
        ** If we are already synchronized with MET or don't care to be, increment current slot
        */
        SCH_AppData.MinorFramesSinceTone++;
    }

    if (SCH_AppData.MinorFramesSinceTone >= SCH_TOTAL_SLOTS)
    {
        /*
        ** If we just rolled over from the last slot to slot zero,
        ** It means that the Major Frame Callback did not cancel the
        ** "long slot" timer that was started in the last slot
        **
        ** It also means that we may now need a "short slot"
        ** timer to make up for the previous long one
        */
        CFE_PSP_TimerSet(SCH_AppData.TimerId, SCH_SHORT_SLOT_PERIOD, SCH_NORMAL_SLOT_PERIOD);
        
        SCH_AppData.MinorFramesSinceTone = 0;
        
        SCH_AppData.MissedMajorFrameCount++;
    }

    /*
    ** Determine the timer delay value for the next slot
    */
    if (SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT)
    {
        /*
        ** Start "long slot" timer (should be stopped by Major Frame Callback)
        */
        CFE_PSP_TimerSet(SCH_AppData.TimerId, SCH_SYNC_SLOT_PERIOD, 0);
    }
    
    /*
    ** Note that if this is neither the first "short" minor frame nor the
    ** last "long" minor frame, the timer is not modified.  This should
    ** provide more stable timing than introducing the dither associated
    ** with software response times to timer interrupts.
    */

    /*
    ** Give "wakeup SCH" semaphore
    */
    OS_BinSemGive(SCH_AppData.TimeSemaphore);

    return;

} /* End of SCH_MinorFrameCallback() */


/************************/
/*  End of File Comment */
/************************/

