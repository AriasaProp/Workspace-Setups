/*
 *  Common Header
 *
 *  Provide basic function and constant for all source code
 *
 *
 *
 */

#ifndef COMMON_INCLUDED_
#define COMMON_INCLUDED_

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <assert.h>


// ================================
//  Primitive Redefinition
// ================================
#if (defined(_MSC_VER) && _MSC_VER < 1600) /*|| defined(__SYMBIAN32__) */
typedef          __int8    int8_t;
typedef          __int16   int16_t;
typedef          __int32   int32_t;
typedef          __int64   int64_t;
typedef          __int128  int128_t;
typedef unsigned __int8 	uint8_t;
typedef unsigned __int16 	uint16_t;
typedef unsigned __int32 	uint32_t;
typedef unsigned __int64 	uint64_t;
typedef unsigned __int128 uint128_t;
typedef unsigned __int64 	size_t;
#else
#include <stdint.h>
#endif

// ================================
//  Global Macro
// ================================
#define ISSUE(X) 
#define TODO(X)               assert(0 && X)
#define PRIVATE_STRINGIFY(X)  #X
#define STRINGIFY(X)          PRIVATE_STRINGIFY(X)
#define ASSERT(X)             assert(X)

#if defined(_MSC_VER)
  #if defined(_WIN32) || defined(WIN32)
    #ifndef _CRT_SECURE_NO_WARNINGS
      #define _CRT_SECURE_NO_WARNINGS
    #endif
    #ifndef _CRT_NONSTDC_NO_DEPRECATE
      #define _CRT_NONSTDC_NO_DEPRECATE
    #endif
  #endif

  #define INLINE        __forceinline
  #define CDECL         __cdecl
  #define UNUSED(x)     ((void)x)
  #define UNUSED_ARG(x) __pragma(warning(suppress : 4100 4101)) x
#elif defined(__GNUC__) || defined(__clang__)
  #define INLINE        inline
  #define CDECL         /* no translate */
  #define UNUSED(x)     ((void)x)
  #define UNUSED_ARG(x) __attribute__((unused)) x
#else /* Unknown compiler */
  #error "Not ready for this compiler"
#endif

// clang/gcc builtin feature
#ifndef __has_builtin
  // non clang/gcc always false
  #define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_expect)
  #define IS_ERROR(x) if (__builtin_expect(!!(x), 0))
  #define LIKELY(x)   __builtin_expect(!!(x), 1)
  #define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
  #define IS_ERROR(x) if (!!(x))
  #define LIKELY(x)   (x)
  #define UNLIKELY(x) (x)
#endif

#define MIN(X,Y)  (((X) < (Y)) ? (X) : (Y))
#define MAX(X,Y)  (((X) > (Y)) ? (X) : (Y))

#ifdef __cplusplus
  #define CAST(T) (decltype(T))
#else
  #define CAST(T) (T)
#endif // __cplusplus

#endif // COMMON_INCLUDED_
