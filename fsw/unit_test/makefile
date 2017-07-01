##############################################################################
## File:
##   $Id: makefile 1.5 2017/01/25 12:29:20EST sstrege Exp  $
##
## Purpose:
##   Makefile for building the CFS Data Storage (DS) UT Assert unit tests
##
##############################################################################
## GNU Makefile for building UT unit tests

#
# Supported MAKEFILE targets:
#   clean - deletes object files, executables, output files, and gcov files
#   all   - makes ds_testrunner.exe
#   run   - runs ds_testrunner.exe
#   gcov  - prints a GCOV coverage report (make all, make run, make gcov)
#
# GCOV is disabled by default.  If you are using the source level debugger you will want to 
# disable GCOV.  To enable GCOV you can override the ENABLE_GCOV variable on the command line 
# by setting it to TRUE.  For example "make ENABLE_GCOV=TRUE".
#

#
# VPATH specifies the search paths for source files outside of the current directory.  Note that
# all object files will be created in the current directory even if the source file is not in the 
# current directory.
#
VPATH := .
VPATH += $(UT_ASSERT_SRC)/src
VPATH += $(CFS_MISSION)/apps/cfs_lib/fsw/src
VPATH += ../src
VPATH += ../for_build

DS_PATH=../../

#
# INCLUDES specifies the search paths for include files outside of the current directory.  
# Note that the -I is required. 
#
INCLUDES := -I.
INCLUDES += -I$(UT_ASSERT_SRC)/inc
INCLUDES += -I$(CFS_MISSION)/build/pc-linux/inc
INCLUDES += -I$(APP_DIR)/cfs_lib/fsw/public_inc
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/os/inc
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/inc
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/time
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/sb
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/es
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/evs
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/fs
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/tbl
INCLUDES += -I$(CFE_DIR)/fsw/mission_inc
INCLUDES += -I$(CFE_DIR)/fsw/platform_inc/cpu1
INCLUDES += -I$(OSAL_DIR)/src/os/inc
INCLUDES += -I$(OSAL_DIR)/build/inc
INCLUDES += -I$(PSP_DIR)/fsw/inc
INCLUDES += -I$(PSP_DIR)/fsw/pc-linux/inc
INCLUDES += -I$(PSP_DIR)/fsw/pc-linux/inc
INCLUDES += -I$(DS_PATH)/fsw/src
INCLUDES += -I$(DS_PATH)/fsw/mission_inc

#
# FLIGHT_OBJS specifies flight software object files.
#
FLIGHT_OBJS := ds_app.o ds_cmds.o ds_file.o ds_table.o

#
# UT_OBJS specifies unit test object files.
#
UT_OBJS := ut_osapi_stubs.o
UT_OBJS += ut_osfileapi_stubs.o
UT_OBJS += ut_cfe_sb_stubs.o
UT_OBJS += ut_cfe_es_stubs.o
UT_OBJS += ut_cfe_evs_stubs.o
UT_OBJS += ut_cfe_time_stubs.o
UT_OBJS += ut_cfe_fs_stubs.o
UT_OBJS += ut_cfe_psp_memutils_stubs.o
UT_OBJS += ut_cfe_tbl_stubs.o
UT_OBJS += ut_cfe_sb_hooks.o
UT_OBJS += ut_cfe_es_hooks.o
UT_OBJS += ut_cfe_evs_hooks.o
UT_OBJS += ut_cfe_tbl_hooks.o
UT_OBJS += ut_cfe_time_hooks.o
UT_OBJS += utassert.o
UT_OBJS += utlist.o
UT_OBJS += uttest.o
UT_OBJS += uttools.o
UT_OBJS += cfs_utils.o
UT_OBJS += ds_testrunner.o
UT_OBJS += ds_app_test.o
UT_OBJS += ds_test_utils.o
UT_OBJS += ds_cmds_test.o
UT_OBJS += ds_file_test.o
UT_OBJS += ds_table_test.o

#
# UT_TEST_RUNNER specifies the name of the test runner.
#
UT_TEST_RUNNER = ds_testrunner.exe

###############################################################################

COMPILER=gcc
LINKER=gcc

#
# Compiler and Linker Options
#
GCOV_COPT = -fprofile-arcs -ftest-coverage -pg -p
GCOV_LOPT = -pg -p -fprofile-arcs -ftest-coverage -lgcov -lm

#WARNINGS = -Wall -W -ansi -Werror -Wstrict-prototypes -Wundef
WARNINGS = -Wall -Wstrict-prototypes -Wundef
DEBUGGER = -g

COPT = $(WARNINGS) $(DEBUGGER) $(GCOV_COPT) -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D_ix86_ -DUT_VERBOSE -DOSAPI_NO_SPECIAL_ATTRIBS

LOPT = $(GCOV_LOPT)

###############################################################################
## Rule to make the specified TARGET
##
%.exe: %.o
	$(LINKER) $(LOPT) $^ -o $*.exe

###############################################################################
##  "C" COMPILER RULE
##
%.o: %.c
	$(COMPILER) -c $(COPT) $(INCLUDES) $<

##############################################################################
##

all:$(UT_TEST_RUNNER)

$(UT_TEST_RUNNER): $(UT_OBJS) $(FLIGHT_OBJS)

clean ::
	rm -f *.o *.exe *.gcda *.gcno *.gcov gmon.out

run ::
	./$(UT_TEST_RUNNER)

gcov ::
	@echo
	@gcov $(FLIGHT_OBJS:.o=.gcda) | sed 'N;s/\n/ /' | \
		sed -n '/File/p' | sed '/ads/d' | sed -e '/\.h/d'  | \
		sed 's/ Lines executed:/ /; s/File/gcov:/; s/of// '
	@rm -f *.gcda *.gcno
	@echo

# end of file
