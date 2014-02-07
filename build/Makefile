include osal-config.mak

#
# all: Don't build the unit tests by default, because some targets might not support
#      all of the unit tests
#
all::
	make -C tests 
	make -C examples 
# 
# unit-tests
#
unit-tests::
	make -C unit-tests 

# 
# gcov 
#
gcov::
	make -C unit-tests gcov

#
# Clean all build dirs
#
clean::
	make -C tests clean
	make -C examples clean

clean-unit-tests::
	make -C unit-tests clean

#
# generate dependancies
#
depend::
	make -C tests depend
	make -C examples depend
	make -C unit-tests depend

#
# Do any configuration necessary after defining the target
# in osal-config.mak
#
config::
	cp -f ../src/bsp/$(BSP)/config/osconfig.h inc	

