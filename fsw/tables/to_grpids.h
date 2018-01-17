/*=======================================================================================
** File Name:  to_grpids.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** Title:  Group IDs used by the default TO table  
**=====================================================================================*/

#ifndef _TO_GRP_IDS_H_
#define _TO_GRP_IDS_H_

/*
** Pragmas
*/

/*
** Include Files
*/

/*
** Local Defines
*/
/* Define multigroups as bit. Max is TO_MULTI_GROUP_MASK (default: 24-bits) */
#define TO_MGROUP_NONE  0 
#define TO_MGROUP_ONE   1 
#define TO_MGROUP_TWO   1 << 1
#define TO_MGROUP_THREE 1 << 2
#define TO_MGROUP_FOUR  1 << 3
#define TO_MGROUP_FIVE  1 << 4
#define TO_MGROUP_SIX   1 << 5
#define TO_MGROUP_SEVEN 1 << 6
#define TO_MGROUP_EIGHT 1 << 7
#define TO_MGROUP_NINE  1 << 8
#define TO_MGROUP_TEN   1 << 9

/* Define group as Hex based on TO_GROUP_NUMBER_MASK (default: 0x00 << 24 to 0xff << 24) */
#define TO_GROUP_NONE   0x00000000
#define TO_GROUP_CFE    0x01000000
#define TO_GROUP_APP    0x02000000
#define TO_GROUP_THREE  0x03000000

#endif /* _TO_GRP_IDS_H_ */

/*=======================================================================================
** End of file sch_grpids.h
**=====================================================================================*/
    
