CPUNAME ?= cpu1
INSTALL_DIR ?= $(O)/exe

ALL_TESTRUNNER_EXES := $(shell ls $(INSTALL_DIR)/$(CPUNAME)/*testrunner.exe)
ALL_UT_EXES := $(shell ls $(INSTALL_DIR)/$(CPUNAME)/*UT.exe)
ALL_FUNC_TEST_EXES := $(shell ls $(INSTALL_DIR)/$(CPUNAME)/*test.exe)

# These functional tests require a network stack,
# so they can be skipped on platforms that may not have network
ifeq ($(SKIP_NET_TESTS), true)
	SKIP_FUNC_TEST := 	\
		$(INSTALL_DIR)/$(CPUNAME)/network-api-test.exe	\
		$(INSTALL_DIR)/$(CPUNAME)/select-test.exe
	ALL_FUNC_TEST_EXES := $(filter-out $(SKIP_FUNC_TEST),$(ALL_FUNC_TEST_EXES))
endif

ALL_TESTRUNNER_TEST_LIST := $(basename $(ALL_TESTRUNNER_EXES))
ALL_UT_TEST_LIST := $(basename $(ALL_UT_EXES))
ALL_FUNC_TEST_LIST := $(basename $(ALL_FUNC_TEST_EXES))


ALL_TEST_LIST := \
	$(ALL_TESTRUNNER_TEST_LIST) \
	$(ALL_UT_TEST_LIST) \
	$(ALL_FUNC_TEST_LIST) \

.PHONY: clean_logs \
	all_tests \
	all_logs \
	all_checks \
	all_testrunner_logs \
	all_ut_logs \
	all_func_test_logs \

clean_logs:
	rm -f $(addsuffix .check,$(ALL_TEST_LIST)) \
		$(addsuffix .log,$(ALL_TEST_LIST))
	rm -f $(INSTALL_DIR)/$(CPUNAME)/failed-tests.log

# run all tests without checking success
all_logs: $(addsuffix .log,$(ALL_TEST_LIST))
# run all tests and check success
all_checks: $(addsuffix .check,$(ALL_TEST_LIST))

# run just tests with suffix *testrunner.exe
all_testrunner_logs: $(addsuffix .log,$(ALL_TESTRUNNER_TEST_LIST))
# run just tests with suffix *UT.exe
all_ut_logs: $(addsuffix .log,$(ALL_UT_TEST_LIST))
# run just tests with suffix *test.exe
all_func_test_logs: $(addsuffix .log,$(ALL_FUNC_TEST_LIST))

all_tests: all_checks
	@echo  '*** SUCCESS ***'

