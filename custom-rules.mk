# This converts the staging directory from "make install" into a virtual
# filesystem that can be mounted in a VM or container

YOCTO_SANDBOX_REPO = https://developer.nasa.gov/cFS/yocto-sandbox
YOCTO_SANDBOX_TAG  = v0.0.3-testing

PARTED_CMD ?= /usr/sbin/parted

IMAGE_TYPE  ?= qcow2
IMAGE_LABEL ?= cFS
IMAGE_SIZE  ?= 128M
DEPLOY_DIR  ?= $(O)/deploy

QEMU_YOCTO_DEPLOY_DIR = $(O_qemu_yocto_linux)/deploy
QEMU_PC686_RTEMS_DEPLOY_DIR = $(O_pc686_rtems5)/deploy

CFS_IMAGE_BASENAME = cfs-$(CPUNAME)

QEMU_MACADDR = 52:54:00$(shell head -c 3 /dev/urandom | hexdump -v -e '/1 ":%02X"')
QEMU_HOSTFWD = udp::1234-:1234,hostfwd=tcp::2222-:22

QEMU_NET_OPTS += -device $(QEMU_NETDEV_TYPE),netdev=net0,mac=$(QEMU_MACADDR)
QEMU_NET_OPTS += -netdev user,id=net0,hostfwd=$(QEMU_HOSTFWD)

ifeq ($(IMAGE_TYPE),qcow2)
IMAGE_FORMAT ?= qcow2
else
IMAGE_FORMAT ?= raw
endif

QEMU_DEVICE_OPTS += -object rng-random,filename=/dev/urandom,id=rng0
QEMU_DEVICE_OPTS += -device virtio-rng-pci,rng=rng0
QEMU_DEVICE_OPTS += -display none
QEMU_DEVICE_OPTS += -serial stdio

RTEMS5_APPEND_OPTS += --console=/dev/com1
RTEMS5_APPEND_OPTS += --batch-mode


$(QEMU_YOCTO_DEPLOY_DIR)/fetch_yocto_rootfs.cpu1.stamp: SYSTEM_PACKAGE = qemuriscv64-target-bin.tar.xz
$(QEMU_YOCTO_DEPLOY_DIR)/fetch_yocto_rootfs.cpu2.stamp: SYSTEM_PACKAGE = qemumips-target-bin.tar.xz

$(DEPLOY_DIR)/fetch_yocto_rootfs.%.stamp:
	mkdir -p $(DEPLOY_DIR)/$(CPUNAME)
	curl -fsL $(YOCTO_SANDBOX_REPO)/releases/download/$(YOCTO_SANDBOX_TAG)/$(SYSTEM_PACKAGE) | tar Jxv -C $(DEPLOY_DIR)/$(CPUNAME) -f -
	touch "$(@)"

# generic rule for creating a CFS tarball
# this uses "fakeroot" so the files in the resulting tarball have root:root ownership
%.tar.xz: $(O)/stamp.install
	mkdir -p "$(dir $(@))"
	rm -f "$(@)"
	tar Jc --owner=root --group=root -f "$(abspath $(@)).tmp" -C $(O)/exe/$(CPUNAME) .
	mv -v "$(@).tmp" "$(@)"

# generic rule for creating an ext4 image from the build
# This is intended for linux so it includes everything including the cfs executable
%.ext4: $(O)/stamp.install
	mkdir -p "$(dir $(@))"
	rm -f "$(@)"
	fakeroot /usr/sbin/mke2fs -t ext4 -L "$(IMAGE_LABEL)" -d "$(O)/exe/$(CPUNAME)" "$(@).tmp" $(IMAGE_SIZE)
	mv -v "$(@).tmp" "$(@)"

# generic rule for creating a qcow overlay image from an ext4 image
%.qcow2: %.ext4
	mkdir -p "$(dir $(@))"
	rm -f "$(@)"
	cd "$(dir $(<))" && qemu-img create -o backing_file="$(notdir $(<))",backing_fmt=raw -f qcow2 "$(notdir $(@)).tmp"
	mv -v "$(@).tmp" "$(@)"

# generic rule for creating a fat filesystem image from the build
# Note that this does NOT include the cfs executable itself, just the eeprom dir
%.fat: $(O)/stamp.install
	mkdir -p "$(dir $(@))"
	truncate -s $(IMAGE_SIZE) $(@).tmp
	/usr/sbin/mkfs.fat "$(@).tmp"
	(cd $(O)/exe/$(CPUNAME) && mcopy -sv -i "$(abspath $(@).tmp)" eeprom ::)
	mv -v "$(@).tmp" "$(@)"

# generic rule for creating a disk image from a fat filesystem image
# The disk image needs to be exactly 63 512-byte sectors larger than the fat FS
# This is the traditional size of the MS-DOS partition table
%.diskimg: FS_SIZE = $(shell stat --printf="%s" "$(<)")
%.diskimg: %.fat
	mkdir -p "$(dir $(@))"
	truncate -s $$(($(FS_SIZE) + 32256)) $(@).tmp
	$(PARTED_CMD) -s $(@).tmp -- mklabel msdos
	$(PARTED_CMD) -a none -s $(@).tmp -- mkpart primary fat32 63s -1s
	dd if=$(<) of=$(@).tmp bs=512 seek=63
	mv -v "$(@).tmp" "$(@)"

# generic rule for creating a disk image from a fat filesystem image
%.diskimg: %.fat
	mkdir -p "$(dir $(@))"
	truncate -s $(IMAGE_SIZE)  $(@)
	$(PARTED_CMD) -s $(@) -- mklabel msdos
	$(PARTED_CMD) -a none -s $(@) -- mkpart primary fat32 63s -1s
	dd if=$(<) of=$(@) bs=512 seek=63

%/rtems-cfs.exe: $(O)/stamp.install
	mkdir -p "$(dir $(@))"
	cp -v "$(O)/exe/$(CPUNAME)/core-$(CPUNAME).exe" "$(@)"

$(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start: CPUNAME = cpu1
$(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start: QEMU_HYPERVISOR = qemu-system-riscv64 -M virt -m 512M -smp 4 -bios fw_jump.elf
$(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start: QEMU_KERNEL = -kernel Image -append 'root=/dev/vda mem=512M earlycon=sbi swiotlb=0'
$(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start: ROOTFS_IMAGE_BASENAME = core-image-cfecfs-qemuriscv64.rootfs
$(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start: QEMU_NETDEV_TYPE = virtio-net-device
$(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start: $(QEMU_YOCTO_DEPLOY_DIR)/cpu1/core-image-cfecfs-qemuriscv64.rootfs.$(IMAGE_TYPE)
$(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start: $(QEMU_YOCTO_DEPLOY_DIR)/cpu1/cfs-cpu1.$(IMAGE_TYPE)
$(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start: $(QEMU_YOCTO_DEPLOY_DIR)/fetch_yocto_rootfs.cpu1.stamp

$(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start: CPUNAME = cpu2
$(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start: QEMU_HYPERVISOR = qemu-system-mips -M malta -cpu 34Kf -m 256M
$(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start: QEMU_KERNEL = -kernel vmlinux
$(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start: ROOTFS_IMAGE_BASENAME = core-image-cfecfs-qemumips.rootfs
$(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start: QEMU_NETDEV_TYPE = virtio-net-pci
$(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start: $(QEMU_YOCTO_DEPLOY_DIR)/cpu2/core-image-cfecfs-qemumips.rootfs.$(IMAGE_TYPE)
$(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start: $(QEMU_YOCTO_DEPLOY_DIR)/cpu2/cfs-cpu2.$(IMAGE_TYPE)
$(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start: $(QEMU_YOCTO_DEPLOY_DIR)/fetch_yocto_rootfs.cpu2.stamp

$(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu1/container-start: CPUNAME = cpu1
$(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu1/container-start: QEMU_KERNEL = -kernel rtems-cfs.exe -append '$(RTEMS5_APPEND_OPTS)'
$(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu1/container-start: $(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu1/rtems-cfs.exe
$(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu1/container-start: $(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu1/cfs-cpu1.diskimg

$(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu2/container-start: CPUNAME = cpu2
$(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu2/container-start: QEMU_KERNEL = -kernel rtems-cfs.exe  -append '$(RTEMS5_APPEND_OPTS)'
$(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu1/container-start: $(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu2/rtems-cfs.exe
$(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu2/container-start: $(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu2/cfs-cpu2.diskimg

%/container-start: custom-rules.mk
	truncate -s 0 "$(@)"
	echo -n "$(QEMU_HYPERVISOR) " >> "$(@)"
	echo -n "$(QEMU_NET_OPTS) " >> "$(@)"
	echo -n "$(QEMU_DISK_OPTS) " >> "$(@)"
	echo -n "$(QEMU_DEVICE_OPTS) " >> "$(@)"
	echo -n "$(QEMU_KERNEL) " >> "$(@)"
	echo "" >> "$(@)"
	chmod +x "$(@)"

$(O_qemu_yocto_linux)/stamp.image: QEMU_DISK_OPTS += -drive id=disk0,file=$(ROOTFS_IMAGE_BASENAME).$(IMAGE_TYPE),format=$(IMAGE_FORMAT)
$(O_qemu_yocto_linux)/stamp.image: QEMU_DISK_OPTS += -drive id=disk1,file=$(CFS_IMAGE_BASENAME).$(IMAGE_TYPE),format=$(IMAGE_FORMAT)
$(O_qemu_yocto_linux)/stamp.image: $(QEMU_YOCTO_DEPLOY_DIR)/cpu1/container-start
$(O_qemu_yocto_linux)/stamp.image: $(QEMU_YOCTO_DEPLOY_DIR)/cpu2/container-start

$(O_pc686_rtems5)/stamp.image: QEMU_HYPERVISOR = qemu-system-i386 -m 128M -no-reboot
$(O_pc686_rtems5)/stamp.image: QEMU_NETDEV_TYPE = i82557b
$(O_pc686_rtems5)/stamp.image: QEMU_DISK_OPTS += -drive id=disk0,file=$(CFS_IMAGE_BASENAME).diskimg,format=raw
$(O_pc686_rtems5)/stamp.image: $(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu1/container-start
$(O_pc686_rtems5)/stamp.image: $(QEMU_PC686_RTEMS_DEPLOY_DIR)/cpu2/container-start

# do not auto-delete these artifacts
.PRECIOUS: %.ext4 %.qcow2
