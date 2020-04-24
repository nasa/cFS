######################################################################
#
# CMAKE build recipe for PC-LINUX Board Support Package (BSP)
#
######################################################################

# NOTE: Although this is traditionally called "pc-linux", it is generic
# enough to be applied to non-PC systems running embedded Linux, such
# as Raspberry Pi, BeagleBoard, Zync, or custom hardware.

add_library(osal_pc-linux_impl OBJECT
	src/bsp_start.c
	src/bsp_voltab.c
	src/bsp_console.c
)

# OSAL needs conformance to at least POSIX.1c (aka POSIX 1995) - this includes all the
# real-time support and threading extensions.
#
# When compiling against glibc, using "_XOPEN_SOURCE=600" enables the X/Open 6 standard.
# XPG6 includes all necessary XPG5, POSIX.1c features as well as SUSv2/UNIX98 extensions.
# This OSAL implementation uses clock_nanosleep(), mq_timedreceive(), and
# mq_timedsend() which are enhancements added in the XPG6 standard.
#
# See http://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
# for a more detailed description of the feature test macros and available values
target_compile_definitions(osal_pc-linux_impl PUBLIC
    _XOPEN_SOURCE=600
)


# This BSP only works with "posix" OS layer.
# Confirming this reduces risk of accidental misconfiguration
set(OSAL_EXPECTED_OSTYPE    "posix" PARENT_SCOPE)
