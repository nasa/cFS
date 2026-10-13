CONFIG_NAMES        += rpi_linux
O_rpi_linux          = build-rpi_linux
ARCH_rpi_linux       = arm-rpi-linux
PREP_OPTS_rpi_linux += -DSIMULATION=$(ARCH)
PREP_OPTS_rpi_linux += -DMISSIONCONFIG=sample
PREP_OPTS_rpi_linux += -DCMAKE_BUILD_TYPE=release
PLATFORM_rpi_linux   = default_cpu1
