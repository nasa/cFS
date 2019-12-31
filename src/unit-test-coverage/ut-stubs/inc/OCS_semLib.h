/* OSAL coverage stub replacement for semLib.h */
#ifndef _OSAL_STUB_SEMLIB_H_
#define _OSAL_STUB_SEMLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in semLib.h */
/* ----------------------------------------- */
#define OCS_VX_BINARY_SEMAPHORE(x)      OCS_SEM x[1]
#define OCS_VX_COUNTING_SEMAPHORE(x)    OCS_SEM x[1]
#define OCS_VX_MUTEX_SEMAPHORE(x)       OCS_SEM x[1]

/* ----------------------------------------- */
/* types normally defined in semLib.h */
/* ----------------------------------------- */
typedef char OCS_SEM;
typedef OCS_SEM* OCS_SEM_ID;

/*for binary semaphores */
typedef enum
{
    OCS_SEM_EMPTY = 0,
    OCS_SEM_FULL =  1
} OCS_SEM_B_STATE;

enum
{
    OCS_SEM_Q_FIFO               = 0x0,
    OCS_SEM_Q_PRIORITY           = 0x1,
    OCS_SEM_DELETE_SAFE          = 0x4,
    OCS_SEM_INVERSION_SAFE       = 0x8,
    OCS_SEM_EVENTSEND_ERR_NOTIFY = 0x10
};


/* ----------------------------------------- */
/* prototypes normally declared in semLib.h */
/* ----------------------------------------- */

extern OCS_SEM_ID OCS_semBInitialize(OCS_SEM *pSemMem, int options, OCS_SEM_B_STATE initialState);
extern OCS_SEM_ID OCS_semBCreate(int options, OCS_SEM_B_STATE initialState);
extern OCS_SEM_ID OCS_semMInitialize(OCS_SEM *pSemMem, int options);
extern OCS_SEM_ID OCS_semMCreate(int options);
extern OCS_SEM_ID OCS_semCInitialize(OCS_SEM *pSemMem, int options, int initialCount);
extern OCS_SEM_ID OCS_semCCreate(int flags, int count);

extern OCS_STATUS OCS_semDelete(OCS_SEM_ID semId);
extern OCS_STATUS OCS_semFlush(OCS_SEM_ID semId);
extern OCS_STATUS OCS_semTake(OCS_SEM_ID semId, int timeout);
extern OCS_STATUS OCS_semGive(OCS_SEM_ID semId);



#endif /* _OSAL_STUB_SEMLIB_H_ */

