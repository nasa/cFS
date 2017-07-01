
/************************************************************************
**
** $Id: ds_file_tbl.c 1.3 2017/01/25 12:05:40EST sstrege Exp  $
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
** CFS Data Storage (DS) sample destination file table
**
** Note: This source file creates a sample table that defines several
**       data storage destination files using a variety of the options
**       available. None of the file system details (name, size, etc.)
**       are known at this time for the target platform. Therefore,
**       the file pathnames are set to "set_by_cmd_b4_enable" which
**       indicates that pathnames must be set by command before trying
**       to enable any of the destination files. Max file size values
**       should also be modified via command before using this table.
**
**       Obviously, a better solution is to replace this sample table
**       and the sample packet filter table (which references this
**       table) with mission specific versions that define the data
**       storage behavior appropriate for the platform.
**
**       But, as long as the target platform has a file system, the
**       sample data storage tables may be used to demonstrate data
**       storage.
**
*************************************************************************/

#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "ds_platform_cfg.h"
#include "ds_appdefs.h"
#include "ds_app.h"
#include "ds_msg.h"


/*
** Note: It is suggested that missions pre-define their file table
**       index numbers in a public header file to be included by
**       both the packet filter table source file and the destination
**       file table source file. Common definitions may also be used
**       when creating ground system database entries that require
**       file index numbers for command arguments.
*/
#define FILE_ALL_EVENTS       0

#define FILE_ALL_APP_HK_PKTS  1
#define FILE_ALL_APP_TLM_PKTS 2

#define FILE_ALL_HW_TLM_PKTS  3

#define FILE_CFE_APP_HK_PKTS  4
#define FILE_CFE_APP_TLM_PKTS 5


/*
** Sample Destination File Table Data
*/
DS_DestFileTable_t DS_DestFileTable =
{
  /* .Descriptor = */ "Sample File Table Data",
  /* .File       = */
  {
    /* File Index 00 -- event packets only */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "set_by_cmd_b4_enable",
      /* .Basename      = */ "events",
      /* .Extension     = */ ".dat",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 32),               /* 32 K-bytes */
      /* .MaxFileAge    = */ (60 * 45),                 /* 45 minutes */
      /* .SequenceCount = */ 1000,
    },
    /* File Index 01 -- application housekeeping packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "set_by_cmd_b4_enable",
      /* .Basename      = */ "app",
      /* .Extension     = */ ".hk",

      /* .FileNameType  = */ DS_BY_TIME,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 2),         /* 2 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 02 -- application telemetry packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "set_by_cmd_b4_enable",
      /* .Basename      = */ "app",
      /* .Extension     = */ ".tlm",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 1024 * 1),  /* 1 G-byte */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 2000,
    },
    /* File Index 03 -- hardware telemetry packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "set_by_cmd_b4_enable",
      /* .Basename      = */ "hw",
      /* .Extension     = */ "tlm",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 2),         /* 2 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 3000,
    },
    /* File Index 04 -- cFE housekeeping packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "set_by_cmd_b4_enable",
      /* .Basename      = */ "cfe",
      /* .Extension     = */ "hk",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 2),         /* 2 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 4000,
    },
    /* File Index 05 -- cFE telemetry packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "set_by_cmd_b4_enable",
      /* .Basename      = */ "cfe",
      /* .Extension     = */ "tlm",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 2),         /* 2 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 5000,
    },
    /* File Index 06 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 07 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 08 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 09 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 10 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 11 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 12 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 13 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 14 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 15 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
  }
};

/*
** Sample Destination File Table Header
*/
CFE_TBL_FILEDEF(DS_DestFileTable, DS.FILE_TBL, DS Destination File Table,ds_file_tbl.tbl)


/************************/
/*  End of File Comment */
/************************/
