# -----------------------------------------------------------------
#  CMake Module for RTEMS
#
# The cmake distribution by default does not contain a module for RTEMS
# -----------------------------------------------------------------

# Note - RTEMS does not support "shared libs" in the traditional sense,
# but it does have a loader that does static linking at runtime.
# This property is set true which allows one to use the CMake shared library logic
# But the code is otherwise built as static -- no PIC flags
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".obj")
set(CMAKE_SHARED_MODULE_SUFFIX ".obj")

# Setting all these to empty string defeats the default behavior
# of adding an -fPIC option to shared library/module code.
set(CMAKE_DL_LIBS "")
set(CMAKE_C_COMPILE_OPTIONS_PIC "")
set(CMAKE_CXX_COMPILE_OPTIONS_PIC "")

set(CMAKE_SHARED_LIBRARY_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "")
set(CMAKE_SHARED_MODULE_C_FLAGS "")
set(CMAKE_SHARED_MODULE_CXX_FLAGS "")

# Set the CMAKE_EXE_EXPORTS_C_FLAG which is for executables
# using dynamic loading/linking
set(CMAKE_EXE_EXPORTS_C_FLAG "-Wl,--export-dynamic")


# Include the system specs directly within the COMPILE_OBJECT definitions
# This way it does not need to be done via CMAKE_C_FLAGS and it simplifies
# the process in general.
set(RTEMS_SYS_SPECS_FLAGS    "-B${RTEMS_TARGET_PATH}/${RTEMS_BSP}/lib -specs bsp_specs -qrtems")

# Basic command templates for compiling C and C++ code
set(CMAKE_C_COMPILE_OBJECT   "<CMAKE_C_COMPILER>   <DEFINES> ${RTEMS_SYS_SPECS_FLAGS} ${RTEMS_BSP_C_FLAGS}   <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
set(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER> <DEFINES> ${RTEMS_SYS_SPECS_FLAGS} ${RTEMS_BSP_CXX_FLAGS} <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")


# This creates a simple relocatable object file, not a shared library
set(CMAKE_SHARED_OBJECT_LINKER_FLAGS -r)
set(CMAKE_C_CREATE_SHARED_MODULE "<CMAKE_LINKER> <LINK_FLAGS> -o <TARGET> ${CMAKE_SHARED_OBJECT_LINKER_FLAGS} <OBJECTS> <LINK_LIBRARIES>")
set(CMAKE_CXX_CREATE_SHARED_MODULE ${CMAKE_C_CREATE_SHARED_MODULE})
set(CMAKE_C_CREATE_SHARED_LIBRARY ${CMAKE_C_CREATE_SHARED_MODULE})
set(CMAKE_CXX_CREATE_SHARED_LIBRARY ${CMAKE_C_CREATE_SHARED_MODULE})

# Additional link flags for entry point and relocation address
# RTEMS uses "Init" rather than "main" as its entry point
# This flag ensures that the Init symbol is not dropped at link time.
set(RTEMS_SYS_LINKFLAGS "-u Init")
if (RTEMS_RELOCADDR)
    set(RTEMS_SYS_LINKFLAGS "${RTEMS_SYS_LINKFLAGS} -Wl,-Ttext,${RTEMS_RELOCADDR}")
endif (RTEMS_RELOCADDR)

# The link procedure to support dynamic loading using the RTEMS dlopen()
# First create a "prelink" executable using a typical link procedure
# Then run "rtems-syms" and re-link the output into a final executable
set(CMAKE_C_LINK_EXECUTABLE
    "<CMAKE_C_COMPILER> ${RTEMS_SYS_SPECS_FLAGS} ${RTEMS_BSP_C_FLAGS} <FLAGS> ${RTEMS_SYS_LINKFLAGS} -o <TARGET>-prelink <OBJECTS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES>"
    "${RTEMS_TOOLS_PREFIX}/bin/rtems-syms -v -e -c ${RTEMS_BSP_C_FLAGS} -C <CMAKE_C_COMPILER> -o <TARGET>-dl-sym.o <TARGET>-prelink"
    "<CMAKE_C_COMPILER> ${RTEMS_SYS_SPECS_FLAGS} ${RTEMS_BSP_C_FLAGS} <FLAGS> ${RTEMS_SYS_LINKFLAGS} -o <TARGET> <TARGET>-dl-sym.o <OBJECTS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES>")

set(RTEMS_TARGET_PATH
    "${RTEMS_BSP_PREFIX}/${CMAKE_SYSTEM_PROCESSOR}-rtems${CMAKE_SYSTEM_VERSION}")

set(RTEMS_TOOLS_PATH
    "${RTEMS_TOOLS_PREFIX}/${CMAKE_SYSTEM_PROCESSOR}-rtems${CMAKE_SYSTEM_VERSION}")

set(CMAKE_FIND_ROOT_PATH
    "${RTEMS_TARGET_PATH}/${RTEMS_BSP}"
    "${RTEMS_TOOLS_PATH}")

set(CMAKE_SYSTEM_PREFIX_PATH
    ${CMAKE_FIND_ROOT_PATH})

set(CMAKE_SYSTEM_INCLUDE_PATH
    "${RTEMS_TARGET_PATH}/${RTEMS_BSP}/lib/include"
    "${RTEMS_TOOLS_PATH}/include")

set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES ${CMAKE_SYSTEM_INCLUDE_PATH})
set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES ${CMAKE_SYSTEM_INCLUDE_PATH})
set(CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${RTEMS_TARGET_PATH}/${RTEMS_BSP}/lib")
set(CMAKE_SYSTEM_LIBRARY_PATH ${CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES})

set(RTEMS TRUE)
set(UNIX TRUE)
