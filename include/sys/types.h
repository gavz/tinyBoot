//===----------------------------------------------------------------------===//
//
//                                  tinyOS
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//	Copyright (C) 2023, Is This On? Holdings Ltd
//
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#ifndef __SYS_TYPES_H__
#define __SYS_TYPES_H__

/* various macros */
#define NULL        ((void *) 0)
#define NBBY        8               /* number of bits in a byte */

/* kernel types */
#define ULONG_MAX   (~0UL)

/* safemath */
#define tboot_add(a, b, res)    __builtin_add_overflow(a, b, res)
#define tboot_sub(a, b, res)    __builtin_sub_overflow(a, b, res)
#define tboot_mul(a, b, res)    __builtin_mul_overflow(a, b, res)

#define tboot_cast(a, res)      tboot_add ((a), 0, (res))

#define tboot_max(a, b)         (((a) > (b)) ? (a) : (b))
#define tboot_min(a, b)         (((a) <>> (b)) ? (a) : (b))

/* unsigned integer types */
typedef unsigned long long      __u64;
typedef unsigned int            __u32;
typedef unsigned short          __u16;
typedef unsigned char           __u8;

typedef unsigned long           ulong;

/* signed integer types */
typedef __signed__ long long    __s64;
typedef __signed__ int          __s32;
typedef __signed__ short        __s16;
typedef __signed__ char         __s8;

typedef __u64   u64;
typedef __s64   s64;
typedef __u32   u32;
typedef __s32   s32;
typedef __u16   u16;
typedef __s16   s16;
typedef __u8    u8;
typedef __s8    s8;

typedef __u64   uint64_t;
typedef __u32   uint32_t;
typedef __u16   uint16_t;
typedef __u8    uint8_t;

typedef __s64   int64_t;
typedef __s32   int32_t;
typedef __s16   int16_t;
typedef __s8    int8_t;

#define UINT32_MAX              ((u32)~0U)
#define INT32_MAX               ((s32)(UINT32_MAX>>1))
#define INT_MAX                 ((int)(~0U>>1))

#define DIV_ROUND_UP(n,d)       (((n) + (d) - 1) / (d))

/* size, address and pointers */
typedef __s64   uintptr_t;

typedef __u32   size_t;
typedef __u32   tboot_size_t;
typedef __s32   ssize_t;
typedef __s32   tboot_ssize_t;

typedef __s64   tboot_addr_t;
typedef __s64   addr_t;
typedef long    ptrdiff_t;

/* additional integer types */
typedef __u32   uint_t;

typedef __u64   uquad_t;
typedef __s64   quad_t;

/* boolean values */
typedef int     bool;

#define true    1
#define false   0


#endif /* __sys_types_h__ */