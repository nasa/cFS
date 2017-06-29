/*
**
** File: utlist.c
**
** $Id: utlist.c 1.3 2015/06/16 16:14:04EDT sstrege Exp  $
**
** Copyright 2012-2013 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: This file contains functions to implement a generic linked list data structure.
**
** $Log: utlist.c  $
** Revision 1.3 2015/06/16 16:14:04EDT sstrege 
** Added copyright information
** Revision 1.2 2015/03/10 15:21:43EDT czogby 
** Add Missing Functionality to UT Assert Library
** --- Added comments ---  czogby [2015/03/31 18:37:17Z]
** No updates were made to this source code in this revision
**
*/

/*
 * Includes
 */

#include "common_types.h"
#include "utlist.h"
#include <stdlib.h>
#include <string.h>

/*
 * Function Definitions
 */

UtListHead_t *UtList_Create(void)
{
    UtListHead_t *NewList;

    NewList = malloc(sizeof(UtListHead_t));
    NewList->First = NULL;
    NewList->Last = NULL;
    NewList->NumberOfEntries = 0;
    return (NewList);
}

void UtList_Destroy(UtListHead_t *ListHead)
{
    UtList_Reset(ListHead);
    free(ListHead);
}

void UtList_Reset(UtListHead_t *ListHead)
{
    while (!UtList_IsEmpty(ListHead)) {
        UtList_DeleteFirst(ListHead);
    }
}

void UtList_Add(UtListHead_t *ListHead, void *Data, uint32 DataSize, uint32 Tag)
{
    UtListNode_t *NewNode = NULL;
    
    NewNode = malloc(sizeof(UtListNode_t));
    if (ListHead->NumberOfEntries == 0) {

        ListHead->First = NewNode;
        ListHead->Last = NewNode;
        ListHead->NumberOfEntries++;

        NewNode->Next = NULL;
        NewNode->Prev = NULL;
        NewNode->Tag = Tag;
        NewNode->DataSize = DataSize;
        NewNode->Data = malloc(DataSize);
        memcpy(NewNode->Data, Data, DataSize);
    }
    else {

        NewNode->Next = NULL;
        NewNode->Prev = ListHead->Last;
        NewNode->Tag = Tag;
        NewNode->DataSize = DataSize;
        NewNode->Data = malloc(DataSize);
        memcpy(NewNode->Data, Data, DataSize);

        ListHead->Last->Next = NewNode;
        ListHead->Last = NewNode;
        ListHead->NumberOfEntries++;
    }
}

void UtList_DeleteFirst(UtListHead_t *ListHead)
{
    UtList_DeleteNode(ListHead, ListHead->First);
}

void UtList_DeleteLast(UtListHead_t *ListHead)
{
    UtList_DeleteNode(ListHead, ListHead->Last);
}

void UtList_DeleteNode(UtListHead_t *ListHead, UtListNode_t *DeleteNode)
{
   
    if (!UtList_IsEmpty(ListHead)) {

        if (ListHead->NumberOfEntries == 1) {
            ListHead->First = NULL;
            ListHead->Last = NULL;
            ListHead->NumberOfEntries = 0;
        }
        else if (DeleteNode == ListHead->First) {
            ListHead->First = DeleteNode->Next;
            ListHead->First->Prev = NULL;
            ListHead->NumberOfEntries--;
        }
        else if (DeleteNode == ListHead->Last) {
            ListHead->Last = DeleteNode->Prev;
            ListHead->Last->Next = NULL;
            ListHead->NumberOfEntries--;        
        }
        else {
            DeleteNode->Prev->Next = DeleteNode->Next;
            DeleteNode->Next->Prev = DeleteNode->Prev;
            ListHead->NumberOfEntries--;
        }

        free(DeleteNode->Data);
        free(DeleteNode);
    }
}

void UtList_RemoveFirst(UtListHead_t *ListHead, void *Data)
{
    UtList_RemoveNode(ListHead, Data, ListHead->First);
}

void UtList_RemoveLast(UtListHead_t *ListHead, void *Data)
{
    UtList_RemoveNode(ListHead, Data, ListHead->Last);
}

void UtList_RemoveNode(UtListHead_t *ListHead, void *Data, UtListNode_t *CurrentNode)
{
    if (!UtList_IsEmpty(ListHead)) {
        memcpy(Data, CurrentNode->Data, CurrentNode->DataSize);
        UtList_DeleteNode(ListHead, CurrentNode);
    }
}

UtListNode_t *UtList_First(UtListHead_t *ListHead)
{
    return(ListHead->First);
}

UtListNode_t *UtList_Last(UtListHead_t *ListHead)
{
    return(ListHead->Last);
}

boolean UtList_IsEmpty(UtListHead_t *ListHead)
{
    return(ListHead->NumberOfEntries == 0);
}

uint32 UtList_Depth(UtListHead_t *ListHead)
{
    return(ListHead->NumberOfEntries);
}
