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

#ifndef __COMPILER_H__
#define __COMPILER_H__

#define uswap_16(x) \
    ((((x) & 0xff00) >> 8) | \
    (((x) & 0x00ff) << 8))
#define uswap_32(x) \
    ((((x) & 0xff000000) >> 24) | \
    (((x) & 0x00ff0000) >>  8) | \
    (((x) & 0x0000ff00) <<  8) | \
    (((x) & 0x000000ff) << 24))
#define _uswap_64(x, sfx) \
    ((((x) & 0xff00000000000000##sfx) >> 56) | \
    (((x) & 0x00ff000000000000##sfx) >> 40) | \
    (((x) & 0x0000ff0000000000##sfx) >> 24) | \
    (((x) & 0x000000ff00000000##sfx) >>  8) | \
    (((x) & 0x00000000ff000000##sfx) <<  8) | \
    (((x) & 0x0000000000ff0000##sfx) << 24) | \
    (((x) & 0x000000000000ff00##sfx) << 40) | \
    (((x) & 0x00000000000000ff##sfx) << 56))

#define cpu_to_le16(x)      (x)
#define cpu_to_le32(x)      (x)
#define cpu_to_le64(x)      (x)
#define le16_to_cpu(x)      (x)
#define le32_to_cpu(x)      (x)
#define le64_to_cpu(x)      (x)
#define cpu_to_be16(x)      uswap_16(x)
#define cpu_to_be32(x)      uswap_32(x)
#define cpu_to_be64(x)      uswap_64(x)
#define be16_to_cpu(x)      uswap_16(x)
#define be32_to_cpu(x)      uswap_32(x)
#define be64_to_cpu(x)      uswap_64(x)

/* Optimization barrier */
#ifndef barrier
# define barrier() __memory_barrier()
#endif

# define __force	__attribute__((force))

#define __always_inline                 inline __attribute__((__always_inline__))

static __always_inline void __write_once_size(volatile void *p, void *res, int size)
{
	switch (size) {
	case 1: *(volatile __u8 *)p = *(__u8 *)res; break;
	case 2: *(volatile __u16 *)p = *(__u16 *)res; break;
	case 4: *(volatile __u32 *)p = *(__u32 *)res; break;
	case 8: *(volatile __u64 *)p = *(__u64 *)res; break;
	default:
		barrier();
		__builtin_memcpy((void *)p, (const void *)res, size);
		barrier();
	}
}

#define WRITE_ONCE(x, val) \
({							\
	union { typeof(x) __val; char __c[1]; } __u =	\
		{ .__val = (__force typeof(x)) (val) }; \
	__write_once_size(&(x), __u.__c, sizeof(x));	\
	__u.__val;					\
})

#endif /* __compiler_h__ */