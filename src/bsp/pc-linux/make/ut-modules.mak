#
#  MODULE_SRC sets the path to the source code needed
#
MODULE_SRC = $(OSAL_SRC)/unit-tests/osloader-test

default::
	mkdir -p eeprom1
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module0 -o eeprom1/MODULE0.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module1 -o eeprom1/MODULE1.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module2 -o eeprom1/MODULE2.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module3 -o eeprom1/MODULE3.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module4 -o eeprom1/MODULE4.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module5 -o eeprom1/MODULE5.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module6 -o eeprom1/MODULE6.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module7 -o eeprom1/MODULE7.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module8 -o eeprom1/MODULE8.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module9 -o eeprom1/MODULE9.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module10 -o eeprom1/MODULE10.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module11 -o eeprom1/MODULE11.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module12 -o eeprom1/MODULE12.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module13 -o eeprom1/MODULE13.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module14 -o eeprom1/MODULE14.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module15 -o eeprom1/MODULE15.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module16 -o eeprom1/MODULE16.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module17 -o eeprom1/MODULE17.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module18 -o eeprom1/MODULE18.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module19 -o eeprom1/MODULE19.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module20 -o eeprom1/MODULE20.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module21 -o eeprom1/MODULE21.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module22 -o eeprom1/MODULE22.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module23 -o eeprom1/MODULE23.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module24 -o eeprom1/MODULE24.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module25 -o eeprom1/MODULE25.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module26 -o eeprom1/MODULE26.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module27 -o eeprom1/MODULE27.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module28 -o eeprom1/MODULE28.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module29 -o eeprom1/MODULE29.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module30 -o eeprom1/MODULE30.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module31 -o eeprom1/MODULE31.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module32 -o eeprom1/MODULE32.so $(MODULE_SRC)/ut_module.c 
	gcc $(OSAL_M32) -shared -Wall -g -O0 -DMODULE_NAME=module33 -o eeprom1/MODULE33.so $(MODULE_SRC)/ut_module.c 

clean::
	rm -rf eeprom1
