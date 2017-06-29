/*
**
** File: utlist.h
**
** $Id: utlist.h 1.3 2015/06/16 16:02:15EDT sstrege Exp  $
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
** $Log: utlist.h  $
** Revision 1.3 2015/06/16 16:02:15EDT sstrege 
** Added copyright information
** Revision 1.2 2015/03/10 15:19:30EDT czogby 
** Add Missing Functionality to UT Assert Library
** --- Added comments ---  czogby [2015/03/31 18:02:36Z]
** No source code changes were made in this revision.
**
*/

#ifndef _utlist_
#define	_utlist_

/*
 * Includes
 */

#include "common_types.h"

/*
 * Macro Definitions
 */

/* Macros to redefine list functions to look like stack and queue functions */
#define UtStack_Push        UtList_Add
#define UtStack_Pop         UtList_RemoveLast
#define UtStack_IsEmpty     UtList_IsEmpty
#define UtStack_Depth       UtList_Depth

#define UtQueue_Add         UtList_Add
#define UtQueue_Get         UtList_RemoveFirst
#define UtQueue_Look        UtList_First
#define UtQueue_Delete      UtList_DeleteFirst
#define UtQueue_IsEmpty     UtList_IsEmpty
#define UtQueue_Depth       UtList_Depth

/*
 * Type Definitions
 */

typedef struct UtListNodeTag {
    struct UtListNodeTag   *Next;
    struct UtListNodeTag   *Prev;
    void                   *Data;
    uint32                  DataSize;
    uint32                  Tag;
} UtListNode_t;

 typedef struct {
    UtListNode_t           *First;
    UtListNode_t           *Last;
    uint32                  NumberOfEntries;
} UtListHead_t;

/*
 * Exported Functions
 */

/* Dynamically allocates a new list head.  A list head could also just be declared, this function is useful
 * if you need to dynamically allocate memory for a new list head.  Note always free list heads allocated by
 * this function by calling UtList_Destroy. */
UtListHead_t               *UtList_Create(void);

/* Frees a list head created by UtList_Create. */
void                        UtList_Destroy(UtListHead_t *ListHead);

/* Deletes all nodes on the list. */
void                        UtList_Reset(UtListHead_t *ListHead);

/* Dynamically adds a new node to the list.  Nodes are always added to the end of the list.  Memory is dynamically
 * allocated for the new node and to hold the data pointed to by Data.  A Tag field is also provided to be used to
 * store user defined information with the node. */
void                        UtList_Add(UtListHead_t *ListHead, void *Data, uint32 DataSize, uint32 Tag);

/* Deletes the first node from the list. */
void                        UtList_DeleteFirst(UtListHead_t *ListHead);

/* Deletes the last node from the list. */
void                        UtList_DeleteLast(UtListHead_t *ListHead);

/* Deletes the specified node from the list, this will screw up if you do not pass in a valid DeleteNode.  I do not
 * verify that DeleteNode is a member of the list. */
void                        UtList_DeleteNode(UtListHead_t *ListHead, UtListNode_t *DeleteNode);

/* Removes the first node from the list by first copying the data from the node to the memory buffer pointed to by the
 * specified Data pointer and then the node is deleted from the list.  Make sure the destination pointer points to a
 * memory buffer large enough to hold the data.  The size of the data on the node is available by referencing UtListNode->DataSize. */
void                        UtList_RemoveFirst(UtListHead_t *ListHead, void *Data);

/* Removes the last node from the list by first copying the data from the node to the memory buffer pointed to by the
 * specified Data pointer and then the node is deleted from the list.  Make sure the destination pointer points to a
 * memory buffer large enough to hold the data.  The size of the data on the node is available by referencing UtListNode->DataSize. */
void                        UtList_RemoveLast(UtListHead_t *ListHead, void *Data);

/* Removes the speciified RemoveNode from the list by first copying the data from the node to the memory buffer pointed to by the
 * specified Data pointer and then the node is deleted from the list.  Make sure the destination pointer points to a
 * memory buffer large enough to hold the data.  The size of the data on the node is available by referencing UtListNode->DataSize. */
void                        UtList_RemoveNode(UtListHead_t *ListHead, void *Data, UtListNode_t *RemoveNode);

/* Returns a pointer to the first node on the list.  This is the same as (UtListHead->First). */
UtListNode_t               *UtList_First(UtListHead_t *ListHead);

/* Returns a pointer to the last node on the list.  This is the same as (UtListHead->Last). */
UtListNode_t               *UtList_Last(UtListHead_t *ListHead);

/* Returns TRUE if the list is empty.  This is the same as (UtListHead->NumberOfEntries == 0). */
boolean                     UtList_IsEmpty(UtListHead_t *ListHead);

/* Returns the number of nodes on the list.  This is the same as (UtListHead->NumberOfEntries). */
uint32                      UtList_Depth(UtListHead_t *ListHead);

#endif
