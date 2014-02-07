###############################################################################
# File: app-rules.mak
#
# Purpose: OSAL Application make rules. This file contains common make rules to
#          be included by application make files. 
#
#
# History:
#
###############################################################################

##
## Default rule for building the App Loadable file
##
default: $(APPTARGET).$(APP_EXT)

##
## Default Compiler rule
##
.c.o:
	$(COMPILER) $(LOCAL_COPTS) $(INCLUDE_PATH) $(COPTS) $(DEBUG_OPTS) -DOS_DEBUG_LEVEL=$(DEBUG_LEVEL) -c -o $@ $<

## 
## Default Assembly Code Rule
##
.s.o:
	$(COMPILER) $(LOCAL_COPTS) $(INCLUDE_PATH) $(ASOPTS) $(COPTS) $(DEBUG_OPTS) -DOS_DEBUG_LEVEL=$(DEBUG_LEVEL) -c -o $@ $<

##
## Rule to generate the dependancy list
##
depend: $(SOURCES)
	$(COMPILER) -MM $(LOCAL_COPTS) $(INCLUDE_PATH) $(COPTS_D) $(DEBUG_OPTS) -DOS_DEBUG_LEVEL=$(DEBUG_LEVEL) -c $^ > $(APPTARGET).d

##
## Include the rule for linking the application.
## This is architecture/compiler/os specific, so it must be included from the ARCH directory
##
include $(OSAL_SRC)/bsp/$(BSP)/make/link-rules.mak

##
## Default rule for installing app
##
install:
	$(CP) $(APPTARGET).$(APP_EXT) $(EXEDIR)

##
## Rule for cleaning
## All of the possible extentions are included here so we can clean up after
## an old build from a different architecture.
##
clean::
	-$(RM) *.o
	-$(RM) *.g*
	-$(RM) *.lis
	-$(RM) *.exe
	-$(RM) *.nxe
	-$(RM) *.elf
	-$(RM) *.bin
	-$(RM) *.d


##
## gcov rule
##
gcov:
	@echo
	@echo "Running OSAL unit test: $@"
	@echo
	./$(APPTARGET).$(APP_EXT)
	@rm -f gmon.out
	@echo
	@gcov   os*.gcda | sed 'N;s/\n/ /' | \
                sed -n '/File/p' | sed '/ads/d'  | \
                sed 's/ Lines executed:/ /; s/File/gcov:/; s/of//'
	@echo


#
# Include the dependancy list
#
-include $(APPTARGET).d



# eof
