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

#include <lib/debug.h>
#include <lib/libc.h>
#include <sys.h>

void _panic (const char *func, const char *fmt, ...)
{
    va_list ap;

    /**
     *  _panic uses the debug serial interface, since there is a non-zero chance
     *  the issue is in the serial driver.
     */
    dprintf (CRITICAL, "tinyBoot panic - " LOG_COLOUR_RESET "%s: ", func);

    /* print the panic body */
    va_start (ap, fmt);
    tboot_vprintf (fmt, ap);    /* need to remove this and use debug serial */
    va_end (ap);

    halt ();
}

void halt ()
{
    tboot_printf ("\nsystem halted.\n");
    asm ("b .");
}