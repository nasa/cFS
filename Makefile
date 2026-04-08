#
# Core Flight Software CMake / GNU make wrapper
#
# This wrapper defines the following major goals:
#  prep -- Runs CMake to create a new or re-configure an existing build tree
#  all -- Build all targets in the CMake build tree
#  install -- Copy all files to the installation tree and run packaging scripts
#  clean -- Clean all targets in the CMake build tree, but not the build tree itself.
#  distclean -- Entirely remove the build directory
#  docs -- Build all doxygen source documentation.
#  test -- Run all unit tests defined in the build.
#  lcov -- Runs the "lcov" tool on the build tree to collect all code coverage
#
# Actual buildable targets combine one of the goals above with a configuration,
# specified as a pair in the form <config>.<goal>
#
# For example, "native.prep" runs prep for the native config, and "flight.test"
# runs the tests for the flight config.
#
# As a shorthand, specifying only the goal (e.g. prep) runs that target for all
# defined configurations.
#

# This file must define all the possible make goals
include goal-configs.mk

# For all targets the O var should be set to the per-config build dir
$(ALL_TARGETS):   CFG = $(basename $(@))
$(ALL_TARGETS):   O = $(O_$(CFG))
$(STAMP_TARGETS): STAMPFILE = $(O)/stamp$(suffix $(@))
$(CFS_TARGETS):   SUBTGT_PREFIX ?= mission-

.PHONY: $(ALL_GOAL_NAMES) $(ALL_TARGETS)

# This makefile just invokes a sub-make, and it is important to not invoke that
# sub-make multiple times in parallel when the -j option is used.  This is still
# a bit heavy-handed because it only really needs to be done for targets on the
# same build tree (e.g. native.compile and native.test need to be serialized, but
# native.compile and flight.test should be fine in parallel).  However, make does
# not seem to offer much granularity in this target.  Furthermore, running tests
# still might conflict (e.g. native.test and flight.test) if those programs run
# on the host and bind to the same network ports, so it is safest to serialize all.
.NOTPARALLEL:

# The "docs" target is just a meta target for all docs (detaildesign, usersguide, osalguide)
$(CFS_DOCS_TARGETS):  %.docs: %.detaildesign
$(CFS_DOCS_TARGETS):  %.docs: %.usersguide
$(CFS_DOCS_TARGETS):  %.docs: %.osalguide

# The "distclean" goal removes the entire build dir, including generated makefiles
$(DISTCLEAN_TARGETS):
	[ ! -z "$(O)" ] && rm -rf "$(O)"

# Each of these targets has a corresponding stamp file in the build dir, reflecting the
# last time the target was successfully built.  The stampfile is removed first, such that
# _directly_ specified targets will always run and rebuild,  but dependent targets may be
# skipped if the stampfile is up to date.
# For example, the .test stampfile depends on .install stampfile, and if "native.test" is
# specified here then the .test stampfile will be removed but not the .install stampfile.
# Thus the install will only run if the .install stampfile does not exist, but the test will
# always run regardless of the stampfile.
$(STAMP_TARGETS):
	rm -f "$(STAMPFILE)"
	env $(ENV_OPTS_$(CFG)) $(MAKE) --no-print-directory -f $(CURDIR)/target-rules.mk \
		CFG=$(CFG) GOAL=$(@) STAMPFILE=$(STAMPFILE) all
	@echo "$(@) successfully completed"

# A generic pattern rule to clean a build area
$(CLEAN_TARGETS):
	$(MAKE) --no-print-directory -C "$(O)" $(SUBTGT_PREFIX)clean
	rm -rf "$(O)"/stamp.*

# Clean up the test state, purges coverage stats and forces all tests to re-run
$(CLEANTEST_TARGETS):
	$(MAKE) --no-print-directory -f $(CFG)-test.mk O="$(O)" clean_lcov
	rm -rf "$(O)"/test-results "$(O)"/stamp.*
	env $(ENV_OPTS_$(CFG)) $(MAKE) --no-print-directory -f $(CURDIR)/target-rules.mk \
		CFG=$(CFG) GOAL=$(CFG).runtest STAMPFILE=$(O)/stamp.runtest all

# also define targets which builds all configs (except world, which has custom rule)
# This utilizes "second expansion" feature of gnu make so that $@ may be referenced in prereq list
.SECONDEXPANSION:
$(ALL_GOAL_NAMES) distclean:  $$(addsuffix .$$(@),$(CONFIG_NAMES))
