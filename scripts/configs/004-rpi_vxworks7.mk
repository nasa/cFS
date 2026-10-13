CONFIG_NAMES           += rpi_vxworks7
O_rpi_vxworks7          = build-rpi_vxworks7
ARCH_rpi_vxworks7       = arm-rpi-vxworks7
PREP_OPTS_rpi_vxworks7 += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_rpi_vxworks7 += -DSIMULATION=$(ARCH)
PREP_OPTS_rpi_vxworks7 += -DMISSIONCONFIG=sample
PREP_OPTS_rpi_vxworks7 += -DCMAKE_BUILD_TYPE=release
PLATFORM_rpi_vxworks7   = default_cpu1
