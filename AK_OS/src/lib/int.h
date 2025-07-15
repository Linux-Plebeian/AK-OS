#ifndef STDINT_H
#define STDINT_H

/* Boolean type support */
#ifndef __cplusplus
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
typedef unsigned char       bool;
#define true                1
#define false               0
#define __bool_true_false_are_defined 1
#else
#include <stdbool.h>
#endif
#endif

/* Exact-width integer types */
typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
typedef unsigned int        uint32_t;
typedef long long           int64_t;
typedef unsigned long long  uint64_t;

/* Minimum-width integer types */
typedef int8_t              int_least8_t;
typedef uint8_t             uint_least8_t;
typedef int16_t             int_least16_t;
typedef uint16_t            uint_least16_t;
typedef int32_t             int_least32_t;
typedef uint32_t            uint_least32_t;
typedef int64_t             int_least64_t;
typedef uint64_t            uint_least64_t;

/* Fastest minimum-width integer types */
typedef int8_t              int_fast8_t;
typedef uint8_t             uint_fast8_t;
typedef int32_t             int_fast16_t;
typedef uint32_t            uint_fast16_t;
typedef int32_t             int_fast32_t;
typedef uint32_t            uint_fast32_t;
typedef int64_t             int_fast64_t;
typedef uint64_t            uint_fast64_t;

/* Integer types capable of holding object pointers */
#if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
typedef int64_t             intptr_t;
typedef uint64_t            uintptr_t;
#else
typedef int32_t             intptr_t;
typedef uint32_t            uintptr_t;
#endif

/* Greatest-width integer types */
typedef int64_t             intmax_t;
typedef uint64_t            uintmax_t;

/* Far pointer types for segmented architectures */
#if defined(__TURBOC__) || defined(__BORLANDC__) || defined(_MSC_VER) || defined(__WATCOMC__)
/* Memory model macros - these go between type and * */
#define far                 __far
#define near                __near
#define huge                __huge
#define FAR                 __far
#define NEAR                __near
#define HUGE                __huge
#define PASCAL              __pascal
#define CDECL               __cdecl

/* Example typedefs showing correct syntax */
typedef void far            *farptr_t;
typedef char far            *farchar_t;
typedef int far             *farint_t;
typedef long far            *farlong_t;
typedef void huge           *hugeptr_t;
typedef char huge           *hugechar_t;
typedef int huge            *hugeint_t;
typedef long huge           *hugelong_t;

/* Far pointer versions of standard types */
typedef int8_t far          *farint8_t;
typedef uint8_t far         *faruint8_t;
typedef int16_t far         *farint16_t;
typedef uint16_t far        *faruint16_t;
typedef int32_t far         *farint32_t;
typedef uint32_t far        *faruint32_t;
typedef int64_t far         *farint64_t;
typedef uint64_t far        *faruint64_t;

/* Common far pointer types */
typedef unsigned char far   *faruchar_t;
typedef signed char far     *farschar_t;
typedef unsigned short far  *farushort_t;
typedef signed short far    *farshort_t;
typedef unsigned int far    *faruint_t;
typedef signed int far      *farsint_t;
typedef unsigned long far   *farulong_t;
typedef signed long far     *farslong_t;
typedef float far           *farfloat_t;
typedef double far          *fardouble_t;

#else
/* For non-segmented architectures, far pointers are just regular pointers */
#define far
#define near
#define huge
#define FAR
#define NEAR
#define HUGE
#define PASCAL
#define CDECL

typedef void                *farptr_t;
typedef char                *farchar_t;
typedef int                 *farint_t;
typedef long                *farlong_t;
typedef void                *hugeptr_t;
typedef char                *hugechar_t;
typedef int                 *hugeint_t;
typedef long                *hugelong_t;

typedef int8_t              *farint8_t;
typedef uint8_t             *faruint8_t;
typedef int16_t             *farint16_t;
typedef uint16_t            *faruint16_t;
typedef int32_t             *farint32_t;
typedef uint32_t            *faruint32_t;
typedef int64_t             *farint64_t;
typedef uint64_t            *faruint64_t;

typedef unsigned char       *faruchar_t;
typedef signed char         *farschar_t;
typedef unsigned short      *farushort_t;
typedef signed short        *farshort_t;
typedef unsigned int        *faruint_t;
typedef signed int          *farsint_t;
typedef unsigned long       *farulong_t;
typedef signed long         *farslong_t;
typedef float               *farfloat_t;
typedef double              *fardouble_t;
#endif

/* Limits of exact-width integer types */
#define INT8_MIN            (-128)
#define INT8_MAX            127
#define UINT8_MAX           255U

#define INT16_MIN           (-32768)
#define INT16_MAX           32767
#define UINT16_MAX          65535U

#define INT32_MIN           (-2147483648L)
#define INT32_MAX           2147483647L
#define UINT32_MAX          4294967295UL

#define INT64_MIN           (-9223372036854775808LL)
#define INT64_MAX           9223372036854775807LL
#define UINT64_MAX          18446744073709551615ULL

/* Limits of minimum-width integer types */
#define INT_LEAST8_MIN      INT8_MIN
#define INT_LEAST8_MAX      INT8_MAX
#define UINT_LEAST8_MAX     UINT8_MAX

#define INT_LEAST16_MIN     INT16_MIN
#define INT_LEAST16_MAX     INT16_MAX
#define UINT_LEAST16_MAX    UINT16_MAX

#define INT_LEAST32_MIN     INT32_MIN
#define INT_LEAST32_MAX     INT32_MAX
#define UINT_LEAST32_MAX    UINT32_MAX

#define INT_LEAST64_MIN     INT64_MIN
#define INT_LEAST64_MAX     INT64_MAX
#define UINT_LEAST64_MAX    UINT64_MAX

/* Limits of fastest minimum-width integer types */
#define INT_FAST8_MIN       INT8_MIN
#define INT_FAST8_MAX       INT8_MAX
#define UINT_FAST8_MAX      UINT8_MAX

#define INT_FAST16_MIN      INT32_MIN
#define INT_FAST16_MAX      INT32_MAX
#define UINT_FAST16_MAX     UINT32_MAX

#define INT_FAST32_MIN      INT32_MIN
#define INT_FAST32_MAX      INT32_MAX
#define UINT_FAST32_MAX     UINT32_MAX

#define INT_FAST64_MIN      INT64_MIN
#define INT_FAST64_MAX      INT64_MAX
#define UINT_FAST64_MAX     UINT64_MAX

/* Limits of integer types capable of holding object pointers */
#if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
#define INTPTR_MIN          INT64_MIN
#define INTPTR_MAX          INT64_MAX
#define UINTPTR_MAX         UINT64_MAX
#else
#define INTPTR_MIN          INT32_MIN
#define INTPTR_MAX          INT32_MAX
#define UINTPTR_MAX         UINT32_MAX
#endif

/* Limits of greatest-width integer types */
#define INTMAX_MIN          INT64_MIN
#define INTMAX_MAX          INT64_MAX
#define UINTMAX_MAX         UINT64_MAX

/* Limits of other integer types */
#define PTRDIFF_MIN         INTPTR_MIN
#define PTRDIFF_MAX         INTPTR_MAX

#define SIZE_MAX            UINTPTR_MAX

#define SIG_ATOMIC_MIN      INT32_MIN
#define SIG_ATOMIC_MAX      INT32_MAX

#define WCHAR_MIN           0
#define WCHAR_MAX           UINT16_MAX

#define WINT_MIN            0
#define WINT_MAX            UINT32_MAX

/* Macros for integer constants */
#define INT8_C(x)           (x)
#define INT16_C(x)          (x)
#define INT32_C(x)          (x##L)
#define INT64_C(x)          (x##LL)

#define UINT8_C(x)          (x##U)
#define UINT16_C(x)         (x##U)
#define UINT32_C(x)         (x##UL)
#define UINT64_C(x)         (x##ULL)

#define INTMAX_C(x)         INT64_C(x)
#define UINTMAX_C(x)        UINT64_C(x)

/* Boolean constants and utilities */
#ifndef __cplusplus
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#define BOOL_TRUE           1
#define BOOL_FALSE          0
#define BOOL_MIN            0
#define BOOL_MAX            1
#endif
#endif

/* Far pointer size constants */
#if defined(__TURBOC__) || defined(__BORLANDC__) || defined(_MSC_VER) || defined(__WATCOMC__)
#define FARPTR_SIZE         4       /* Far pointers are typically 32-bit (16-bit segment + 16-bit offset) */
#define HUGEPTR_SIZE        4       /* Huge pointers are also 32-bit but can span segments */
#else
#define FARPTR_SIZE         sizeof(void*)
#define HUGEPTR_SIZE        sizeof(void*)
#endif

/* Convenience macros for memory allocation with far pointers */
#if defined(__TURBOC__) || defined(__BORLANDC__) || defined(_MSC_VER) || defined(__WATCOMC__)
#define FARMALLOC(size)     _farmalloc(size)
#define FARFREE(ptr)        _farfree(ptr)
#define HUGEFREE(ptr)       _hugefree(ptr)
#else
#define FARMALLOC(size)     malloc(size)
#define FARFREE(ptr)        free(ptr)
#define HUGEFREE(ptr)       free(ptr)
#endif

#endif /* STDINT_H */