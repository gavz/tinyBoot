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
#include <lib/heap.h>
#include <sys.h>

#include <plat/mmap.h>

/* struct sizes */
#define HEAP_REGION_OVERHEAD        sizeof (struct heap_region)
#define HEAP_BLOCK_OVERHEAD         sizeof (struct heap_block_header)

/* base memory region */
struct heap_region *heap_region_base;

/* malloc backend */
static void *heap_internal_malloc (size_t align, size_t size);

void
heap_region_init (void *addr, size_t size)
{
    struct heap_region *r, **p, *q;
    struct heap_block_header *hdr;

    /* check region is within bounds */
    if (addr < HEAP_BASE || addr > (HEAP_BASE + HEAP_SIZE))
        _panic ("heap_region_init", 
            "cannot allocate memory outside of heap bounds: 0x%x\n", addr);

    /* allocate the heap region and properly align the address */
    r = ALIGN_ADDR_UP ((addr_t) addr, HEAP_MEM_ALIGN);
    
    /* check the region is large enough */
    if (size < HEAP_MEM_ALIGN + (char *) r - (char *) addr + sizeof (*r))
        _panic ("heap_region_init", "region size less than minimum: %d\n",
            size);

    size -= (char *) r - (char *) addr + sizeof (*r);

    dprintf (DEBUG, "creating heap region: 0x%p-0x%p (%d bytes)\n",
        addr, addr + size, size);

    /**
     * while this may initially create a rather large memory block, the first
     * malloc that comes across it will resize it, therefore it doesn't matter.
     */
    hdr = (struct heap_block_header *) (r + 1);
    hdr->next = hdr;
    hdr->state = HEAP_BLOCKSTATE_FREE;
    hdr->size = size - HEAP_REGION_OVERHEAD - HEAP_BLOCK_OVERHEAD;

    /* create region struct */
    r->first_block = hdr;
    r->pre_size = (addr_t) r - (addr_t) addr;
    r->size = size;
    r->post_size = 0;

    /* find the best place to insert the region in the list */
    for (p = &heap_region_base, q = *p; q; p = &(q->next), q = *p)
        if (q->size > r->size)
            break;

    *p = r;
    r->next = q;
}

void
heap_free (void *ptr)
{
    struct heap_block_header *blk, *blk_next, *fb, *lb;
    struct heap_region *r, *lr;
    
    if (!ptr)
        _panic ("heap_free", "cannot free NULL ptr\n");

    /* find a region that contains this pointer */
    for (r = heap_region_base; r != lr; r = r->next, lr = heap_region_base)
        if (((addr_t) ptr >= (addr_t) r) && ((addr_t) ptr < (addr_t) (r + r->size)))
            break;

    /* find the block for this pointer */
    for (blk = r->first_block; blk != fb; lb = blk, blk = blk->next, fb = r->first_block) {
        if (ptr >= (addr_t) blk && ptr < (addr_t) blk + blk->size)
            goto cleanup_region;
    }

    _panic ("heap_free", "failed to free pointer: %p\n", ptr);

cleanup_region:

    /* mark the current block as free */
    blk->state = HEAP_BLOCKSTATE_FREE;

    /* can the current block be merged with the next block? */
    if (blk->next->state == HEAP_BLOCKSTATE_FREE) {
        blk->size += blk->next->size;
        blk->next = blk->next->next;
    }

    /* can the current block be merged with the last block? */
    if (lb->state == HEAP_BLOCKSTATE_FREE) {
        lb->size += blk->size;
        lb->next = blk->next;
    }
}

void *
heap_malloc (size_t size)
{
    return heap_internal_malloc (0, size);
}

void *
heap_calloc (size_t count, size_t size)
{
    return heap_internal_malloc (0, count * size);
}

void
heap_region_dump ()
{
    struct heap_block_header *blk;
    struct heap_region *r;

    for (r = heap_region_base; r; r = r->next) {
        dprintf (NORMAL, "Region:\n");
        for (blk = r->first_block; ; blk = blk->next) {
            if (blk->state == HEAP_BLOCKSTATE_FREE)
                dprintf (NORMAL, "    Block[ FREE]:");
            else if (blk->state == HEAP_BLOCKSTATE_ALLOC)
                dprintf (NORMAL, "    Block[ALLOC]:");
            else
                dprintf (NORMAL, "    Block[ UNK ]:");

            dprintf (NORMAL, " %d bytes at 0x%p\n", blk->size, blk);
            if (blk->next == r->first_block)
                break;
        }
    }
}

void *
heap_internal_malloc (size_t align, size_t size)
{
    struct heap_block_header *cur, *first;
    struct heap_region *r;
    size_t n, grow;

    /* fixup alignment */
    align = (align >> HEAP_MEM_ALIGN_LOG2);
    if (align == 0) 
        align = 1;

    /**
     *  Search through each memory region, and then each block in each region,
     *  until either a free block with sufficient space is found, or there is 
     *  sufficient free space in the region that a new block can be created.
     */
    for (r = heap_region_base; r; r = r->next) {
        for (cur = r->first_block; cur != first; cur = cur->next, first = r->first_block) {

            /* if the current block is already allocated, skip */
            if (cur->state == HEAP_BLOCKSTATE_ALLOC)
                continue;

            /* check the current block is free and has sufficient space */
            if (cur->size >= size) {

                /* check if the block is larger than the size to allocate */
                if (size != cur->size) {
                    struct heap_block_header *new;
                    new = (struct heap_block_header *) (((addr_t) cur) + size + 1);
                    new->size = cur->size - size;
                    new->state = HEAP_BLOCKSTATE_FREE;
                    new->next = cur->next;

                    /* update the current block */
                    cur->next = new;
                }

                /* update the current state and size */
                cur->state = HEAP_BLOCKSTATE_ALLOC;
                cur->size = size;

                break;
            }
        }

        if (cur)
            return (void *) (cur + HEAP_BLOCK_OVERHEAD);
    }

    _panic ("heap_internal_malloc", "failed to find free block to allocate\n");
    return 0;
}