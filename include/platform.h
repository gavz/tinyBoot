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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <sys/boot.h>

/* platform init functions that should be implemented for each */
void platform_early_init (void);    // clock, debug uart, memory regions
void platform_init (void);          // device tree discovery, nvram? 

/* platform halt/poweroff */
void platform_poweroff (void);
void platform_reset (void);
void platform_halt (void);

/* platform boot */
int platform_get_boot_device (enum boot_device *boot_device);

/* platform debug */
void platform_debug_init (void);


#endif /* __platform_h__ */