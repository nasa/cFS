/*
**
** File: ut_cfe_evs_hooks.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_evs_hooks.c 1.1 2011/05/04 11:20:52EDT rmcgraw Exp  $
**
** Purpose: Unit test hooks for cFE Event Services routines
**
** $Log: ut_cfe_evs_hooks.c  $
** Revision 1.1 2011/05/04 11:20:52EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:38EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.2 2011/03/04 14:56:05EST sslegel 
** Added a event text length check
** Revision 1.1 2011/02/15 11:13:01EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

#include "cfe.h"
#include "utlist.h"
#include "uttools.h"
#include <string.h>

UtListHead_t        EventQueue;

typedef struct {
    uint16      EventID;
    uint16      EventType;
    char        EventText[CFE_EVS_MAX_MESSAGE_LENGTH];
} Ut_CFE_EVS_Event_t;

void Ut_CFE_EVS_ClearEventQueue(void)
{
    UtList_Reset(&EventQueue);
}

uint32 Ut_CFE_EVS_GetEventQueueDepth(void)
{
    return(UtList_Depth(&EventQueue));
}

uint32 Ut_CFE_EVS_GetEventCount(uint16 EventID, uint16 EventType, char *EventText)
{
    UtListNode_t        *CurrentNode;
    Ut_CFE_EVS_Event_t  *EventMessagePtr;
    uint32               EventCount = 0;

    CurrentNode = UtList_First(&EventQueue);
    while (CurrentNode) {
        EventMessagePtr = CurrentNode->Data;
        if ((EventMessagePtr->EventID == EventID) &&
            (EventMessagePtr->EventType == EventType) &&
            (strncmp(EventText, EventMessagePtr->EventText, strlen(EventText)) == 0)) {
            EventCount++;
        }
        CurrentNode = CurrentNode->Next;
    }
    return(EventCount);
}

int32 Ut_CFE_EVS_SendEventHook(uint16 EventID, uint16 EventType, char *EventText)
{
    Ut_CFE_EVS_Event_t  EventMessage;

    if (strlen(EventText) >= CFE_EVS_MAX_MESSAGE_LENGTH) {
        UtPrintf("WARNING - Event Message Too Long: %s", EventText);
    }

    EventMessage.EventID = EventID;
    EventMessage.EventType = EventType;
    strncpy(&EventMessage.EventText[0], EventText, CFE_EVS_MAX_MESSAGE_LENGTH);
    UtList_Add(&EventQueue, &EventMessage, sizeof(EventMessage), 0);

    if (EventType == CFE_EVS_DEBUG)
        UtPrintf("DEBUG EVENT ID=%d %s\n", EventID, EventText);
    else if (EventType == CFE_EVS_INFORMATION)
        UtPrintf("INFO EVENT ID=%d %s\n", EventID, EventText);
    else if (EventType == CFE_EVS_ERROR)
        UtPrintf("ERROR EVENT ID=%d %s\n", EventID, EventText);
    else if (EventType == CFE_EVS_CRITICAL)
        UtPrintf("CRITICAL EVENT ID=%d %s\n", EventID, EventText);
    else
        UtPrintf("Invalid Event Type %d ID=%d %s\n", EventType, EventID, EventText);

    return CFE_SUCCESS;
}

boolean Ut_CFE_EVS_EventSent(uint16 EventID, uint16 EventType, char *EventText)
{
    UtListNode_t        *CurrentNode;
    Ut_CFE_EVS_Event_t  *EventMessagePtr;

    CurrentNode = UtList_First(&EventQueue);
    while (CurrentNode) {
        EventMessagePtr = CurrentNode->Data;
        if ((EventMessagePtr->EventID == EventID) &&
            (EventMessagePtr->EventType == EventType) &&
            (strncmp(EventText, EventMessagePtr->EventText, strlen(EventText)) == 0)) {
            return(TRUE);
        }
        CurrentNode = CurrentNode->Next;
    }
    return(FALSE);
}
