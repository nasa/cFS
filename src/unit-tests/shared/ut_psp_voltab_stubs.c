/*================================================================================*
** File:  ut_psp_voltab_stubs.c
** Owner: Tam Ngo
** Date:  March 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_stubs.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/* 
** OSAL volume table. This is the only file in the PSP that still has the
** OS_ naming convention, since it belongs to the OSAL.
*/
OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES] = 
{
/* DevName     PhysDev      VolType     Volatile?  Free?   IsMounted?  VolName  MountPnt  BlockSz */
{"/ramdev0",  "./ram0",     FS_BASED,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev1",  "./ram1",     FS_BASED,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev2",  "./ram2",     FS_BASED,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev3",  "./ram3",     FS_BASED,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev4",  "./ram4",     FS_BASED,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },

/*
** The following entry is a "pre-mounted" path to a non-volatile device
*/
/* DevName     PhysDev      VolType     Volatile?  Free?   IsMounted?  VolName  MountPnt  BlockSz */
{"/eedev0",   "./eeprom1",  FS_BASED,   FALSE,     FALSE,  TRUE,       "CF",    "/cf",    512      },

/* DevName     PhysDev      VolType     Volatile?  Free?   IsMounted?  VolName  MountPnt  BlockSz */
{"/ramdev5",    "./ram5",     FS_BASED,   TRUE,      TRUE,   FALSE,     " ",    " ",      0        },
{"/ramdev6",    "./ram6",     FS_BASED,   TRUE,      TRUE,   FALSE,     " ",    " ",      0        },
{"/ramdev7",    "./ram7",     FS_BASED,   TRUE,      TRUE,   FALSE,     " ",    " ",      0        },
{"/ramdev8",    "./ram8",     FS_BASED,   TRUE,      TRUE,   FALSE,     " ",    " ",      0        },
{"/ramdev9",    "./ram9",     FS_BASED,   TRUE,      TRUE,   FALSE,     " ",    " ",      0        },
{"/ramdev10",   "./ram10",    FS_BASED,   TRUE,      TRUE,   FALSE,     " ",    " ",      0        },
{"/ramdev11",   "./ram11",    FS_BASED,   TRUE,      TRUE,   FALSE,     " ",    " ",      0        },
{"/ramdev12",   "./ram12",    FS_BASED,   TRUE,      TRUE,   FALSE,     " ",    " ",      0        }
};

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/

/*================================================================================*
** End of File: ut_psp_voltab_stubs.c
**================================================================================*/
