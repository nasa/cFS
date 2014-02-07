
default::
	mkdir -p eeprom1
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module0 -o eeprom1/MODULE0.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module1 -o eeprom1/MODULE1.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module2 -o eeprom1/MODULE2.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module3 -o eeprom1/MODULE3.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module4 -o eeprom1/MODULE4.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module5 -o eeprom1/MODULE5.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module6 -o eeprom1/MODULE6.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module7 -o eeprom1/MODULE7.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module8 -o eeprom1/MODULE8.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module9 -o eeprom1/MODULE9.so $(MODULE_SRC)/ut_module.c 
	gcc -shared -Wall -g -O0 -DMODULE_NAME=module10 -o eeprom1/MODULE10.so $(MODULE_SRC)/ut_module.c 

clean::
	rm -rf eeprom1
