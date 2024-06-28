######################################################################
#
# Master config file for cFS target boards
#
# This file indicates the architecture and configuration of the
# target boards that will run core flight software.
#
# The following variables are defined per board, where <cpuname>
# is a specific name within MISSION_CPUNAMES list:
#
#  MISSION_CPUNAMES : list of user-friendly cpu names.  Should be simple
#       words with no punctuation.  This MUST be specified.
#  <cpuname>_PROCESSORID : numerical identifier for the processor
#  <cpuname>_APPLIST : list of applications to build and install on the CPU.
#       These are built as dynamically-loaded applications and installed
#       as files in the non-volatile storage of the target, and loaded
#       at runtime via the startup script or commands.
#  <cpuname>_STATIC_APPLIST : list of applications to build and statically
#       link with the CFE executable.  This is similar to the "APPLIST"
#       except the application is built with STATIC linkage, and it is
#       included directly when linking the CFE core executable itself.
#       No separate application file is generated for these apps.
#  <cpuname>_STATIC_SYMLIST : list of symbols to include in the OSAL static
#       symbol lookup table.  Each entry is a comma-separated pair containing
#       the symbol name and virtual module/app name, such as
#           My_C_Function_Name,MY_APP
#       The first item must be a publicly-exposed C symbol name available to
#       the linker at static link time, generally the entry point/main function
#       of the module or library (see STATIC_APPLIST).  The second item is the
#       module name that should match the name used in the CFE startup script
#       (4th parameter).
#       IMPORTANT:  For this to work, the OS_STATIC_LOADER configuration option
#       must be specified in the osconfig.h for that CPU.
#  <cpuname>_PSP_MODULELIST : additional PSP "modules" to link into the
#       CFE executable for this target.  These can be device drivers or
#       other bits of modular PSP functionality that provide I/O or other
#       low level functions.
#  <cpuname>_FILELIST : list of extra files to copy onto the target.  No
#       modifications of the file will be made.  In order to differentiate
#       between different versions of files with the same name, priority
#       will be given to a file named <cpuname>_<filename> to be installed
#       as simply <filename> on that cpu (prefix will be removed).  These
#       files are intended to be copied to the non-volatile storage on the
#       target for use during runtime.
#  <cpuname>_EMBED_FILELIST : list of extra files which are to be converted
#       into data arrays and linked with/embedded into the CFE executable,
#       so the content of the files can be available at runtime on systems
#       that do not have run time non-volatile storage.  The format of each
#       list entry is a comma-separated pair of variable and file name:
#            VARIABLE_NAME,FILE_NAME
#       The binary contents of the file will subsequently be available as:
#            extern const char VARIABLE_NAME_DATA[] and
#            extern const unsigned long VARIABLE_NAME_SIZE
#       The same prefix-based filename mapping as used on FILELIST is also
#       employed here, allowing CPU-specific data files to be used.
#  <cpuname>_SYSTEM : the toolchain to use for building all code.  This
#       will map to a CMake toolchain file called "toolchain-<ZZZ>"
#       If not specified then it will default to "cpu<x>" so that
#       each CPU will have a dedicated toolchain file and no objects
#       will be shared across CPUs.
#       Otherwise any code built using the same toolchain may be
#       copied to multiple CPUs for more efficient builds.
#  <cpuname>_PLATFORM : configuration for the CFE core to use for this
#       cpu.  This determines the cfe_platform_cfg.h to use during the
#       build.  Multiple files/components may be concatenated together
#       allowing the config to be generated in a modular fashion.  If
#       not specified then it will be assumed as "default <cpuname>".
#

# The MISSION_NAME will be compiled into the target build data structure
# as well as being passed to "git describe" to filter the tags when building
# the version string.
SET(MISSION_NAME "SampleMission")

# SPACECRAFT_ID gets compiled into the build data structure and the PSP may use it.
# should be an integer.
SET(SPACECRAFT_ID 0x42)

# The "MISSION_CORE_MODULES" will be built and statically linked as part
# of the CFE core executable on every target.  These can be used to amend
# or override parts of the CFE core on a mission-specific basis.
#list(APPEND MISSION_CORE_MODULES mymodule)

# The "MISSION_GLOBAL_APPLIST" is a set of apps/libs that will be built
# for every defined target.  These are built as dynamic modules
# and must be loaded explicitly via startup script or command.
# This list is effectively appended to every TGTx_APPLIST in targets.cmake.
# Example:
list(APPEND MISSION_GLOBAL_APPLIST sample_app sample_lib)

# The "MISSION_GLOBAL_STATIC_APPLIST" is similar to MISSION_GLOBAL_APPLIST
# but the apps are statically linked.
# This list is effectively appended to every TGTx_STATIC_APPLIST in targets.cmake.
# Example:
#   list(APPEND MISSION_GLOBAL_STATIC_APPLIST my_static_app)

# FT_INSTALL_SUBDIR indicates where the black box test data files (lua scripts) should
# be copied during the install process.
SET(FT_INSTALL_SUBDIR "host/functional-test")

# Each target board can have its own HW arch selection and set of included apps
SET(MISSION_CPUNAMES cpu1)

SET(cpu1_PROCESSORID 1)
SET(cpu1_APPLIST ci_lab to_lab sch_lab)
SET(cpu1_FILELIST cfe_es_startup.scr)

# CPU2 example.  This is not built by default anymore but
# serves as an example of how one would configure multiple cpus.
SET(cpu2_PROCESSORID 2)
SET(cpu2_APPLIST ci_lab to_lab sch_lab)
SET(cpu2_FILELIST cfe_es_startup.scr)

