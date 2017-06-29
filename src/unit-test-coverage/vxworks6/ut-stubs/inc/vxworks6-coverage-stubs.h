/*
 * Supply "coverage stub" definitions and prototypes for all of the underlying
 * OS definitions and functions that the VxWorks6 OSAL uses.  The OSAL source
 * code is compiled against these stubs, rather than the real VxWorks-supplied definitions.
 *
 * PROBLEM:
 *  Some of these definitions may overlap with POSIX or other definitions supplied by
 *  the actual C library being used to run these tests, possibly even vxworks itself if
 *  it is being compiled to actually run on the target.
 *
 * WORKAROUND:
 *  All definitions here should be prefixed so the namespaces will not conflict.  A "VCS_"
 *  (vxworks coverage stub) prefix is added.
 *
 *  To build the (unmodified) OSAL code, the non-prefixed names can be #define'd to map
 *  to the prefixed name.  This should ideally be built using "-nostdinc" (or similar) to
 *  ensure that no bits of the real system C library get used during the OSAL build, but
 *  that doesn't work because we still need e.g. "real" fixed with types and va_list, etc.
 *
 *  Everywhere else, the "stubs" can use the prefixed names.  This should allow other code to
 *  simultaneously use the REAL underlying C library when needed as well as this stub-out library
 *  without having the names step on each other.
 *
 */

#ifndef _VXWORKS_COVERAGE_STUBS_H_
#define _VXWORKS_COVERAGE_STUBS_H_

#include "common_types.h"
#include <stddef.h>
#include <stdarg.h>     /* we cannot stub out va_list, va_start, va_end, etc. */

/*******************************************************************************
 * MACRO definitions normally supplied by VxWorks (any header)
 *******************************************************************************/

#define VCS_LOAD_ALL_SYMBOLS 0

#define VCS_O_RDONLY         00
#define VCS_O_WRONLY         01
#define VCS_O_RDWR           02
#define VCS_O_CREAT        0100
#define VCS_O_TRUNC       01000

#define VCS_MSG_Q_FIFO       0x0
#define VCS_MSG_Q_PRIORITY   0x1
#define VCS_WIND_MSG_Q_OPTION_MASK (VCS_MSG_Q_FIFO|VCS_MSG_Q_PRIORITY)

#define VCS_MSG_PRI_NORMAL 0
#define VCS_MSG_PRI_URGENT 1

#define VCS_M_objLib    (61 << 16)
#define VCS_S_objLib_OBJ_ID_ERROR           (VCS_M_objLib | 1)
#define VCS_S_objLib_OBJ_UNAVAILABLE        (VCS_M_objLib | 2)
#define VCS_S_objLib_OBJ_DELETED            (VCS_M_objLib | 3)
#define VCS_S_objLib_OBJ_TIMEOUT            (VCS_M_objLib | 4)
#define VCS_S_objLib_OBJ_NO_METHOD          (VCS_M_objLib | 5)

#define VCS_VX_FP_TASK       0x0008   /* execute with floating-point coprocessor support. */

#define VCS_INUM_TO_IVEC(uint32) ((VCS_VOIDFUNCPTR *) (uint32))

#define VCS_NULLDEV             0
#define VCS_DOS_CHK_ONLY        0x00000001
#define VCS_DOS_CHK_REPAIR      0x00000002
#define VCS_DOS_CHK_VERB_0      0x0000ff00 /* verbosity level/flags */
#define VCS_DOS_CHK_VERB_SILENT VCS_DOS_CHK_VERB_0
#define VCS_DOS_OPT_BLANK       0x0002  /* create a clean boot block */
#define VCS_FIOUNMOUNT          39      /* unmount disk volume */
#define VCS_FIOCHKDSK           48

#define VCS_S_IRUSR 1
#define VCS_S_IWUSR 2
#define VCS_S_IRGRP 4
#define VCS_S_IWGRP 8
#define VCS_S_IROTH 16
#define VCS_S_IWOTH 32

#define VCS_CLOCK_REALTIME 0

/*******************************************************************************
 * ENUM definitions normally supplied by VxWorks (any header)
 *******************************************************************************/

enum
{
    VCS_SEM_Q_FIFO               = 0x0,
    VCS_SEM_Q_PRIORITY           = 0x1,
    VCS_SEM_DELETE_SAFE          = 0x4,
    VCS_SEM_INVERSION_SAFE       = 0x8,
    VCS_SEM_EVENTSEND_ERR_NOTIFY = 0x10

};

/*for binary semaphores */
typedef enum {
    VCS_SEM_EMPTY =0,
    VCS_SEM_FULL =1
} VCS_SEM_B_STATE;

typedef enum
{
    VCS_ERROR = -1,
    VCS_OK    =  0
} VCS_STATUS;

enum
{
    VCS_WAIT_FOREVER = -1,
    VCS_NO_WAIT      = 0
};


/*******************************************************************************
 * TYPE definitions normally supplied by VxWorks (any header)
 *******************************************************************************/

typedef int VCS_BLK_DEV;
typedef int VCS_device_t;
typedef int VCS_timer_t;
typedef int VCS_BOOL;
typedef void* VCS_MODULE_ID;
typedef unsigned char VCS_SYM_TYPE;
typedef void* VCS_PART_ID;
typedef uint32 VCS_UINT;
typedef uint16 VCS_UINT16;
typedef uint32 VCS_UINT32;
typedef void* VCS_MSG_Q_ID;


typedef void (*VCS_FUNCPTR)(long, long, long, long, long, long, long, long, long, long);
typedef void (*VCS_VOIDFUNCPTR) (void);  /* ptr to function returning VOID */

typedef struct
{
    unsigned long numBytesFree;    /* Number of Free Bytes in Partition       */
    unsigned long numBlocksFree;   /* Number of Free Blocks in Partition      */
    unsigned long maxBlockSizeFree;/* Maximum block size that is free.        */
    unsigned long numBytesAlloc;   /* Number of Allocated Bytes in Partition  */
    unsigned long numBlocksAlloc;  /* Number of Allocated Blocks in Partition */
}  VCS_MEM_PART_STATS;


/* simplified definition that can be used in Linux and is available in VxWorks */
typedef struct VCS_semaphore /* SEMAPHORE */
{
    unsigned short  recurse;    /* 0x42: semaphore recursive take count */
} VCS_SEMAPHORE;


typedef struct VCS_semaphore * VCS_SEM_ID;


typedef struct
{
    unsigned int format;
    unsigned int group;
    char name[32];
    struct
    {
        void* bssAddr;
        unsigned long bssSize;
        unsigned long bssFlags;
        void* textAddr;
        unsigned long textSize;
        unsigned long textFlags;
        void* dataAddr;
        unsigned long dataSize;
        unsigned long dataFlags;
    } segInfo;
} VCS_MODULE_INFO;

struct VCS_statfs {
    long f_bfree;
    long f_bsize;
};

struct VCS_stat
{
    char status;
};

typedef struct VCS_symbol           /* SYMBOL - entry in symbol table */
{
    int     nameHNode;      /* hash node (must come first) */
    char        *name;          /* pointer to symbol name */
    void        *value;         /* symbol value */
    int      symRef;         /* moduleId of module, or predefined SYMREF. */
    int      group;          /* symbol group */
    int    type;           /* symbol type */
} VCS_SYMBOL;

typedef VCS_SYMBOL * VCS_SYMBOL_ID;

typedef struct VCS_symtab /* SYMTAB - symbol table */
{
    int handle; /* object maintanance */
    int nameHashId; /* hash table for names */
    int symMutex; /* symbol table mutual exclusion sem */
    int symPartId; /* memory partition id for symbols */
    unsigned char sameNameOk; /* symbol table name clash policy */
    int nsymbols; /* current number of symbols in table */
} VCS_SYMTAB;

typedef struct VCS_symtab * VCS_SYMTAB_ID;


/*******************************************************************************
 * FUNCTION PROTOTYPE definitions normally supplied by VxWorks (any header)
 *******************************************************************************/

//int dosFsVolFormat(const char *, int , void *);

VCS_MODULE_ID VCS_loadModule(int fd, unsigned int flags);
int VCS_moduleInfoGet(VCS_MODULE_ID, VCS_MODULE_INFO*);

VCS_BLK_DEV *VCS_ramDevCreate (char *ramAddr, int bytesPerSec, int secPerTrack,
                       int nSectors, int secOffset);

int VCS_statfs(char *_name, struct VCS_statfs *_pStat);

int VCS_symFindByName (VCS_SYMTAB_ID, char *, char **SymValue, VCS_SYM_TYPE *);
VCS_SYMBOL * VCS_symEach(VCS_SYMTAB_ID, VCS_FUNCPTR, int );


int VCS_timer_connect(VCS_timer_t, VCS_VOIDFUNCPTR, int);

int VCS_unldByModuleId(VCS_MODULE_ID, int);

/* Define missing functions */
VCS_SEM_ID VCS_semMCreate(int options);

VCS_STATUS VCS_semTake(VCS_SEM_ID semId, int timeout);

VCS_STATUS VCS_semGive(VCS_SEM_ID semId);

int VCS_open(const char *path, int oflag, ... );

VCS_STATUS VCS_close (int fd);

int VCS_read (int fd, char *buffer, int maxbytes);

int VCS_write(int fd, char *buffer, int nbytes);

int VCS_lseek(int fd, long offset, int whence);

VCS_STATUS VCS_cp(const char *src, const char *dest);

VCS_STATUS VCS_rmdir(const char *dirName);

VCS_STATUS VCS_intConnect(VCS_VOIDFUNCPTR * vector,       /* interrupt vector to attach to */
        VCS_VOIDFUNCPTR   routine,      /* routine to be called */
        int           parameter     /* parameter to be passed to routine */);

int VCS_intDisable(int level                      /* new interrupt bits (0x0 - 0xff00) */);

int VCS_intEnable(int level                       /* new interrupt bits (0x00 - 0xff00) */);

int VCS_intLock(void);

int VCS_intUnlock(int lockKey                     /* lock-out key returned by preceding intLock() */);

VCS_STATUS VCS_memPartShow(VCS_PART_ID partId,            /* partition ID */
                   int     type               /* 0 = statistics, 1 = statistics & list */);

VCS_STATUS VCS_memPartInfoGet(VCS_PART_ID          partId,    /* partition ID */
        VCS_MEM_PART_STATS * ppartStats /* partition stats structure */);

VCS_MSG_Q_ID VCS_msgQCreate(int maxMsgs,              /* max messages that can be queued */
                    int maxMsgLength,         /* max bytes in a message */
                    int options               /* message queue options */);

VCS_STATUS VCS_msgQDelete(VCS_MSG_Q_ID msgQId             /* message queue to delete */);

int VCS_msgQReceive(VCS_MSG_Q_ID msgQId,              /* message queue from which to receive */
                char *   buffer,              /* buffer to receive message */
                uint32   maxNBytes,           /* length of buffer */
                int      timeout              /* ticks to wait */);

VCS_STATUS VCS_msgQSend(VCS_MSG_Q_ID msgQId,              /* message queue on which to send */
                char *   buffer,              /* message to send */
                uint32   nBytes,              /* length of message */
                int      timeout,             /* ticks to wait */
                int      priority             /* MSG_PRI_NORMAL or MSG_PRI_URGENT */);

VCS_SEM_ID VCS_semBCreate(int         options,        /* semaphore options */
        VCS_SEM_B_STATE initialState    /* initial semaphore state */);

VCS_SEM_ID VCS_semCCreate(int flags, int count);

VCS_STATUS VCS_semDelete(VCS_SEM_ID semId                 /* semaphore ID to delete */);

VCS_STATUS VCS_semFlush(VCS_SEM_ID semId                  /* semaphore ID to unblock everyone for */);

int VCS_shellGenericInit(const char *, const char *, const char *, char **, int, int, int, int, int);

VCS_STATUS VCS_taskDelay(int ticks);

VCS_STATUS VCS_taskDelete(int tid                     /* task ID of task to delete */);

VCS_STATUS VCS_taskDeleteForce(int tid                /* task ID of task to delete */);

void VCS_taskExit(int code);

int VCS_taskIdSelf(void);

int VCS_taskNameToId(char *name);

VCS_STATUS VCS_taskPrioritySet(int tid,               /* task ID */
                       int newPriority        /* new priority */);

int VCS_taskSpawn(char *  name,             /* name of new task (stored at pStackBase) */
              int     priority,         /* priority of new task */
              int     options,          /* task option word */
              int     stackSize,        /* size (bytes) of stack needed plus name */
              VCS_FUNCPTR entryPt,          /* entry point of new task */
              int     arg1,             /* 1st of 10 req'd task args to pass to func */
              int     arg2,
              int     arg3,
              int     arg4,
              int     arg5,
              int     arg6,
              int     arg7,
              int     arg8,
              int     arg9,
              int     arg10);

VCS_STATUS VCS_taskVarAdd(int   tid,            /* ID of task to have new variable */
                  int * pVar            /* pointer to variable to be switched for task */);

int VCS_sysClkRateGet(void);

uint32 VCS_vxFpscrGet(void);

void VCS_vxFpscrSet (uint32 mask);

VCS_STATUS VCS_close (int fd);

VCS_BLK_DEV *VCS_ataDevCreate(int ctrl, int drive, unsigned int nBlocks, unsigned int blkOffset);
VCS_STATUS VCS_dosFsVolFormat(char *path, int opt, VCS_FUNCPTR pPromptFunc);

VCS_BLK_DEV *VCS_ramDevCreate (char *ramAddr, int bytesPerSec, int secPerTrack,
                       int nSectors, int secOffset);

VCS_device_t VCS_xbdBlkDevCreateSync  (VCS_BLK_DEV *bd, const char *name);



/* Global variables */
extern VCS_PART_ID VCS_memSysPartId;
extern int VCS_errno;


struct VCS_timespec
{
    long tv_sec;
    long tv_nsec;
};

struct VCS_itimerspec
{
    struct VCS_timespec it_value;
    struct VCS_timespec it_interval;
};

struct VCS_sigevent
{
};

typedef unsigned int VCS_clockid_t;

int VCS_mkdir(const char *pathname);
int VCS_open(const char *path, int oflag, ...);
int VCS_stat(const char *path, struct VCS_stat *buf);
int VCS_gethostname(char *name, int len);
int VCS_hostGetByName(char *host_name);
int VCS_ioctl(int d, int request, ...);
int VCS_clock_getres(VCS_clockid_t, struct VCS_timespec *);
int VCS_timer_create(VCS_clockid_t, struct VCS_sigevent *, VCS_timer_t *);
int VCS_timer_delete(VCS_timer_t);
int VCS_clock_gettime(unsigned int, struct VCS_timespec *);
int VCS_clock_settime(unsigned int, const struct VCS_timespec *);
int VCS_timer_settime(VCS_timer_t, int, const struct VCS_itimerspec*, struct VCS_itimerspec*);


typedef unsigned int VCS_mode_t;
typedef void         VCS_DIR;

size_t VCS_strlen(const char *str);
int VCS_strcmp(const char *str1, const char *str2);
char *VCS_strcpy(char *str1, const char *str2);
int VCS_vsnprintf(char *str, size_t size, const char *format, va_list ap);
char *VCS_strncpy(char *str1, const char *str2, size_t size);
char *VCS_strrchr(const char *string, int c);
char *VCS_strchr(const char *string, int c);


VCS_DIR *VCS_opendir(const char * dirName);
int VCS_closedir(VCS_DIR * dirp);

struct VCS_dirent
{
    char d_name[32];
};
struct VCS_dirent *VCS_readdir (VCS_DIR *__dirp);
void VCS_rewinddir (VCS_DIR *);


#define VCS_EXIT_SUCCESS            100
#define VCS_EXIT_FAILURE            101
int VCS_printf(const char *format, ...);
void VCS_memset(void *, int c, unsigned int size);
void VCS_exit(int c);

#define VCS_SEEK_SET             201
#define VCS_SEEK_CUR             202
#define VCS_SEEK_END             203
int VCS_remove(const char *file);
int VCS_rename(const char *oldname, const char *newname);

char *VCS_strcat(char *dest, const char *src);
char *VCS_strncat(char *dest, const char *src, unsigned int size);
int VCS_strncmp(const char *s1, const char *s2, unsigned int size);
int VCS_snprintf(char *buf, unsigned int sz, const char *format, ...);

VCS_SEM_ID VCS_semBInitialize(char *pSemMem, int options, VCS_SEM_B_STATE initialState);
#define VCS_VX_BINARY_SEMAPHORE(x) char x[4]

#endif /* _VXWORKS_COVERAGE_STUBS_H_ */
