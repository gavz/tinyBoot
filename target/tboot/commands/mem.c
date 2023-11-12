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

#include <commands/mem.h>
#include <lib/display.h>
#include <lib/libc.h>
#include <console.h>
#include <global.h>
#include <lib/heap.h>

DECLARE_GLOBAL_DATA_PTR;

int mem_read (int argc, char **argv)
{
    void *mem;
    uint32_t addr, size;

    /* check that enough args were given */
    if (argc < 2) {
        tboot_printf ("Error: insufficinet arguments to print memory\n");
        return 0;
    }

    /* convert address and size */
    addr = strtoul (argv[1], NULL, 0);
    size = strtoul (argv[2], NULL, 0);
    mem = (void *) addr;

    tboot_printf ("Reading '%d' bytes from 0x%x\n", size, addr);
    print_formatted_hexdump (mem, addr, (size) ? size : 32);
}

int mem_write (int argc, char **argv)
{
    uint32_t addr, data;
    void *mem;

    /* check enough args were given */
    if (argc < 3) {
        tboot_printf ("Error: insufficient arguments to write memory\n");
        return 0;
    }

    /* convert address and data */
    addr = strtoul (argv[1], NULL, 0);
    data = strtoul (argv[2], NULL, 0);

    mem = (void *)(uintptr_t) addr;
    *((uint32_t *) mem) = data;
}

int mem_info (int argc, char **argv)
{
    tboot_printf ("Dumping memory regions:\n");
    tboot_mm_dump ();
}

int mem_alloc (int argc, char **argv)
{
    uint32_t size;
    void *mem;

    /* Check that enough arguments are given */
    if (argv[1]) {
        /* convert given address */
        size = strtoul (argv[1], NULL, 0);
    } else {
        size = 64;
    }

    mem = tboot_malloc (size);
    tboot_printf ("Allocated %d bytes at %p\n", size, mem);
}

int mem_free (int argc, char **argv)
{
    uint32_t addr;
    
    /* Check enough arguments are given */
    if (!argv[1]) {
        tboot_printf ("Invalid address\n");
        return 0;
    }

    addr = strtoul (argv[1], NULL, 0);
    tboot_free (addr);
}

int mem_help (int argc, char **argv)
{
    tboot_printf (
        "Usage: \n"
        "mem info               Dump memory regions\n"
        "mem read addr [count]  Read bytes (default 32) from `addr`\n"
        "mem write addr data    Write `data` to `addr`\n"
        "mem alloc [size]       Allocate a new block\n"
        "mem free addr          Free a given block\n"
        "\n"
    );
}

/* valid mem commands options */
static struct console_command_opt commands[] =
{
    { "help", mem_help },
    { "read", mem_read },
    { "write", mem_write },
    { "info", mem_info },
    { "alloc", mem_alloc },
    { "free", mem_free },
    { NULL, NULL },
};
static size_t csz = sizeof (commands) / sizeof (commands[0]);

int
tboot_command_mem (console_command_t *cmd, int argc, char **argv)
{
    console_command_parse_args (commands, csz, argc, argv);
}