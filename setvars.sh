#
# setvars.sh 
#
# This shell script is used to set the OSAL_SRC environment variable for the makefile
#
cd src
export OSAL_SRC=`pwd`

# For RTEMS targets, set the location of the RTEMS libraries.
# This allows the OSAL makefiles to remain unchanged.
# Example: standard RTEMS build/install directory:
#    export RTEMS_BSP_BASE=/opt/rtems-4.10
# The following is my personal RTEMS BSP build directory
export RTEMS_BSP_BASE="/home/acudmore/Projects/rtems-devkit/rtems-bsps/rtems-4.10"

cd ..
