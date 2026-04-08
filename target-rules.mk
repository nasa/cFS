#
# Core Flight Software CMake / GNU make wrapper
#
# ABOUT THIS MAKEFILE:
# This is a helper makefile to execute the build for a single config
# It should not be invoked directly, it is invoked from the main Makefile
#

include goal-configs.mk

O               ?= $(O_$(CFG))
ARCH            ?= $(ARCH_$(CFG))
PREP_OPTS       ?= $(PREP_OPTS_$(CFG))
PLATFORM  		?= $(PLATFORM_$(CFG))
CPUNAME   		?= $(CPUNAME_$(CFG))
BUILDTYPE 		?= $(BUILDTYPE_$(CFG))

DESTDIR   ?= $(CURDIR)/$(O)

# Fallback/Default values for variables
# Note that the above may have defined a variable to be the empty string,
# so this needs to check for empty string and cannot use ?=
ifeq ($(PLATFORM),)
PLATFORM  := default
endif
ifeq ($(CPUNAME),)
CPUNAME   := cpu1
endif
ifeq ($(BUILDTYPE),)
BUILDTYPE := debug
endif

$(ALL_TARGETS): PREP_OPTS += -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
$(ALL_TARGETS): PREP_OPTS += -DCMAKE_BUILD_TYPE=$(BUILDTYPE)

$(CFS_TARGETS): PREP_OPTS += -S "$(CURDIR)/cfe"
$(CFS_TARGETS): PREP_OPTS += -DCMAKE_INSTALL_PREFIX=/exe
$(CFS_TARGETS): SUBTGT_PREFIX ?= mission-
$(CFS_TARGETS): TEST_SUBDIR = $(ARCH)/$(PLATFORM)

export O
export ARCH
export PLATFORM
export CPUNAME

# A generic pattern rule to invoke CMake for the "prep" (makefile generation) step
%/stamp.prep:
	mkdir -p "$(O)"
	cmake $(PREP_OPTS) -B "$(O)" $(PREP_SOURCE_DIR)
	echo '$(PREP_OPTS)' > "$(@)"

# A generic pattern rule to invoke a sub-make for the appliction compile only
%/stamp.compile: %/stamp.prep
	$(MAKE) --no-print-directory -C "$(O)" $(SUBTGT_PREFIX)all
	touch "$(@)"

# A generic pattern rule to invoke a sub-make for the appliction build/install to staging area
%/stamp.install: %/stamp.prep
	$(MAKE) --no-print-directory -C "$(O)" DESTDIR="$(DESTDIR)" $(SUBTGT_PREFIX)install
	touch "$(@)"

# custom-rules.mk should add dependencies to stamp.image and define the rules for building them
%/stamp.image: %/stamp.install
	touch "$(@)"

# Check which tests exist
# Extract the list of tests and associated commands from JSON
%/stamp.checktest:  %/stamp.install
	(cd $(O)/$(TEST_SUBDIR) && ctest --show-only=json-v1) > "$(O)/test-list.json.tmp1"
	jq '[.tests[] | { "name":.name, "command":.command[0], "workdir":(.properties[] | if .name == "WORKING_DIRECTORY" then .value else empty end) } ]' \
		"$(O)/test-list.json.tmp1" > "$(O)/test-list.json.tmp2"
	rm -f "$(O)/test-list.json.tmp1"
	mv -v "$(O)/test-list.json.tmp2" "$(O)/test-list.json"
	touch "$(@)"

# A generic pattern rule to invoke a sub-make for running tests
%/stamp.runtest: %/stamp.checktest
	mkdir -p "$(O)/test-results"
	jq -r '.[] | if .workdir then "$(O)/test-results/" + .name + ".log: WORKDIR=" + .workdir else empty end' "$(O)/test-list.json" > "$(O)/test-deps.mk.tmp"
	jq -r '.[] | if .command then "$(O)/test-results/" + .name + ".log: " + .command else empty end' "$(O)/test-list.json" >> "$(O)/test-deps.mk.tmp"
	jq -r '.[] | "TEST_OUTPUT_FILES += $(O)/test-results/" + .name + ".log"' "$(O)/test-list.json" >> "$(O)/test-deps.mk.tmp"
	mv -v "$(O)/test-deps.mk.tmp" "$(O)/test-deps.mk"
	$(MAKE) --no-print-directory -f $(CURDIR)/$(CFG)-test.mk all_tests
	touch "$(@)"

# A generic pattern rule to invoke a sub-make for running coverage analysis
%/stamp.lcov: %/stamp.runtest
	$(MAKE) --no-print-directory -f $(CFG)-test.mk all_lcov
	touch "$(@)"

# A generic pattern rule to invoke a sub-make for building detail design doc
%/stamp.detaildesign: %/stamp.prep
	$(MAKE) --no-print-directory -C "$(O)" mission-doc
	touch "$(@)"

# A generic pattern rule to invoke a sub-make for building user guide doc
%/stamp.usersguide: %/stamp.prep
	$(MAKE) --no-print-directory -C "$(O)" cfe-usersguide
	touch "$(@)"

# A generic pattern rule to invoke a sub-make for building osal guide doc
%/stamp.osalguide: %/stamp.prep
	$(MAKE) --no-print-directory -C "$(O)" osal-apiguide
	touch "$(@)"

# Do not delete intermediate stamp files
.PRECIOUS: %/stamp.prep
.PRECIOUS: %/stamp.compile
.PRECIOUS: %/stamp.install
.PRECIOUS: %/stamp.image
.PRECIOUS: %/stamp.checktest
.PRECIOUS: %/stamp.runtest
.PRECIOUS: %/stamp.lcov
.PRECIOUS: %/stamp.detaildesign
.PRECIOUS: %/stamp.usersguide
.PRECIOUS: %/stamp.osalguide

.PHONY: all $(GOAL)

all: $(GOAL)

$(GOAL): $(STAMPFILE)

# Any additional customization for certain targets can go here
include custom-rules.mk
