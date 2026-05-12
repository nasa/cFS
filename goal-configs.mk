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
#  docs -- Build all doxygen source documentation.  The HTML documentation will be
#      generated under the build tree specified by "O".
#
#  test -- Run all unit tests defined in the build.  Unit tests will typically only
#      be executable when building with the "SIMULATION=native" option.  Otherwise
#      it is up to the user to copy the executables to the target and run them.
#
#  lcov -- Runs the "lcov" tool on the build tree to collect all code coverage
#      analysis data and build the reports.  Code coverage data may be output by
#      the "make test" target above.
#

# The "stamp" target names are associated with a file in the build dir to indicate last run time
# The "nostamp" target names do not have this, and are always executed
STAMP_GOAL_NAMES     := prep compile install checktest runtest lcov
CFS_STAMP_GOAL_NAMES := detaildesign usersguide osalguide image
ALL_GOAL_NAMES       := $(STAMP_GOAL_NAMES) $(CFS_STAMP_GOAL_NAMES)

# This file must define CONFIG_NAMES and all other per-config vars
include target-configs.mk

# The actual buildable targets are a combination of CONFIG.NAME (e.g. native.install)
STAMP_TARGETS   := $(foreach CFG,$(CONFIG_NAMES),$(addprefix $(CFG).,$(STAMP_GOAL_NAMES)))
STAMP_TARGETS   += $(foreach CFG,$(CFS_CONFIG_NAMES),$(addprefix $(CFG).,$(CFS_STAMP_GOAL_NAMES)))
CFS_TARGETS     := $(foreach CFG,$(CFS_CONFIG_NAMES),$(addprefix $(CFG).,$(ALL_GOAL_NAMES)))
NONCFS_TARGETS  := $(filter-out $(CFS_TARGETS),$(STAMP_TARGETS))
ALL_TARGETS     := $(CFS_TARGETS) $(NONCFS_TARGETS)

DISTCLEAN_TARGETS := $(addsuffix .distclean,$(CONFIG_NAMES))
ALL_TARGETS 	  += $(DISTCLEAN_TARGETS)
CFS_DOCS_TARGETS  := $(addsuffix .docs,$(CFS_CONFIG_NAMES))
ALL_TARGETS 	  += $(CFS_DOCS_TARGETS)
CLEAN_TARGETS     := $(addsuffix .clean,$(CONFIG_NAMES))
ALL_TARGETS 	  += $(CLEAN_TARGETS)
CFS_TARGETS       += $(addsuffix .clean,$(CFS_CONFIG_NAMES))
CLEANTEST_TARGETS := $(addsuffix .cleantest,$(CONFIG_NAMES))
ALL_TARGETS 	  += $(CLEANTEST_TARGETS)
CFS_TARGETS       += $(addsuffix .cleantest,$(CFS_CONFIG_NAMES))
