/*
**
** File: ut_cfe_evs_hooks.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_evs_hooks.h 1.1 2011/05/04 11:20:18EDT rmcgraw Exp  $
**
** Purpose: Unit test header file for cFE Event Services hooks.
**
** $Log: ut_cfe_evs_hooks.h  $
** Revision 1.1 2011/05/04 11:20:18EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/04/08 16:25:52EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.3 2011/03/10 11:16:45EST sslegel 
** Added EventNotSent and PacketNotSent asserts
** Revision 1.2 2011/02/16 17:06:44EST rmcgraw 
** Added "extern UtListHead_t EventQueue;" to ut_cfe_evs_hooks.h
** Revision 1.1 2011/02/15 11:12:32EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_CFE_EVS_HOOKS_H_
#define UT_CFE_EVS_HOOKS_H_

#include "cfe.h"
#include "utassert.h"
#include "utlist.h"

extern UtListHead_t        EventQueue;

#define     UtAssert_EventSent(EventID, EventType, EventText, Description) \
                UtAssert(Ut_CFE_EVS_EventSent(EventID, EventType, EventText), Description, __FILE__, __LINE__)

#define     UtAssert_EventNotSent(EventID, EventType, EventText, Description) \
                UtAssert(Ut_CFE_EVS_EventSent(EventID, EventType, EventText) == FALSE, Description, __FILE__, __LINE__)

#define     UtAssert_NoEventSent(Description) \
                UtAssert(UtList_IsEmpty(&EventQueue), Description, __FILE__, __LINE__)

void        Ut_CFE_EVS_ClearEventQueue(void);
uint32      Ut_CFE_EVS_GetEventQueueDepth(void);
uint32      Ut_CFE_EVS_GetEventCount(uint16 EventID, uint16 EventType, char *EventText);
int32       Ut_CFE_EVS_SendEventHook(uint16 EventID, uint16 EventType, char *EventText);
boolean     Ut_CFE_EVS_EventSent(uint16 EventID, uint16 EventType, char *EventText);

#endif
