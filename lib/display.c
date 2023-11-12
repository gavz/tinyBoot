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

#include <lib/display.h>
#include <sys/types.h>
#include <lib/debug.h>
#include <lib/libc.h>
#include <version.h>
#include <global.h>

DECLARE_GLOBAL_DATA_PTR;

void print_boot_banner ()
{
    dprintf (NORMAL, "================================\n");
    dprintf (NORMAL, ":: %s for %s, Copyright 2023, Is This On? Ltd.\n", 
                                TBOOT_BUILD_NAME, PLATFORM_NAME, PLATFORM_ID);
    dprintf (NORMAL, ":: %s (%s)\n", TBOOT_BUILD_VERSION, __TIMESTAMP__);
    dprintf (NORMAL, ":: Build type: %s\n", TBOOT_BUILD_STYLE);
    dprintf (NORMAL, "================================\n");
}

void print_boot_banner_single_line ()
{
    dprintf (NORMAL, "tBoot %s~%s (%s)\n",
        TBOOT_BUILD_VERSION, TBOOT_BUILD_STYLE, __TIMESTAMP__);
}

void print_formatted_hexdump (char *mem, uint32_t base, uint32_t size)
{
    uint32_t offset;
    int ws, lines, count, pos;

    offset = base;
    lines = size / 16;

    if (size % 16)
        lines += 1;

    pos = 0;
    for (int i = 0; i < lines; i++) {
        uint8_t ln[16];

        count = 16;
        if ((size - (i * 16)) / 16 == 0)
            count = size % 16;

        tboot_printf ("%08x  ", offset);
        for (int j = 0; j < count; j++) {
            uint8_t byte = (uint8_t) mem[pos];
            tboot_printf ("%02x ", byte);

            pos++;
            ln[j] = byte;
        }

        for (int k = 0, ws = 16 - count; k < ws; k++)
            tboot_printf ("   ");

        tboot_printf (" |");
        for (int l = 0; l < 16; l++) {
            if (ln[l] < 0x20 || ln[l] > 0x7e)
                tboot_printf (".");
            else
                tboot_printf ("%c", (char) ln[l]);
        }

        tboot_printf ("|\n");
        offset += 0x10;
    }
    tboot_printf ("\n");
}