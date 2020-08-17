###############################################################################
# File: link-rules.mak
#
# Purpose:
#   Makefile for linking code and producing the cFE Core executable image.
#
# History:
#
###############################################################################
##
## Executable target. This is target specific
##
EXE_TARGET=cfe-core.o

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
## cFE Core Link Rule
## 
$(EXE_TARGET): $(CORE_OBJS)
	$(COMPILER) $(DEBUG_FLAGS) -r -nostdlib -o $(EXE_TARGET) $(CORE_OBJS)

##
## Application Link Rule
##
$(APPTARGET).$(APP_EXT): $(OBJS)
	$(LINKER) -r $(OBJS) -o $@	

