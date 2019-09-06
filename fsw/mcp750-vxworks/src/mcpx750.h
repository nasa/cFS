/* mcpx750.h - Motorola PowerPlus board header */

/* Copyright 1984-2001 Wind River Systems, Inc. */
/* Copyright 1996,1997,1998,1999 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01u,25mar02,mil  Changed FLOPPY_INT_ macros to FD_INT_. (SPR #68689)
01t,16sep01,dat  Use of WRS_ASM macro
01s,01may00,rcs  removed
                 PCI_ID_VEND_DEC21554,PCI_ID_DEV_DEC21554,PCI_ID_BR_DEC21554
01r,15dec99,rhv  Adding support for non-MCPN750 cPCI boards.
01q,07sep99,tm   Added missing ZCIO_DD_PORTA/B/C macros for z8536 (SPR 22830)
01p,30apr99,srr  Make WR/S code review changes.
01o,22feb99,scb  Moved PCI2DRAM_BASE_ADRS out of "mcpx750.h" and into "config.h".
01n,26jan99,scb  Added SYS_REG_MCR_... defines for MCPN750 256MB board.
01m,18jan99,scb  Added support for END driver on dec21143.
01l,11nov98,scb  Added DEC2115X_PCI_DEV_NUMBER
01k,05nov98,scb  Added _SYS_SM_ANCHOR_POLLING_LIST
01j,04nov98,rhv  Corrected an error in the definition of
                 DEC2155X_DOORBELL11_INT_LVL.
01i,23oct98,rhv  Modified ATA support to include MCPN750 and correcting errors
                 in EIDE interrupt vector definitions.
01h,21sep98,rhv  Added Vital Product Data support.
01g,04sep98,scb  Added shared memory support (MCP750 master and mem repository)
01f,28aug98,rhv  Added new defines to support processor data bus parity.
01e,05aug98,rhv  Removing #ifndef's from Dec2155x defines.
01d,05aug98,scb  Changed #ifndef guard to the correct name.
01c,30jul98,scb  Changed CPU_PCI_MEM_ADRS to 0xc2000000.
01b,29jul98,rhv  Added PCI insert routines.
01a,29jul98,scb  Derived from mcp750/mv2600.h, revision 01l.
*/

/*
This file contains I/O addresses and related constants for the
Motorola PowerPlus board.
*/

#ifndef __INCmcpx750h
#define __INCmcpx750h

#ifdef __cplusplus
extern "C" {
#endif

/* ATA/EIDE support */

#include "drv/pcmcia/pccardLib.h"
#include "drv/hdisk/ataDrv.h"

/*
 * Factor board features here, the CONFIGx... defines refer to
 * functionality related items such as ISA interrupts.
 */

#ifdef MCP750
#   define CONFIG1_MPIC_INT_TYPE	/* MPIC interrupt configuration */
#   define CONFIG1_ISA_INT_TYPE 	/* ISA interrupt configuration */
#   define CONFIG1_PLANAR_PCI		/* Planar PCI device configuration */
#   define CONFIG1_PLANAR_ISA		/* Planar ISA device configuration */
#   define CONFIG1_SYS_REG_CCR		/* System Configuration register */
#   define CONFIG1_SYS_REG_MCR		/* Memory control register */
#   define CONFIG1_SYS_REG_BMFR 	/* Base Module Features register */
#   define CONFIG1_SYS_REG_BMEFR	/* Base Module Extended Features reg */
#   define CONFIG1_SYS_REG_BMSR 	/* Base Module Status register */
#   define CONFIG1_SYS_REG_SYSLED	/* System LED register */
#   define CONFIG1_SYS_REG_BSR		/* Board Status register */
#   define CONFIG1_SYS_REG_BLRR 	/* Board Last Reset register */
#   define CONFIG1_SYS_REG_BRDFAIL_LED	/* Board Fail LED register */
#endif

#ifdef MCPN750
#   define CONFIG2_MPIC_INT_TYPE	/* MPIC interrupt configuration */
#   define CONFIG2_ISA_INT_TYPE 	/* ISA interrupt configuration */
#   define CONFIG2_PLANAR_PCI		/* Planar PCI device configuration */
#   define CONFIG2_PLANAR_ISA		/* Planar ISA device configuration */
#   define CONFIG1_SYS_REG_CCR		/* System Configuration register */
#   define CONFIG2_SYS_REG_MCR		/* Memory control register */
#   define CONFIG2_SYS_REG_BMFR 	/* Base Module Features register */
#   define CONFIG2_SYS_REG_BMEFR	/* Base Module Extended Features reg */
#   define CONFIG1_SYS_REG_BMSR 	/* Base Module Status register */
#   define CONFIG1_SYS_REG_SYSLED	/* System LED register */
#   define CONFIG1_SYS_REG_BSR		/* Board Status register */
#   define CONFIG2_SYS_REG_BLRR 	/* Board Last Reset register */
#   define CONFIG2_SYS_REG_CPGAR	/* cPCI Geographic Address register */
#   define CONFIG2_SYS_REG_PINTA	/* PINTA routing register */
#   define CONFIG2_SYS_REG_ABRTBTN	/* Abort button register */
#   define CONFIG2_SYS_REG_BRDFAIL_LED	/* Board Fail LED register */
#endif

/* Floppy disk support */

#define FD_MAX_DRIVES	4
#define FD_BASE_ADDR	pc87303_FDC	/* See super I/O section */
#define FD_DMA_CHAN	2

#ifdef	INCLUDE_FD
#   ifndef	INCLUDE_DOSFS
#	define	INCLUDE_DOSFS	/* file system to be used */
#   endif
#   ifndef	INCLUDE_ISADMA
#	define	INCLUDE_ISADMA	/* uses ISA DMA driver */
#   endif
#   ifndef _ASMLANGUAGE
	/* to stop compiler warnings */
#	include "blkIo.h"
	IMPORT STATUS	fdDrv (UINT, UINT);
	IMPORT BLK_DEV* fdDevCreate (UINT, UINT, UINT, UINT);
#   endif
#endif

/* Boot Line parameters are stored in the 2nd 256 byte block */

#undef	NV_BOOT_OFFSET
#define NV_BOOT_OFFSET		256 /* skip 1st 256 bytes */
#define NV_RAM_SIZE		BBRAM_SIZE
#define NV_RAM_ADRS		((char *) BBRAM_ADRS)
#define NV_RAM_INTRVL		1

/* PCI I/O function defines */

#define INT_NUM_IRQ0		INT_VEC_IRQ0

#ifndef _ASMLANGUAGE

#ifndef PCI_IN_BYTE
#   define PCI_IN_BYTE(x)	sysPciInByte (x)
    IMPORT  UINT8		sysPciInByte  (UINT32 address);
#endif
#ifndef PCI_IN_WORD
#   define PCI_IN_WORD(x)	sysPciInWord (x)
    IMPORT  UINT16		sysPciInWord  (UINT32 address);
#endif
#ifndef PCI_IN_LONG
#   define PCI_IN_LONG(x)	sysPciInLong (x)
    IMPORT  UINT32		sysPciInLong  (UINT32 address);
#endif
#ifndef PCI_OUT_BYTE
#   define PCI_OUT_BYTE(x,y)	sysPciOutByte (x,y)
    IMPORT  void		sysPciOutByte (UINT32 address, UINT8  data);
#endif
#ifndef PCI_OUT_WORD
#   define PCI_OUT_WORD(x,y)   sysPciOutWord (x,y)
    IMPORT  void	       sysPciOutWord (UINT32 address, UINT16 data);
#endif
#ifndef PCI_OUT_LONG
#   define PCI_OUT_LONG(x,y)	sysPciOutLong (x,y)
    IMPORT  void		sysPciOutLong (UINT32 address, UINT32 data);
#endif
#ifndef PCI_INSERT_LONG
#   define PCI_INSERT_LONG(a,m,d) sysPciInsertLong((a),(m),(d))
    IMPORT  void                  sysPciInsertLong(UINT32 adrs, UINT32 mask, \
                                                   UINT32 data);
#endif
#ifndef PCI_INSERT_WORD
#   define PCI_INSERT_WORD(a,m,d) sysPciInsertWord((a),(m),(d))
    IMPORT  void                  sysPciInsertWord(UINT32 adrs, UINT16 mask, \
                                                   UINT16 data);
#endif
#ifndef PCI_INSERT_BYTE
#   define PCI_INSERT_BYTE(a,m,d) sysPciInsertByte((a),(m),(d))
    IMPORT  void                  sysPciInsertByte(UINT32 adrs, UINT8 mask, \
                                                   UINT8 data);
#endif

#endif	/* _ASMLANGUAGE */

/* Cache Line Size - 8 32-bit value = 64 bytes */

#define PCI_CLINE_SZ		0x8

/* Latency Timer value - 255 PCI clocks */

#define PCI_LAT_TIMER		0xff

/* clock rates */

/* Calculate Memory Bus Rate in Hertz */

#define MEMORY_BUS_SPEED	( sysGetBusSpd() * 1000000)

/* System clock (decrementer counter) frequency determination */

#define DEC_CLOCK_FREQ		((sysGetBusSpd()==67)?66666666:33333333)

/* CIO clocks and stuff */

#define CIO_RESET_DELAY 	5000
#define ZCIO_HZ 		2500000 /* 2.5 MHz clock */
#define CIO_INT_VEC		9
#define Z8536_TC		ZCIO_HZ

/*
 * The PowerPC Decrementer is used as the system clock.
 * It is always included in this BSP.  The following defines
 * are used by the system clock library.
 */

#define SYS_CLK_RATE_MIN	10		/* minimum system clock rate */
#define SYS_CLK_RATE_MAX	5000		/* maximum system clock rate */

/*
 * This macro returns the positive difference between two unsigned ints.
 * Useful for determining delta between two successive decrementer reads.
 */

#define DELTA(a,b)		( abs((int)a - (int)b) )

/*
 * Auxiliary Clock support is an optional feature that is not supported
 * by all BSPs.  The following defines are used by the aux clock library.
 */

#define AUX_CLK_RATE_MIN	40		/* min auxiliary clock */
#define AUX_CLK_RATE_MAX	5000		/* max auxiliary clock rate */

/* Common I/O synchronizing instructions */

#ifndef EIEIO_SYNC
#   define EIEIO_SYNC  _WRS_ASM (" eieio; sync")
#endif	/* EIEIO_SYNC */
#ifndef EIEIO
#   define EIEIO    _WRS_ASM (" eieio")
#endif	/* EIEIO */

/* 
 * The following macros define access to PCI and ISA space from the CPU
 * and the base addresses of PCI and ISA spaces on the bus.
 * These macros contain the correct starting addresses and sizes
 * for use in the BSP and user programming.
 */

/* Legacy ISA space size. */

#define ISA_LEGACY_SIZE 0x00004000

/* Access to PCI ISA I/O space */

#define ISA_MSTR_IO_LOCAL   	0x80000000
#define ISA_MSTR_IO_BUS     	0x00000000	/* 0 based addressing */
#define ISA_MSTR_IO_SIZE 	0x00010000	/* 64 kbytes */

/* Access to PCI I/O space */

#define PCI_MSTR_IO_LOCAL	(ISA_MSTR_IO_LOCAL + ISA_MSTR_IO_SIZE)
#define PCI_MSTR_IO_BUS		(ISA_MSTR_IO_BUS   + ISA_MSTR_IO_SIZE)
#define PCI_MSTR_IO_SIZE	0x00800000	/* 8MB */

/*
 * Access to PCI nonprefetchable memory space (including ISA memory space)
 * Note the size PCIMSTR_MEMIO_SIZE is defined in config.h
 */

#define PCI_MSTR_MEMIO_LOCAL	0xc0000000
#define PCI_MSTR_MEMIO_BUS	0x00000000	/* 0 based addressing */
#define PCI_MSTR_MEMIO_1MB_OFF	0x00100000	/* Offset:PCI 1MB memory space*/

/* Access to PCI memory space (size defined in config.h) */

#define PCI_MSTR_MEM_LOCAL	(PCI_MSTR_MEMIO_LOCAL + PCI_MSTR_MEMIO_SIZE)
#define PCI_MSTR_MEM_BUS	(PCI_MSTR_MEMIO_BUS   + PCI_MSTR_MEMIO_SIZE)

/* PCI Access to local memory space */

#define PCI_SLV_MEM_LOCAL	LOCAL_MEM_LOCAL_ADRS
#define PCI_SLV_MEM_BUS		0x80000000
#define PCI_SLV_MEM_SIZE	DRAM_SIZE

/* setup the dynamic PCI memory space for PReP */

#define PCI_DYNAMIC_MEM_LOCAL (PCI_MSTR_MEM_LOCAL + 0x0)
#define PCI_DYNAMIC_MEM_BUS   (PCI_MSTR_MEM_BUS + 0x0)

#define PCI2DRAM_BASE_ADRS	PCI_SLV_MEM_BUS	 /* needed for usrNetwork.c */

/*
 * PCI MASTER MEMORY WINDOW LIMITS
 *
 * These values are strictly defined by the base memory addresses and window
 * sizes of the spaces defined above.  These values must be correct for the
 * sysBusProbe() memory range checks for the PCI bus to work properly.
 */

#define PCI_MSTR_LO_ADRS	(ISA_MSTR_IO_LOCAL)
#define PCI_MSTR_HI_ADRS	(PCI_MSTR_MEM_LOCAL + PCI_MSTR_MEM_SIZE)

/* Base address of HW devices as seen from CPU */

#define FALCON_BASE_ADRS	0xfef80000
#define FALCON_REG_SIZE 	0x00010000
#define FALCON_BASE_UPPER_ADRS	(FALCON_BASE_ADRS>>16)
#define FALCON_DRAM_ATTR	0xfef80010
#define FALCON_RESET_STAT_U	0xfef80400
#define FALCON_REV_ID		0xfef80009
#define RAVEN_BASE_ADRS 	0xfeff0000
#define RAVEN_REG_SIZE		0x00010000
#define FLASH_BASE_ADRS 	0xFF000000
#define FLASH_MEM_SIZE		0x01000000

/* MPIC configuration defines */

#define MPIC_BASE_ADRS		0xfc000000
#define MPIC_REG_SIZE		0x00040000
#define MPIC_PCI_BASE_ADRS	( MPIC_BASE_ADRS - PCI_MSTR_MEMIO_LOCAL )

/* memory map as seen on the PCI bus */

#define PCI_CNFG_ADRS		0x00800000	/* base of PCI config space */
#define PCI_IO_ADRS		0x01000000	/* base of PCI I/O address */
#define PCI_MEM_ADRS		0x01000000	/* base of PCI MEM address */

/*
 * Primary PCI bus configuration space address and data register addresses
 * as seen by the CPU on the local bus.
 */

#define PCI_PRIMARY_CAR 	0x80000CF8 /* PCI config address register */
#define PCI_PRIMARY_CDR 	0x80000CFC /* PCI config data	 register */

/*
 * PCI Config Space device addresses based on their device number
 *
 * Bit 32 is set to enable CONFIG_DATA accesses to PCI Cycles
 */

#define CNFG_START_SEARCH	0x5800	   /* PCI Space starting offset */
#define CNFG_RAVEN_ADRS 	ISA_MSTR_IO_LOCAL /* Raven PCI and MPIC ASIC*/
#define CNFG_IBC_ADRS		0x80005800 /* IBC */
#define CNFG_SCSI_ADRS		0x80006000 /* SCSI */
#define CNFG_VGA_ADRS		0x80007800 /* Graphics Device */
#define CNFG_PMC1_ADRS		0x80008000 /* PMC Slot1 */
#define CNFG_SCSI2_ADRS 	0x80008800 /* Secondary SCSI */
#define CNFG_PMC2_ADRS		0x80009800 /* PMC Slot 2 or PCIX */

/* Number of PCI devices */

#define NUM_PCI_DEVS		4

/* PCI Device/Vendor IDs */

#ifdef CONFIG1_PLANAR_PCI
#   define PCI_ID_PRI_LAN	PCI_ID_LN_DEC21140 /* Id for Primary LAN */
#   define PCI_ID_IBC		0x05861106	/* Id for VT82586 PBC */
#   define PCI_ID_IDE		0x05711106	/* Id for VT82586 IDE */
#   define DEC2115X_PCI_DEV_NUMBER 0x14         /* Dec 21154 dev number */
#endif

#ifdef CONFIG2_PLANAR_PCI
#   define PCI_ID_PRI_LAN	PCI_ID_LN_DEC21143 /* Id for Primary LAN */
#   define PCI_ID_IBC		0x05861106	/* Id for VT82586 PBC */
#   define PCI_ID_IDE		0x05711106	/* Id for VT82586 IDE */
#endif

#define PCI_ID_RAVEN		0x48011057	/* Id for RAVEN ASIC */
#define PCI_ID_LN_DEC21140	0x00091011	/* Id DEC chip 21140 */
#define PCI_ID_LN_DEC21143	0x00191011	/* Id DEC chip 21143 */
#define PCI_ID_LN_DEC21040	0x00021011	/* Id DEC chip 21040 */
#define PCI_ID_BR_DEC21150	0x00221011	/* Id DEC 21150 PCI bridge */
#define PCI_ID_SEC_LAN		PCI_ID_LN_DEC21040 /* Id for Secondary LAN */
#define PCI_ID_NCR810		0x00011000	/* Id for SYM53C810A Chip */
#define PCI_ID_NCR825		0x00031000	/* Id for SYM53C825 Chip */
#define PCI_ID_NCR860		0x00061000	/* Id for SYM53C860 Chip */
#define PCI_ID_NCR875		0x000f1000	/* Id for SYM53C875 Chip */
#define PCI_ID_SCSI		PCI_ID_NCR825	/* Id for Primary SCSI */
#define PCI_ID_SEC_SCSI 	PCI_ID_NCR825	/* Id for Secondary SCSI */
#define PCI_ID_5434		0x00a81013	/* Id for CL-GD534 chip */
#define PCI_ID_5436		0x00ac1013	/* Id for CL-GD536 chip */
#define PCI_ID_USB		0x30381106	/* USB (function 2 of PBC) */
#define PCI_ID_PM		0x30401106	/* Power Mgmt (func 3 of PBC) */


/* Special dec21143 configuration device driver area register */

#define PCI_CFG_21143_DA	0x40

/* Base address register defines */

#define PCI_BASEADDR_IO           (1<<0) /* PCI I/O space */
#define PCI_BASEADDR_MEM	  (0<<0) /* PCI Memory space */
#define PCI_BASEADDR_MEM_TYPE     (3<<1) /* memory type mask */
#define PCI_BASEADDR_MEM_32BIT    (0<<1) /* map anywhere in 32-bit addr space */
#define PCI_BASEADDR_MEM_ONEMEG   (1<<1) /* map below 1MB */
#define PCI_BASEADDR_MEM_64BIT    (2<<1) /* map anywhere in 64-bit addr space */
#define PCI_BASEADDR_MEM_PREFETCH (1<<3) /* prefetchable */

/* cmd_reg defines */

#define PCI_CMDREG_IOSP      (1<<0)  /* Enable IO space accesses */
#define PCI_CMDREG_MEMSP     (1<<1)  /* Enable MEM space accesses */
#define PCI_CMDREG_MASTR     (1<<2)  /* Enable PCI Mastership */
#define PCI_CMDREG_SPCYC     (1<<3)  /* Monitor special cycles */
#define PCI_CMDREG_MEMWINV   (1<<4)  /* Enable memory write and invalidate */
#define PCI_CMDREG_VGASNP    (1<<5)  /* Enable VGA palette snooping */
#define PCI_CMDREG_PERR      (1<<6)  /* Enable Parity error response */
#define PCI_CMDREG_STEP      (1<<7)  /* Enable address/data stepping */
#define PCI_CMDREG_SERR      (1<<8)  /* Enable SERR driver */
#define PCI_CMDREG_FSTB2B    (1<<9)  /* Enable back to back transactions */

/* PCI Space Definitions  -- For configuring the RAVEN */

/* PPC Slave Attribute bit definitions */

#define CPU2PCI_ATTR_REN     (1<<7)  /* Read enable */
#define CPU2PCI_ATTR_WEN     (1<<6)  /* Write enable */
#define CPU2PCI_ATTR_WPEN    (1<<4)  /* Write post enable */
#define CPU2PCI_ATTR_MEM     (1<<1)  /* PCI memory cycle */
#define CPU2PCI_ATTR_IOM     (1<<0)  /* PCI I/O mode */

/* CPU to PCI definitions */

#define CPU2PCI_MSATT_MEM	( CPU2PCI_ATTR_REN | CPU2PCI_ATTR_WEN | \
				  CPU2PCI_ATTR_MEM )

#define CPU2PCI_MSATT_IO	( CPU2PCI_ATTR_REN | CPU2PCI_ATTR_WEN )
#define CPU2PCI_MSATT_DISABLED  ( CPU2PCI_MSATT_MEM & ~( CPU2PCI_ATTR_REN | \
						         CPU2PCI_ATTR_WEN ) )

/* STANDARD ( PREP ) mapping of PCI space */

/* disable PCI address space 0 */

#define CPU2PCI_ADDR0_START_VAL 0x0000
#define CPU2PCI_ADDR0_END_VAL	0x0000
#define CPU2PCI_OFFSET0_VAL	((0x0-CPU2PCI_ADDR0_START_VAL) & 0xffff)
#define CPU2PCI_MSATT0_VAL	CPU2PCI_MSATT_DISABLED

/* disable PCI address space 1 */

#define CPU2PCI_ADDR1_START_VAL 0x0000
#define CPU2PCI_ADDR1_END_VAL	0x0000
#define CPU2PCI_OFFSET1_VAL	((0x0-CPU2PCI_ADDR1_START_VAL) & 0xffff)
#define CPU2PCI_MSATT1_VAL	CPU2PCI_MSATT_DISABLED

/* setup address space 2 for PCI MEM space */

#define CPU2PCI_ADDR2_START_VAL (PCI_MSTR_MEMIO_LOCAL>>16)
#define CPU2PCI_ADDR2_END_VAL	0xfcff
#define CPU2PCI_OFFSET2_VAL	((0x0-CPU2PCI_ADDR2_START_VAL) & 0xffff)

/* setup address space 3 for PCI I/O space */

#define CPU2PCI_ADDR3_START_VAL (ISA_MSTR_IO_LOCAL>>16)
#define CPU2PCI_ADDR3_END_VAL	0xbf7f
#define CPU2PCI_OFFSET3_VAL	((0x0-CPU2PCI_ADDR3_START_VAL) & 0xffff)

/* defines that are used in romInit.s */

#define CPU2PCI_ADDR0_START	CPU2PCI_ADDR0_START_VAL
#define CPU2PCI_ADDR0_END	CPU2PCI_ADDR0_END_VAL
#define CPU2PCI_OFFSET0 	CPU2PCI_OFFSET0_VAL
#define CPU2PCI_MSATT0		CPU2PCI_MSATT0_VAL
#define CPU2PCI_ADDR1_START	CPU2PCI_ADDR1_START_VAL
#define CPU2PCI_ADDR1_END	CPU2PCI_ADDR1_END_VAL
#define CPU2PCI_OFFSET1 	CPU2PCI_OFFSET1_VAL
#define CPU2PCI_MSATT1		CPU2PCI_MSATT1_VAL
#define CPU2PCI_ADDR2_START	CPU2PCI_ADDR2_START_VAL
#define CPU2PCI_ADDR2_END	CPU2PCI_ADDR2_END_VAL
#define CPU2PCI_OFFSET2 	CPU2PCI_OFFSET2_VAL
#define CPU2PCI_MSATT2		CPU2PCI_MSATT_MEM
#define CPU2PCI_ADDR3_START	CPU2PCI_ADDR3_START_VAL
#define CPU2PCI_ADDR3_END	CPU2PCI_ADDR3_END_VAL
#define CPU2PCI_OFFSET3 	CPU2PCI_OFFSET3_VAL
#define CPU2PCI_MSATT3		CPU2PCI_MSATT_IO

/* PCI to CPU definitions */

#ifdef LOCAL_MEM_AUTOSIZE
#   define DRAM_SIZE		((ULONG)sysPhysMemTop() - LOCAL_MEM_LOCAL_ADRS)
#else
#   define DRAM_SIZE		(LOCAL_MEM_SIZE - LOCAL_MEM_LOCAL_ADRS)
#endif

#define PCI2CPU_ADDR0_START	(PCI_SLV_MEM_BUS & 0xffff0000)
#define PCI2CPU_ADDR0_END	((PCI_SLV_MEM_BUS + PCI_SLV_MEM_SIZE \
				  - 0x10001) >> 16)
#define PCI2CPU_ADDR0_RANGE	(PCI2CPU_ADDR0_START | PCI2CPU_ADDR0_END)
#define PCI2CPU_OFFSET0 	(((0x0-PCI_SLV_MEM_BUS)>>16) & 0x0000ffff)
#define PCI2CPU_ATT0		0xf2

#define PCI2CPU_ADDR1_START	((PCI_SLV_MEM_BUS + PCI_SLV_MEM_SIZE \
				  - 0x10000) & 0xffff0000)
#define PCI2CPU_ADDR1_END	((PCI_SLV_MEM_BUS + PCI_SLV_MEM_SIZE \
				  - 0x10000) >> 16)
#define PCI2CPU_ADDR1_RANGE	(PCI2CPU_ADDR1_START | PCI2CPU_ADDR1_END)
#define PCI2CPU_OFFSET1 	(((0x0-PCI_SLV_MEM_BUS)>>16) & 0x0000ffff)
#define PCI2CPU_ATT1		0xe2

/*
 * Address decoders 2 and 3 are not currently used, so they are
 * set to point to an address that is not used on the PCI bus
 */

#define PCI2CPU_ADDR2_RANGE	0xfff0fff0
#define PCI2CPU_OFFSET2 	0x0
#define PCI2CPU_ATT2		0x0
#define PCI2CPU_ADDR3_RANGE	0xfff0fff0
#define PCI2CPU_OFFSET3 	0x0
#define PCI2CPU_ATT3		0x0

/*
 * Allocate PCI Memory and I/O Space Offsets for PCI devices
 *
 * All devices on the primary and secondary busses are allocated 64 kb spaces.
 * The PMC Span can control up to four PMCs.
 */

/* LAN_DEV_SPACE autoconfigured but still needed for clean compile */

#define LAN_DEV_SPACE		0x20000
#define SCSI_DEV_SPACE		0x10000

/* PCI view of PCI I/O Space for PCI devices */

#define PCI_IO_SCSI_ADRS	(PCI_IO_ADRS + SCSI_DEV_SPACE)	/* 53C825 */

/* PCI view of PCI Memory Space for PCI devices */

/* Allocated base address of HW devices as seen from CPU */

#define SCSI_BASE_ADRS		( PCI_MSTR_IO_LOCAL + SCSI_DEV_SPACE )

/* LAN_BASE_ADRS autoconfigured but still needed for clean compile */

#define LAN_BASE_ADRS		( PCI_MSTR_IO_LOCAL + LAN_DEV_SPACE )

/* pc87303 ISA super IO device (ISASIO) keybrd, serial, Parallel port */

#define pc87303_KBD_CTRL	(ISA_MSTR_IO_LOCAL + 0x0064)	/* keyboard */
#define pc87303_INDX_REG	(ISA_MSTR_IO_LOCAL + 0x0398)	/* index reg */
#define pc87303_DATA_REG	(ISA_MSTR_IO_LOCAL + 0x0399)	/* data reg */
#define pc87303_PP		(ISA_MSTR_IO_LOCAL + 0x03bc)	/* parallel */
#define pc87303_COM1		(ISA_MSTR_IO_LOCAL + 0x03f8)	/* serial 1 */
#define pc87303_COM2		(ISA_MSTR_IO_LOCAL + 0x02f8)	/* serial 2 */
#ifdef CONFIG2_PLANAR_ISA
#   define pc87303_COM3 	(ISA_MSTR_IO_LOCAL + 0x03e8)	/* serial 3 */
#   define pc87303_COM4 	(ISA_MSTR_IO_LOCAL + 0x02e8)	/* serial 4 */
#endif
#define pc87303_FDC		(ISA_MSTR_IO_LOCAL + 0x03f0)	/* floppy */

/* z85230 synchronous & Asynchronous serial communications chip */

#define z85230_PORTB_CTRL	(ISA_MSTR_IO_LOCAL + 0x0840)	/* serial 4 */
#define z85230_PORTB_DATA	(ISA_MSTR_IO_LOCAL + 0x0841)
#define z85230_PORTA_CTRL	(ISA_MSTR_IO_LOCAL + 0x0842)	/* serial 3 */
#define z85230_PORTA_DATA	(ISA_MSTR_IO_LOCAL + 0x0843)

/* z8536 aux timer and I/O chip */

#define z8536_PORTC_DATA	(ISA_MSTR_IO_LOCAL + 0x0844)
#define z8536_PORTB_DATA	(ISA_MSTR_IO_LOCAL + 0x0845)
#define z8536_PORTA_DATA	(ISA_MSTR_IO_LOCAL + 0x0846)
#define z8536_PORT_CTRL 	(ISA_MSTR_IO_LOCAL + 0x0847)
#define ZCIO_CNTRL_ADRS 	(UINT8 *)(ISA_MSTR_IO_LOCAL + 0x847)
#define ZCIO_IACK_ADRS		(UINT8 *)(ISA_MSTR_IO_LOCAL + 0x84F)
#define ZCIO_DD_PORT_A          ((UINT8) 0x07)
#define ZCIO_DD_PORT_B          ((UINT8) 0xC7)
#define ZCIO_DD_PORT_C          ((UINT8) 0x00)


/* m48TXX non volatile ram, RTC and watchdog timer */

#define m48TXX_LSB_REG		(ISA_MSTR_IO_LOCAL + 0x0074)
#define m48TXX_MSB_REG		(ISA_MSTR_IO_LOCAL + 0x0075)
#define m48TXX_DAT_REG		(ISA_MSTR_IO_LOCAL + 0x0077)

#ifdef	INCLUDE_I2C

/* I2C registers */

#define I2C_CTRL_REG		(ISA_MSTR_IO_LOCAL + 0x0981)
#define I2C_CMD_STAT		(ISA_MSTR_IO_LOCAL + 0x0980)

#endif /* INCLUDE_I2C */

#ifdef INCLUDE_VPD

/* Vital Product Data Support */

#define VPD_BRD_EEPROM_ADRS 0xa0    /* i2c address of board's SROM */
#define VPD_TM_EEPROM_ADRS  0xa4    /* i2c address of transition modules SROM */

#define VPD_BRD_OFFSET      0   /* offset into board's eeprom for vpd data */
#define VPD_TM_OFFSET       0   /* offset into tm's eeprom for vpd data */

#define VPD_PKT_LIMIT       25  /* Max number of packets expected */

#endif /* INCLUDE_VPD */

/* CPU type */

#define CPU_TYPE		((vxPvrGet() >> 16) & 0xffff)
#define CPU_TYPE_601		0x01	/* PPC 601 CPU */
#define CPU_TYPE_602		0x02	/* PPC 602 CPU */
#define CPU_TYPE_603		0x03	/* PPC 603 CPU */
#define CPU_TYPE_603E		0x06	/* PPC 603e CPU */
#define CPU_TYPE_603P		0x07	/* PPC 603p CPU */
#define CPU_TYPE_750		0x08	/* PPC 750 CPU */
#define CPU_TYPE_604		0x04	/* PPC 604 CPU */
#define CPU_TYPE_604E		0x09	/* PPC 604e CPU */
#define CPU_TYPE_604R		0x0A	/* PPC 604r CPU */

/* L2CR register (MPC750 - Arthur) */

#define MPC750_L2CR_E		0x80000000
#define MPC750_L2CR_256K	0x10000000
#define MPC750_L2CR_512K	0x20000000
#define MPC750_L2CR_1024K	0x30000000
#define MPC750_L2CR_I		0x00200000
#define MPC750_L2CR_SL		0x00008000
#define MPC750_L2CR_IP		0x00000001

/* System Configuration register */

#define SYS_REG_GCR		((unsigned int *)(FALCON_BASE_ADRS + 0x08))
#define SYS_REG_GCR_MSK 	0x0000011e
#define SYS_REG_GCR_FREF	0x00000008
#define SYS_REG_GCR_DRAM_70ns	0x00000000
#define SYS_REG_GCR_DRAM_60ns	0x00000002
#define SYS_REG_GCR_DRAM_50ns	0x00000006


#ifdef CONFIG1_SYS_REG_CCR
#   define SYS_REG_CCR_ 	(FALCON_BASE_ADRS + 0x400)
#   define SYS_REG_CCR		((unsigned int *)(FALCON_BASE_ADRS + 0x400))

#   define SYS_REG_CCR_ID_MSK		0xff000000	/* System ID mask */
#   define SYS_REG_CCR_MCP750		0xfc000000	/* MCP750 board type */
#   define SYS_REG_CCR_MCPN750		0xf9000000	/* MCPN750 board type */
#   define SYS_REG_CCR_CLK_MSK		0x00f00000	/* Bus clock Mask */
#   define SYS_REG_CCR_CPU_CLK_66	0x00f00000 /* cpu ext Bus clk 66 Mhz */
#   define SYS_REG_CCR_CPU_CLK_60	0x00e00000 /* cpu ext Bus clk 60 Mhz */
#   define SYS_REG_CCR_CPU_CLK_50	0x00d00000 /* cpu ext Bus clk 50 Mhz */

#   define SYS_REG_CCR_SYSXC_MSK	0x000f0000 /* lkaside l2 cache mask */
#   define SYS_REG_CCR_SYSXC_256	0x000e0000 /* lkaside 256kb L2 cache */
#   define SYS_REG_CCR_SYSXC_512	0x000d0000 /* lkaside 512kb L2 cache */
#   define SYS_REG_CCR_SYSXC_1024	0x000c0000 /* lkaside 1Mb L2 cache */
#   define SYS_REG_CCR_SYSXC_NC 	0x000f0000 /* lkaside no cache */

#   define SYS_REG_CCR_P0STAT_MSK	0x0000f000 /* in-line l2 cache mask */
#   define SYS_REG_CCR_P0STAT_256	0x00006000 /* in-line 256kb L2 cache */
#   define SYS_REG_CCR_P0STAT_512	0x00005000 /* in-line 512kb L2 cache */
#   define SYS_REG_CCR_P0STAT_1024	0x00004000 /* in-line 1Mb L2 cache */
#   define SYS_REG_CCR_P0STAT_NC	0x00007000 /* in-line no cache */
#endif /* CONFIG1_SYS_REG_CCR */

/* DRAM configuration registers */

#ifdef CONFIG1_SYS_REG_MCR
#   define SYS_REG_MCR_ 	(FALCON_BASE_ADRS + 0x404)
#   define SYS_REG_MCR		((unsigned int *)(FALCON_BASE_ADRS + 0x404))

#   define SYS_REG_MCR_FREF_UNK 	0x10000000
#   define SYS_REG_MCR_FREF_TRUE	0x10000000
#   define SYS_REG_MCR_FREF_FALSE	0x00000000

#   define SYS_REG_MCR_DRAM_MSK 	0x03000000
#   define SYS_REG_MCR_DRAM_50ns	0x03000000
#   define SYS_REG_MCR_DRAM_60ns	0x01000000
#   define SYS_REG_MCR_DRAM_70ns	0x00000000

#   define SYS_REG_MCR_ROMAB_MSK	0x00700000
#   define SYS_REG_MCR_ROMAB_ITL	0x00600000
#   define SYS_REG_MCR_ROMAB_UNK	0x00700000

#   define SYS_REG_MCR_L2TYPE_MSK	0x0000F000
#   define SYS_REG_MCR_L2TYPE_LWP	0x00000000
#   define SYS_REG_MCR_L2TYPE_BP	0x00001000
#   define SYS_REG_MCR_L2TYPE_LWNP	0x00002000
#   define SYS_REG_MCR_L2TYPE_BNP	0x00003000

#   define SYS_REG_MCR_L2PLL_MSK	0x00000F00
#   define SYS_REG_MCR_L2PLL_DIS	0x00000000
#   define SYS_REG_MCR_L2PLL_1_1	0x00000100
#   define SYS_REG_MCR_L2PLL_3_2	0x00000200
#   define SYS_REG_MCR_L2PLL_2_1	0x00000300
#   define SYS_REG_MCR_L2PLL_5_2	0x00000400
#   define SYS_REG_MCR_L2PLL_3_1	0x00000500

#   define SYS_REG_MCR_FLASH_MSK	0x00000038
#   define SYS_REG_MCR_FLASH_1M 	0x00000000
#   define SYS_REG_MCR_FLASH_2M 	0x00000008
#   define SYS_REG_MCR_FLASH_4M 	0x00000010
#   define SYS_REG_MCR_FLASH_8M 	0x00000018
#   define SYS_REG_MCR_FLASH_16M	0x00000020
#   define SYS_REG_MCR_FLASH_32M	0x00000028
#   define SYS_REG_MCR_FLASH_64M	0x00000030
#   define SYS_REG_MCR_FLASH_NO 	0x00000038

#endif /* CONFIG1_SYS_REG_MCR */

#ifdef CONFIG2_SYS_REG_MCR
#   define SYS_REG_MCR_ 	(FALCON_BASE_ADRS + 0x404)
#   define SYS_REG_MCR		((unsigned int *)(FALCON_BASE_ADRS + 0x404))

#   define SYS_REG_MCR_MSIZ_MSK 	0xC0000001
#   define SYS_REG_MCR_MSIZ_16M 	0x00000000
#   define SYS_REG_MCR_MSIZ_32M 	0x40000000
#   define SYS_REG_MCR_MSIZ_64M 	0x80000000
#   define SYS_REG_MCR_MSIZ_128M	0xC0000000
#   define SYS_REG_MCR_MSIZ_256M	0x00000001

#   define SYS_REG_MCR_FREF_UNK 	0x10000000
#   define SYS_REG_MCR_FREF_TRUE	0x10000000
#   define SYS_REG_MCR_FREF_FALSE	0x00000000

#   define SYS_REG_MCR_DRAM_MSK 	0x03000000
#   define SYS_REG_MCR_DRAM_50ns	0x03000000
#   define SYS_REG_MCR_DRAM_60ns	0x01000000
#   define SYS_REG_MCR_DRAM_70ns	0x0

#   define SYS_REG_MCR_ROMAB_MSK	0x00700000
#   define SYS_REG_MCR_ROMAB_ITL	0x00600000
#   define SYS_REG_MCR_ROMAB_UNK	0x00700000

#   define SYS_REG_MCR_L2TYPE_MSK	0x0000F000
#   define SYS_REG_MCR_L2TYPE_LWP	0x00000000
#   define SYS_REG_MCR_L2TYPE_BP	0x00001000
#   define SYS_REG_MCR_L2TYPE_LWNP	0x00002000
#   define SYS_REG_MCR_L2TYPE_BNP	0x00003000

#   define SYS_REG_MCR_L2PLL_MSK	0x00000F00
#   define SYS_REG_MCR_L2PLL_DIS	0x00000000
#   define SYS_REG_MCR_L2PLL_1_1	0x00000100
#   define SYS_REG_MCR_L2PLL_3_2	0x00000200
#   define SYS_REG_MCR_L2PLL_2_1	0x00000300
#   define SYS_REG_MCR_L2PLL_5_2	0x00000400
#   define SYS_REG_MCR_L2PLL_3_1	0x00000500

#   define SYS_REG_MCR_FLASH_MSK	0x00000038
#   define SYS_REG_MCR_FLASH_1M 	0x00000000
#   define SYS_REG_MCR_FLASH_2M 	0x00000008
#   define SYS_REG_MCR_FLASH_4M 	0x00000010
#   define SYS_REG_MCR_FLASH_8M 	0x00000018
#   define SYS_REG_MCR_FLASH_16M	0x00000020
#   define SYS_REG_MCR_FLASH_32M	0x00000028
#   define SYS_REG_MCR_FLASH_64M	0x00000030
#   define SYS_REG_MCR_FLASH_NO 	0x00000038
#endif /* CONFIG2_SYS_REG_MCR */

#define DRAM_REG_SIZE		(FALCON_BASE_ADRS + 0x10)
#define DRAM_REG_BASE		(FALCON_BASE_ADRS + 0x18)

/* Base Module Feature Register */

#ifdef CONFIG1_SYS_REG_BMFR
#   define SYS_REG_BMFR 	((char *)(ISA_MSTR_IO_LOCAL + 0x0802))
#   define SYS_REG_BMFR_SCCP	0x40	/* z85230 sync serial Port */
#   define SYS_REG_BMFR_RSV1	0x20	/* reserved */
#   define SYS_REG_BMFR_PMCP	0x10	/* pmc Present */
#   define SYS_REG_BMFR_RSV2	0x08	/* reserved */
#   define SYS_REG_BMFR_RSV3	0x04	/* reserved */
#   define SYS_REG_BMFR_LANP	0x02	/* ethernet Present */
#   define SYS_REG_BMFR_RSV4	0x01	/* reserved */
#endif
#ifdef CONFIG2_SYS_REG_BMFR
#   define SYS_REG_BMFR 	((char *)(ISA_MSTR_IO_LOCAL + 0x0802))
#   define SYS_REG_BMFR_RSV1	0x40	/* z85230 sync serial Port */
#   define SYS_REG_BMFR_PMC2P	0x20	/* PMC slot 2 card present */
#   define SYS_REG_BMFR_PMC1P	0x10	/* PMC slot 1 card present */
#   define SYS_REG_BMFR_RSV2	0x08	/* reserved */
#   define SYS_REG_BMFR_RSV3	0x04	/* reserved */
#   define SYS_REG_BMFR_LANP	0x02	/* ethernet Present */
#   define SYS_REG_BMFR_RSV4	0x01	/* reserved */
#endif

#define DEVICE_PRESENT(x)	(!((*SYS_REG_BMFR) & (x)))

/* Base Module Status Register */

#ifdef CONFIG1_SYS_REG_BMSR
#   define	SYS_REG_BMSR	((char *)(ISA_MSTR_IO_LOCAL + 0x0803))
#endif

#define SYS_REG_BMSR_MCP750	0xe0	/* MCP750 */
#define SYS_REG_BMSR_MCPN750	0xe1	/* MCPN750 */

/* Base Module Extended Features Register */

#ifdef CONFIG1_SYS_REG_BMEFR
#   define SYS_REG_BMEFR	((char *)(ISA_MSTR_IO_LOCAL + 0x0810))
#   define SYS_REG_BMEFR_USB1_OK	0x80	/* USB 1 Voltage sending 5V */
#   define SYS_REG_BMEFR_USB0_OK	0x40	/* USB 0 Voltage sending 5V */
#   define SYS_REG_BMEFR_CFPR		0x08	/* Compact FLASH Present */
#   define SYS_REG_BMEFR_NVRAMP 	0x04	/* NVRAM Present */
#   define SYS_REG_BMEFR_ISA_REWORK	0x02	/* Lvl sens ISA int rework */
#   define SYS_REG_BMEFR_PPB1P		0x01	/* PCI-PCI bridge present */
#endif

#ifdef CONFIG2_SYS_REG_BMEFR
#   define SYS_REG_BMEFR	((char *)(ISA_MSTR_IO_LOCAL + 0x0810))
#   define SYS_REG_BMEFR_USB1_OK	0x80	/* USB 1 Voltage sending 5V */
#   define SYS_REG_BMEFR_USB0_OK	0x40	/* USB 0 Voltage sending 5V */
#   define SYS_REG_BMEFR_TMPRSNT	0x20	/* USB 0 Voltage sending 5V */
#   define SYS_REG_BMEFR_RSV1		0x08	/* reserved */
#   define SYS_REG_BMEFR_NVRAMP 	0x04	/* NVRAM Present */
#   define SYS_REG_BMEFR_RSV2		0x01	/* reserved */
#endif

#define EXTENDED_FEATURE_PRESENT(x) (!((*SYS_REG_BMEFR) & (x)))

/* SYSLED Register */

#ifdef CONFIG1_SYS_REG_SYSLED
#   define SYS_REG_SYSLED	((char *)(ISA_MSTR_IO_LOCAL + 0x0820))
#   define SYS_REG_SYSLED_SYSEN_	0x04	/* System Slot Identification */
#   define SYS_REG_SYSLED_GRN_LEN	0x02	/* Green LED */
#   define SYS_REG_SYSLED_YEL_LEN	0x01	/* Yellow LED */
#endif

/* Board Status Register */

#ifdef CONFIG1_SYS_REG_BSR
#   define SYS_REG_BSR		((char *)(ISA_MSTR_IO_LOCAL + 0x0824))
#   define SYS_REG_BSR_LAN_OK	0x10	/* Ethernet channel ok */
#   define SYS_REG_BSR_USB_OK	0x08	/* USB channels ok */
#   define SYS_REG_BSR_CPCI_OK	0x04	/* CompactPCI ok */
#   define SYS_REG_BSR_BOOT_OK	0x02	/* Boot ok */
#   define SYS_REG_BSR_ONLINE	0x01	/* Board online */
#endif

/* Board Last Reset Register */

#ifdef CONFIG1_SYS_REG_BLRR
#   define SYS_REG_BLRR 	((char *)(ISA_MSTR_IO_LOCAL + 0x0828))
#   define SYS_REG_BLRR_PBRST	0x80	/* cPCI push button reset */
#   define SYS_REG_BLRR_SWSRST	0x40	/* Software Soft Reset */
#   define SYS_REG_BLRR_SWHRST	0x20	/* Software Hard Reset, port 92 */
#   define SYS_REG_BLRR_RSV1	0x10	/* reserved */
#   define SYS_REG_BLRR_RSV2	0x08	/* reserved */
#   define SYS_REG_BLRR_WDT2	0x04	/* Watchdog Timer Level 2 Reset */
#   define SYS_REG_BLRR_FBTN	0x02	/* Front Panel Button Reset */
#   define SYS_REG_BLRR_PWRON	0x01	/* Power-On Reset */
#endif

#ifdef CONFIG2_SYS_REG_BLRR
#   define SYS_REG_BLRR 	((char *)(ISA_MSTR_IO_LOCAL + 0x0828))
#   define SYS_REG_BLRR_RSV1	0x80	/* Reserved */
#   define SYS_REG_BLRR_SWSRST	0x40	/* Software Soft Reset */
#   define SYS_REG_BLRR_SWHRST	0x20	/* Software Hard Reset, port 92 */
#   define SYS_REG_BLRR_CMDRST	0x10	/* Compact PCI command reset */
#   define SYS_REG_BLRR_CPCIRST 0x08	/* Compact PCI hard reset (RST#) */
#   define SYS_REG_BLRR_WDT2	0x04	/* Watchdog Timer Level 2 Reset */
#   define SYS_REG_BLRR_FBTN	0x02	/* Front Panel Button Reset */
#   define SYS_REG_BLRR_PWRON	0x01	/* Power-On Reset */
#endif

/* cPCI Geographic Address Register */

#ifdef CONFIG2_SYS_REG_CPGAR
#   define SYS_REG_CPGAR	((char *)(ISA_MSTR_IO_LOCAL + 0x0830))
#   define SYS_REG_GPGAR_MSK	0x1f
#endif

#ifdef CONFIG2_SYS_REG_PINTA
#   define SYS_REG_PINTA	((char *)(ISA_MSTR_IO_LOCAL + 0x0832))
#   define SYS_REG_PINTA_MSK	0x00000003
#   define SYS_REG_GPGAR_INTA	0x0	/* INTA# routine */
#   define SYS_REG_GPGAR_INTB	0x1	/* INTB# routine */
#   define SYS_REG_GPGAR_INTC	0x2	/* INTC# routine */
#   define SYS_REG_GPGAR_INTD	0x3	/* INTD# routine */
#endif

#ifdef CONFIG2_SYS_REG_ABRTBTN
#   define SYS_REG_ABRTBTN	((char *)(ISA_MSTR_IO_LOCAL + 0x0845))
#   define SYS_REG_ABRTBTN_MSK		0x80
#   define SYS_REG_ABRTBTN_DOWN 	0x00
#   define SYS_REG_ABRTBTN_UP		0x00
#endif

#ifdef CONFIG2_SYS_REG_BRDFAIL_LED
#   define SYS_REG_BRDFAIL_LED	((char *)(ISA_MSTR_IO_LOCAL + 0x0846))
#   define SYS_REG_BRDFAIL_LED_ON	0x40
#endif


/* Assembly define for L2 cache */

#define SYS_REG_SXCCR_A 	(FALCON_BASE_ADRS + 0x8000)

/* defines for L2 cache routines */

#define SYS_REG_SXCCR		((unsigned char *)(FALCON_BASE_ADRS + 0x8000))

#define L2_DISABLE		0x80
#define L2_RESET		0x40
#define L2_ENABLE		0x80
#define L2_FLUSH		0x10
#define L2_END_FLUSH		0x10
#define L2_FLUSH_LOOP		4100

/* z8536 I/O port bit mapping */

#ifdef CONFIG1_SYS_REG_BRDFAIL_LED
#   define	z8536_PORTA_BRDFAIL	0x40
#endif

#ifdef CONFIG1_PLANAR_ISA
#   define	z8536_PORTB_FUSE	0x40
#   define	z8536_PORTB_ABORT	0x80
#endif

/*
 * Raven Extensions to Standard PCI Header
 *
 * Type declarations for the PCI Header and the macros in regards to the
 * PCI BUS.  These definitions have been made with respect to PCI LOCAL
 * BUS SPECIFICATION REVISION 2.1.  Every device on the PCI BUS has to
 * support 256 bytes of configuration space of which the first 64 bytes
 * are a predefined header portion defined by the PCI commitee.  Bytes
 * 64 to 255 are dedicated to the device specific registers.
 *
 * Note: the PCI bus is inherently little endian.
 */

#define PCI_CFG_RAVEN_PSADD0	0x80
#define PCI_CFG_RAVEN_PSATT0	0x84
#define PCI_CFG_RAVEN_PSOFF0	0x86
#define PCI_CFG_RAVEN_PSADD1	0x88
#define PCI_CFG_RAVEN_PSATT1	0x8c
#define PCI_CFG_RAVEN_PSOFF1	0x8e
#define PCI_CFG_RAVEN_PSADD2	0x90
#define PCI_CFG_RAVEN_PSATT2	0x94
#define PCI_CFG_RAVEN_PSOFF2	0x96
#define PCI_CFG_RAVEN_PSADD3	0x98
#define PCI_CFG_RAVEN_PSATT3	0x9c
#define PCI_CFG_RAVEN_PSOFF3	0x9e

/* Raven MPC registers */

#define RAVEN_MPC_VENID 	0x00
#define RAVEN_MPC_DEVID 	0x02
#define RAVEN_MPC_REVID 	0x05
#define RAVEN_MPC_GCSR		0x08
#define RAVEN_MPC_FEAT		0x0a
#define RAVEN_MPC_MARB		0x0e
#define RAVEN_MPC_PADJ		0x13
#define RAVEN_MPC_MEREN 	0x22
#define RAVEN_MPC_MERST 	0x27
#define RAVEN_MPC_MERAD 	0x28
#define RAVEN_MPC_MERAT 	0x2e
#define RAVEN_MPC_PIACK 	0x30
#define RAVEN_MPC_MSADD0	0x40
#define RAVEN_MPC_MSOFF0	0x44
#define RAVEN_MPC_MSATT0	0x47
#define RAVEN_MPC_MSADD1	0x48
#define RAVEN_MPC_MSOFF1	0x4c
#define RAVEN_MPC_MSATT1	0x4f
#define RAVEN_MPC_MSADD2	0x50
#define RAVEN_MPC_MSOFF2	0x54
#define RAVEN_MPC_MSATT2	0x57
#define RAVEN_MPC_MSADD3	0x58
#define RAVEN_MPC_MSOFF3	0x5c
#define RAVEN_MPC_MSATT3	0x5f
#define RAVEN_MPC_WDT1CNTL	0x60
#define RAVEN_MPC_WDT2CNTL	0x68
#define RAVEN_MPC_GPREG0_U	0x70
#define RAVEN_MPC_GPREG0_L	0x74
#define RAVEN_MPC_GPREG1_U	0x78
#define RAVEN_MPC_GPREG1_L	0x7c

/*
 * Raven register bit masks
 *
 * Bits marked with 'C' indicate conditions which can be cleared by
 * writing a 1 to the bits.
 */

/* Raven MPC Error Enable (MEREN) register bit masks */

#define RAVEN_MPC_MEREN_RTAI	0x0001	/* PCI mstr Recvd Target Abort Int */
#define RAVEN_MPC_MEREN_SMAI	0x0002	/* PCI mstr Signld Target Abort Int */
#define RAVEN_MPC_MEREN_SERRI	0x0004	/* PCI System Error Int */
#define RAVEN_MPC_MEREN_PERRI	0x0008	/* PCI Parity Error Int */
#define RAVEN_MPC_MEREN_MDPEI	0x0010	/* MPC Data Parity Error Int */
#define RAVEN_MPC_MEREN_MATOI	0x0020	/* MPC Address Bus Time-out Int */
#define RAVEN_MPC_MEREN_RTAM	0x0100	/* RTAI machine check enable */
#define RAVEN_MPC_MEREN_SMAM	0x0200	/* SMAI machine check enable */
#define RAVEN_MPC_MEREN_SERRM	0x0400	/* SERRI machine check enable */
#define RAVEN_MPC_MEREN_PERRM	0x0800	/* PERRI machine check enable */
#define RAVEN_MPC_MEREN_MDPEM	0x1000	/* MDPEI machine check enable */
#define RAVEN_MPC_MEREN_MATOM	0x2000	/* MATOI machine check enable */
#define RAVEN_MPC_MEREN_DFLT	0x4000	/* Default MPC Master ID select */
#define RAVEN_MPC_MEREN_VALID	0x7F3F	/* Mask for valid MEREN bits */

/* Raven MPC Error Status (MERST) register bit masks */

#define RAVEN_MPC_MERST_RTA	0x01	/* C PCI mstr Recvd Target Abort */
#define RAVEN_MPC_MERST_SMA	0x02	/* C PCI mstr Signld Target Abort */
#define RAVEN_MPC_MERST_SERR	0x04	/* C PCI System Error */
#define RAVEN_MPC_MERST_PERR	0x08	/* C PCI Parity Error */
#define RAVEN_MPC_MERST_MDPE	0x10	/* C MPC Data Bus Parity Error */
#define RAVEN_MPC_MERST_MATO	0x20	/* C MPC Address Bus Time-out */
#define RAVEN_MPC_MERST_OVF	0x80	/* C Error Status Overflow */
#define RAVEN_MPC_MERST_VALID	0xBF	/*   Mask for valid MERST bits */
#define RAVEN_MPC_MERST_CLR	0xBF	/*   Clears all errors */

/* Raven PCI Configuration Status register bit masks */

#define RAVEN_PCI_CFG_STATUS_FAST    0x0010  /*   Fast back-to-back capable */
#define RAVEN_PCI_CFG_STATUS_DPAR    0x0100  /* C Data prity error detected */
#define RAVEN_PCI_CFG_STATUS_SELTIM0 0x0200  /*   Device select timing bit 0 */
#define RAVEN_PCI_CFG_STATUS_SELTIM1 0x0400  /*   Device select timing bit 1 */
#define RAVEN_PCI_CFG_STATUS_SIGTA   0x0800  /* C Signalled Target Abort */
#define RAVEN_PCI_CFG_STATUS_RCVTA   0x1000  /* C Received Target Abort */
#define RAVEN_PCI_CFG_STATUS_RCVMA   0x2000  /* C Received Master Abort */
#define RAVEN_PCI_CFG_STATUS_SIGSE   0x4000  /* C Signalled System Error */
#define RAVEN_PCI_CFG_STATUS_RCVPE   0x8000  /* C Detected Parity Error */
#define RAVEN_PCI_CFG_STATUS_VALID   0xFF10  /*   Valid status bits */
#define RAVEN_PCI_CFG_STATUS_CLR     0xF900  /*   Clears all conditions */

/*
 * Falcon Register Offsets
 */

#define FALCON_DPE_LOG_REG 0x68
#define FALCON_DPE_ENA_REG 0x6a

#define FALCON_DPELOG    (1 << 7) /* assuming byte-wide access */
#define FALCON_DPE_CKALL (1 << 1) /* assuming byte-wode access */
#define FALCON_DPE_ME    (1 << 0) /* assuming byte-wide access */

/*
 * VIA Extensions to Standard PCI Header
 *
 * Type declarations for the PCI Header and the macros in regards to the
 * PCI BUS.  These definitions have been made with respect to PCI LOCAL
 * BUS SPECIFICATION REVISION 2.1.  Every device on the PCI BUS has to
 * support 256 bytes of configuration space of which the first 64 bytes
 * are a predefined header portion defined by the PCI commitee.  Bytes
 * 64 to 255 are dedicated to the device specific registers.
 *
 * Note: the PCI bus is inherently little endian.
 */

#define PCI_CFG_VIA_ISA_BC	0x40
#define PCI_CFG_VIA_ISA_TM	0x41
#define PCI_CFG_VIA_ISA_CC	0x42
#define PCI_CFG_VIA_ROM_DC	0x43
#define PCI_CFG_VIA_KEYBD_CC	0x44
#define PCI_CFG_VIA_DMA_C	0x45
#define PCI_CFG_VIA_MC1 	0x46
#define PCI_CFG_VIA_MC2 	0x47
#define PCI_CFG_VIA_MC3 	0x48
#define PCI_CFG_VIA_IDE_IR	0x4a
#define PCI_CFG_VIA_MEM_AC1	0x4c
#define PCI_CFG_VIA_MEM_AC2	0x4d
#define PCI_CFG_VIA_MEM_AC3	0x4e
#define PCI_CFG_VIA_EGLV_SEL	0x54

/* VIA ISA Test Mode register bit masks */

#define VIA_PCI_CFG_ISA_TM_MSK	0xff		/* Register Mask */
#define VIA_PCI_CFG_ISA_TM_P92	0x20		/* Port 92 Enable */

/* VIA Miscellaneous Control 1 register bit masks */

#define VIA_PCI_CFG_MC1_MSK	0xff		/* Register Mask */
#define VIA_PCI_CFG_MC1_POST_M	0x01		/* Powt Memory Write Enable */

/* VIA Miscellaneous Control 2 register bit masks */

#define VIA_PCI_CFG_MC2_4D0_PORT 0x20		/* Enable 4d0/4d1 port */

/* VIA Miscellaneous Control 3 register bit masks */

#define VIA_PCI_CFG_MC3_MSK	0xff		/* Register Mask */
#define VIA_PCI_CFG_MC3_RTC_E	0x08		/* RTC Port 74/75 Enable */
#define VIA_PCI_CFG_MC3_512_M	0x01		/* 512K PCI Memory Decode */

/* VIA IDE Interrupt Routing register bit masks */

#define VIA_PCI_CFG_IDE_MSK	0xff		/* Register Mask */
#define VIA_PCI_CFG_IDE_SEC_15	0x04		/* IDE Second Channel IRQ15 */
#define VIA_PCI_CFG_IDE_PRI_14	0x00		/* IDE Primary Channel IRQ14 */

/* VIA ISA DMA/Master Memory Access Control 3 register bit masks */

#define VIA_PCI_CFG_MEM_AC3_MSK 0xff		/* Register Mask */
#define VIA_PCI_CFG_MEM_AC3_1M	0x0000		/* Top of Memory for ISA - 1M */
#define VIA_PCI_CFG_MEM_AC3_2M	0x1000		/* Top of Memory for ISA - 2M */
#define VIA_PCI_CFG_MEM_AC3_16M 0xf000		/* Top of Memory for ISA - 16M*/
#define VIA_PCI_CFG_MEM_0_7FFFF 0x0100		/* Forward 0 - 7FFFF to PCI */

/* VIA PCI-IRQ edge/level select bits (1 = edge, 0 = level) */

#define VIA_PCI_CFG_EGLV_LV 0x0f

/* VIA Legacy I/O Ports register bit masks */

#define VIA_ISA_PORT92_ADDRESS	(ISA_MSTR_IO_LOCAL + 0x92) /* Port 92 ADDRESS */
#define VIA_ISA_PORT92_RESET	0x01		/* Port 92 System Reset bit */

/*
 * PMC Span DEC21150 PCI-to-PCI Bridge device-specific registers
 *
 * These registers are in Configuration Space and are extensions to a
 * standard type 1 PCI header.
 */

#define PCI_CFG_DEC21150_CHIP_CTRL 0x40
#define PCI_CFG_DEC21150_DIAG_CTRL 0x41
#define PCI_CFG_DEC21150_ARB_CTRL  0x42
#define PCI_CFG_DEC21150_EVNT_DSBL 0x64
#define PCI_CFG_DEC21150_GPIO_DOUT 0x65
#define PCI_CFG_DEC21150_GPIO_CTRL 0x66
#define PCI_CFG_DEC21150_GPIO_DIN  0x67
#define PCI_CFG_DEC21150_SEC_CLK   0x68     /* secondary clock controll reg */
#define PCI_CFG_DEC21150_SERR_STAT 0x6A

/* programmable interrupt controller (PIC) */

#define PIC_REG_ADDR_INTERVAL	1	/* address diff of adjacent regs. */

/* programmable interrupt timers */

#define PIT_BASE_ADR		SL82565_TMR1_CNT0	/* timeraddrs */
#define PIT_REG_ADDR_INTERVAL	1
#define PIT_CLOCK		1193180

/* serial ports (COM1 - COM4) */

#ifdef INCLUDE_I8250_SIO
#   define COM1_BASE_ADR		pc87303_COM1	/* serial port 1 */
#   define COM2_BASE_ADR		pc87303_COM2	/* serial port 2 */

#   ifdef CONFIG2_PLANAR_ISA
#	define COM3_BASE_ADR		pc87303_COM3	/* serial port 3 */
#	define COM4_BASE_ADR		pc87303_COM4	/* serial port 4 */
#   endif

#   define UART_REG_ADDR_INTERVAL	1	/* addr diff of adjacent regs */
#   ifdef CONFIG1_PLANAR_ISA
#	define N_UART_CHANNELS		2	/* No. serial I/O channels */
#   endif
#   ifdef CONFIG2_PLANAR_ISA
#	define N_UART_CHANNELS		4	/* No. serial I/O channels */
#   endif
#endif /* INCLUDE_I8250_SIO */

#ifdef INCLUDE_Z85230_SIO
#   define BAUD_CLK_FREQ	10000000	/* 10 MHz "P Clock" (fixed) */
#   define REG_8530_WRITE(reg,val) sysOutByte((UINT32)(reg), (UINT8)(val))
#   define REG_8530_READ(reg,pVal) *(UINT8 *)pVal = sysInByte((UINT32)reg)
#   define DATA_REG_8530_DIRECT
/* #	define Z8530_RESET_DELAY_COUNT 2000 */
#define Z8530_RESET_DELAY \
	{ \
	int i; \
	for (i = 0; i < Z8530_RESET_DELAY_COUNT; i++) \
		; /* do nothing */ \
	}
#endif /* INCLUDE_Z85230_SIO */

/* total number of serial ports */

#ifdef CONFIG1_PLANAR_ISA
#   if defined(INCLUDE_I8250_SIO) && defined(INCLUDE_Z85230_SIO)
#	define N_SIO_CHANNELS	4		/* No. serial I/O channels */
#   elif defined(INCLUDE_I8250_SIO)
#	define N_SIO_CHANNELS	2		/* No. serial I/O channels */
#   elif defined(INCLUDE_Z85230_SIO)
#	define N_SIO_CHANNELS	2		/* No. serial I/O channels */
#   endif
#endif

#ifdef CONFIG2_PLANAR_ISA
#   ifdef INCLUDE_I8250_SIO
#	define N_SIO_CHANNELS	4		/* No. serial I/O channels */
#   endif
#endif

#ifndef N_SIO_CHANNELS
#   define N_SIO_CHANNELS	0		/* No. serial I/O channels */
#endif

/* non-volatile (battery-backed) ram defines
 *
 * the top 16 bytes are used for the RTC registers
 */

#define BBRAM_ADRS		0		/* base address */
#define BBRAM_SIZE		0x1ff0		/* 8k NVRAM Total Size */

/* factory ethernet address */

#define BB_ENET 		((char *)(BBRAM_ADRS + 0x1f2c))

/* MK48TXX register settings */

/* flag register */

#define MK48T_FLAGS		((char *)(BBRAM_ADRS + 0x1ff0))

/* alarm clock registers, 4 1byte locations */

#define ALARM_CLOCK		((char *)(BBRAM_ADRS + 0x1ff2))

/* interrupt register */

#define MK48T_INTR		((char *)(BBRAM_ADRS + 0x1ff6))

/* watchdog timer register */

#define WD_TIMER		((char *)(BBRAM_ADRS + 0x1ff7))

/* MK48TXX bb time of day clk, 8 1byte locations */

#define TOD_CLOCK		((char *)(BBRAM_ADRS + 0x1ff8))

#define NV_RAM_IO_MAPPED	/* nvram is io mapped in ISA space */
#define NV_RAM_READ(x)		sysNvRead (x)
#define NV_RAM_WRITE(x,y)	sysNvWrite (x,y)
#define NV_RAM_LSB_REG		m48TXX_LSB_REG
#define NV_RAM_MSB_REG		m48TXX_MSB_REG
#define NV_RAM_DAT_REG		m48TXX_DAT_REG

/* ncr810/ncr825 delay loop count */

#define NCR810_DELAY_MULT	10

/* SUPER I/O defines */

#define SUPER_IO_BASE1		0x002e
#define SUPER_IO_BASE2		0x015c
#define SUPER_IO_KBC_BASE_ADR	0x60
#define SUPER_IO_KBC_CMND_ADR	0x64
#define SUPER_IO_FDC_BASEHI	0x03
#define SUPER_IO_FDC_BASELO	0xf0
#define SUPER_IO_COM1_BASEHI	0x03
#define SUPER_IO_COM1_BASELO	0xf8
#define SUPER_IO_COM2_BASEHI	0x02
#define SUPER_IO_COM2_BASELO	0xf8
#define SUPER_IO_PP_BASEHI	0x03
#define SUPER_IO_PP_BASELO	0xbc
#define SUPER_IO_FDC_PS2	0x40
#define SUPER_IO_PP_CFG 	0x12
#define SUPER_IO_ENBL_INTS	0x02

/* Dec 2155x defines */

#define DEC2155X_MAILBOX_INT_VEC (DEC2155X_DOORBELL0_INT_VEC + \
				     DEC2155X_SM_DOORBELL_BIT)
#ifdef INCLUDE_DEC2155X


#   define DEC2155X_BIST_VAL 0x00
#   define DEC2155X_PRI_PRG_IF_VAL 0x00
#   define DEC2155X_PRI_SUBCLASS_VAL 0x20
#   define DEC2155X_PRI_CLASS_VAL 0x0b
#   define DEC2155X_SEC_PRG_IF_VAL 0x00
#   define DEC2155X_SEC_SUBCLASS_VAL 0x80
#   define DEC2155X_SEC_CLASS_VAL 0x06
#   define DEC2155X_MAX_LAT_VAL 0x00
#   define DEC2155X_MIN_GNT_VAL 0xff

#   define DEC2155X_PRI_INT_TO_CPCI_INTA 0x00
#   define DEC2155X_PRI_INT_TO_CPCI_INTB 0x01
#   define DEC2155X_PRI_INT_TO_CPCI_INTC 0x02
#   define DEC2155X_PRI_INT_TO_CPCI_INTD 0x03

#   define DEC2155X_HW_INT_ROUTING_ADRS (ISA_MSTR_IO_LOCAL + 0x832)
#   define DEC2155X_CPCI_HW_INT_ROUTING_VAL DEC2155X_PRI_INT_TO_CPCI_INTA

#   define DEC2155X_CHP_CTRL0_VAL 0x0000
#   define DEC2155X_CHP_CTRL1_VAL 0x0000

#   define DEC2155X_PRI_SERR_VAL (DEC2155X_SERR_DIS_DLYD_TRNS_MSTR_ABRT | \
			       DEC2155X_SERR_DIS_DLYD_RD_TRNS_TO | \
			       DEC2155X_SERR_DIS_DLYD_WRT_TRNS_DISC | \
			       DEC2155X_SERR_DIS_PSTD_WRT_DATA_DISC | \
			       DEC2155X_SERR_DIS_PSTD_WRT_TRGT_ABRT | \
			       DEC2155X_SERR_DIS_PSTD_WRT_MSTR_ABRT | \
			       DEC2155X_SERR_DIS_PSTD_WRT_PAR_ERROR)

#   define DEC2155X_SEC_SERR_VAL (DEC2155X_SERR_DIS_DLYD_TRNS_MSTR_ABRT | \
			       DEC2155X_SERR_DIS_DLYD_RD_TRNS_TO | \
			       DEC2155X_SERR_DIS_DLYD_WRT_TRNS_DISC | \
			       DEC2155X_SERR_DIS_PSTD_WRT_DATA_DISC | \
			       DEC2155X_SERR_DIS_PSTD_WRT_TRGT_ABRT | \
			       DEC2155X_SERR_DIS_PSTD_WRT_MSTR_ABRT | \
			       DEC2155X_SERR_DIS_PSTD_WRT_PAR_ERROR)

#endif /* INCLUDE_DEC2155X */

/* Interrupt bases  */

#define ISA_INTERRUPT_BASE	0x00
#define EXT_INTERRUPT_BASE	0x10
#define TIMER_INTERRUPT_BASE	0x20
#define IPI_INTERRUPT_BASE	0x24
#define ERR_INTERRUPT_BASE	0x28
#define ESCC_INTERRUPT_BASE	0x00

#define DEC2155X_INTERRUPT_BASE 0x60

/* interrupt Level definitions */

/* ISA interrupt defines (NOTE: these are int. NUMBERS, not levels) */

#ifdef CONFIG1_ISA_INT_TYPE  /* Config1 ISA int levels (e.g. MCP750 ) */

    /* programmable timer interrupt level */

#   define PIT_INT_LVL			( 0x00 + ISA_INTERRUPT_BASE )

    /* com port 2 interrupt level */

#   define COM2_INT_LVL 		( 0x03 + ISA_INTERRUPT_BASE )

    /* com port 1 interrupt level */

#   define COM1_INT_LVL 		( 0x04 + ISA_INTERRUPT_BASE )

    /* floppy interrupt level */

#   define FD_INT_LVL			( 0x06 + ISA_INTERRUPT_BASE )

    /* parallel port interrupt level (not supported ) */

#   define PP_INT_LVL			( 0x07 + ISA_INTERRUPT_BASE )

    /* z8536 timer interrupt level (shared with z85230) */

#   define Z8536_INT_LVL		( 0x09 + ISA_INTERRUPT_BASE )

    /* z85230 ESCC interrupt level (shared with z8536) */

#   define Z85230_INT_LVL		( 0x09 + ISA_INTERRUPT_BASE )

    /* Universal Serial Bus interrupt level (not supported) */

#   define USB_INT_LVL			( 0x0b + ISA_INTERRUPT_BASE )

    /* mouse interrupt ( currently not supported ) */

#   define MOUSE_INT_LVL		( 0x0c + ISA_INTERRUPT_BASE )

#endif /* Config1 ISA int levels (e.g. MCP750 ) */


#ifdef CONFIG2_ISA_INT_TYPE  /* Config2 ISA int levels (e.g. MCPN750 ) */

    /* programable timer interrupt level */

#   define PIT_INT_LVL			( 0x00 + ISA_INTERRUPT_BASE )

    /* COM2 and COM4 UART interrupt level */

#   define COM2_INT_LVL 		( 0x03 + ISA_INTERRUPT_BASE )
#   define COM4_INT_LVL 		( 0x03 + ISA_INTERRUPT_BASE )

    /* COM1 and COM3 UART interrupt level */

#   define COM1_INT_LVL 		( 0x04 + ISA_INTERRUPT_BASE )
#   define COM3_INT_LVL 		( 0x04 + ISA_INTERRUPT_BASE )

    /* Abort button interrupt level (not supported) */

#   define ABORT_INT_LVL		( 0x08 + ISA_INTERRUPT_BASE )

    /* Universal Serial Bus interrupt level (not supported) */

#   define USB_INT_LVL			( 0x0b + ISA_INTERRUPT_BASE )

#endif /* Config2 ISA int levels (e.g. MCPN750 ) */

#   ifdef	INCLUDE_ATA

    /* IDE controller interrupt level */

#	define	IDE_CNTRLR0_INT_LVL	( 0x0e + ISA_INTERRUPT_BASE )
#	define	IDE_CNTRLR1_INT_LVL	( 0x0f + ISA_INTERRUPT_BASE )
#   endif	/* INCLUDE_ATA */

/* ISA interrupt base */

#define INT_VEC_IRQ0		0x00	/* vector for IRQ0 */

/* ISA interrupt vectors */

#ifdef CONFIG1_ISA_INT_TYPE  /* Config1 ISA int vectors (e.g. MCP750 ) */

#   define PIT_INT_VEC		( INT_VEC_IRQ0 + PIT_INT_LVL )
#   define COM2_INT_VEC 	( INT_VEC_IRQ0 + COM2_INT_LVL )
#   define COM1_INT_VEC 	( INT_VEC_IRQ0 + COM1_INT_LVL )
#   define FD_INT_VEC		( INT_VEC_IRQ0 + FD_INT_LVL )
#   define PP_INT_VEC		( INT_VEC_IRQ0 + PP_INT_LVL )
#   define Z8536_INT_VEC	( INT_VEC_IRQ0 + Z8536_INT_LVL )
#   define Z85230_INT_VEC	( INT_VEC_IRQ0 + Z85230_INT_LVL )
#   define USB_INT_VEC		( INT_VEC_IRQ0 + USB_INT_LVL )
#   define MOUSE_INT_VEC	( INT_VEC_IRQ0 + MOUSE_INT_LVL )
#   ifdef	INCLUDE_ATA
#	define IDE_CNTRLR0_INT_VEC	( INT_VEC_IRQ0 + IDE_CNTRLR0_INT_LVL )
#	define IDE_CNTRLR1_INT_VEC	( INT_VEC_IRQ0 + IDE_CNTRLR1_INT_LVL )
#   endif	/* INCLUDE_ATA */

#endif /* Config1 ISA int vectors (e.g. MCP750 ) */

#ifdef CONFIG2_ISA_INT_TYPE  /* Config2 ISA int vectors (e.g. MCPN750 ) */

#   define PIT_INT_VEC		( INT_VEC_IRQ0 + PIT_INT_LVL )
#   define COM2_INT_VEC 	( INT_VEC_IRQ0 + COM2_INT_LVL )
#   define COM4_INT_VEC 	( INT_VEC_IRQ0 + COM4_INT_LVL )
#   define COM1_INT_VEC 	( INT_VEC_IRQ0 + COM1_INT_LVL )
#   define COM3_INT_VEC 	( INT_VEC_IRQ0 + COM3_INT_LVL )
#   define ABORT_INT_VEC	( INT_VEC_IRQ0 + ABORT_INC_LVL )
#   define USB_INT_VEC		( INT_VEC_IRQ0 + USB_INT_LVL )
#   ifdef	INCLUDE_ATA
#	define IDE_CNTRLR0_INV_VEC	( INT_VEC_IRQ0 + IDE_CNTRLR0_INT_LVL )
#	define IDE_CNTRLR1_INT_VEC	( INT_VEC_IRQ0 + IDE_CNTRLR1_INT_LVL )
#   endif	/* INCLUDE_ATA */

#endif /* Config2 ISA int vectors */

/* Timer interrupt vectors */

#define TIMER0_INT_VEC		( INT_VEC_IRQ0 + TIMER0_INT_LVL )

/* MPIC interrupt levels */

#ifdef CONFIG1_MPIC_INT_TYPE  /* Config1 MPIC int levels (e.g. MCP750 ) */

    /* PIB (8259) interrupt connection level */

#   define PIB_INT_LVL		( 0x00 + EXT_INTERRUPT_BASE )

    /* Falcon-ECC error interrupt level */

#   define ECC_INT_LVL		( 0x01 + EXT_INTERRUPT_BASE )

    /* ethernet interrupt level */

#   define LN_INT_LVL		( 0x02 + EXT_INTERRUPT_BASE )

    /* PCI Mezzanine Card interrupt level */

#   define PMC_INT_LVL		( 0x03 + EXT_INTERRUPT_BASE )

    /* Watchdog Timer Level 1 interrupt level */

#   define WDT1_INT_LVL 	( 0x04 + EXT_INTERRUPT_BASE )

    /* CompactPCI PRST# signal level */

#   define CPCI_PRST_		( 0x05 + EXT_INTERRUPT_BASE )

    /* CompactPCI FAL# level */

#   define CPCI_FAL_		( 0x06 + EXT_INTERRUPT_BASE )

    /* CompactPCI DEG# level */

#   define CPCI_DEG_		( 0x07 + EXT_INTERRUPT_BASE )

    /* CompactPCI Bus INTA# level */

#   define CPCIA_INT_LVL	( 0x08 + EXT_INTERRUPT_BASE )

    /* CompactPCI Bus INTB# level */

#   define CPCIB_INT_LVL	( 0x09 + EXT_INTERRUPT_BASE )

    /* CompactPCI Bus INTC# level */

#   define CPCIC_INT_LVL	( 0x0a + EXT_INTERRUPT_BASE )

    /* CompactPCI Bus INTD# level */

#   define CPCID_INT_LVL	( 0x0b + EXT_INTERRUPT_BASE )

#   define MAX_MPIC_INT_LVL	(EXT_INTERRUPT_BASE + 0xb)

#endif /* Config1 MPIC int levels (e.g. MCP750 ) */

#ifdef CONFIG2_MPIC_INT_TYPE  /* Config2 MPIC int levels (e.g. MCPN750 ) */

    /* PIB (8259) interrupt connection */

#   define PIB_INT_LVL		( 0x00 + EXT_INTERRUPT_BASE )

    /* Falcon-ECC error interrupt */

#   define ECC_INT_LVL		( 0x01 + EXT_INTERRUPT_BASE )

    /* ethernet interrupt level */

#   define LN_INT_LVL		( 0x02 + EXT_INTERRUPT_BASE )

    /* Watchdog Timer Level 1 interrupt level */

#   define WDTL1_INT_LVL	( 0x03 + EXT_INTERRUPT_BASE )

    /* 21554 Secondary PCI Bus interrupt level */

#   define PCI2_INT_LVL 	( 0x04 + EXT_INTERRUPT_BASE )

    /* CompactPCI Bus INTA# */

#   define CPCIA_INT_LVL	( 0x05 + EXT_INTERRUPT_BASE )

    /* CompactPCI Bus INTB# */

#   define CPCIB_INT_LVL	( 0x06 + EXT_INTERRUPT_BASE )

    /* CompactPCI Bus INTC# */

#   define CPCIC_INT_LVL	( 0x07 + EXT_INTERRUPT_BASE )

    /* CompactPCI Bus INTD# */

#   define CPCID_INT_LVL	( 0x08 + EXT_INTERRUPT_BASE )

    /* PMC1 INTA#, PMC2 INTB# */

#   define PMC1A_INT_LVL	( 0x09 + EXT_INTERRUPT_BASE )
#   define PMC2B_INT_LVL	( 0x09 + EXT_INTERRUPT_BASE )

    /* PMC1 INTB#, PMC2 INTC# */

#   define PMC1B_INT_LVL	( 0x0a + EXT_INTERRUPT_BASE )
#   define PMC2C_INT_LVL	( 0x0a + EXT_INTERRUPT_BASE )

    /* PMC1 INTC#, PMC2 INTD# */

#   define PMC1C_INT_LVL	( 0x0b + EXT_INTERRUPT_BASE )
#   define PMC2D_INT_LVL	( 0x0b + EXT_INTERRUPT_BASE )

    /* PMC1 INTD#, PMC2 INTA# */

#   define PMC1D_INT_LVL	( 0x0c + EXT_INTERRUPT_BASE )
#   define PMC2A_INT_LVL	( 0x0c + EXT_INTERRUPT_BASE )

#   define MAX_MPIC_INT_LVL	(EXT_INTERRUPT_BASE + 0xc)

#endif /* Config2 MPIC int levels (e.g. MCPN750 ) */

/* Timer Interrupt (IPI0) */

#define TIMER0_INT_LVL		  	( 0x00 + TIMER_INTERRUPT_BASE )

#define DEC2155X_DOORBELL0_INT_LVL	( 0x00 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL1_INT_LVL	( 0x01 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL2_INT_LVL	( 0x02 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL3_INT_LVL	( 0x03 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL4_INT_LVL	( 0x04 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL5_INT_LVL	( 0x05 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL6_INT_LVL	( 0x06 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL7_INT_LVL	( 0x07 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL8_INT_LVL	( 0x08 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL9_INT_LVL	( 0x09 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL10_INT_LVL 	( 0x0a + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL11_INT_LVL 	( 0x0b + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL12_INT_LVL 	( 0x0c + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL13_INT_LVL 	( 0x0d + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL14_INT_LVL 	( 0x0e + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_DOORBELL15_INT_LVL 	( 0x0f + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_PWR_MGMT_INT_LVL	( 0x10 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_I2O_INT_LVL		( 0x11 + DEC2155X_INTERRUPT_BASE )
#define DEC2155X_PG_CRSSNG_INT_LVL	( 0x12 + DEC2155X_INTERRUPT_BASE )

/* MPIC interrupt vectors */

#ifdef CONFIG1_MPIC_INT_TYPE  /* Config1 MPIC int vectors (e.g. MCP750 ) */

#   define PIB_INT_VEC			( INT_VEC_IRQ0 + PIB_INT_LVL )
#   define ECC_INT_VEC			( INT_VEC_IRQ0 + ECC_INT_LVL )
#   define LN_INT_VEC			( INT_VEC_IRQ0 + LN_INT_LVL )
#   define PMC_INT_VEC			( INT_VEC_IRQ0 + PMC_INT_LVL )
#   define WDT1_INT_VEC 		( INT_VEC_IRQ0 + WDT1_INT_LVL )
#   define CPCI_PRST_VEC		( INT_VEC_IRQ0 + CPCI_PRST_LVL )
#   define CPCI_FAL_VEC 		( INT_VEC_IRQ0 + CPCI_FAL_LVL )
#   define CPCI_DEG_VEC 		( INT_VEC_IRQ0 + CPCI_DEG_LVL )
#   define CPCIA_INT_VEC		( INT_VEC_IRQ0 + CPCIA_INT_LVL )
#   define CPCIB_INT_VEC		( INT_VEC_IRQ0 + CPCIB_INT_LVL )
#   define CPCIC_INT_VEC		( INT_VEC_IRQ0 + CPCIC_INT_LVL )
#   define CPCID_INT_VEC		( INT_VEC_IRQ0 + CPCID_INT_LVL )

#endif /* Config1 MPIC int vectors (e.g. MCP750 ) */

#ifdef CONFIG2_MPIC_INT_TYPE  /* Config2 MPIC int vectors (e.g. MCPN750 ) */

#   define PIB_INT_VEC			( INT_VEC_IRQ0 + PIB_INT_LVL )
#   define ECC_INT_VEC			( INT_VEC_IRQ0 + ECC_INT_LVL )
#   define LN_INT_VEC			( INT_VEC_IRQ0 + LN_INT_LVL )
#   define WDT1_INT_VEC 		( INT_VEC_IRQ0 + WDT1_INT_LVL )
#   define PCI2_INT_VEC 		( INT_VEC_IRQ0 + PCI2_INT_LVL )
#   define CPCIA_INT_VEC		( INT_VEC_IRQ0 + CPCIA_INT_LVL )
#   define CPCIB_INT_VEC		( INT_VEC_IRQ0 + CPCIB_INT_LVL )
#   define CPCIC_INT_VEC		( INT_VEC_IRQ0 + CPCIC_INT_LVL )
#   define CPCID_INT_VEC		( INT_VEC_IRQ0 + CPCID_INT_LVL )
#   define PMC1A_INT_VEC		( INT_VEC_IRQ0 + PMC1A_INT_LVL )
#   define PMC2B_INT_VEC		( INT_VEC_IRQ0 + PMC2B_INT_LVL )
#   define PMC1B_INT_VEC		( INT_VEC_IRQ0 + PMC1B_INT_LVL )
#   define PMC2C_INT_VEC		( INT_VEC_IRQ0 + PMC2C_INT_LVL )
#   define PMC1C_INT_VEC		( INT_VEC_IRQ0 + PMC1C_INT_LVL )
#   define PMC2D_INT_VEC		( INT_VEC_IRQ0 + PMC2D_INT_LVL )
#   define PMC1C_INT_VEC		( INT_VEC_IRQ0 + PMC1C_INT_LVL )
#   define PMC2A_INT_VEC		( INT_VEC_IRQ0 + PMC2A_INT_LVL )
#endif /* Config2 MPIC int vectors (e.g. MCPN750 ) */

#define DEC2155X_DOORBELL0_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL0_INT_LVL)
#define DEC2155X_DOORBELL1_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL1_INT_LVL)
#define DEC2155X_DOORBELL2_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL2_INT_LVL)
#define DEC2155X_DOORBELL3_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL3_INT_LVL)
#define DEC2155X_DOORBELL4_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL4_INT_LVL)
#define DEC2155X_DOORBELL5_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL5_INT_LVL)
#define DEC2155X_DOORBELL6_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL6_INT_LVL)
#define DEC2155X_DOORBELL7_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL7_INT_LVL)
#define DEC2155X_DOORBELL8_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL8_INT_LVL)
#define DEC2155X_DOORBELL9_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_DOORBELL9_INT_LVL)
#define DEC2155X_DOORBELL10_INT_VEC (INT_VEC_IRQ0 + DEC2155X_DOORBELL10_INT_LVL)
#define DEC2155X_DOORBELL11_INT_VEC (INT_VEC_IRQ0 + DEC2155X_DOORBELL11_INT_LVL)
#define DEC2155X_DOORBELL12_INT_VEC (INT_VEC_IRQ0 + DEC2155X_DOORBELL12_INT_LVL)
#define DEC2155X_DOORBELL13_INT_VEC (INT_VEC_IRQ0 + DEC2155X_DOORBELL13_INT_LVL)
#define DEC2155X_DOORBELL14_INT_VEC (INT_VEC_IRQ0 + DEC2155X_DOORBELL14_INT_LVL)
#define DEC2155X_DOORBELL15_INT_VEC (INT_VEC_IRQ0 + DEC2155X_DOORBELL15_INT_LVL)
#define DEC2155X_PWR_MGMT_INT_VEC   (INT_VEC_IRQ0 + DEC2155X_PWR_MGMT_INT_LVL)
#define DEC2155X_I2O_INT_VEC        (INT_VEC_IRQ0 + DEC2155X_I2O_INT_LVL)
#define DEC2155X_PG_CRSSNG_INT_VEC  (INT_VEC_IRQ0 + DEC2155X_PG_CRSSNG_INT_LVL)

/*
 * Address range definitions for PCI bus.
 *
 * Used with vxMemProbe() hook sysBusProbe().
 */

#define IS_PCI_ADDRESS(adrs) (((UINT32)(adrs) >= (UINT32)PCI_MSTR_LO_ADRS) && \
((UINT32)(adrs) < (UINT32)PCI_MSTR_HI_ADRS))

#define SYS_REG_SIOP_HW_REGS	{0,0,0,0,0,1,0,0,0,0,0}

#ifdef SYS_SM_ANCHOR_POLL_LIST
#   ifndef _ASMLANGUAGE
        /*
         * Shared memory anchor polling list
         */

        typedef struct _SYS_SM_ANCHOR_POLLING_LIST
            {
            UINT devVend;
            UINT subIdVend;
            } SYS_SM_ANCHOR_POLLING_LIST;
#   endif
#endif

#ifndef _ASMLANGUAGE
    /*
     * Shared memory device list
     */

    typedef struct _SYS_SM_DEV_LIST
        {
        UINT devVend;
        UINT subIdVend;
        } SYS_SM_DEV_LIST;
#endif

/*
 * Support for determining if we're ROM based or not.  _sysInit
 * saves the startType parameter at location ROM_BASED_FLAG.
 */

#define PCI_AUTOCONFIG_FLAG_OFFSET ( 0x4c00 )
#define PCI_AUTOCONFIG_FLAG ( *(UCHAR *)(LOCAL_MEM_LOCAL_ADRS + \
				     PCI_AUTOCONFIG_FLAG_OFFSET) )
#define PCI_AUTOCONFIG_DONE ( PCI_AUTOCONFIG_FLAG != 0 )

#ifdef __cplusplus
}
#endif

#endif /* __INCmcpx750h */
