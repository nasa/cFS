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
# OSAL_CONFIG_MAX_CPUS
# --------------------------------
#
#  Defines the maximum number of CPU cores that the OSAL can manage and interact with.
#  This parameter dictates the sizing of internal OSAL data structures, sets the bounds 
#  for CPU-specific API checks, and determines the required bit width for operations 
#  like setting and getting Task Affinity masks.
#
#  This should be set to the highest number of logical CPU cores available on your 
#  target platform (or the maximum expected across all platforms in a multi-target 
#  mission) to ensure masks and arrays are sized correctly without wasting memory.
#
# Limits:
#   Must be an integer greater than 0. 
#
set(OSAL_CONFIG_MAX_CPUS 8)
