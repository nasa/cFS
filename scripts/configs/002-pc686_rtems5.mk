CONFIG_NAMES           += pc686_rtems5
O_pc686_rtems5          = build-pc686_rtems5
ARCH_pc686_rtems5       = i686-rtems5
PREP_OPTS_pc686_rtems5 += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_pc686_rtems5 += -DSIMULATION=$(ARCH)
PREP_OPTS_pc686_rtems5 += -DMISSIONCONFIG=sample
PREP_OPTS_pc686_rtems5 += -DCMAKE_BUILD_TYPE=debug
PLATFORM_pc686_rtems5   = default_cpu1
