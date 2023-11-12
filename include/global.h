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

#ifndef __BOOT_GLOBAL_H__
#define __BOOT_GLOBAL_H__

#include <sys.h>

/**
 *  This idea of a Global Data struct is again taken from U-boot. This struct is
 *  created in boot.c, which can then be accessed anywhere.
 * 
 */
typedef struct global_data gd_t;
struct global_data
{
    /**
     *  Flattened Device Tree pointer.
     */
    void        *fdt_blob;
    uint64_t    mem_base;

    /* serial, uart and console */
    struct serial_device     *serial_devs;      /* list of all serial devices */
    struct serial_device     *serial;           /* selected device */
    int                     (*putc) (const char c);
    int                     (*getc) ();

    /* blockdevice */
    struct blockdev     *bdev_root;
    int                  bdev_counter;

};

#define DECLARE_GLOBAL_DATA_PTR		register volatile gd_t *gd asm ("r25")


#endif /* __boot_global_h__ */