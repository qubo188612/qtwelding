#ifndef TISTDTYPES_H
#define TISTDTYPES_H

#include <stdint.h>

#ifndef UINT_BIT
#define UINT_BIT

typedef uint32_t  Uint32;
typedef uint16_t  Uint16;
typedef uint8_t   Uint8;
/* Signed integer definitions (32bit, 16bit, 8bit) follow... */
typedef int32_t   Int32;
typedef int16_t   Int16;
typedef int8_t    Int8;

#else
#endif

#ifndef UINT_BIT64
#define UINT_BIT64
typedef uint64_t  Uint64;
typedef int64_t   Int64;
#else
#endif

#ifndef STD_

/*
 * Aliases for standard C types
 */
typedef int                 Int;        /**< Signed base integer quanity */
typedef unsigned            Uns;        /**< This is depricated support only */
typedef char                Char;       /**< Character quantity */

/* pointer to null-terminated character sequence */
typedef char                *String;    /**< Character pointer quantity */

typedef void                *Ptr;       /**< Arbitrary (void) pointer (works for pointing to any datum) */

typedef unsigned short      Bool;       /* boolean */

#endif /* STD_ */

#ifndef TRUE
#define TRUE  (1u)
#define FALSE (0u)
#endif

#endif // TISTDTYPES_H

