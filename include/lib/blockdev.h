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

#ifndef __LIB_BLOCKDEV_H__
#define __LIB_BLOCKDEV_H__

#include <sys/types.h>

/**
 *  Common Block Device structure
 */
struct blockdev
{
    const char      *name;
    int              index;

    uint32_t         block_size;
    uint32_t         block_count;
    uint32_t         total_size;

    /* blockdev operations */
    int (*read) (struct blockdev *bdev, uint32_t sector, uint8_t *data);
    int (*write) (struct blockdev *bdev, uint32_t sector, uint8_t *data);

    /* next */
    struct blockdev *next;
};

struct blockdev_ops
{
    int (*read) (struct blockdev *bdev, uint32_t sector, uint8_t *data);
    int (*write) (struct blockdev *bdev, uint32_t sector, uint8_t *data);
};

int blockdev_create (struct blockdev *bdev, const char *name, uint32_t blksize, uint32_t blkcount);
int blockdev_register (struct blockdev *bdev);

struct blockdev *blockdev_lookup (const char *name);
struct blockdev *blockdev_first ();
struct blockdev *blockdev_next (struct blockdev *bdev);

/* generic read/write */
int blockdev_read (struct blockdev *bdev, int sector, int len, void *buf);
int blockdev_write (struct blockdev *bdev, int sector, int len, void *buf);

#endif /* __lib_blockdev_h__ */