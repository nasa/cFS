cmake_minimum_required(VERSION 2.6.4)
project(CFS_DS C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)
include_directories(fsw/src)
include_directories(fsw/tables)
include_directories(${MISSION_SOURCE_DIR}/apps/cfs_lib/fsw/public_inc)

aux_source_directory(fsw/src APP_SRC_FILES)
aux_source_directory(fsw/tables APP_TABLE_FILES)

# Create the app module
add_cfe_app(ds ${APP_SRC_FILES})
add_cfe_tables(ds ${APP_TABLE_FILES})
