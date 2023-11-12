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

#include <commands/bdev.h>
#include <lib/display.h>
#include <lib/debug.h>
#include <lib/libc.h>
#include <lib/heap.h>
#include <console.h>
#include <global.h>

#include <lib/blockdev.h>

DECLARE_GLOBAL_DATA_PTR;

static void dump_bdev_info (struct blockdev *bdev)
{
    dprintf (NORMAL, "  name:   %s\n", bdev->name);
    dprintf (NORMAL, "  size:   %d (0x%x)\n", bdev->total_size, bdev->total_size);
    dprintf (NORMAL, "  blocks: %d (%d block size)\n", bdev->block_count, bdev->block_size);
}

int bdev_read (int argc, char **argv)
{
    unsigned char *buf;
    struct blockdev *bdev;
    int sector, count;
    char *name;

    name = argv[1];
    sector = strtoul (argv[2], NULL, 0);
    count = strtoul (argv[3], NULL, 0);

    bdev = blockdev_lookup (name);
    if (!bdev)
        dprintf (CRITICAL, "could not find blockdev: %s\n", name);

    dump_bdev_info (bdev);

    buf = tboot_malloc (count);
    blockdev_read (bdev, sector, count, buf);

    tboot_printf ("Read '%d' bytes from bdev: %s\n\n", count, bdev->name);
    print_formatted_hexdump (buf, sector * 512, count);
}

int bdev_info (int argc, char **argv)
{
    struct blockdev *bdev;
    int i;
    for (i = 0, bdev = gd->bdev_root; bdev; bdev = bdev->next, i++) {
        dprintf (NORMAL, "Blockdev: %d\n", i);
        dump_bdev_info (bdev);
    }
}

static int print_help (int argc, char **argv)
{
    tboot_printf (
        "Usage: \n" \
        "bdev info                          Print info about attached blockdevices\n" \
        "bdev read [bdev] [sector] [len]    Read `len` bytes from `sector` base.\n" \
        "\n"
    );
}

static struct console_command_opt commands[] =
{
    { "help", print_help },
    { "info", bdev_info },
    { "read", bdev_read },
    { NULL, NULL },
};
static size_t csz = sizeof (commands) / sizeof (commands[0]);

int
tboot_command_bdev (console_command_t *cmd, int argc, char **argv)
{
    console_command_parse_args (commands, csz, argc, argv);
}