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

#ifndef __LIB_SERIAL_H__
#define __LIB_SERIAL_H__

#include <sys/types.h>

/**
 *  Serial Device Driver structure
 */
struct serial_device
{
    const char          *name;

    /* serial operations */
    int (*getc) (struct serial_device *dev);
    int (*putc) (struct serial_device *dev, const char c);
    int (*puts) (struct serial_device *dev, const char *s);

    /* next */
    struct blockdev     *next;
};

enum serial_type
{
    SERIAL_TYPE_SECURE = 0,
    SERIAL_TYPE_NON_SECURE,
};

/* serial initialisation */
int serial_init (enum serial_type type);

/* serial operations */
int serial_getc ();
int serial_putc (const char c);
int serial_puts (const char *s);


#endif /* __sys_serial_h__ */