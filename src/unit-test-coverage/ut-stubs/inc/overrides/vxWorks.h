/* OSAL coverage stub replacement for vxWorks.h */
#ifndef _OSAL_STUB_VXWORKS_H_
#define _OSAL_STUB_VXWORKS_H_

/* ----------------------------------------- */
/* constants normally defined in vxWorks.h */
/* ----------------------------------------- */
enum
{
    OCS_ERROR = -1,
    OCS_OK    =  0
};

enum
{
    OCS_WAIT_FOREVER = -1,
    OCS_NO_WAIT      = 0
};

/* Fixme: these don't all normally come from vxworks.h, just
 * being held here for now.
 */


#define OCS_NULLDEV             0
#define OCS_FIOUNMOUNT          39      /* unmount disk volume */
#define OCS_FIOCHKDSK           48



/* ----------------------------------------- */
/* types normally defined in vxWorks.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in vxWorks.h */
/* ----------------------------------------- */



#endif /* _OSAL_STUB_VXWORKS_H_ */

