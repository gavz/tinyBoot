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

#include <global.h>
#include <version.h>

#include <lib/blockdev.h>
#include <lib/display.h>
#include <lib/debug.h>
#include <lib/image.h>
#include <lib/heap.h>

#include <platform.h>
#include <plat/mmap.h>

#include <sys/boot.h>
#include <console.h>

DECLARE_GLOBAL_DATA_PTR;

static inline int global_data_init ();

int _tboot_main (void)
{
    /* initialise global data */
    global_data_init ();

    /* architecture init */
    arm_arch_cpu_init ();

    /* platform early init */
    platform_early_init ();
    dprintf (DEBUG, "platform early init complete\n");

    /* sys init (heap) */
    sys_init ();
    dprintf (NORMAL, "Booting tinyBoot tBoot2\n");

    /* print boot banner */
    print_boot_banner ();
    print_boot_banner_single_line ();

    /* platform late init */
    platform_late_init ();
    dprintf (DEBUG, "platform init complete\n");

    dprintf (NORMAL, "\n");
    dprintf (NORMAL, "State:  AArch64-EL%d\n", _arm64_read_currentel ());
    dprintf (NORMAL, "Serial: %s\n", gd->serial->name);
    dprintf (NORMAL, "\n");

    dprintf (DEBUG, "loaded tboot at: 0x%p\n", _tboot_main);

    /* console init & cli start */
    console_init ();
    console_cli_start ();
    console_command_execute ("bootm", NULL, NULL);

    /* platform reset, no boot device */
    dprintf (CRITICAL, "No valid boot device, resetting...\n");
    platform_reset ();
}

static inline int
global_data_init ()
{
    gd_t data;
    memset (&data, '\0', sizeof (data));
    gd = &data;
}
