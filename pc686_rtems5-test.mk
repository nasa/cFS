
# At a minimum the variable O must be set by the caller
ifeq ($(O),)
$(error O must be set prior to calling this makefile)
endif

TEST_RESULT_DIR := $(O)/test-results
OBJDIR := $(O)/$(ARCH)

include $(O)/test-deps.mk

CFE_IMG_MB ?= 32
CFE_DISK_IMG ?= $(TEST_RESULT_DIR)/nonvol-disk.img
CFE_FS_IMG ?= $(TEST_RESULT_DIR)/nonvol-fs.img
QEMU_COMMAND ?= qemu-system-i386 -m 128
MACADDR = 00:04:9F$(shell head -c 3 /dev/urandom | hexdump -v -e '/1 ":%02X"')

PARTED_CMD = /usr/sbin/parted
MKFS_CMD = /usr/sbin/mkfs.fat

# when using RTEMS 5 it needs console specified
APPEND_OPTS += --console=/dev/com1
APPEND_OPTS += --batch-mode

.PHONY: all_tests cfe-disk

all_tests: $(OBJDIR)/all_tests_complete.stamp

$(OBJDIR)/all_tests_complete.stamp: $(TEST_OUTPUT_FILES)
	touch "$(OBJDIR)/all_tests_complete.stamp"

cfe-disk: $(CFE_DISK_IMG).stamp

$(CFE_DISK_IMG): FS_SIZE := $(shell echo $$(($(CFE_IMG_MB) * 1048576)))

$(CFE_DISK_IMG):
	truncate -s $(FS_SIZE)  $(@)
	$(PARTED_CMD) -s $(@) -- mklabel msdos
	$(PARTED_CMD) -a none -s $(@) -- mkpart primary fat32 63s -1s

$(CFE_FS_IMG): $(O)/stamp.install
	truncate -s $$((($(CFE_IMG_MB) * 1048576) - 32256))  $(@)
	$(MKFS_CMD) $(@)

$(CFE_DISK_IMG).stamp: $(CFE_DISK_IMG) $(CFE_FS_IMG)
	dd if=$(CFE_FS_IMG) of=$(CFE_DISK_IMG) bs=512 seek=63
	touch $(@)

clean_img:
	rm -f $(INSTALL_DIR)/*.img

clean_lcov:

# Special rule for the OS loader image, it needs the modules added
$(TEST_RESULT_DIR)/osal_loader_UT.img: $(CFE_DISK_IMG).stamp
	cp "$(CFE_DISK_IMG)" "$(TEST_RESULT_DIR)/osal_loader_UT.img"
	mcopy -i $(TEST_RESULT_DIR)/osal_loader_UT.img@@63s $(WORKDIR)/utmod ::

# The cow image will be created with the special image above
$(TEST_RESULT_DIR)/osal_loader_UT.cow: BACKING_IMG=$(TEST_RESULT_DIR)/osal_loader_UT.img
$(TEST_RESULT_DIR)/osal_loader_UT.cow: $(TEST_RESULT_DIR)/osal_loader_UT.img

%.cow: BACKING_IMG ?= $(CFE_DISK_IMG)
%.cow: $(CFE_DISK_IMG).stamp
	qemu-img create -o backing_file="$(abspath $(BACKING_IMG))",backing_fmt=raw -f qcow2 "$(@)"

$(TEST_RESULT_DIR)/%-testrunner.exe:
	if [ -e "$(WORKDIR)/$(*).exe" ]; then ln -s "$(WORKDIR)/$(*).exe" "$(@)"; fi
	if [ -e "$(WORKDIR)/$(*)-testrunner.exe" ]; then ln -s "$(WORKDIR)/$(*)-testrunner.exe" "$(@)"; fi
	if [ ! -e "$(@)" ]; then echo "Cannot find testrunner"; /bin/false; fi

# The network and select tests do not currently run
$(TEST_RESULT_DIR)/network-api-test.log \
$(TEST_RESULT_DIR)/select-test.log:
	echo "TEST SKIPPED" > "$(@)"

# It is important NOT to create the ".log" file unless it was successful
# The intent is if the test failed it will stop at the temp file and not rename it
# This needs an extra grep for the SUCCESS line because exit code is from QEMU not the test
$(TEST_RESULT_DIR)/%.log: $(TEST_RESULT_DIR)/%-testrunner.exe $(TEST_RESULT_DIR)/%.cow
	$(QEMU_COMMAND) -no-reboot -display none \
	    -kernel $(<) \
	    -drive file=$(TEST_RESULT_DIR)/$(*).cow,format=qcow2 \
	    -device i82557b,netdev=net0,mac=$(MACADDR) \
	    -netdev user,id=net0 \
		-append '$(APPEND_OPTS)' \
	    -serial file:"$(@).tmp"
	grep -q '^Application exit status: SUCCESS' "$(@).tmp"
	@mv -v "$(@).tmp" $(@)
