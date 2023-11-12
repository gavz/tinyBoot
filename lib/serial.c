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

#include <lib/serial.h>
#include <sys/types.h>
#include <sys/errno.h>

// TMP: Need to determine which one here using a build option
#include <drivers/serial/serial-pl011.h>

#include <lib/libc.h>
#include <global.h>

DECLARE_GLOBAL_DATA_PTR;

enum serial_type;

int
serial_init (enum serial_type type)
{
    if (type == SERIAL_TYPE_SECURE) {
        pl011_serial_device_init ("serial0_s", 0x9040000, 0x16e3600, 115200);
    } else {
        pl011_serial_device_init ("serial0_ns", 0x9000000, 0x16e3600, 115200);
    }
}

int
serial_putc (const char c)
{
    struct serial_device *serial = gd->serial;
    return serial->putc (serial, c);
}

int
serial_puts (const char *s)
{
    struct serial_device *serial = gd->serial;
    return serial->puts (serial, s);
}

int
serial_getc ()
{
    struct serial_device *serial = gd->serial;
    return serial->getc (serial);
}