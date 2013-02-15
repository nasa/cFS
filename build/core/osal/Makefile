##
##
## OSAL Core Subsystem Makefile
##
##  

##
## Identify the Core Subsystem here. For almost all cases, this subsystem
## variable is the only thing that has to change in the makefile.
##
SUBSYS = osal

##
## Specify extra C Flags needed to build this subsystem
##
LOCAL_COPTS = 

## 
## Include all necessary make rules
## Any of these can be copied to a local file and 
## changed if needed.
##
##
##       cfe-config.mak contians arch, BSP, and OS selection
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
##       SUBSYS.mak file contains the list of objects to compile
##
include $(OSAL_SRC)/os/$(OS)/$(SUBSYS).mak

##
## Setup the include path for this subsystem
## The OS specific includes are in the compiler-opts.mak file
##
## The static-loader path is for the GSFC static loader used by
## the RTEMS port. It can be ignored or removed.
##
INCLUDE_PATH = \
-I$(OSAL_SRC)/inc \
-I$(OSAL_SRC)/os/inc \
-I$(OSAL_SRC)/os/$(OS) \
-I../../inc \
-I/opt/static-loader/inc

##
## Define the VPATH make variable. 
## This can be modified to include source from another directory.
##
VPATH = $(OSAL_SRC)/os/$(OS)

##
## Include the common make rules for the Core subsystems
##
include $(OSAL_SRC)/make/make-rules.mak
