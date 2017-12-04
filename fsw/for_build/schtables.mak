###############################################################################
# File: CFS Application Table Makefile 
#
#
# History:
#
###############################################################################
#
# The Application needs to be specified here
#
APPTARGET = sch

#
# List the tables that are generated here.
# Restrictions:
# 1. The table file name must be the same as the C source file name
# 2. There must be a single C source file for each table
#
TABLES = sch_def_msgtbl.tbl sch_def_schtbl.tbl

##################################################################################
# Normally, nothing has to be changed below this line
# The following are changes that may have to be made for a custom app environment:
# 1. INCLUDE_PATH - This may be customized to tailor the include path for an app
# 2. VPATH - This may be customized to tailor the location of the table sources.
#            For example: if the tables were stored in a "tables" subdirectory
#                        ( build/cpu1/sch/tables )
#################################################################################

#
# Object files required for tables
#
OBJS = $(TABLES:.tbl=.o)

#
# Source files required to build tables.
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
EXEDIR=../exe

########################################################################
# Should not have to change below this line, except for customized 
# Mission and cFE directory structures
########################################################################

#
# Set build type to CFE_APP. This allows us to 
# define different compiler flags for the cFE Core and Apps.
# 
BUILD_TYPE = CFE_TABLE

## 
## Include all necessary cFE make rules
## Any of these can be copied to a local file and 
## changed if needed.
##
##
##       cfe-config.mak contians arch, BSP, and OS selection
##
include ../cfe/cfe-config.mak

##
##       debug-opts.mak contains debug switches -- Note that the table must be
##       built with -g for the elf2tbl utility to work.
##
include ../cfe/debug-opts.mak

##
##       compiler-opts.mak contains compiler definitions and switches/defines
##
include $(CFE_PSP_SRC)/$(PSP)/make/compiler-opts.mak

##
## Setup the include path for this subsystem
## The OS specific includes are in the build-rules.make file
##
## If this subsystem needs include files from another app, add the path here.
##
INCLUDE_PATH = \
-I$(OSAL_SRC)/inc \
-I$(CFE_CORE_SRC)/inc \
-I$(CFE_PSP_SRC)/$(PSP)/inc \
-I$(CFE_PSP_SRC)/inc \
-I$(CFS_APP_SRC)/inc \
-I$(CFS_APP_SRC)/$(APPTARGET)/fsw/src \
-I$(CFS_MISSION_INC) \
-I../cfe/inc \
-I../inc

##
## Define the VPATH make variable. 
## This can be modified to include source from another directory.
## If there is no corresponding app in the cfe-apps directory, then this can be discarded, or
## if the mission chooses to put the src in another directory such as "src", then that can be 
## added here as well.
##
VPATH = $(CFS_APP_SRC)/$(APPTARGET)/fsw/tables 

##
## Include the common make rules for building a cFE Application
##
include $(CFE_CORE_SRC)/make/table-rules.mak
