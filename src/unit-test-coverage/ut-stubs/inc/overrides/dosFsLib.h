/* OSAL coverage stub replacement for dosFsLib.h */
#ifndef _OSAL_STUB_DOSFSLIB_H_
#define _OSAL_STUB_DOSFSLIB_H_

/* ----------------------------------------- */
/* constants normally defined in dosFsLib.h */
/* ----------------------------------------- */
#define OCS_DOS_CHK_ONLY        0x00000001
#define OCS_DOS_CHK_REPAIR      0x00000002
#define OCS_DOS_CHK_VERB_0      0x0000ff00 /* verbosity level/flags */
#define OCS_DOS_CHK_VERB_SILENT OCS_DOS_CHK_VERB_0
#define OCS_DOS_OPT_BLANK       0x0002  /* create a clean boot block */

/* ----------------------------------------- */
/* types normally defined in dosFsLib.h */
/* ----------------------------------------- */
typedef int OCS_STATUS;
typedef int (*OCS_FUNCPTR)();

/* ----------------------------------------- */
/* prototypes normally declared in dosFsLib.h */
/* ----------------------------------------- */
extern OCS_STATUS OCS_dosFsVolFormat(char *path, int opt, OCS_FUNCPTR pPromptFunc);



#endif /* _OSAL_STUB_DOSFSLIB_H_ */

