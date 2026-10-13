CONFIG_NAMES           += gr712_rtems5
O_gr712_rtems5          = build-gr712_rtems5
ARCH_gr712_rtems5       = leon3-gaisler-rtems5
PREP_OPTS_gr712_rtems5 += -DSIMULATION=$(ARCH)
PREP_OPTS_gr712_rtems5 += -DMISSIONCONFIG=sample
PREP_OPTS_gr712_rtems5 += -DCMAKE_BUILD_TYPE=release
PLATFORM_gr712_rtems5   = default_cpu1
