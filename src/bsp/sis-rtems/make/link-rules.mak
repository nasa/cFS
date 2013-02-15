###############################################################################
# File: link-rules.mak
#
# Purpose:
#   Makefile for linking code and producing the OSAL Core executable image.
#
# History:
#
###############################################################################

CORE_INSTALL_FILES = $(EXE_TARGET)

##
## Linker flags that are needed
##
LDFLAGS = 

##
## Libraries to link in
##
LIBS = 


##
## Application Link Rule
##
$(APPTARGET).$(APP_EXT): $(OBJS)
	$(COMPILER) --pipe -B$(RTEMS_BSP_BASE)/sparc-rtems4.10/sis/lib/ -specs bsp_specs -qrtems   \
        -g -Wall  -O2 -g -mcpu=cypress \
         -o $(APPTARGET).$(APP_EXT) \
        $(OBJS) $(CORE_OBJS) 

