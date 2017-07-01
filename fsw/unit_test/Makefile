##############################################################################
## File:
##   $Id: Makefile 1.4 2012/05/24 11:06:26EDT jmdagost Exp  $
##
## Purpose:
##   Makefile for building the CFS Scheduler (SCH) UTF unit tests
##
##   $Log: Makefile  $
##   Revision 1.4 2012/05/24 11:06:26EDT jmdagost 
##   Modified makefile to reference environment variables
##   Revision 1.3 2012/01/04 16:14:18EST jmdagost 
##   Replaced gcov rule in SCH unit test makefile.  Let previous rule as comment for reference.
##   Revision 1.2 2011/06/30 21:03:40EDT aschoeni 
##   Updated unit tests for new timers and customization interfaces
##   Revision 1.1 2008/10/16 15:10:14EDT dkobe 
##   Initial revision
##   Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/unit_test/project.pj
##
##############################################################################
#
# Compiler/linker definitions
#
COMPILER=gcc
LINKER=gcc
#
# Compiler/linker options
#
DEFAULT_COPT= -m32 -fPIC -Wall -Wstrict-prototypes -Wunreachable-code -ansi -g 
COPT= -D_ix86_ -DSOFTWARE_LITTLE_BIT_ORDER -D_EL
LOPT= -m32
#
# Options for gcov
#
GCOV_COPT= -fprofile-arcs -ftest-coverage
GCOV_LOPT= -pg -fprofile-arcs -ftest-coverage
#
###########################################################################
#
# Search path definitions
#
#
###########################################################################
#
# Source file path definitions
#
VPATH := .
VPATH += ..
VPATH += $(CFS_APP_SRC)/sch/fsw/src
VPATH += $(CFS_APP_SRC)/sch/fsw/tables
VPATH += $(CFS_APP_SRC)/cfs_lib/fsw/src
VPATH += $(UTF_SRC)/src
VPATH += $(CFE_CORE_SRC)/sb
VPATH += $(CFE_CORE_SRC)/time
VPATH += $(CFE_CORE_SRC)/es
VPATH += $(CFE_CORE_SRC)/evs
VPATH += $(CFE_CORE_SRC)/fs
VPATH += $(CFE_CORE_SRC)/tbl
VPATH += $(OSAL_SRC)/posix
VPATH += $(OSAL_SRC)/../bsp/pc-linux/src
VPATH += $(CFE_PSP_SRC)/pc-linux/src
VPATH += $(CFS_APP_SRC)/hs/fsw/src
#
###########################################################################
#
# Header file path definitions
#
INCLUDES := -I.
INCLUDES += -I..
INCLUDES += -I$(CFS_APP_SRC)/sch/fsw/src
INCLUDES += -I$(CFS_APP_SRC)/sch/fsw/platform_inc
INCLUDES += -I$(CFS_APP_SRC)/sch/fsw/mission_inc
INCLUDES += -I$(CFS_APP_SRC)/sch/fsw/public_inc
INCLUDES += -I$(OSAL_SRC)/../../build/inc
INCLUDES += -I$(OSAL_SRC)/inc
INCLUDES += -I$(CFE_PSP_SRC)/inc
INCLUDES += -I$(CFE_PSP_SRC)/pc-linux/inc
INCLUDES += -I$(UTF_SRC)/inc
INCLUDES += -I$(CFS_APP_SRC)/cfs_lib/fsw/public_inc
INCLUDES += -I$(CFE_CORE_SRC)/../../mission_inc
INCLUDES += -I$(CFE_CORE_SRC)/../../platform_inc/cpu1
INCLUDES += -I$(CFE_CORE_SRC)/inc
INCLUDES += -I$(CFE_CORE_SRC)/time
INCLUDES += -I$(CFE_CORE_SRC)/sb
INCLUDES += -I$(CFE_CORE_SRC)/es
INCLUDES += -I$(CFE_CORE_SRC)/evs
INCLUDES += -I$(CFE_CORE_SRC)/fs
INCLUDES += -I$(CFE_CORE_SRC)/tbl
#
###########################################################################
#
# UTF object files
#
UTF_OBJS := utf_osapi.o
UTF_OBJS += utf_cfe_psp_memory.o
UTF_OBJS += utf_cfe_psp_memrange.o
UTF_OBJS += utf_cfe_psp_memutils.o
UTF_OBJS += utf_cfe_psp_eeprom.o
UTF_OBJS += utf_cfe_psp_ram.o
UTF_OBJS += utf_cfe_psp_support.o
UTF_OBJS += utf_cfe_psp_port.o
#UTF_OBJS += utf_bsp_support.o
UTF_OBJS += utf_osfilesys.o
UTF_OBJS += utf_osfileapi.o
UTF_OBJS += utf_ostimer.o
UTF_OBJS += utf_cfe_sb_api.o
UTF_OBJS += utf_cfe_time_api.o
UTF_OBJS += utf_cfe_time_utils.o
UTF_OBJS += utf_cfe_sb.o
UTF_OBJS += utf_sim.o
UTF_OBJS += utf_cfe_es_api.o
UTF_OBJS += utf_cfe_es_cds.o
UTF_OBJS += utf_cfe_esmempool.o
UTF_OBJS += utf_cfe_evs.o
UTF_OBJS += utf_cfe_tbl_api.o
UTF_OBJS += utf_cfe_tbl_internal.o
#
###########################################################################
#
# cFE object files
#
CFE_OBJS := ccsds.o
CFE_OBJS += cfe_sb_util.o
CFE_OBJS += cfe_es_cds_mempool.o
CFE_OBJS += cfe_fs_api.o
CFE_OBJS += cfe_psp_timer.o
#CFE_OBJS += cfe_time_api.o
#
###########################################################################
#
# SCH object files
#
SCH_OBJS := sch_app.o
SCH_OBJS += sch_cmds.o
SCH_OBJS += sch_custom.o
SCH_OBJS += sch_api.o
#SCH_OBJS += osal_stubs.o
#
###########################################################################
#
# Rules to make the specified targets
#
%.o: %.c
	$(COMPILER) -c $(COPT) $(DEFAULT_COPT) $(GCOV_COPT) $(INCLUDES) $<
#
%.exe: %.o
	$(LINKER) $(LOPT) $(GCOV_LOPT) $^ -o $*.exe 
#
all: utf_test_sch.exe 
#
utf_test_sch.exe: utf_test_sch.o $(UTF_OBJS) $(CFE_OBJS) $(SCH_OBJS)
#
clean ::
	rm -f *.o *.exe *.da *.out *.bbg *.bb *.gcda *.gcno *.gcov
##
## gcov rule
##
gcov ::
	@echo
	@gcov $(SCH_OBJS:.o=.gcda) | sed 'N;s/\n/ /' | \
         sed -n '/File/p' | sed -e '/\.h/d'  | \
         sed 's/ Lines executed:/ /; s/File/gcov:/; s/of// '
	@rm -f *.gcda *.gcno
	@echo
#
###########################################################################
#
# end of file
