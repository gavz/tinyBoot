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

#ifndef __TBOOT_CLI_H__
#define __TBOOT_CLI_H__

#include <sys/symbols.h>
#include <sys/types.h>
#include <sys/errno.h>

/* console command function ptr */
struct console_command;
typedef int (*console_command_func_t) (struct console_command *cmd, 
                                       int argc, char **argv);

/* console command */
struct console_command
{
    /* next elem and command handler */
    struct console_command  *next;
    console_command_func_t   func;

    /* name, arg count, and list of args */
    const char              *name;
    int                      argc;
    char                   **argv;
};
typedef struct console_command      console_command_t;

/* console command options */
struct console_command_opt
{
    char        *name;
    int         (*handler) (int argc, char **argv);
};

/* global command list */
extern console_command_t *EXPORT_VAR(console_command_global_list);

/* registering and unregistering commands */
int                 console_init                ();
int                 console_command_unregister  (const char *name);
console_command_t  *console_command_register    (const char *name, 
                                                 console_command_func_t func);

/* find, execute and parse */
console_command_t  *console_command_find        (const char *name);
int                 console_command_execute     (const char *name, int argc, 
                                                 char **argv);
int                 console_command_parse_args  (struct console_command_opt *cmds,
                                                 size_t csz, int argc, 
                                                 char **argv);

/* console cli initialisation */
size_t      console_parse_input_args    (char *s, size_t argsz, char **argv);
int         console_cli_start           ();


#endif /* __tboot_cli_h__ */
