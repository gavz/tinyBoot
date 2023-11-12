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
#include <sys.h>

int test_printf ()
{
    tboot_printf ("=============================\n");
    tboot_printf (" tinyLibc Testing - printf()\n");
    tboot_printf ("=============================\n\n");

    char buf[200];
    int res;

    memset(buf, 1, sizeof(buf));
	res = tboot_snprintf (buf, sizeof(buf), "%s: %04d", "test", 1);
    tboot_printf ("buf2: %s\n", buf);
	//TEST_ASSERT_STR_EQ(buf, "0001");
	//TEST_ASSERT_EQ(result, 4);

    /* pointer */
    char *test_ptr = "testing";
    tboot_printf ("ptr: %p\n", test_ptr);

    /* single string output */
    tboot_printf ("abc\n");
    tboot_printf ("%%\n");

    /* string formatting */
    tboot_printf ("%s\n", "abc");
    tboot_printf ("123%sSDF\n", "abc");
    tboot_printf ("%10s\n", "abc");
    tboot_printf ("%-10s\n", "abc");
    tboot_printf ("%1s\n", "abc");
    tboot_printf ("%-1s\n", "abc");

    /* single character */
    tboot_printf ("%c\n", "A");

    /* number formatting */
    tboot_printf ("%d\n", 0);
    tboot_printf ("%d\n", 1);
    tboot_printf ("%d\n", 1234567890);
    tboot_printf ("%d\n", -1);
    tboot_printf ("%d\n", -1234567890);
    tboot_printf ("%d\n", 2147483647);
    tboot_printf ("%d\n", -2147483647);
    tboot_printf ("%d\n", (int) -2147483647);

    tboot_printf ("%04d\n", 1);
    tboot_printf ("%04d\n", 12345);
    tboot_printf ("%04d\n", -123);
    tboot_printf ("%04d\n", -1234);
    tboot_printf ("%04d\n", -12345);

    tboot_printf ("\n");
    return 0;
}
