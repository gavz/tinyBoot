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
#include <lib/debug.h>
#include <lib/libc.h>
#include <global.h>

DECLARE_GLOBAL_DATA_PTR;

/* printf formatting */
#define POPT_LONG       (1 << 1)
#define POPT_LONGLONG   (1 << 2)
#define POPT_PADRIGHT   (1 << 3)

/* printf info structure */
typedef struct {
    size_t offset;
    size_t size;
    char *buf;
    void (*putc) (void *arg, char c);
} printf_info_t;

/* static function definitions */
static int internal_printf (printf_info_t *info, const char *fmt, va_list args);
static void output_stdout (void *arg, char c);
static void output_string (void *arg, char c);
static int printf_info_create (printf_info_t *info, void (*func), size_t size, 
                               char *buf);

int dprintf (enum log_type type, const char *fmt, ...)
{
    va_list args;
    int res;

    switch (type) {
        case NORMAL:
            tboot_printf (LOG_COLOUR_BLUE   "NOTICE:  " LOG_COLOUR_RESET);
            break;
        case DEBUG:
#ifdef DEBUG_ACTIVE
            tboot_printf (LOG_COLOUR_GREEN  " DEBUG:  " LOG_COLOUR_RESET);
            break;
#else
            return res;
#endif
        case WARNING:
            tboot_printf (LOG_COLOUR_YELLOW "WARNING: " LOG_COLOUR_RESET);
            break;
        case CRITICAL:
            tboot_printf (LOG_COLOUR_RED    "CRITICAL: " LOG_COLOUR_RESET);
            break;
        default:
            break;
    }

    va_start (args, fmt);
    res = tboot_vprintf (fmt, args);
    va_end (args);

    return res;
}

int tboot_printf (const char *fmt, ...)
{
    va_list args;
    int res;

    va_start (args, fmt);
    res = tboot_vprintf (fmt, args);
    va_end (args);

    return res;
}

int tboot_snprintf (char *str, size_t size, const char *fmt, ...)
{
    va_list args;
    int res;

    va_start (args, fmt);
    res = tboot_vsnprintf (str, size, fmt, args);
    va_end (args);

    return res;
}

int tboot_vprintf (const char *fmt, va_list args)
{
    printf_info_t info;
    printf_info_create (&info, output_stdout, 0, NULL);
    return internal_printf (&info, fmt, args);
}

int tboot_vsnprintf (char *str, size_t sz, const char *fmt, va_list args)
{
    printf_info_t info;
    char dummy[2];

    if (sz == 0) {
        str = dummy;
        sz = 2;
    }

    printf_info_create (&info, output_string, sz, str);
    return internal_printf (&info, fmt, args);
}

/**
 *  The following functions are for the internal operations of printf, like for
 *  converting a long long into a string.
*/

static int 
printf_info_create (printf_info_t *info, void (*func), size_t size, char *buf)
{
    info->offset = 0;
    info->size = size;
    info->buf = buf;
    info->putc = func;

    if ((info->buf != NULL) && (info->size > 0))
        info->buf[0] = '\0';
}

static void output_stdout (void *arg, char c)
{
    if (c != '\0')
        serial_putc (c);
}

static void output_string (void *arg, char c)
{
    printf_info_t *info = (printf_info_t *) arg;
    if ((info->buf != NULL) && (info->offset < (info->size - 1))) {
        info->buf[info->offset++] = c;
        if (c != '\0')
            info->buf[info->offset] = '\0';
    }
}

static char *llstr (char *buf, unsigned long long n, int len)
{
    int digit;
    int pos = len;
    int neg = 0;

    buf[--pos] = 0;
    while (n >= 10) {
        digit = n % 10;
        n /= 10;
        buf[--pos] = digit + '0';
    }
    buf[--pos] = n + '0';
    return &buf[pos];
}

static char *llhex (char *buf, unsigned long long u, int len)
{
    int pos = len;
    unsigned int d;
    static const char hextable[] = { '0', '1', '2', '3', '4', '5', '6', '7', 
                                    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    buf[--pos] = 0;
    do {
        d = u % 16;
        u /= 16;
        buf[--pos] = hextable[d];
    } while (u != 0);
    return &buf[pos];
}

static int internal_printf (printf_info_t *info, const char *fmt, va_list ap)
{
    unsigned long long n;
    const char *str;
    uint32_t opts;
    int width = 0, len = 0;
    char buf[64];
    char c;

    for (;;) {
        /**
         *  Output normal characters over serial, but break if the formatter
         *  '%' is found.
         */
        while ((c = *fmt++) != 0) {
            if (c == '%')
                break;
            info->putc (info, c);
        }

        /* check that the character is not NULL */
        if (c == 0) 
            break;

next:
        /* next character */
        c = *fmt++;
        if (c == 0)
            break;

        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                width = (width * 10) + c - '0';
                goto next;

            case '%':
                info->putc (info, '%');
                break;

            case '-':
                opts |= POPT_PADRIGHT;
                goto next;

            case 'c':
                c = va_arg (ap, unsigned int);
                info->putc (info, c);
                break;

            case 'l':
                if (opts & POPT_LONG)
                    opts |= POPT_LONGLONG;
                opts |= POPT_LONG;
                goto next;

            case 'i':
            case 'd':
                n = va_arg (ap, unsigned int);
                str = llstr (buf, n, sizeof (buf));
                goto output;

            case 'p':
            case 'x':
                n = va_arg (ap, long long);
                
                str = llhex (buf, n, sizeof (buf));
                goto output;

            case 's':
                str = va_arg (ap, const char *);
                goto output;
        }

        continue;

output:
        width -= strlen (str);
        if (!(opts & POPT_PADRIGHT))
            while (width-- > 0)
                info->putc (info, '0');

        while (*str != 0)
            info->putc (info, *str++);

        if (opts & POPT_PADRIGHT)
            while (width-- > 0)
                info->putc (info, ' ');

        width = 0;
        continue;
    }
    return width;
}
