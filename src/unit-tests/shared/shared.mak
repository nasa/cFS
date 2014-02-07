###############################################################################
# File: osal.mak
#
# Purpose:
#   Compile the OS Abstraction layer library.
#
# History:
#   2004/04/12  A. Cudmore   : Initial revision for SDO.
#   2004/05/24  P. Kutt      : Modified for new directory structure; rewrote comments.
#
###############################################################################

# Subsystem produced by this makefile.
TARGET = shared.o

#==============================================================================
# Object files required to build subsystem.

# OBJS=ut_oscore_stubs.o ut_osfile_stubs.o ut_osfilesys_stubs.o ut_osloader_stubs.o ut_osnetwork_stubs.o ut_os_stubs.o ut_ostimer_stubs.o
OBJS=ut_os_stubs.o 
#==============================================================================
# Source files required to build subsystem; used to generate dependencies.

SOURCES = $(OBJS:.o=.c)

