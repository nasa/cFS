#######################################################################################
#
# File:    Linux Makefile for Unit Testing of comms Application
# Author:  Paula Bosca
# Date:    2020-06-24
#
#######################################################################################

#
# Compiler/linker definitions
#

#
# Search path definitions to modify
#
export CFS_MISSION=$(YOUR_PROJ_HOME)
CFS_APP_SRC=$(CFS_MISSION)/apps/
APPTARGET=ut_COMMS

# Set build type to CFE_APP. This allows us to 
# define different compiler flags for the cFE Core and Apps.
# 
BUILD_TYPE = CFE_APP

# 
# Include all necessary cFE make rules
# Any of these can be copied to a local file and changed if needed.
#
#     cfe-config.mak contains PSP and OS selection
#
include $(CFS_MISSION)/build/linux/cfe/cfe-config.mak

#
# Setup the source path for this build
#
VPATH := .

#
# Setup the include path for this build
# The OS specific includes are in the build-rules.make file
#
# If this build needs include files from another app, add the path here.
#
INC_PATH := -I. \
            -I../platform_inc \
            -I$(CFS_MISSION)/osal/src/os/inc \
            -I$(CFS_MISSION)/osal/build/inc  \
            -I$(CFS_MISSION)/psp/fsw/inc \
            -I$(CFS_MISSION)/psp/fsw/$(PSP)/inc \
            -I$(CFS_MISSION)/cfe/fsw/cfe-core/src/inc 

#
# The default "make" target 
# 
all:: ut_comms.bin

clean::
	-rm -f *.o
	-rm -f *.bin

ut_comms.bin: ut_comms.c
	gcc $(LOCAL_COPTS) $(INC_PATH) $(COPTS) $(DEBUG_OPTS) \
            -DOS_DEBUG_LEVEL=$(DEBUG_LEVEL) -m32 $^ \
            -o ut_comms.bin

#######################################################################################
    