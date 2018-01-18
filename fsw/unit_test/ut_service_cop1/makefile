##############################################################################
## GNU Makefile for building UT unit tests

#
# Supported MAKEFILE targets:
#   clean - deletes object files, executables, output files, and gcov files
#   all   - makes utf_test_runner.exe
#   run   - runs utf_test_runner.exe
#   gcov  - prints a GCOV coverage report (make all, make run, make gcov)
#
# GCOV is disabled by default.  If you are using the source level debugger you will want to 
# disable GCOV.  To enable GCOV you can override the ENABLE_GCOV variable on the command line 
# by setting it to TRUE.  For example "make ENABLE_GCOV=TRUE".
#

APP=cop1

CFE_PATH  = $(CFE_FSW)/cfe-core
OSAL_PATH = $(OSAL_DIR)
PSP_PATH  = $(PSP_DIR)

#
# VPATH specifies the search paths for source files outside of the current directory.  Note that
# all object files will be created in the current directory even if the source file is not in the 
# current directory.
#
VPATH := ../../src/services
VPATH += ../ut-assert/src

#
# INCLUDES specifies the search paths for include files outside of the current directory.  
# Note that the -I is required. 
#
INCLUDES := -I.
INCLUDES += -I..
INCLUDES += -I../../src
INCLUDES += -I../../public_inc
INCLUDES += -I../ut-assert/inc
INCLUDES += -I$(CFE_PATH)/os/inc
INCLUDES += -I$(CFE_PATH)/src/inc
INCLUDES += -I$(CFE_PATH)/src/time
INCLUDES += -I$(CFE_PATH)/src/sb
INCLUDES += -I$(CFE_PATH)/src/es
INCLUDES += -I$(CFE_PATH)/src/evs
INCLUDES += -I$(CFE_PATH)/src/fs
INCLUDES += -I$(CFE_PATH)/src/tbl
INCLUDES += -I$(CFE_PATH)/../mission_inc
INCLUDES += -I$(CFE_PATH)/../platform_inc/cpu1
INCLUDES += -I$(OSAL_PATH)/src/os/inc
INCLUDES += -I$(OSAL_PATH)/build/inc
INCLUDES += -I$(OSAL_PATH)/src/bsp/pc-linux/config
INCLUDES += -I$(PSP_PATH)/fsw/inc
INCLUDES += -I$(PSP_PATH)/fsw/pc-linux/inc

#
# APP_OBJS specifies flight software object files.
#
APP_OBJS := $(APP).o


#
# UT_OBJS specifies unit test object files.
#
UT_OBJS := ut_osapi_stubs.o
UT_OBJS += ut_osfileapi_stubs.o
UT_OBJS += ut_cfe_psp_memutils_stubs.o
UT_OBJS += ut_cfe_sb_stubs.o
UT_OBJS += ut_cfe_sb_hooks.o
UT_OBJS += ut_cfe_es_stubs.o
UT_OBJS += ut_cfe_es_hooks.o
UT_OBJS += ut_cfe_evs_stubs.o
UT_OBJS += ut_cfe_evs_hooks.o
UT_OBJS += ut_cfe_tbl_stubs.o
UT_OBJS += ut_cfe_tbl_hooks.o
UT_OBJS += ut_cfe_time_stubs.o
UT_OBJS += ut_cfe_fs_stubs.o
UT_OBJS += utassert.o
UT_OBJS += utlist.o
UT_OBJS += uttest.o
UT_OBJS += uttools.o
UT_OBJS += $(APP)_testcase.o
UT_OBJS += $(APP)_stubs.o

###############################################################################

COMPILER=gcc
LINKER=gcc

#
# Compiler and Linker Options
#
ENABLE_GCOV = TRUE
ifeq ($(ENABLE_GCOV), TRUE)
GCOV_COPT = -fprofile-arcs -ftest-coverage -pg -p
GCOV_LOPT = -pg -p -fprofile-arcs -ftest-coverage -lgcov
endif

#WARNINGS = -Wall -W -ansi -Werror -Wstrict-prototypes -Wundef
WARNINGS = -Wall -Wstrict-prototypes
DEBUGGER = -g

COPT = $(WARNINGS) $(DEBUGGER) $(GCOV_COPT) -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D__x86_64__ -D_LINUX_OS_
#COPT = $(WARNINGS) $(DEBUGGER) $(GCOV_COPT) -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D_ix86_ -DUT_VERBOSE 

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

all:$(APP)_testrunner.exe

$(APP)_testrunner.exe: $(APP)_testrunner.o $(UT_OBJS) $(APP_OBJS)

clean ::
	rm -f *.o *.exe *.gcda *.gcno *.gcov gmon.out

run ::
	./$(APP)_testrunner.exe

#gcov ::
#	@echo
#	@gcov $(UT_OBJS:.o=.gcda) | sed 'N;s/\n/ /' | \
#		sed -n '/File/p' | sed '/ads/d'  | \
#		sed 's/ Lines executed:/ /; s/File/gcov:/; s/of//'
#	@rm -f *.gcda *.gcno
#	@echo

gcov ::
	@echo
	@gcov -b $(APP_OBJS:.o=.gcda) | sed 'N;s/\n/ /' | \
         sed -n '/File/p' | sed '/ads/d' | sed -e '/\.h/d'  | \
         sed 's/ Lines executed:/ /; s/File/gcov:/; s/of// '
	@rm -f *.gcda *.gcno
	@echo

# end of file
