#
# Makefile for ut osal
#

#
# INCLUDES specifies the search paths for include files outside of the current directory.  
# Note that the -I is required. 
#
INCLUDES += -I./inc
INCLUDES += -I../ut-stubs/inc
INCLUDES += -I$(OSAL)/src/os/inc
INCLUDES += -I$(OSAL)/build/inc
INCLUDES += -I$(OSAL)/src


#
# UT_OBJS specifies unit test object files.
#
UT_OBJS := osapi.o
UT_OBJS += osfileapi.o
UT_OBJS += osfilesys.o
UT_OBJS += osloader.o
UT_OBJS += osnetwork.o
UT_OBJS += ostimer.o

###############################################################################

COMPILER=gcc
LINKER=ld

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

COPT = $(WARNINGS) $(DEBUGGER) $(GCOV_COPT) -DOSAPI_NO_SPECIAL_ATTRIBS -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D_ix86_ $(OSAL_M32) 

LOPT = $(GCOV_LOPT) $(OSAL_M32)

###############################################################################
##  "C" COMPILER RULE
##
%.o: src/%.c
	$(COMPILER) -c $(COPT) $(INCLUDES) $<

##############################################################################
##

all: $(UT_OBJS)

clean ::
	rm -f *.o *.exe *.gcda *.gcno *.gcov gmon.out

gcov ::
	@echo
	@gcov -b $(UT_OBJS:.o=.gcda) | \
	 awk -f $(OSAL)/build/gcov-parse.awk
	@rm -f *.gcda *.gcno
	@echo

# end of file

