# At a minimum the variable O must be set by the caller
ifeq ($(O),)
$(error O must be set prior to native-test.mk)
endif

include $(O)/test-deps.mk

ARCH   ?= native
OBJDIR := $(O)/$(ARCH)

COVERAGE_MODE ?= basic

.PHONY: all_tests all_lcov clean_lcov force

all_tests: $(OBJDIR)/all_tests_complete.stamp

all_lcov: $(OBJDIR)/coverage_$(COVERAGE_MODE).info
	genhtml "$(<)" --branch-coverage --output-directory "$(OBJDIR)/lcov"
	@/bin/echo -e "\n\nCoverage Report Link: file://$(abspath $(OBJDIR))/lcov/index.html\n"

clean_lcov:
	find "$(OBJDIR)" -type f -name '*.info' -o -name '*.gcno' -o -name '*.gcda' -print0 | xargs -0 rm -f
	rm -f "$(OBJDIR)/all_tests_complete.stamp"
	rm -rf "$(OBJDIR)/lcov"

# It is important NOT to create the ".log" file unless it was successful
# The intent is if the test failed it will stop at the temp file and not rename it
# This relies on the exit code of the test process - it returns nonzero on error
%.log:
	(cd $(dir $(<)) && ./$(notdir $(<))) > "$(@).tmp"
	@mv -v "$(@).tmp" "$(@)"

$(OBJDIR)/all_tests_complete.stamp: $(TEST_OUTPUT_FILES)
	touch "$(OBJDIR)/all_tests_complete.stamp"

$(OBJDIR)/coverage_basic.info: $(OBJDIR)/all_tests_complete.stamp
	lcov --capture --rc branch_coverage=1 --directory "$(OBJDIR)" --output-file "$(OBJDIR)/coverage_basic.info"

$(OBJDIR)/coverage_mcdc.info: $(OBJDIR)/all_tests_complete.stamp
	lcov --capture --rc branch_coverage=1 --directory "$(OBJDIR)" --output-file "$(OBJDIR)/coverage_mcdc.info"
