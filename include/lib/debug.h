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

#ifndef __LIB_DEBUG_H__
#define __LIB_DEBUG_H__

#include <lib/libc.h>
#include <sys.h>

#define DEBUG_ACTIVE        1

/**
 * Logging output depends on the macro passed to dprintf. The different logging
 * macros have a different colour to differenciate the logs from eachother. The
 * DEBUG logs are only enable if the Build Style is set to Debug. All other
 * types will show regardless of build options.
*/

/* Log colours */
#define LOG_COLOUR_YELLOW          "\x1b[38;5;214m"
#define LOG_COLOUR_RED             "\x1b[38;5;88m"
#define LOG_COLOUR_BLUE            "\x1b[38;5;32m"
#define LOG_COLOUR_GREEN           "\x1b[32m"
#define LOG_COLOUR_RESET           "\x1b[0m"

/* Log types */
enum log_type {
    NORMAL,
    WARNING,
    CRITICAL,
    DEBUG,
};

/* Debug printf */
int dprintf (enum log_type type, const char *fmt, ...);

/* Additional debugging macros */
#define mb()                asm ("dsb #0")
#define debug_halt()        asm ("brk #1")
#define debug_barrier()     \
    asm ("nop");            \
    asm ("nop");            \
    asm ("nop");

#endif /* __lib_debug_h__ */
