##########################################################################
#
# CPU/Arch-specific configuration options for OSAL
#
# This file specifies the CFE-specific values for various compile options
# that are used only when compiling using a specific toolchain.
#
# OSAL has many configuration options, which may vary depending on the
# specific version of OSAL in use.  The complete list of OSAL options,
# along with a description of each, can be found OSAL source in the file:
#
#    osal/default_config.cmake
#
# This file is an addendum to the CFE-specific overrides that will be
# used/enabled when building with the "SIMULATION=native" mode.
#
# See "default_osconfig.cmake" for options which apply only to all builds,
# regardless of toolchain in use.
#
##########################################################################

#
# OSAL_CONFIG_DEBUG_PERMISSIVE_MODE
# ---------------------------------
#
# When building with SIMULATION=native, enable the PERMISSIVE option,
# which allows for easier testing.  This option causes the OSAL to
# continue through certain privileged operations (ignores errors) when
# running as a standard/non-root user.
#
# Typically a regular user on a default Linux workstation configuration
# would not have permission to create realtime priority threads or FIFO
# queues deeper than the soft limit in /proc/sys/fs/mqueue/msg_max.
#
# Note that even with this enabled, OSAL will still _attempt_ to create
# resources as requested, this only makes it so the overall request will
# continue, regardless of whether the privileged operation succeeded or not.
#
set(OSAL_CONFIG_DEBUG_PERMISSIVE_MODE TRUE)


#
# OSAL_CONFIG_UTILITYTASK_PRIORITY
# --------------------------------
#
# Elevate the priority level of the console output helper task
#
# By default OSAL uses a low-priority utility task to write
# "OS_printf" messages in a deferred manner. However this deferred
# write can potentially cause the messages to appear on the console
# out of sync with the events they are related to.
#
# Raising the priority of this task from its default to be _higher_
# than the CFE core tasks means that OS_printf() messages should
# appear on the console in a timely manner, which helps during debug.
# However for a flight deployment this may cause undesired delays.
#
set(OSAL_CONFIG_UTILITYTASK_PRIORITY 10)
