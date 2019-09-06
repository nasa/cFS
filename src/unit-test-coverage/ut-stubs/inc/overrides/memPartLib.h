/* OSAL coverage stub replacement for memPartLib.h */
#ifndef _OSAL_STUB_MEMPARTLIB_H_
#define _OSAL_STUB_MEMPARTLIB_H_

/* ----------------------------------------- */
/* constants normally defined in memPartLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in memPartLib.h */
/* ----------------------------------------- */
typedef void* OCS_PART_ID;
typedef int OCS_STATUS;

typedef struct
{
    unsigned long numBytesFree;    /* Number of Free Bytes in Partition       */
    unsigned long numBlocksFree;   /* Number of Free Blocks in Partition      */
    unsigned long maxBlockSizeFree;/* Maximum block size that is free.        */
    unsigned long numBytesAlloc;   /* Number of Allocated Bytes in Partition  */
    unsigned long numBlocksAlloc;  /* Number of Allocated Blocks in Partition */
}  OCS_MEM_PART_STATS;



/* ----------------------------------------- */
/* prototypes normally declared in memPartLib.h */
/* ----------------------------------------- */
extern OCS_STATUS OCS_memPartShow(OCS_PART_ID partId,            /* partition ID */
                   int     type               /* 0 = statistics, 1 = statistics & list */);

extern OCS_STATUS OCS_memPartInfoGet(OCS_PART_ID          partId,    /* partition ID */
        OCS_MEM_PART_STATS * ppartStats /* partition stats structure */);



extern OCS_PART_ID OCS_memSysPartId;


#endif /* _OSAL_STUB_MEMPARTLIB_H_ */

