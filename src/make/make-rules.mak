##
##
## Common Make Rules for building OSAL core subsystems
## This file is included by the individual  Core subsystems ( bsp, os, hal )
##
##  

##
## The default "make" target is the subsystem object module.
## 
default::$(TARGET)

##
## Compiler rule
##
.c.o:
	$(COMPILER) $(LOCAL_COPTS) $(INCLUDE_PATH) $(COPTS) $(DEBUG_OPTS) -DOS_DEBUG_LEVEL=$(DEBUG_LEVEL) -c -o $@ $<

## 
## Assembly Code Rule
##
.s.o:
	$(COMPILER) $(LOCAL_COPTS) $(INCLUDE_PATH) $(ASOPTS) $(COPTS) $(DEBUG_OPTS) -DOS_DEBUG_LEVEL=$(DEBUG_LEVEL) -c -o $@ $<

##
## Rule to generate the dependancy list
##
depend: $(SOURCES)
	$(COMPILER) -MM $(LOCAL_COPTS) $(INCLUDE_PATH) $(COPTS_D) $(DEBUG_OPTS) -DOS_DEBUG_LEVEL=$(DEBUG_LEVEL) -c $^ > $(SUBSYS).d


##
## Subsystem Link rule
## 
$(TARGET): $(OBJS)
	$(LINKER) -o $(TARGET) -r $(OBJS)

##
## Make clean rule
##
clean:
	-$(RM) *.o
	-$(RM) *.lis
	-$(RM) *.g*
	-$(RM) *.d

#
# Include the dependancy list
#
-include $(SUBSYS).d

