###############################################################################
# File: psp.mak
#
# Purpose:
#   Platform Support Package routines for the Linux desktop build
#
# History:
#   2005/07/27  A. Cudmore   : Updated for cFE.
#   2004/04/12  A. Cudmore   : Initial revision for SDO.
#   2004/05/24  P. Kutt      : Modified for new directory structure; rewrote comments.
#
###############################################################################

# Subsystem produced by this makefile.
TARGET = psp.o

#==============================================================================
# Object files required to build subsystem.
#==============================================================================
OBJS = cfe_psp_start.o \
cfe_psp_support.o \
cfe_psp_ssr.o \
cfe_psp_voltab.o \
cfe_psp_memtab.o \
cfe_psp_timer.o \
cfe_psp_watchdog.o \
cfe_psp_memory.o \
cfe_psp_exception.o \
cfe_psp_ram.o \
cfe_psp_eeprom.o \
cfe_psp_port.o \
cfe_psp_memrange.o \
cfe_psp_memutils.o

#==============================================================================
# Source files required to build subsystem; used to generate dependencies.
SOURCES = cfe_psp_start.c \
cfe_psp_support.c \
cfe_psp_ssr.c  \
cfe_psp_voltab.c \
cfe_psp_memtab.c \
cfe_psp_timer.c \
cfe_psp_watchdog.c \
cfe_psp_memory.c \
cfe_psp_exception.c \
cfe_psp_ram.c \
cfe_psp_eeprom.c \
cfe_psp_port.c \
cfe_psp_memrange.c \
cfe_psp_memutils.c


