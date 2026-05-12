####################################################################################
#
# Configuration options for PSP
#
# PSP has configuration options, which may vary depending on the
# specific version of PSP in use.  The complete list of PSP options,
# along with a description of each, can be found PSP source in the file:
#
#    psp/default_config.cmake
#
# A CFE framework build utilizes mostly the PSP default configuration.
# This file contains a few specific overrides for the deployment environment.
#
# ALSO NOTE: These settings can be overriden based on a CPU-specific or 
# ARCH-specific version of this file by renaming this file "default_pspconfig.cmake"
# in the form of:
#
#    ${CPU_NAME}_pspconfig.cmake or ${TOOLCHAIN_NAME}_pspconfig.cmake
#
# NOTE: When the "default_pspconfig.cmake" file exists these options will
#       apply to all builds, regardless of cpuname or toolchain file being present.
#
####################################################################################

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

#
# PSP_CONFIG_CF_VIRTUAL_MAPPING
# ----------------------------------
#
# For PSP builds this setting controls the virtual FS mapping for the
# "/cf" directory. Changing this value will override the default mapping.
#
set(PSP_CONFIG_CF_VIRTUAL_MAPPING "./cf")

