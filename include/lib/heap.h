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

/**
 * tinyBoot Memory Manager / Heap implementation.
 */

#ifndef __LIB_HEAP_H__
#define __LIB_HEAP_H__

#include <sys/symbols.h>
#include <sys/types.h>

#define HEAP_MEM_ALIGN_LOG2         5
#define HEAP_MEM_ALIGN              (1 << HEAP_MEM_ALIGN_LOG2)

/* block states, use GRUBs values */
#define HEAP_BLOCKSTATE_FREE        0x2d3c2808
#define HEAP_BLOCKSTATE_ALLOC       0x6db08fa4

/* macros */
#define ALIGN_ADDR_UP(addr, align) \
	(((addr) + (typeof (addr)) (align) - 1) & ~((typeof (addr)) (align) - 1))

/* memory block header structure */
struct heap_block_header
{
    /* Next block in the current region */
    struct heap_block_header    *next;

    /**
     * Block size and current state. The block size is stored as number of cells
     * of HEAP_MEM_ALIGN bytes, rather than the actual byte count. The state can
     * be either HEAP_BLOCKSTATE_FREE or HEAP_BLOCKSTATE_ALLOC.
     */
    size_t      size;
    uint32_t    state;

    /* Additional padding */
    char        padding[8];
};

/* memory region */
struct heap_region
{
    /* First block in the region */
    struct heap_block_header    *first_block;

    /* Next region */
    struct heap_region          *next;

    /**
     *  The same issue exists in GRUB. Regions are aligned to block size, the
     *  following `pre_size` and `post_size` define the number of bytes wasted
     *  on each end of the region while trying to align it.
     */
    size_t      pre_size;
    size_t      post_size;

    /* Size of the memory region in bytes */
    size_t      size;

    /* Additional padding */
    char        padding[8];
};

/* initialise the heap/memory region */
void heap_region_init (void *addr, size_t size);
void heap_region_dump ();

void *heap_malloc (size_t size);
void *heap_calloc (size_t count, size_t size);
void heap_free (void *ptr);

/* backward-compatibility */
#define tboot_malloc(_x)        heap_malloc(_x)
#define tboot_calloc(_x, _y)    heap_calloc(_x, _y)
#define tboot_free(_x)          heap_free(_x)
#define tboot_mm_dump()         heap_region_dump()

#endif /* __lib_heap_h__ */