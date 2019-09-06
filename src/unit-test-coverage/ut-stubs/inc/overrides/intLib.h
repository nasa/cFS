/* OSAL coverage stub replacement for intLib.h */
#ifndef _OSAL_STUB_INTLIB_H_
#define _OSAL_STUB_INTLIB_H_

/* ----------------------------------------- */
/* constants normally defined in intLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in intLib.h */
/* ----------------------------------------- */
typedef int OCS_STATUS;
typedef void (*OCS_VOIDFUNCPTR)(void);

/* ----------------------------------------- */
/* prototypes normally declared in intLib.h */
/* ----------------------------------------- */


extern OCS_STATUS OCS_intConnect(OCS_VOIDFUNCPTR *vector, OCS_VOIDFUNCPTR routine, int parameter);
extern int OCS_intDisable(int level);
extern int OCS_intEnable(int level);
extern int OCS_intLock(void);
extern int OCS_intUnlock(int lockKey);
extern OCS_VOIDFUNCPTR * OCS_INUM_TO_IVEC(unsigned int ui);

#endif /* _OSAL_STUB_INTLIB_H_ */

