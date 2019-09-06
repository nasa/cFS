# -----------------------------------------------------------------
#  CFE-specific CMake Module for VxWorks
#
# The cmake distribution by default may contain a module for VxWorks,
# but different link rules are needed in order to build loadable CFE apps.
# -----------------------------------------------------------------

# Note - this is not building "shared libs" in the traditional sense,
# This property is set true which allows one to use the CMake shared library logic
# But the code is otherwise built as static -- no PIC flags 
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".o")
set(CMAKE_SHARED_MODULE_SUFFIX ".o")

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
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <LINK_FLAGS> -o <TARGET> ${CMAKE_SHARED_OBJECT_LINKER_FLAGS} <OBJECTS> <LINK_LIBRARIES>")

set(VXWORKS_TARGET_PATH "${WIND_HOME}/vxworks-${CMAKE_SYSTEM_VERSION}/target")

set(CMAKE_FIND_ROOT_PATH "${VXWORKS_TARGET_PATH}")

set(CMAKE_SYSTEM_PREFIX_PATH "${VXWORKS_TARGET_PATH}")
set(CMAKE_SYSTEM_INCLUDE_PATH "${VXWORKS_TARGET_PATH}/h")
    
set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES ${CMAKE_SYSTEM_INCLUDE_PATH})
set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES ${CMAKE_SYSTEM_INCLUDE_PATH})
#set(CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${VXWORKS_TARGET_PATH}/lib")
#set(CMAKE_SYSTEM_LIBRARY_PATH ${CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES})

#set(CMAKE_C_FLAGS_INIT "${VXWORKS_BSP_C_FLAGS}")
#set(CMAKE_CXX_FLAGS_INIT "${VXWORKS_BSP_CXX_FLAGS}")

