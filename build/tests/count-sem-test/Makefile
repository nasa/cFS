###############################################################################
# File: OSAL Application Makefile 
#
#
# History:
#
###############################################################################
#
# Subsystem produced by this makefile.
#
APPTARGET = count-sem-test

#
# Object files required to build subsystem.
#
OBJS = count-sem-test.o

#
# Core object files
# 
OBJS += osapi.o osfileapi.o osfilesys.o osloader.o osnetwork.o ostimer.o

#
# BSP Object files
#
OBJS += bsp_start.o bsp_voltab.o
#

#
# Source files required to build subsystem; used to generate dependencies.
# As long as there are no assembly files this can be automated.
#
SOURCES = $(OBJS:.o=.c)


##
## Specify extra C Flags needed to build this subsystem
##
LOCAL_COPTS = 


##
## EXEDIR is defined here, just in case it needs to be different for a custom
## build
##
EXEDIR=./

########################################################################
# Should not have to change below this line, except for customized 
# directory structures
########################################################################

CORE_OBJS = 

## 
## Include all necessary make rules
## Any of these can be copied to a local file and 
## changed if needed.
##
##
##       osal-config.mak contians arch, BSP, and OS selection
##
include ../../osal-config.mak
##
##       debug-opts.mak contains debug switches
##
include ../../debug-opts.mak
##
##       compiler-opts.mak contains compiler definitions and switches/defines
##
include $(OSAL_SRC)/bsp/$(BSP)/make/compiler-opts.mak

##
## Setup the include path for this subsystem
## The OS specific includes are in the build-rules.make file
##
## If this subsystem needs include files from another app, add the path here.
##
INCLUDE_PATH = \
-I$(OSAL_SRC)/inc \
-I$(OSAL_SRC)/os/inc \
-I$(OSAL_SRC)/tests/$(APPTARGET) \
-I../../inc

##
## Define the VPATH make variable. 
## This can be modified to include source from another directory.
## If there is no corresponding app in the apps directory, then this can be discarded, or
## if the mission chooses to put the src in another directory such as "src", then that can be 
## added here as well.
##
VPATH = $(OSAL_SRC)/tests/$(APPTARGET) 
VPATH += $(OSAL_SRC)/os/$(OS)
VPATH += $(OSAL_SRC)/bsp/$(BSP)/src

##
## Include the common make rules for building an OSAL Application
##
include $(OSAL_SRC)/make/app-rules.mak
