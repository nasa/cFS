/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_STUB_UTBITS_FILEMODES_H_
#define _OSAL_STUB_UTBITS_FILEMODES_H_

/* 
 * note these constants are used as bitmasks and so
 * they must be single bit flags to work properly.
 *
 * However, they are defined differently from a typical UNIX
 * in order to increase the likelihood that a hard-coded value
 * is caught by UT.
 *
 * These are exposed to apps through fcntl.h and sys/stat.h
 */
#define OCS_S_IXOTH     0x1000
#define OCS_S_IXGRP     0x2000
#define OCS_S_IXUSR     0x4000

#define OCS_S_IROTH     0x0100
#define OCS_S_IRGRP     0x0200
#define OCS_S_IRUSR     0x0400

#define OCS_S_IWOTH     0x0010
#define OCS_S_IWGRP     0x0020
#define OCS_S_IWUSR     0x0040

#define OCS_S_IFDIR     0x0001
#define OCS_S_ISDIR(x)  (((x) & OCS_S_IFDIR) == OCS_S_IFDIR)


#endif /* _OSAL_STUB_UTBITS_FILEMODES_H_ */

