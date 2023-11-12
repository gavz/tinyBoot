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

#include <drivers/blockdev/mem-blk.h>
#include <lib/blockdev.h>
#include <lib/debug.h>
#include <lib/heap.h>

struct mem_blockdev
{
    struct blockdev bdev;
    void *base;
};

static int mem_blockdev_read (struct mem_blockdev *dev, uint32_t sector, 
                              uint8_t *data)
{
    void *ptr;

    /* calculate the offset from the mem-blk base */
    ptr = (void *) (dev->base + (sector * dev->bdev.block_size));

    //tboot_printf ("Read sector: %d (base: %x, %x)\n", sector, dev->base, ptr);
    memcpy (data, ptr, dev->bdev.block_size);

    return 0;
}

static int mem_blockdev_write (struct mem_blockdev *dev, uint32_t sector, 
                               uint8_t *data)
{
    tboot_printf ("Write\n");
    return 0;
}

int mem_blockdev_create (const char *name, void *base,
                                      uint32_t blksize, uint32_t blkcount)
{
    struct mem_blockdev *mbdev;

    mbdev = tboot_malloc (sizeof (struct mem_blockdev));

    blockdev_create (&mbdev->bdev, name, blksize, blkcount);
    mbdev->base = base;

    mbdev->bdev.read = mem_blockdev_read;
    mbdev->bdev.write = mem_blockdev_write;

    return blockdev_register (mbdev);
}
