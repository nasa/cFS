#
# Core Flight Software CMake / GNU make wrapper
#
# ABOUT THIS MAKEFILE:
# This defines the configuration targets for the multi-target build
# It is moved to a separate file to isolate this from the other logic
#
# This should reside in the same top-level directory as the main Makefile

THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# The config names is a complete list of configurations to build
CONFIG_NAMES :=

# The CFS config names is a subset of the CONFIG_NAMES which use CFS
# Being in this list means the standard set of CFS options are applied
NONCFS_CONFIG_NAMES :=

FOUND_MAKEFILES := $(wildcard $(THIS_DIR)/configs/*.mk)

SORTED_MAKEFILES := $(sort $(FOUND_MAKEFILES))

include $(SORTED_MAKEFILES)

$(info $(SORTED_MAKEFILES))
$(info $(CONFIG_NAMES))
$(info $(NONCFS_CONFIG_NAMES))

CFS_CONFIG_NAMES := $(filter-out $(NONCFS_CONFIG_NAMES),$(CONFIG_NAMES))
