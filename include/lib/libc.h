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

/**
 *  Mini libc implementation for tBoot. Most functions are wrapped as LIBC_FUNC
 *  as it could be possible to have more than one back-end for these.
 */

#ifndef __LIB_LIBC_H__
#define __LIB_LIBC_H__

#include <sys/types.h>
#include <sys/symbols.h>

/* va_list */
#define va_list     __builtin_va_list
#define va_start    __builtin_va_start
#define va_arg      __builtin_va_arg
#define va_end      __builtin_va_end

/* va_list */
#define va_list     __builtin_va_list
#define va_start    __builtin_va_start
#define va_arg      __builtin_va_arg
#define va_end      __builtin_va_end

/**
 *  macro which declares to the compiler that a given function is 'printf-like'
 */
#define __printflike(fmt, firstvaarg) \
        __attribute__ ((format (__printf__, fmt, firstvaarg)))

/* tiny stdio */
extern int tboot_printf     (const char *fmt, ...) __printflike(1, 2);
extern int tboot_vprintf    (const char *fmt, va_list args);
extern int tboot_vsnprintf  (char *str, size_t sz, const char *fmt, va_list args);
extern int tboot_snprintf   (char *str, size_t n, const char *fmt, ...)
                            __printflike(3, 4);

/* string */
extern size_t   strlcpy     (char *dest, const char *src, tboot_size_t size);
extern size_t   strlen      (const char *str);
extern int      strcmp      (const char *cs, const char *ct);
extern int      strncmp     (const char *s1, const char *s2, tboot_size_t n);
extern size_t   strnlen     (const char *s, tboot_size_t count);
extern char *   strrchr     (const char *s, int c);
extern char *   strchr      (const char *s, int c);
extern unsigned long strtoul (const char *nptr, char **endptr, register int base);

/* memory */
extern void *   memcpy      (void *dest, const void *src, size_t count);
extern void *   memset      (void *s, int c, size_t count);
extern void *   memchr      (const void *s, int c, size_t n);
extern void *   memmove     (void *dest, const void *src, size_t count);
extern int      memcmp      (const void *cs, const void *ct, size_t count);

/* ctype */
#define toupper(c)  ((c) - 0x20 * (((c) >= 'a') && ((c) <= 'z')))

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

extern const unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
#define isspace(c)	((__ismask(c)&(_S)) != 0)
#define isupper(c)	((__ismask(c)&(_U)) != 0)
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)

#define isblank(c)	(c == ' ' || c == '\t')



#endif /* __lib_libc_h__ */