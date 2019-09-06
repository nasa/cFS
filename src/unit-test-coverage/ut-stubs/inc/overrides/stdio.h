/* OSAL coverage stub replacement for stdio.h */
#ifndef _OSAL_STUB_STDIO_H_
#define _OSAL_STUB_STDIO_H_

/* ----------------------------------------- */
/* constants normally defined in stdio.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in stdio.h */
/* ----------------------------------------- */

typedef void            OCS_FILE;
typedef void*           OCS_va_list;
typedef unsigned long   OCS_size_t;


/* ----------------------------------------- */
/* prototypes normally declared in stdio.h */
/* ----------------------------------------- */

extern int OCS_fclose (OCS_FILE * stream);
extern char *OCS_fgets (char * s, int n, OCS_FILE * stream);
extern OCS_FILE *OCS_fopen (const char * filename, const char * modes);
extern int OCS_fputs (const char * s, OCS_FILE * stream);
extern int OCS_remove (const char * filename);
extern int OCS_rename (const char * old, const char * nw);
extern int OCS_snprintf (char * s, OCS_size_t maxlen, const char * format, ...);
extern int OCS_vsnprintf (char * s, OCS_size_t maxlen, const char * format, OCS_va_list arg);
extern int OCS_printf (const char * format, ...);
extern int OCS_putchar (int c);

extern OCS_FILE*    OCS_stdin;
extern OCS_FILE*    OCS_stdout;
extern OCS_FILE*    OCS_stderr;


#endif /* _OSAL_STUB_STDIO_H_ */

