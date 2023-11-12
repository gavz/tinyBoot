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

#include <sys/types.h>
#include <lib/libc.h>

void *
memmove (void *dest, const void *src, size_t count)
{
    char *tmp, *s;

    if (dest <= src || (src + count) <= dest) {
        memcpy (dest, src, count);
    } else {
        tmp = (char *) dest + count;
        s = (char *) src + count;
        while (count--)
            *--tmp = *--s;
    }
    return dest;
}