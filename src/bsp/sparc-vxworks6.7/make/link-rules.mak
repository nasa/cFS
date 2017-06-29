###############################################################################
# File: link-rules.mak
#
# Purpose:
#   Makefile for linking code and producing the OSAL Core executable image.
#
# History:
#
###############################################################################
##
## Executable target. This is target specific
##

##
## Linker flags that are needed
##
LDFLAGS = 

##
## Libraries to link in
##
LIBS = 

##
## OSAL Core Link Rule
## 
$(EXE_TARGET): $(CORE_OBJS)
	$(COMPILER) $(DEBUG_FLAGS) -r -nostdlib -o $(EXE_TARGET) $(CORE_OBJS)

##
## Application Link Rule
##
$(APPTARGET).$(APP_EXT): $(OBJS)
	$(LINKER) -r $(OBJS) $(CORE_OBJS) -o $@	

