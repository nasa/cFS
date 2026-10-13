# At a minimum the variable O must be set by the caller
ifeq ($(O),)
$(error O must be set prior to osal-test.mk)
endif

TEST_PARALLEL_JOBS ?= 8

.PHONY: all_tests all_lcov clean_lcov force

$(O)/coverage_test.info: force
	lcov --capture --rc lcov_branch_coverage=1 --exclude '/usr/*' --directory "$(O)" --output-file "$(O)/coverage_test.info"

clean_lcov:
	find "$(O)" -type f -name '*.gcno' -o -name '*.gcda' -print0 | xargs -0 rm -f
	-rm -f $(O)/coverage_test.info

all_tests: | clean_lcov
all_tests:
	cd "$(O)" && ctest -j$(TEST_PARALLEL_JOBS) --output-on-failure -O osal-test.log

all_lcov: $(O)/ $(O)/coverage_test.info
	genhtml "$(O)/coverage_test.info" --branch-coverage --output-directory "$(O)/lcov"
	@/bin/echo -e "\n\nCoverage Report Link: file://$(abspath $(O))/lcov/index.html\n"
