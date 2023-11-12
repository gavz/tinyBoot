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

#include <commands/exception.h>
#include <lib/libc.h>
#include <console.h>

/* Invalid register to crash the CPU */
#define INVALID_REGISTER "S3_0_C0_C7_2"

/**
 *  Command handlers
 */

static int
cause_exception_undefined (int argc, char **argv)
{
    tboot_printf ("Generating '%s' exception\n", "undefined");
    asm volatile (".word 0x00001234\n");
}

static int
cause_exception_unaligned (int argc, char **argv)
{
    tboot_printf ("Generating '%s' exception\n", "unaligned");
    uint64_t reg;
    asm ("mrs %0, "INVALID_REGISTER"\n" : "=r" (reg));
    asm volatile(
        "mov    x1, sp\n\t"
        "orr    x1, x1, #3\n\t"
        "ldar   x0, [x1]\n"
        ::: "x0", "x1");
}

static int
cause_exception_breakpoint (int argc, char **argv)
{
    tboot_printf ("Generating '%s' exception\n", "breakpoint");
    asm volatile ("brk	#123\n");
}

/**
 *  Exception command help menu
 */
static int
print_help (int argc, char **argv)
{
    tboot_printf (
        "Usage: \n" \
        "exception undefined    Generate undefined exception\n" \
        "exception unaligned    Generate unaligned exception\n" \
        "exception breakpoint   Generate breakpoint exception\n" \
        "\n"
    );
}

/* List of valid commands */
static struct console_command_opt commands[] =
{
    { "help",       print_help                  },
    { "undefined",  cause_exception_undefined   },
    { "unaligned",  cause_exception_unaligned   },
    { "breakpoint", cause_exception_breakpoint  },
    { NULL,         NULL                        },
};
static size_t csz = sizeof (commands) / sizeof (commands[0]);

/**
 *  Command handler
 */
int
tboot_command_exception (console_command_t *cmd, int argc, char **argv)
{
    console_command_parse_args (commands, csz, argc, argv);
}