cmake_minimum_required(VERSION 2.6.4)
project(CFS_CFS_LIB C)

include_directories(fsw/public_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(cfs_lib ${APP_SRC_FILES})
