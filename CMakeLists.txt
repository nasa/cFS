cmake_minimum_required(VERSION 2.6.4)
project(CFS_DS C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)
include_directories(${cfs_lib_MISSION_DIR}/fsw/public_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(ds ${APP_SRC_FILES})
