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
#include <sys/types.h>
#include <plat/mmap.h>
#include <lib/debug.h>
#include <sys.h>


int
sys_init ()
{
    /**
     *  Initialise the heap using the base address and size defined in the
     *  platform memory map.
     * 
     *  TODO:   Ideally the heap should reside in the SRAM, but it keeps 
     *          crashing with a Prefetch Abort.
     */
    heap_region_init ((void *) HEAP_BASE, HEAP_SIZE);

    return 0;
}