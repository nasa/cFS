#
# Master $(MAKE)file for unit tests
#

all: testrunner.o
	$(MAKE) -C ut-osal
	$(MAKE) -C osapi-test
	$(MAKE) -C osfileapi-test
	$(MAKE) -C osfilesys-test
	$(MAKE) -C osloader-test
	$(MAKE) -C osnetwork-test
	$(MAKE) -C ostimer-test

clean:
	rm -f ./*.o
	$(MAKE) -C ut-osal        clean
	$(MAKE) -C osapi-test     clean
	$(MAKE) -C osfileapi-test clean
	$(MAKE) -C osfilesys-test clean
	$(MAKE) -C osloader-test  clean
	$(MAKE) -C osnetwork-test clean
	$(MAKE) -C ostimer-test   clean

run:
# note the leading "-" makes it always run and ignore errors
	-$(MAKE) -C osapi-test     run
	-$(MAKE) -C osfileapi-test run
	-$(MAKE) -C osfilesys-test run
	-$(MAKE) -C osloader-test  run
	-$(MAKE) -C osnetwork-test run
	-$(MAKE) -C ostimer-test   run

gcov:
	$(MAKE) -C ut-osal        gcov


# Build the "testrunner" library here since it can be shared by all tests
VPATH = $(OSAL)/ut_assert/src
VPATH += $(OSAL)/src/bsp/pc-linux/ut-src
INCLUDES = -I$(OSAL)/src/os/inc -I$(OSAL)/ut_assert/inc -I$(OSAL)/build/inc
DEFINES = -D_UNIT_TEST_

%.o: %.c
	gcc $(OSAL_M32) -c -o $@ $(INCLUDES) $(DEFINES) $^

ifeq ($(OSAL_M32),-m32)
OSAL_LD32 ?= -melf_i386
endif

testrunner.o: utassert.o utlist.o utstubs.o uttest.o uttools.o bsp_ut.o
	ld $(OSAL_LD32) -o $@ -r $^

