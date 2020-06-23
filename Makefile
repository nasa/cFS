#
# Core Flight Software CMake / GNU make wrapper
#
# ABOUT THIS MAKEFILE:
# This is actually part of the CMake alternative build system.
# It is a GNU-make wrapper that calls the CMake tools appropriately
# so that setting up a new build is fast and easy with no need to
# learn the CMake commands.  It also makes it easier to integrate
# the build with IDE tools such as Eclipse by providing a default
# makefile that has the common targets such as all/clean/etc.
#
# Use of this file is optional.
#
# This file is intended to be placed at the TOP-MOST level of the mission
# source tree, i.e. a level above "cfe".  Note this is outside the cfe
# repository which is why it cannot be delivered directly in place.
# To use it, simply copy it to the top directory.  As this just contains
# wrappers for the CMake targets, it is unlikely to change.  Projects
# are also free to customize this file and add their own targets after
# copying it to the top of the source tree.
#
# For _ALL_ targets defined in this file the build tree location may
# be specified via the "O" variable (i.e. make O=<my-build-dir> all).
# If not specified then the "build" subdirectory will be assumed.
#
# This wrapper defines the following major targets:
#  prep -- Runs CMake to create a new or re-configure an existing build tree
#    Note that multiple build trees can exist from a single source
#    Other control options (such as "SIMULATION") may be passed to CMake via
#    make variables depending on the mission build scripts.  These will be
#    cached in the build tree so they do not need to be set again thereafter.
#
#  all -- Build all targets in the CMake build tree
#
#  install -- Copy all files to the installation tree and run packaging scripts
#     The "DESTDIR" environment variable controls where the files are copied
#
#  clean -- Clean all targets in the CMake build tree, but not the build tree itself.
#
#  distclean -- Entirely remove the build directory specified by "O"
#      Note that after this the "prep" step must be run again in order to build.
#      Use caution with this as it does an rm -rf - don't set O to your home dir!
#
#  doc -- Build all doxygen source documentation.  The HTML documentation will be
#      generated under the build tree specified by "O".
#
#  usersguide -- Build all API/Cmd/Tlm doxygen documentation.  The HTML documentation
#      will be generated under the build tree specified by "O".
#
#  osalguide -- Build OSAL API doxygen documentation.  The HTML documentation will
#      be generated under the build tree specified by "O".
#
#  test -- Run all unit tests defined in the build.  Unit tests will typically only
#      be executable when building with the "SIMULATION=native" option.  Otherwise
#      it is up to the user to copy the executables to the target and run them.
#
#  lcov -- Runs the "lcov" tool on the build tree to collect all code coverage
#      analysis data and build the reports.  Code coverage data may be output by
#      the "make test" target above.
#

# Establish default values for critical variables.  Any of these may be overridden
# on the command line or via the make environment configuration in an IDE
O ?= build
ARCH ?= native
BUILDTYPE ?= debug
INSTALLPREFIX ?= /exe
DESTDIR ?= $(O)

# The "DESTDIR" variable is a bit more complicated because it should be an absolute
# path for CMake, but we want to accept either absolute or relative paths.  So if
# the path does NOT start with "/", prepend it with the current directory.
ifeq ($(filter /%, $(DESTDIR)),)
DESTDIR := $(CURDIR)/$(DESTDIR)
endif

# The "LOCALTGTS" defines the top-level targets that are implemented in this makefile
# Any other target may also be given, in that case it will simply be passed through.
LOCALTGTS := doc usersguide osalguide prep all clean install distclean test lcov
OTHERTGTS := $(filter-out $(LOCALTGTS),$(MAKECMDGOALS))

# As this makefile does not build any real files, treat everything as a PHONY target
# This ensures that the rule gets executed even if a file by that name does exist
.PHONY: $(LOCALTGTS) $(OTHERTGTS)

# If the target name appears to be a directory (ends in /), do a make all in that directory
DIRTGTS := $(filter %/,$(OTHERTGTS))
ifneq ($(DIRTGTS),)
$(DIRTGTS):
	$(MAKE) -C $(O)/$(patsubst $(O)/%,%,$(@)) all
endif

# For any other goal that is not one of the known local targets, pass it to the arch build
# as there might be a target by that name.  For example, this is useful for rebuilding
# single unit test executable files while debugging from the IDE
FILETGTS := $(filter-out $(DIRTGTS),$(OTHERTGTS))
ifneq ($(FILETGTS),)
$(FILETGTS):
	$(MAKE) -C $(O)/$(ARCH) $(@)
endif

# The "prep" step requires extra options that are specified via enviroment variables.
# Certain special ones should be passed via cache (-D) options to CMake.
# These are only needed for the "prep" target but they are computed globally anyway.
PREP_OPTS :=

ifneq ($(INSTALLPREFIX),)
PREP_OPTS += -DCMAKE_INSTALL_PREFIX=$(INSTALLPREFIX)
endif

ifneq ($(VERBOSE),)
PREP_OPTS += --trace
endif

ifneq ($(BUILDTYPE),)
PREP_OPTS += -DCMAKE_BUILD_TYPE=$(BUILDTYPE)
endif

all:
	$(MAKE) --no-print-directory -C "$(O)" mission-all

install:
	$(MAKE) --no-print-directory -C "$(O)" DESTDIR="$(DESTDIR)" mission-install

prep $(O)/.prep:
	mkdir -p "$(O)"
	(cd "$(O)/$(BUILDDIR)" && cmake $(PREP_OPTS) "$(CURDIR)/cfe")
	echo "$(PREP_OPTS)" > "$(O)/.prep"

clean:
	$(MAKE) --no-print-directory -C "$(O)" mission-clean

distclean:
	rm -rf "$(O)"

# Grab lcov baseline before running tests
test:
	lcov --capture --initial --directory $(O)/$(ARCH) --output-file $(O)/$(ARCH)/coverage_base.info
	(cd $(O)/$(ARCH) && ctest -O ctest.log)

lcov:
	lcov --capture --rc lcov_branch_coverage=1 --directory $(O)/$(ARCH) --output-file $(O)/$(ARCH)/coverage_test.info
	lcov --rc lcov_branch_coverage=1 --add-tracefile $(O)/$(ARCH)/coverage_base.info --add-tracefile $(O)/$(ARCH)/coverage_test.info --output-file $(O)/$(ARCH)/coverage_total.info
	genhtml $(O)/$(ARCH)/coverage_total.info --output-directory $(O)/$(ARCH)/lcov
	@/bin/echo -e "\n\nCoverage Report Link: file:$(CURDIR)/$(O)/$(ARCH)/lcov/index.html\n"

doc:
	$(MAKE) --no-print-directory -C "$(O)" mission-doc
	@/bin/echo -e "\n\nDetail Design: \nfile://$(CURDIR)/$(O)/doc/detaildesign/html/index.html\n"

usersguide:
	$(MAKE) --no-print-directory -C "$(O)" cfe-usersguide
	@/bin/echo -e "\n\ncFE Users Guide: \nfile://$(CURDIR)/$(O)/doc/users_guide/html/index.html\n"

osalguide:
	$(MAKE) --no-print-directory -C "$(O)" osalguide
	@/bin/echo -e "\n\nOsal Users Guide: \nfile://$(CURDIR)/$(O)/doc/osalguide/html/index.html\n"

# Make all the commands that use the build tree depend on a flag file
# that is used to indicate the prep step has been done.  This way
# the prep step does not need to be done explicitly by the user
# as long as the default options are sufficient.
$(filter-out prep distclean,$(LOCALTGTS)): $(O)/.prep
