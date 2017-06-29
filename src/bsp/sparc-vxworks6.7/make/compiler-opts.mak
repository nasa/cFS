##############################################################################
## compiler-opts.mak - compiler definitions and options for building the cFE 
##
## Target: SPARC V8 GR-UT699 vxWorks 6.x
##
## Modifications:
##
###############################################################################
## 
## Warning Level Configuration
##
## WARNINGS=-Wall -ansi -pedantic -Wstrict-prototypes
WARNINGS    = -Wall  -std=c99

##
## A fix for Windows systems on vxWorks 6.4
## When generating dependancies, the Windows GCC cannot seem to deal
## with the Windows style path separators in the WIND_BASE macro.
##
FIXED_WIND_BASE = $(subst \,/,$(WIND_BASE))


##
## vxWorks system includes -- including Gaisler drivers/libraries
##
VXINCDIR = $(FIXED_WIND_BASE)/target/h \
$(FIXED_WIND_BASE)/target/h/wrn/coreip \
$(FIXED_WIND_BASE)/target/h/drv \
$(FIXED_WIND_BASE)/target/h/drv/os_ext \
$(FIXED_WIND_BASE)/target/src/config \
$(FIXED_WIND_BASE)/target/src/drv \
$(FIXED_WIND_BASE)/target/config/comps/src \
$(FIXED_WIND_BASE)/target/config/comps/src/dosfs2 \

SYSINCS = $(VXINCDIR:%=-I%)

##
## Target Defines for the OS, Hardware Arch, etc..
##
TARGET_DEFS += -D_VXWORKS_OS_ -D__SPARC__ $(CFE_SB_NET) -D$(OS) -D_EMBED_ \
         -DTOOL_FAMILY=gnu -DTOOL=gnuv8 -D_WRS_KERNEL -DCPU=SPARC \
         "-D_VSB_CONFIG_FILE=<../lib/h/config/vsbConfig.h>"

# IF SMP Mode, Append SMP Compiler Directives (Kernel mode is not binary compatible, user-mode is)
ifeq (${OS_MODE},SMP)
    TARGET_DEFS := $(TARGET_DEFS) -DVXBUILD=SMP -D_WRS_VX_SMP -D_WRS_CONFIG_SMP
endif
## 
## Endian Defines
##
ENDIAN_DEFS=-D_EB -DENDIAN=_EB -DSOFTWARE_BIG_BIT_ORDER 

##
## Compiler Architecture Switches
## 
##ARCH_OPTS = -mv8 -mfpu -m32 -fno-builtin -fvolatile -mfaster-structs -Wcast-align
#ARCH_OPTS = -m32 -fno-builtin -fvolatile -Wcast-align -mtune=ut699
ARCH_OPTS = -fno-builtin -fvolatile -Wcast-align -m32 -mtune=ut699 -mv8 -ffloat-int-mode

##
## Application specific compiler switches 
##
ifeq ($(BUILD_TYPE),CFE_APP)
# cc1: error: unrecognized command line option "-mlongcall" by sparc gcc
##   APP_COPTS = -mlongcall  
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
APP_EXT = o

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
COMPILER   = sparc-wrs-vxworks-gcc
ASSEMBLER  = sparc-wrs-vxworks-gcc
LINKER     = sparc-wrs-vxworks-ld
AR         = sparc-wrs-vxworks-ar
NM         = sparc-wrs-vxworks-nm
OBJCPY     = sparc-wrs-vxworks-objcopy
TABLE_BIN  = elf2cfetbl
