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

#ifndef __SYS_H__
#define __SYS_H__

#include <lib/serial.h>

/* hard-coded machine/board values */
#define MACHINE_QEMU_FDT_BASE       0x40000000

/* system initialisation */
int sys_init ();

/* memory initialisation */
int sys_dram_init ();

/* panic/halt */
#define panic(x, ...)   _panic(__func__, x)

void _panic (const char *func, const char *fmt, ...) __attribute__ ((noreturn));
void halt (void) __attribute__ ((noreturn));

#endif /* __sys_h__ */