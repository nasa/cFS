#
# Core Flight Software CMake / GNU make wrapper
#
# ABOUT THIS MAKEFILE:
# This defines the configuration targets for the multi-target build
# It is moved to a separate file to isolate this from the other logic
#
# This should reside in the same top-level directory as the main Makefile
#
# This file may define extra logic or dependencies for a given target


# This converts the staging directory from "make install" into a virtual
# filesystem that can be mounted in a VM or container
IMAGE_TYPE  ?= ext4
IMAGE_LABEL ?= cFS
TARBALL_DIR ?= .
DEPLOY_DIR  ?= .

IMAGE_NAME  = cfs-$(CPUNAME)
TARFS_FILE  = $(IMAGE_NAME).tar.xz
IMAGE_FILE  = $(IMAGE_NAME).$(IMAGE_TYPE)
IMAGE_SIZE  = 128M

IMAGE_SET += $(DEPLOY_DIR)/$(CONFIG)-cfs-cpu1.$(IMAGE_TYPE).xz
IMAGE_SET += $(DEPLOY_DIR)/$(CONFIG)-cfs-cpu2.$(IMAGE_TYPE).xz

# generic rule for creating an ext4 image from the tarball
$(DEPLOY_DIR)/$(CONFIG)-cfs-%.$(IMAGE_TYPE): $(TARBALL_DIR)/$(CONFIG)-%-bin.tar.xz
	rm -f "$(@)"
	/usr/sbin/mke2fs -t "$(IMAGE_TYPE)" -L "$(IMAGE_LABEL)" -d "$(<)" "$(@)" $(IMAGE_SIZE)

$(DEPLOY_DIR)/%.xz: $(DEPLOY_DIR)/%
	xz -k "$(<)"

.PHONY: all

all: $(IMAGE_SET)

# do not auto-delete the artifacts
.PRECIOUS: $(DEPLOY_DIR)/%.xz
