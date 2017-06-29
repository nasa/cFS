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
{"/ramdev0",   "/ram0",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev1",   "/ram1",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev2",   "/ram2",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev3",   "/ram3",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev4",   "/ram4",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev5",   "/ram5",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev6",   "/ram6",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev7",   "/ram7",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev8",   "/ram8",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev9",   "/ram9",     RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev10",  "/ram10",    RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev11",  "/ram11",    RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
{"/ramdev12",  "/ram12",    RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
#if defined(UT_OS_USE_RAM)
{"/cf",        "/ram0/cf",  FS_BASED,   FALSE,     FALSE,  TRUE,       "CF",    "/cf",    1024     }
#elif defined(UT_OS_USE_ATA)
{"/cf",        "/ata0a/cf", FS_BASED,   FALSE,     FALSE,  TRUE,       "CF",    "/cf",    1024     }
#else
{"/ramdev13",  "/ram13",    RAM_DISK,   TRUE,      TRUE,   FALSE,      " ",     " ",      0        },
#endif
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
