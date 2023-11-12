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

#include <sys/errno.h>
#include <lib/libc.h>
#include <console.h>
#include <lib/heap.h>
#include <lib/debug.h>

#include <commands/exception.h>
#include <commands/version.h>
#include <commands/drvinfo.h>
#include <commands/bootm.h>
#include <commands/bdev.h>
#include <commands/mem.h>

/* command list */
console_command_t *console_command_global_list = NULL;

console_command_t *
console_command_register (const char *name, console_command_func_t func)
{
    console_command_t *cmd, *p;

    /* allocate the command memory */
    cmd = (console_command_t *) tboot_malloc (sizeof (console_command_t));
    if (!cmd)
        return NULL;

    /* assign command properties */
    cmd->name = name;
    cmd->func = func;

    /* initialise the command list if no commands are registered yet */
    if (!console_command_global_list) {
        console_command_global_list = cmd;
        return cmd;
    }

    /* iterate the command list to check if the command is already registered */
    for (p = console_command_global_list; ; p = p->next) {
        if (!p->next) {
            p->next = cmd;
            break;
        }
    }

    return cmd;
}

int
console_command_unregister (const char *name)
{
    console_command_t *c, *last;

    last = console_command_global_list;
    for (c = last; c; c = c->next) {
        if (!strcmp (c->name, name)) {
            last->next = c->next;
            tboot_free (c);
            break;
        }
        last = c;
    }
    return 0;
}

console_command_t *
console_command_find (const char *name)
{
    console_command_t *c;
    for (c = console_command_global_list; c; c = c->next)
        if (!strcmp (c->name, name))
            return c;
    return NULL;
}

int
console_command_execute (const char *name, int argc, char **argv)
{
    console_command_t *cmd;
    cmd = console_command_find (name);
    return (cmd) ? cmd->func (cmd, argc, argv) : -EINVAL;
}

int
console_command_parse_args (struct console_command_opt *cmds, size_t csz, 
                            int argc, char **argv)
{
    struct console_command_opt ec;
    int i;

    /* if less than 2 args, invalid */
    if (argc < 2)
        goto fail;

    for (i = 0; i < csz; i++) {
        ec = cmds[i];
        if (!strcmp (argv[1], ec.name))
            goto exec;
    }

fail:
    ec = cmds[0];

exec:
    return ec.handler (argc - 1, &argv[1]);
}

int
console_init ()
{
    console_command_register ("exception", tboot_command_exception);
    console_command_register ("bootm", tboot_command_bootm);
    console_command_register ("mem", tboot_command_mem);
    console_command_register ("bdev", tboot_command_bdev);
    console_command_register ("version", tboot_command_version);
}
