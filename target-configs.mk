#
# Core Flight Software CMake / GNU make wrapper
#
# ABOUT THIS MAKEFILE:
# This defines the configuration targets for the multi-target build
# It is moved to a separate file to isolate this from the other logic
#
# This should reside in the same top-level directory as the main Makefile
#


# The config names is a complete list of configurations to build
CONFIG_NAMES := native_std native_eds
CONFIG_NAMES += pc686_rtems5 gr712_rtems5
CONFIG_NAMES += rpi_vxworks7 rpi_linux
CONFIG_NAMES += qemu_yocto_linux
CONFIG_NAMES += osal edslib

# The CFS config names is a subset of the CONFIG_NAMES which use CFS
# Being in this list means the standard set of CFS options are applied
NONCFS_CONFIG_NAMES := osal edslib
CFS_CONFIG_NAMES    := $(filter-out $(NONCFS_CONFIG_NAMES),$(CONFIG_NAMES))

# Define the output dir (O) for each target group
# this is required for everything listed in CONFIG_NAMES

O_native_std = build-native_std
O_native_eds = build-native_eds
O_pc686_rtems5 = build-pc686_rtems5
O_gr712_rtems5 = build-gr712_rtems5
O_rpi_vxworks7 = build-rpi_vxworks7
O_rpi_linux = build-rpi_linux
O_qemu_yocto_linux = build-qemu_yocto_linux
O_osal = build-osal
O_edslib = build-edslib

# Define the ARCH used for each target group
# this is required for everything listed in CFS_CONFIG_NAMES
ARCH_native_std = native
ARCH_native_eds = native
ARCH_pc686_rtems5 = i686-rtems5
ARCH_gr712_rtems5 = leon3-gaisler-rtems5
ARCH_rpi_vxworks7 = arm-rpi-vxworks7
ARCH_rpi_linux = arm-rpi-linux
ARCH_qemu_yocto_linux = riscv64-poky-linux

# Define extra prep options for each target group

##############
# Native build (traditional standard development config)
##############
PREP_OPTS_native_std += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_native_std += -DSIMULATION=$(ARCH)
PREP_OPTS_native_std += -DCFE_EDS_ENABLED=OFF
PREP_OPTS_native_std += -DMISSIONCONFIG=sample
PREP_OPTS_native_std += -DCMAKE_BUILD_TYPE=debug
PLATFORM_native_std  =  default_cpu1

##############
# EDS build
##############
PREP_OPTS_native_eds += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_native_eds += -DSIMULATION=$(ARCH)
PREP_OPTS_native_eds += -DCFE_EDS_ENABLED=ON
PREP_OPTS_native_eds += -DEDSLIB_PYTHON_BUILD_STANDALONE_MODULE=ON
PREP_OPTS_native_eds += -DCFE_MISSIONLIB_PYTHON_BUILD_STANDALONE_MODULE=ON
PREP_OPTS_native_eds += -DMISSIONCONFIG=sample
PREP_OPTS_native_eds += -DCMAKE_BUILD_TYPE=debug
PLATFORM_native_eds  =  default_cpu1

##############
# RTEMS builds
##############
PREP_OPTS_pc686_rtems5 += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_pc686_rtems5 += -DSIMULATION=$(ARCH)
PREP_OPTS_pc686_rtems5 += -DMISSIONCONFIG=sample
PREP_OPTS_pc686_rtems5 += -DCMAKE_BUILD_TYPE=debug
PLATFORM_pc686_rtems5  =  default_cpu1
PREP_OPTS_gr712_rtems5 += -DSIMULATION=$(ARCH)
PREP_OPTS_gr712_rtems5 += -DMISSIONCONFIG=sample
PREP_OPTS_gr712_rtems5 += -DCMAKE_BUILD_TYPE=release
PLATFORM_gr712_rtems5  =  default_cpu1

##############
# RPi builds
##############
PREP_OPTS_rpi_vxworks7 += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_rpi_vxworks7 += -DSIMULATION=$(ARCH)
PREP_OPTS_rpi_vxworks7 += -DMISSIONCONFIG=sample
PREP_OPTS_rpi_vxworks7 += -DCMAKE_BUILD_TYPE=release
PLATFORM_rpi_vxworks7  =  default_cpu1
PREP_OPTS_rpi_linux    += -DSIMULATION=$(ARCH)
PREP_OPTS_rpi_linux    += -DMISSIONCONFIG=sample
PREP_OPTS_rpi_linux    += -DCMAKE_BUILD_TYPE=release
PLATFORM_rpi_linux     =  default_cpu1

##############
# Flight-like build (embedded yocto linux targets)
##############
PREP_OPTS_qemu_yocto_linux += -DMISSIONCONFIG=sample
PREP_OPTS_qemu_yocto_linux += -DCMAKE_BUILD_TYPE=release
PLATFORM_qemu_yocto_linux  =  default_cpu1

##############
# Standalone OSAL
##############
PREP_OPTS_osal += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_osal += -DOSAL_OMIT_DEPRECATED=TRUE
PREP_OPTS_osal += -DOSAL_SYSTEM_BSPTYPE=generic-linux
PREP_OPTS_osal += -DOSAL_CONFIG_DEBUG_PERMISSIVE_MODE=on
PREP_OPTS_osal += -DOSAL_VALIDATE_API=on
PREP_OPTS_osal += -DOSAL_USER_C_FLAGS='-Wall -Werror'
PREP_OPTS_osal +=  -S "$(CURDIR)/osal"

##############
# Standalone Edslib
##############
PREP_OPTS_edslib += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_edslib += -DCMAKE_PREFIX_PATH=$(HOME)/code/local/lib/cmake
PREP_OPTS_edslib +=  -S "$(CURDIR)/tools/eds"
