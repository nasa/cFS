##########################################################################
#
# Configuration options for PSP
#
# PSP has configuration options, which may vary depending on the
# specific version of PSP in use.  The complete list of PSP options,
# along with a description of each, can be found PSP source in the file:
#
#    psp/default_config.cmake
#
# This file is an addendum to the CFE-specific overrides that will be
# used/enabled when building with the "SIMULATION=native" mode.
#
# The "default_pspconfig.cmake" file options applies to all other builds,
# regardless of toolchain in use.
#
##########################################################################


#
# PSP_CONFIG_DEBUG_PRINTF
# ------------------------
#
# For CFE builds this can be helpful during debugging as it will display more
# specific error messages for various PSP error/warning events, such as if a
# module cannot be loaded or a file cannot be opened for some reason.
#
set(PSP_CONFIG_DEBUG_PRINTF FALSE)

#
# PSP_CONFIG_DEBUG_LEVEL
# ----------------------------------
#
# When debug messages are enabled this seting controls the debug level.
#
set(PSP_CONFIG_DEBUG_LEVEL 1)

