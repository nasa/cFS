/************************************************************************
** File:
**   $Id: ds_verify.h 1.4 2017/01/25 12:05:50EST sstrege Exp  $
**
**  Copyright (c) 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: 
**  Define the CFS Data Storage (DS) Application compile-time checks
**
** Notes:
**
*************************************************************************/
#ifndef _ds_verify_h_
#define _ds_verify_h_


#include "cfe.h"

#include "ds_platform_cfg.h"


#ifndef DS_DESTINATION_TBL_NAME
    #error DS_DESTINATION_TBL_NAME must be defined!
#endif


#ifndef DS_DEF_FILTER_FILENAME
    #error DS_DEF_FILTER_FILENAME must be defined!
#endif


#ifndef DS_DEST_FILE_CNT
    #error DS_DEST_FILE_CNT must be defined!
#elif (DS_DEST_FILE_CNT  <  1)
    #error DS_DEST_FILE_CNT cannot be less than 1!
#endif


#ifndef DS_PATHNAME_BUFSIZE
    #error DS_PATHNAME_BUFSIZE must be defined!
#elif (DS_PATHNAME_BUFSIZE  <  1)
    #error DS_PATHNAME_BUFSIZE cannot be less than 1!
#elif ((DS_PATHNAME_BUFSIZE % 4) != 0)
    #error DS_PATHNAME_BUFSIZE must be a multiple of 4!
#elif (DS_PATHNAME_BUFSIZE  >  OS_MAX_PATH_LEN)
    #error DS_PATHNAME_BUFSIZE cannot be greater than OS_MAX_PATH_LEN!
#endif


#ifndef DS_BASENAME_BUFSIZE
    #error DS_BASENAME_BUFSIZE must be defined!
#elif (DS_BASENAME_BUFSIZE  <  1)
    #error DS_BASENAME_BUFSIZE cannot be less than 1!
#elif ((DS_BASENAME_BUFSIZE % 4) != 0)
    #error DS_BASENAME_BUFSIZE must be a multiple of 4!
#elif (DS_BASENAME_BUFSIZE  >  OS_MAX_PATH_LEN)
    #error DS_BASENAME_BUFSIZE cannot be greater than OS_MAX_PATH_LEN!
#endif


#ifndef DS_EXTENSION_BUFSIZE
    #error DS_EXTENSION_BUFSIZE must be defined!
#elif (DS_EXTENSION_BUFSIZE  <  1)
    #error DS_EXTENSION_BUFSIZE cannot be less than 1!
#elif ((DS_EXTENSION_BUFSIZE % 4) != 0)
    #error DS_EXTENSION_BUFSIZE must be a multiple of 4!
#elif (DS_EXTENSION_BUFSIZE  >  OS_MAX_PATH_LEN)
    #error DS_EXTENSION_BUFSIZE cannot be greater than OS_MAX_PATH_LEN!
#endif


#ifndef DS_FILTER_TBL_NAME
    #error DS_FILTER_TBL_NAME must be defined!
#endif


#ifndef DS_DEF_DEST_FILENAME
    #error DS_DEF_DEST_FILENAME must be defined!
#endif


#ifndef DS_PACKETS_IN_FILTER_TABLE
    #error DS_PACKETS_IN_FILTER_TABLE must be defined!
#elif (DS_PACKETS_IN_FILTER_TABLE  <  1)
    #error DS_PACKETS_IN_FILTER_TABLE cannot be less than 1!
#endif


#ifndef DS_FILTERS_PER_PACKET
    #error DS_FILTERS_PER_PACKET must be defined!
#elif (DS_FILTERS_PER_PACKET  <  1)
    #error DS_FILTERS_PER_PACKET cannot be less than 1!
#elif (DS_FILTERS_PER_PACKET  >  DS_DEST_FILE_CNT)
    #error DS_FILTERS_PER_PACKET cannot be greater than DS_DEST_FILE_CNT!
#endif


#ifndef DS_DESCRIPTOR_BUFSIZE
    #error DS_DESCRIPTOR_BUFSIZE must be defined!
#elif (DS_DESCRIPTOR_BUFSIZE  <  1)
    #error DS_DESCRIPTOR_BUFSIZE cannot be less than 1!
#elif ((DS_DESCRIPTOR_BUFSIZE % 4) != 0)
    #error DS_DESCRIPTOR_BUFSIZE must be a multiple of 4!
#endif


#ifndef DS_SEQUENCE_DIGITS
    #error DS_SEQUENCE_DIGITS must be defined!
#elif (DS_SEQUENCE_DIGITS  <  1)
    #error DS_SEQUENCE_DIGITS cannot be less than 1!
#endif


#ifndef DS_MAX_SEQUENCE_COUNT
    #error DS_MAX_SEQUENCE_COUNT must be defined!
#elif (DS_MAX_SEQUENCE_COUNT  <  1)
    #error DS_MAX_SEQUENCE_COUNT cannot be less than 1!
#endif


#ifndef DS_TOTAL_FNAME_BUFSIZE
    #error DS_TOTAL_FNAME_BUFSIZE must be defined!
#elif (DS_TOTAL_FNAME_BUFSIZE  <  1)
    #error DS_TOTAL_FNAME_BUFSIZE cannot be less than 1!
#elif ((DS_TOTAL_FNAME_BUFSIZE % 4) != 0)
    #error DS_TOTAL_FNAME_BUFSIZE must be a multiple of 4!
#elif (DS_TOTAL_FNAME_BUFSIZE  >  OS_MAX_PATH_LEN)
    #error DS_TOTAL_FNAME_BUFSIZE cannot be greater than OS_MAX_PATH_LEN!
#endif


#ifndef DS_FILE_HDR_SUBTYPE
    #error DS_FILE_HDR_SUBTYPE must be defined!
#endif


#ifndef DS_FILE_HDR_DESCRIPTION
    #error DS_FILE_HDR_DESCRIPTION must be defined!
#endif


#ifndef DS_FILE_MIN_SIZE_LIMIT
    #error DS_FILE_MIN_SIZE_LIMIT must be defined!
#elif (DS_FILE_MIN_SIZE_LIMIT  <  1)
    #error DS_FILE_MIN_SIZE_LIMIT cannot be less than 1!
#endif


#ifndef DS_FILE_MIN_AGE_LIMIT
    #error DS_FILE_MIN_AGE_LIMIT must be defined!
#elif (DS_FILE_MIN_AGE_LIMIT  <  1)
    #error DS_FILE_MIN_AGE_LIMIT cannot be less than 1!
#endif


#ifndef DS_APP_PIPE_NAME
    #error DS_APP_PIPE_NAME must be defined!
#endif


#ifndef DS_APP_PIPE_DEPTH
    #error DS_APP_PIPE_DEPTH must be defined!
#elif (DS_APP_PIPE_DEPTH  <  1)
    #error DS_APP_PIPE_DEPTH cannot be less than 1!
#endif

/*
 * JPH 2015-06-29 - Removed check of:
 *  DS_APP_PIPE_DEPTH  >  CFE_SB_MAX_PIPE_DEPTH
 *
 * This is not a valid check anymore, as the DS app does not have knowledge
 * of CFE_SB_MAX_PIPE_DEPTH.  But if the configuration violates this rule it will
 * show up as an obvious run-time error so the compile-time check is redundant.
 */


#ifndef DS_MAKE_TABLES_CRITICAL
    #error DS_MAKE_TABLES_CRITICAL must be defined!
#elif ((DS_MAKE_TABLES_CRITICAL != 0) && (DS_MAKE_TABLES_CRITICAL != 1))
    #error DS_MAKE_TABLES_CRITICAL must be 0 or 1!
#endif


#if (DS_SECS_PER_HK_CYCLE  <  1)
    #error DS_SECS_PER_HK_CYCLE cannot be less than 1!
#endif


#ifndef DS_DEF_ENABLE_STATE
    #error DS_DEF_ENABLE_STATE must be defined!
#elif ((DS_DEF_ENABLE_STATE != 0) && (DS_DEF_ENABLE_STATE != 1))
    #error DS_DEF_ENABLE_STATE must be 0 or 1!
#endif


#ifndef DS_CDS_ENABLE_STATE
    #error DS_CDS_ENABLE_STATE must be defined!
#elif ((DS_CDS_ENABLE_STATE != 0) && (DS_CDS_ENABLE_STATE != 1))
    #error DS_CDS_ENABLE_STATE must be 0 or 1!
#endif


#ifndef DS_MISSION_REV
    #error DS_MISSION_REV must be defined!
#elif (DS_MISSION_REV < 0)
    #error DS_MISSION_REV must be greater than or equal to zero!
#endif 


#ifndef DS_FILE_HEADER_TYPE
    #error DS_FILE_HEADER_TYPE must be defined!
#elif ((DS_FILE_HEADER_TYPE != 0) && (DS_FILE_HEADER_TYPE != 1) && (DS_FILE_HEADER_TYPE != 2))
    #error DS_FILE_HEADER_TYPE must be 0 or 1 or 2!
#endif


#ifndef DS_MOVE_FILES
    #error DS_MOVE_FILES must be defined!
#elif ((DS_MOVE_FILES != TRUE) && (DS_MOVE_FILES != FALSE))
    #error DS_MOVE_FILES must be TRUE or FALSE!
#endif


#ifndef DS_PER_PACKET_PIPE_LIMIT
    #error DS_PER_PACKET_PIPE_LIMIT must be defined!
#elif (DS_PER_PACKET_PIPE_LIMIT  <  1)
    #error DS_PER_PACKET_PIPE_LIMIT cannot be less than 1!
#elif (DS_PER_PACKET_PIPE_LIMIT > DS_APP_PIPE_DEPTH)
    #error DS_PER_PACKET_PIPE_LIMIT cannot be greater than DS_APP_PIPE_DEPTH!
#endif


#endif /* _ds_verify_h_ */

/************************/
/*  End of File Comment */
/************************/
