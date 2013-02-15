###############################################################################
# File: osal.mak
#
# Purpose:
#   Compile the OS Abstraction layer library.
#
# History:
#
###############################################################################

# Subsystem produced by this makefile.
TARGET = osal.o

#==============================================================================
# Object files required to build subsystem.

OBJS=osapi.o osfileapi.o osfilesys.o osnetwork.o osloader.o ostimer.o

#==============================================================================
# Source files required to build subsystem; used to generate dependencies.

SOURCES = $(OBJS:.o=.c)

