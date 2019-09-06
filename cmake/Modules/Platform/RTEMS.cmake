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


# This creates a simple relocatable object file, not a shared library
set(CMAKE_SHARED_OBJECT_LINKER_FLAGS -r)
set(CMAKE_C_CREATE_SHARED_MODULE "<CMAKE_LINKER> <LINK_FLAGS> -o <TARGET> ${CMAKE_SHARED_OBJECT_LINKER_FLAGS} <OBJECTS> <LINK_LIBRARIES>")
set(CMAKE_CXX_CREATE_SHARED_MODULE ${CMAKE_C_CREATE_SHARED_MODULE})
set(CMAKE_C_CREATE_SHARED_LIBRARY ${CMAKE_C_CREATE_SHARED_MODULE})
set(CMAKE_CXX_CREATE_SHARED_LIBRARY ${CMAKE_C_CREATE_SHARED_MODULE})

# The link procedure to support dynamic loading using the RTEMS dlopen()
# First create a "prelink" executable using a typical link procedure
# Then run "rtems-syms" and re-link the output into a final executable
set(CMAKE_C_LINK_EXECUTABLE
    "<CMAKE_C_COMPILER> <FLAGS> -o <TARGET>-prelink <OBJECTS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES>"
    "${RTEMS_TOOLS_PREFIX}/bin/rtems-syms -v -e -c ${RTEMS_BSP_C_FLAGS} -C <CMAKE_C_COMPILER> -o <TARGET>-dl-sym.o <TARGET>-prelink"
    "<CMAKE_C_COMPILER> <FLAGS> -o <TARGET> <TARGET>-dl-sym.o <OBJECTS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES>")

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

set(CMAKE_C_FLAGS_INIT
    "-B${RTEMS_TARGET_PATH}/${RTEMS_BSP}/lib -specs bsp_specs -qrtems ${RTEMS_BSP_C_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT
    "-B${RTEMS_TARGET_PATH}/${RTEMS_BSP}/lib -specs bsp_specs -qrtems ${RTEMS_BSP_CXX_FLAGS}")

# RTEMS uses "Init" rather than "main" as its entry point
# This flag ensures that the Init symbol is not dropped at link time.
set(CMAKE_EXE_LINKER_FLAGS_INIT "-u Init")
if (RTEMS_RELOCADDR)
    set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} -Wl,-Ttext,${RTEMS_RELOCADDR}")
endif (RTEMS_RELOCADDR)

set(RTEMS TRUE)
set(UNIX TRUE)
