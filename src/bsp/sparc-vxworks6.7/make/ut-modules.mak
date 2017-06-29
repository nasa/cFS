
CC = sparc-wrs-vxworks-gcc

# This section is common to all vxworks platforms
CFLGS  = -fno-builtin -fvolatile -O0 -m32
CFLGS += -Wall -g
CFLGS += -D_EB -DENDIAN=_EB -DSOFTWARE_BIG_BIT_ORDER
CFLGS += -D_VXWORKS_OS_ -Dvxworks6 -D_EMBED_ -DTOOL_FAMILY=gnu -D_WRS_KERNEL

# Uncomment this section for Sparc platform
CFLGS += -D__SPARC__ -DTOOL=gnuv8 -DCPU=SPARC
CFLGS += -mtune=ut699 -mv8 -ffloat-int-mode

default:
	-mkdir eeprom1
	$(CC) $(CFLGS) -DMODULE_NAME=module0  -o eeprom1/MODULE0.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module1  -o eeprom1/MODULE1.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module2  -o eeprom1/MODULE2.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module3  -o eeprom1/MODULE3.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module4  -o eeprom1/MODULE4.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module5  -o eeprom1/MODULE5.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module6  -o eeprom1/MODULE6.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module7  -o eeprom1/MODULE7.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module8  -o eeprom1/MODULE8.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module9  -o eeprom1/MODULE9.o  $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 
	$(CC) $(CFLGS) -DMODULE_NAME=module10 -o eeprom1/MODULE10.o $(OSAL_SRC)/unit-tests/osloader-test/ut_module.c 

clean:
	-rm -f eeprom1/*.o
