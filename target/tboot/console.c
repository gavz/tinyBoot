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

#include <lib/libc.h>
#include <lib/debug.h>
#include <lib/serial.h>
#include <console.h>

#include <global.h>

DECLARE_GLOBAL_DATA_PTR;

/* console buffer size */
#define CONSOLE_BUFFER_MAX_SIZE     64
char console_buffer[CONSOLE_BUFFER_MAX_SIZE + 1];

static const char erase_seq[] = "\b \b";
static const char tab_seq[] = "        ";


static char *
console_delete_char (char *buffer, char *p, int *np)
{
    if (*np == 0)
        return p;

    *(--p);
    tboot_printf (erase_seq);
    (*np)--;
    return p;
}

static char *
console_put_char (char c)
{
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';
    tboot_printf (buf);
}

static size_t
console_readline_into_buffer (char *buffer)
{
    char *p, *p_orig;       /* current and original buffer ptr */
    int n = 0;              /* buffer index */
    char c;

    p_orig = p = buffer;

    for (;;) {
        c = serial_getc ();

        /* handle certain characters */
        switch (c) {
            /* enter/return key */
            case '\r':
            case '\n':
                *p++ = '\0';
                serial_putc ("\r\n");
                return p - p_orig;

            /* backspace */
            case 0x08:      /* backspace */
            case 0x7f:      /* DEL */
                p = console_delete_char (p_orig, p, &n);
                continue;

            /* regular character */
            default:
                console_put_char (c);
                *p++ = c;
                n++;
        }
    }
    return 0;
}

static size_t
console_readline ()
{
    console_buffer[0] = '\0';
    return console_readline_into_buffer (console_buffer);
}

size_t
conosle_parse_input_args (char *s, size_t argvsz, char *argv[])
{
    int argc = 0;

    /* split string into argv */
    while (argc < argvsz - 1) {

        /* skip whitespace */
        while (isblank(*s))
            ++s;

        if (*s == '\0')
            break;

        argv[argc++] = s;

        /* find the end of the string */
        while (*s && !isblank(*s))
            ++s;

        if (*s == '\0')
            break;
        *s++ = '\0';
    }
    argv[argc] = '\0';
    return argc; 
}

int
console_cli_start ()
{
    char lastcommand[CONSOLE_BUFFER_MAX_SIZE + 1] = { 0, };
    int err, len, argc;
    char *argv[7];

    dprintf (NORMAL, "Starting Console\n");
    for (;;) {
        tboot_printf ("tBoot> ");

        len = console_readline ();
        if (!len) {
            tboot_printf ("Error: Invalid input\n");
            continue;
        }

        /* tokenize the input */
        strlcpy (lastcommand, console_buffer, len);
        argc = conosle_parse_input_args (lastcommand, 
                        sizeof (argv) / sizeof (argv[0]), argv);

        /* try to find and execute the given command */
        tboot_printf ("\n");
        if (console_command_execute (lastcommand, argc, argv) == -EINVAL)
            tboot_printf ("Error: could not find command: %s\n", lastcommand);
    }
}
