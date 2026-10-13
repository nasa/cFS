CONFIG_NAMES               += qemu_yocto_linux
O_qemu_yocto_linux          = build-qemu_yocto_linux
ARCH_qemu_yocto_linux       = riscv64-poky-linux
PREP_OPTS_qemu_yocto_linux += -DMISSIONCONFIG=sample
PREP_OPTS_qemu_yocto_linux += -DCMAKE_BUILD_TYPE=release
PLATFORM_qemu_yocto_linux   = default_cpu1
