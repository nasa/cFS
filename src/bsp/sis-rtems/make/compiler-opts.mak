###############################################################################
## compiler-opts.mak - compiler definitions and options for building the OSAL
##
## Target: m5235BCC Coldfire board with RTEMS and CEXP dynamic loader
##
## Modifications:
##
###############################################################################
## 
## Warning Level Configuration
##
## WARNINGS=-Wall -ansi -pedantic -Wstrict-prototypes
WARNINGS	= -Wall 

SYSINCS = 

##
## Target Defines for the OS, Hardware Arch, etc..
##
TARGET_DEFS = -D_RTEMS_OS_ -D__SPARC__ -D$(OS) -DSPARC_SIS -D_EMBED_

## 
## Endian Defines
##
ENDIAN_DEFS=-D_EB -DENDIAN=_EB -DSOFTWARE_BIG_BIT_ORDER 

##
## Compiler Architecture Switches ( double check arch switch -m52xx, m523x etc.. )
## 
ARCH_OPTS = --pipe -mcpu=cypress  -B$(RTEMS_BSP_BASE)/sparc-rtems4.10/sis/lib/ -specs bsp_specs -qrtems

##
## Application specific compiler switches 
##
ifeq ($(BUILD_TYPE),CFE_APP)
   APP_COPTS =   
   APP_ASOPTS   =
else
   APP_COPTS =  
   APP_ASOPTS   =
endif

##
## Extra Cflags for Assembly listings, etc.
##
LIST_OPTS    = -Wa,-a=$*.lis 

##
## gcc options for dependancy generation
## 
COPTS_D = $(APP_COPTS) $(ENDIAN_DEFS) $(TARGET_DEFS) $(ARCH_OPTS) $(SYSINCS) $(WARNINGS)

## 
## General gcc options that apply to compiling and dependency generation.
##
COPTS=$(LIST_OPTS) $(COPTS_D)

##
## Extra defines and switches for assembly code
##
ASOPTS = $(APP_ASOPTS) -P -xassembler-with-cpp 

##---------------------------------------------------------
## Application file extention type
## This is the defined application extention.
## Known extentions: Mac OS X: .bundle, Linux: .so, RTEMS:
##   .s3r, vxWorks: .o etc.. 
##---------------------------------------------------------
APP_EXT = nxe

####################################################
## Host Development System and Toolchain defintions
##
## Host OS utils
##
RM=rm -f
CP=cp

##
## Compiler tools
##
COMPILER   = sparc-rtems4.10-gcc
ASSEMBLER  = sparc-rtems4.10-gcc
LINKER	   = sparc-rtems4.10-ld
AR	   = sparc-rtems4.10-ar
NM         = sparc-rtems4.10-nm
OBJCPY     = sparc-rtems4.10-objcopy
