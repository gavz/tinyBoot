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

#include <lib/blockdev.h>
#include <lib/debug.h>
#include <lib/libc.h>
#include <lib/heap.h>
#include <global.h>

DECLARE_GLOBAL_DATA_PTR;

int blockdev_create (struct blockdev *bdev, const char *name, uint32_t blksize, 
                     uint32_t blkcount)
{
    bdev->name = name;

    bdev->block_size = blksize;
    bdev->block_count = blkcount;
    bdev->total_size = blksize * blkcount;

    return 0;
}

int blockdev_register (struct blockdev *bdev)
{
    bdev->next = gd->bdev_root;
    bdev->index = gd->bdev_root + 1;
    
    gd->bdev_root = bdev;
    gd->bdev_counter++;

    dprintf (DEBUG, "blockdev: registered blockdev: %s\n", bdev->name);
    return 0;
}

struct blockdev *blockdev_lookup (const char *name)
{
    struct blockdev *bdev = gd->bdev_root;

    while (bdev != NULL) {
        if (!strcmp (bdev->name, name))
            return bdev;
        bdev = bdev->next;
    }

    return 0;
}

struct blockdev *blockdev_first ()
{
    return gd->bdev_root;
}

struct blockdev *blockdev_next (struct blockdev *bdev)
{
    return bdev->next;
}

int blockdev_read (struct blockdev *bdev, int sector, int len, void *buf)
{
//    tboot_printf ("blockdev: reading '%d' bytes starting from sector '%d'\n",
//        len, sector);

    uint32_t blksize = bdev->block_size;
    uint8_t *tmp_buf[blksize];// = tboot_malloc (blksize);
    int cur, limit;

    limit = (len / blksize) ? (len / blksize) : 1;
    for (cur = 0; cur < limit; cur++) {
        bdev->read (bdev, sector + cur, &tmp_buf);
        
        if (tmp_buf) {
            //dprintf (DEBUG, "blockdev_read: copying sector '%d' to '%p'\n",
            //    sector + cur, &buf[cur * blksize]);
            memcpy (&buf[cur * blksize], &tmp_buf, blksize);
        }
    }
}

int blockdev_write (struct blockdev *bdev, int sector, int len, void *buf)
{
    return bdev->write (bdev, sector, buf);
}