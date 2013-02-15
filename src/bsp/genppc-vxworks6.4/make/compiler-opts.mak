###############################################################################
## compiler-opts.mak - compiler definitions and options for building the OSAL and apps
##
## Target: PPC COTS boards with vxWorks 6.x
##
## Modifications:
##
###############################################################################
## 
## Warning Level Configuration
##
## WARNINGS=-Wall -ansi -pedantic -Wstrict-prototypes
WARNINGS	= -Wall  -ansi

##
## A fix for Windows systems on vxWorks 6.4
## When generating dependancies, the Windows GCC cannot seem to deal
## with the Windows style path separators in the WIND_BASE macro.
##
FIXED_WIND_BASE = $(subst \,/,$(WIND_BASE))

##
## vxWorks system includes 
##
VXINCDIR = $(FIXED_WIND_BASE)/target/h \
$(FIXED_WIND_BASE)/target/h/wrn/coreip \
$(FIXED_WIND_BASE)/target/h/drv \
$(FIXED_WIND_BASE)/target/src/config \
$(FIXED_WIND_BASE)/target/src/drv \
$(FIXED_WIND_BASE)/target/config/comps/src \
$(FIXED_WIND_BASE)/target/config/comps/src/dosfs2 \

SYSINCS = $(VXINCDIR:%=-I%)

##
## Target Defines for the OS, Hardware Arch, etc..
##
TARGET_DEFS = -D_VXWORKS_OS_ -D_PPC_ -D__PPC__  $(CFE_SB_NET) -D$(OS) -DGENPPC -D_EMBED_ \
	     -DTOOL_FAMILY=gnu -DTOOL=gnu -D_WRS_KERNEL -DCPU=PPC604

## 
## Endian Defines
##
ENDIAN_DEFS=-D_EB -DENDIAN=_EB -DSOFTWARE_BIG_BIT_ORDER 

##
## Compiler Architecture Switches
## removed -fvolatile - not needed for vxworks 6.x
## 
ARCH_OPTS = -mcpu=750 -mstrict-align -fno-builtin -mlongcall 


##
## Extra Cflags for Assembly listings, etc.
##
LIST_OPTS    = -Wa,-a=$*.lis 

##
## gcc options for dependancy generation
## 
COPTS_D = $(ENDIAN_DEFS) $(TARGET_DEFS) $(ARCH_OPTS) $(SYSINCS) $(WARNINGS)

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
APP_EXT = elf

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
COMPILER   = ccppc
ASSEMBLER  = ccppc
LINKER	  = ldppc
AR	        = arppc
NM         = nmppc
OBJCPY     = objcopyppc
