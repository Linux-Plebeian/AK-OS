#ifndef _STDARG_H_
#define _STDARG_H_


/* Define __gnuc_va_list.  */

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

/* Note that the type used in va_arg is supposed to match the
   actual type **after default promotions**.
   Thus, va_arg (..., short) is not valid.  */

#define va_start(ap, last)	__builtin_va_start((ap), last)
#define va_end(ap)		__builtin_va_end((ap))
#define va_arg(ap, type)	__builtin_va_arg((ap), type)
#define __va_copy(dst, src)	__builtin_va_copy((dst),(src))

typedef __gnuc_va_list va_list;

#if __ISO_C_VISIBLE >= 1999
#define	va_copy(dst, src)	__va_copy((dst), (src))
#endif

#endif /* not _STDARG_H_ */