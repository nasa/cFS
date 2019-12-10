/* OSAL coverage stub replacement for symLib.h */
#ifndef _OSAL_STUB_SYMLIB_H_
#define _OSAL_STUB_SYMLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in symLib.h */
/* ----------------------------------------- */
#define OCS_LOAD_ALL_SYMBOLS        0x1A00

/* ----------------------------------------- */
/* types normally defined in symLib.h */
/* ----------------------------------------- */
typedef int OCS_SYM_TYPE;


typedef struct OCS_SYMBOL  OCS_SYMBOL;
typedef struct OCS_SYMTAB  OCS_SYMTAB;


typedef OCS_SYMTAB * OCS_SYMTAB_ID;
typedef OCS_SYMBOL * OCS_SYMBOL_ID;

/* ----------------------------------------- */
/* prototypes normally declared in symLib.h */
/* ----------------------------------------- */

extern OCS_SYMTAB_ID OCS_sysSymTbl;


extern OCS_STATUS OCS_symFindByName(OCS_SYMTAB_ID symTblId, char * name, char ** pValue, OCS_SYM_TYPE * pType);
extern OCS_SYMBOL * OCS_symEach(OCS_SYMTAB_ID symTblId, OCS_FUNCPTR routine, int routineArg);


#endif /* _OSAL_STUB_SYMLIB_H_ */

