SKIP_NET_TESTS := true

# At a minimum the variable O must be set by the caller
ifeq ($(O),)
$(error O must be set prior to native-test.mk)
endif

CFE_IMG_MB ?= 32
CFE_DISK_IMG ?= $(INSTALL_DIR)/$(CPUNAME)/nonvol-disk.img
CFE_FS_IMG ?= $(INSTALL_DIR)/$(CPUNAME)/nonvol-fs.img
QEMU_COMMAND ?= qemu-system-i386 -m 128
MACADDR = 00:04:9F$(shell head -c 3 /dev/urandom | hexdump -v -e '/1 ":%02X"')
RTEMS_VERSION ?= i686-rtems5

# default rule to just create qemu disk image
all: cfe-disk

# include list of all unit tests to run
include .github/scripts/common-test.mk

.PHONY: run cfe-disk
.SECONDARY: $(addsuffix .log,$(ALL_TEST_LIST)))

cfe-disk: $(CFE_DISK_IMG)

$(CFE_DISK_IMG): FS_SIZE := $(shell echo $$(($(CFE_IMG_MB) * 1048576)))

$(CFE_DISK_IMG): $(wildcard $(O)/$(RTEMS_VERSION)/default_cpu1/osal/unit-tests/osloader-test/utmod/*) $(wildcard $(INSTALL_DIR)/$(CPUNAME)/eeprom/*)
	# Basic disk
	truncate -s $(FS_SIZE)  $(@)
	parted -s $(@) -- mklabel msdos
	parted -a none -s $(@) -- mkpart primary fat32 63s -1s
	# File system partition
	mkfs.fat --offset 63 $(@)
	mcopy -i $(@)@@32256 -sv $(O)/$(RTEMS_VERSION)/default_cpu1/osal/unit-tests/osloader-test/utmod :: || /bin/true
	mcopy -i $(@)@@32256 -sv $(INSTALL_DIR)/$(CPUNAME)/eeprom ::

run: $(CFE_DISK_IMG)
	$(QEMU_COMMAND) -display none -no-reboot -serial mon:stdio \
	    -kernel $(INSTALL_DIR)/$(CPUNAME)/$(KERNEL_NAME).exe \
	    -drive file=$(CFE_DISK_IMG),format=raw \
    	-device i82557b,netdev=net0,mac=$(MACADDR) \
	    -netdev user,id=net0,hostfwd=udp:127.0.0.1:1235-:1235 \
			-append '--console=/dev/com1'

clean_img:
	rm -f $(INSTALL_DIR)/$(CPUNAME)/*.img

%.cow: $(CFE_DISK_IMG)
	qemu-img create -o backing_file=$(notdir $(CFE_DISK_IMG)),backing_fmt=raw -f qcow2 $(@)

%.log: %.exe %.cow
	$(QEMU_COMMAND) -no-reboot -display none \
	    -kernel $(<) \
	    -append '--batch-mode --console=/dev/com1' \
	    -drive file=$(*).cow,format=qcow2 \
	    -device i82557b,netdev=net0,mac=$(MACADDR) \
	    -netdev user,id=net0 \
	    -serial file:$(@).tmp
	@mv -v $(@).tmp $(@)

%.check: %.log
	@(grep -q '^Application exit status: SUCCESS' $(<)) || (echo $(*): ---FAIL--- | tee -a $(INSTALL_DIR)/$(CPUNAME)/failed-tests.log; /bin/false )
