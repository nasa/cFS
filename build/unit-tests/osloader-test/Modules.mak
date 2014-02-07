#
# MakeModules.mak
#
# This makefile compiles the loadable modules for the os loader unit test
# The compile is OS specific, so it must include a makefile from the bsp directory

#
#  osal-config.mak contians BSP and OS selection
#
include ../../osal-config.mak

#
#  MODULE_SRC sets the path to the source code needed
#
MODULE_SRC = $(OSAL_SRC)/unit-tests/osloader-test

#
# Include the target OS specific rules
#
include $(OSAL_SRC)/bsp/$(BSP)/make/ut-modules.mak


