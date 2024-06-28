##########################################################################
#
# CFE-specific configuration options for OSAL
#
# This file specifies the CFE-specific values for various compile options
# supported by OSAL.
#
# OSAL has many configuration options, which may vary depending on the
# specific version of OSAL in use.  The complete list of OSAL options,
# along with a description of each, can be found OSAL source in the file:
#
#    osal/default_config.cmake
#
# A CFE framework build utilizes mostly the OSAL default configuration.
# This file only contains a few specific overrides that tune for a debug
# environment, rather than a deployment environment.
#
# ALSO NOTE: There is also an arch-specific addendum to this file
# to allow further tuning on a per-arch basis, in the form of:
#
#    ${TOOLCHAIN_NAME}_osconfig.cmake
#
# See "native_osconfig.cmake" for options which apply only to "native" builds.
#
##########################################################################

#
# OSAL_CONFIG_DEBUG_PRINTF
# ------------------------
#
# For CFE builds this can be helpful during debugging as it will display more
# specific error messages for various OSAL error/warning events, such as if a
# module cannot be loaded or a file cannot be opened for some reason.
#
set(OSAL_CONFIG_DEBUG_PRINTF TRUE)
