/* OSAL coverage stub replacement for string.h */
#ifndef _OSAL_STUB_STRING_H_
#define _OSAL_STUB_STRING_H_

/* ----------------------------------------- */
/* constants normally defined in string.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in string.h */
/* ----------------------------------------- */
typedef unsigned long   OCS_size_t;

/* ----------------------------------------- */
/* prototypes normally declared in string.h */
/* ----------------------------------------- */

extern void *OCS_memcpy (void * dest, const void * src, OCS_size_t n);
extern void *OCS_memset (void * s, int c, OCS_size_t n);
extern int OCS_strcmp (const char * s1, const char * s2);
extern char *OCS_strcpy (char * dest, const char * src);
extern OCS_size_t OCS_strlen (const char * s);
extern int OCS_strncmp (const char * s1, const char * s2, OCS_size_t n);
extern char *OCS_strncpy (char * dest, const char * src, OCS_size_t n);
extern char *OCS_strchr(const char *s, int c);
extern char *OCS_strrchr (const char * s, int c);
extern char *OCS_strcat(char *dest, const char *src);
extern char *OCS_strncat(char *dest, const char *src, OCS_size_t n);
extern char *OCS_strerror(int errnum);

#endif /* _OSAL_STUB_STRING_H_ */

