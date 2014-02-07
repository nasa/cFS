all:
	make -C oscore-test 
	make -C osloader-test 
	make -C osloader-test -f Modules.mak
	make -C osfilesys-test 
	make -C osfile-test 
	make -C osnetwork-test 
	make -C ostimer-test

clean:
	make -C oscore-test clean
	make -C osloader-test clean
	make -C osloader-test -f Modules.mak clean 
	make -C osfilesys-test clean 
	make -C osfile-test clean
	make -C osnetwork-test clean
	make -C ostimer-test clean

depend:
	make -C oscore-test depend 
	make -C osloader-test depend 
	make -C osfilesys-test depend 
	make -C osfile-test depend 
	make -C osnetwork-test depend 
	make -C ostimer-test depend 

gcov:
	make -C oscore-test gcov 
	make -C ostimer-test gcov 
	make -C osfilesys-test gcov 
	make -C osfile-test gcov
	make -C osloader-test gcov 
	make -C osnetwork-test gcov 
