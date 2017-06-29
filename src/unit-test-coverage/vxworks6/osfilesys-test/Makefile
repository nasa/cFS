##############################################################################
## GNU Makefile for building UT unit tests in a Linux environment for easy
##  debug and code coverage

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

APP=osfilesys

OSAL ?= $(CFS_HOME)/osal

#
# INCLUDES specifies the search paths for include files outside of the current directory.  
# Note that the -I is required. 
#
INCLUDES += -I../ut-stubs/inc
INCLUDES += -I$(OSAL)/ut_assert/inc
INCLUDES += -I$(OSAL)/src/os/inc
INCLUDES += -I$(OSAL)/build/inc

#
# UT_OBJS specifies unit test object files.
#
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
GCOV_COPT = --coverage
GCOV_LOPT = --coverage 
endif

#WARNINGS = -Wall -W -ansi -Werror -Wstrict-prototypes -Wundef
WARNINGS = -Wall -Wstrict-prototypes
DEBUGGER = -g

#COPT = $(WARNINGS) $(DEBUGGER) $(GCOV_COPT) -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D__x86_64__ -D_LINUX_OS_
COPT = $(WARNINGS) $(DEBUGGER) -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D_ix86_ $(OSAL_M32)

LOPT = $(OSAL_M32)

###############################################################################
##  "C" COMPILER RULE
##
%.o: %.c
	$(COMPILER) -c $(COPT) $(INCLUDES) $<

##############################################################################
##

all:$(APP)_testrunner.exe

$(APP)_testrunner.exe: $(UT_OBJS)
	$(LINKER) $(GCOV_LOPT) $(LOPT) -o $@ $^ ../ut-osal/osfilesys.o ../testrunner.o

clean ::
	rm -f *.o *.exe *.gcda *.gcno *.gcov gmon.out $(APP)_log.txt

run ::
	./$(APP)_testrunner.exe | tee ./$(APP)_log.txt 

# end of file

