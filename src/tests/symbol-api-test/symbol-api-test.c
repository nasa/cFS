#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"

/* *************************************** MAIN ************************************** */
#ifdef OS_INCLUDE_MODULE_LOADER

void OS_Application_Startup(void)
{
    int32  status;
    uint32 SymAddress;

    OS_API_Init();
  
    OS_printf("Symbol and Symbol Table Dump test\n");

    /* Make the file system */
    status = OS_mkfs(0,"/ramdev0","RAM",512,2048);
    OS_printf("status after mkfs = %d\n",status);

    if ( status != OS_SUCCESS )
    {
       OS_printf("symtest Error. OS_mkfs failed\n");
       return;
    }

    status = OS_mount("/ramdev0","/ram");
    OS_printf("status after mount = %d\n",status);
 
   /*
   ** dump the symbol table with a 32768 byte limit
   */
   OS_printf("Dumping symbol table with a limit of 32768 bytes\n");
   status = OS_SymbolTableDump("/ram/SymbolTable32k.dat", 32768);
   if ( status == OS_SUCCESS )
   {
      OS_printf("Symbol table dump complete\n");
   }
   else
   {
      OS_printf("Error calling OS_SymbolTableDump\n");
   }

   /*
   ** dump the symbol table with a 128k byte limit
   */
   OS_printf("Dumping symbol table with a limit of 131072 bytes\n");
   status = OS_SymbolTableDump("/ram/SymbolTable128k.dat", 131072);
   if ( status == OS_SUCCESS )
   {
      OS_printf("Symbol table dump complete\n");
   }
   else
   {
      OS_printf("Error calling OS_SymbolTableDump\n");
   }

   /*
   ** dump the symbol table with a 512k byte limit
   */
   OS_printf("Dumping symbol table with a limit of 524288 bytes\n");
   status = OS_SymbolTableDump("/ram/SymbolTable512k.dat", 524288);
   if ( status == OS_SUCCESS )
   {
      OS_printf("Symbol table dump complete\n");
   }
   else
   {
      OS_printf("Error calling OS_SymbolTableDump\n");
   }

   /*
   ** Test the symbol lookup
   */
   status =  OS_SymbolLookup (&SymAddress, "OS_BSPMain" );
   if ( status == OS_SUCCESS )
   {
      OS_printf("SymbolLookup --> OS_BSPMain symbol = 0x%08X\n",SymAddress);     
   }
   else
   {
      OS_printf("Error calling OS_SymbolLookup\n");
   }

   /*
   ** Test a symbol lookup that does not exist
   */
   status = OS_SymbolLookup(&SymAddress, "ShouldNotExist");
   if ( status == OS_SUCCESS )
   {
      OS_printf("SymbolLookup --> ShouldNotExist symbol = 0x%08X\n",SymAddress);     
   }
   else
   {
      OS_printf("Expected Error calling OS_SymbolLookup\n");
   }

   OS_printf("Complete: Symbol Table files should be in the RAM drive ( use devs cmd )\n"); 

} /* end OS_Application Startup */

#else
void OS_Application_Startup(void)
{
   OS_printf("Loader not configured. No symbol table support\n");
}
#endif
