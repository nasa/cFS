##############################################################################
## File:
##   $Id: Makefile 1.1 2011/06/30 21:00:08EDT aschoeni Exp  $
##
## Purpose:
##   Makefile for building the CFS Scheduler (SCH) unit test tables
##
##   $Log: Makefile  $
##   Revision 1.1 2011/06/30 21:00:08EDT aschoeni 
##   Initial revision
##   Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/unit_test/cf/apps/project.pj
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
DEFAULT_COPT := -Wall -Wstrict-prototypes -ansi -g 
COPT = -D_ix86_ -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -DCLOCKS_PER_SEC=1000000
LOPT=
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
UTF_PATH=$(CFS_MISSION)/cfe/tools/utf
CFE_PATH=$(CFS_MISSION)/cfe/fsw/cfe-core
BUILD_PATH=$(CFS_MISSION)/build
OS_PATH=$(CFS_MISSION)/osal/src/
#
###########################################################################
#
# Source file path definitions
#
VPATH := .
VPATH += ..
VPATH += $(UTF_PATH)/src
VPATH += $(CFE_PATH)/src/sb
VPATH += $(CFE_PATH)/src/time
VPATH += $(CFE_PATH)/src/es
VPATH += $(CFE_PATH)/src/evs
VPATH += $(CFE_PATH)/src/fs
VPATH += $(CFE_PATH)/src/tbl
VPATH += $(OS_PATH)/arch/x86/pc/linux/bsp
VPATH += $(CFE_PSP_SRC)/pc-linux/src
VPATH += $(CFS_APP_SRC)/sch/fsw/src
#
###########################################################################
#
# Header file path definitions
#
INCLUDES := -I.
INCLUDES += -I..
INCLUDES += -I$(UTF_PATH)/inc
INCLUDES += -I$(BUILD_PATH)/inc
INCLUDES += -I$(BUILD_PATH)/mission_inc
INCLUDES += -I$(BUILD_PATH)/cpu1/inc
INCLUDES += -I$(BUILD_PATH)/cpu1/cfe/inc
INCLUDES += -I$(CFE_PATH)/os/inc
INCLUDES += -I$(CFE_PATH)/src/inc
INCLUDES += -I$(CFE_PATH)/src/time
INCLUDES += -I$(CFE_PATH)/src/sb
INCLUDES += -I$(CFE_PATH)/src/es
INCLUDES += -I$(CFE_PATH)/src/evs
INCLUDES += -I$(CFE_PATH)/src/fs
INCLUDES += -I$(CFE_PATH)/src/tbl
INCLUDES += -I$(OS_PATH)/os/inc
INCLUDES += -I$(OS_PATH)/arch/x86/pc/inc
INCLUDES += -I$(CFE_PSP_SRC)/inc
INCLUDES += -I$(CFE_PSP_SRC)/pc-linux/inc
INCLUDES += -I$(CFS_APP_SRC)/sch/fsw/src
INCLUDES += -I$(CFS_APP_SRC)/sch/fsw/mission_inc
INCLUDES += -I$(CFS_APP_SRC)/sch/fsw/platform_inc
INCLUDES += -I$(CFS_APP_SRC)/sch/fsw/public_inc
#
###########################################################################
#
# SCH object files
#
SCH_OBJS := sch_def_msgtbl.o
SCH_OBJS += sch_def_schtbl.o
SCH_OBJS += sch_def_schtbl1.o
SCH_OBJS += sch_def_schtbl2.o
SCH_OBJS += sch_def_schtbl3.o
SCH_OBJS += sch_def_schtbl4.o
SCH_OBJS += sch_def_schtbl5.o
SCH_OBJS += sch_def_schtbl6.o
SCH_OBJS += sch_def_msgtbl7.o
SCH_OBJS += sch_def_msgtbl8.o
SCH_OBJS += sch_def_msgtbl9.o
#
###########################################################################
#
# SCH table files
#
SCH_TBLS := sch_def_msgtbl.tbl
SCH_TBLS += sch_def_schtbl.tbl
SCH_TBLS += sch_def_schtbl1.tbl
SCH_TBLS += sch_def_schtbl2.tbl
SCH_TBLS += sch_def_schtbl3.tbl
SCH_TBLS += sch_def_schtbl4.tbl
SCH_TBLS += sch_def_schtbl5.tbl
SCH_TBLS += sch_def_schtbl6.tbl
SCH_TBLS += sch_def_msgtbl7.tbl
SCH_TBLS += sch_def_msgtbl8.tbl
SCH_TBLS += sch_def_msgtbl9.tbl
#
###########################################################################
#
# Rules to make the specified targets
#   
%.o: %.c
	$(COMPILER) -c $(COPT) $(DEFAULT_COPT) $(GCOV_COPT) $(INCLUDES) $<

%.tbl: %.o
	$(CFS_MISSION)/cfe/tools/elf2cfetbl/elf2cfetbl $<

all: $(SCH_OBJS) $(SCH_TBLS)

clean ::
	rm -f *.o *.bin *.da *.out *.bbg *.bb *.tbl *.gcda *.gcno *.gcov
	
gcov ::
	@echo
	@gcov $(SCH_OBJS:.o=.gcda) | sed 'N;s/\n/ /' | \
		sed -n '/File/p' | sed '/ads/d'  | \
		sed 's/ Lines executed:/ /; s/File/gcov:/; s/of//'
	@rm -f *.gcda *.gcno
	@echo	
#
###########################################################################
#
# end of file

