/* OSAL coverage stub basic data types */
#ifndef _OSAL_STUB_BASETYPES_H_
#define _OSAL_STUB_BASETYPES_H_

/*
 * NOTE: These header files are intentionally _not_ overridden
 * in the replacement/override header directory, so this should
 * pull in the actual (native system) version of these files.
 *
 * It is important to pull in these definitions first before any
 * potential re-mapping (#define) statements are done.
 */

#include <stddef.h>   /* for correct size_t and ptrdiff_t types */
#include <stdint.h>   /* for correct fixed-width integer types */
#include <limits.h>   /* for correct INT_MAX, etc. */
#include <stdbool.h>  /* for correct boolean semantics */


#endif /* _OSAL_STUB_BASETYPES_H_ */

