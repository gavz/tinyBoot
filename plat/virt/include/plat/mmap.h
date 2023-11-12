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

#ifndef __PLATFORM_MMAP_H__
#define __PLATFORM_MMAP_H__


/* QEMU-defined values for aarch64-virt */
#define QEMU_RAMLIMIT_GB        255
#define QEMU_RAMLIMIT_BYTES     (QEMU_RAMLIMIT_GB * (1 << 30))

/* ROM/Flash Region */
#define ROM_REGION_BASE         0x00000000
#define ROM_REGION_SIZE         0x08000000

/* SRAM Region (Secure Memory) */
#define SRAM_REGION_BASE        0x0e000000
#define SRAM_REGION_SIZE        0x01000000

/* DRAM Region (Non-secure Memory) */
#define DRAM_REGION_BASE        0x40000000
#define DRAM_REGION_SIZE        QEMU_RAMLIMIT_BYTES     /* Maximum value, actual
                                                        depends on qemu args */

/**
 * EL3 Firmware Location.
 *
 * tinyROM and tBoot should run at the base of SRAM. The relocation loop will
 * move both components from where they originally reside, e.g. 0x0 for tinyROM
 * to SRAM_REGION_BASE.
 *
 * The base of EL3 firmware is defined as FIRMWARE_BASE, and FIRMWARE_SIZE bytes
 * long. The size value is the maximum size an EL3 firmware component can be.
*/
#define FIRMWARE_BASE           SRAM_REGION_BASE
#define FIRMWARE_SIZE           0x500000

/**
 * EL3 Firmware Heap & Stack.
 *
 * The location of the heap is the same for both tinyROM and tBoot. This is placed
 * in the SRAM Region so the Kernel cannot read the EL3 firmware heap.
*/
#define HEAP_BASE               (SRAM_REGION_BASE + 0xf00000)
#define HEAP_SIZE               0x20000

#define STACK_BASE              (HEAP_BASE + HEAP_SIZE + 0xff)
#define STACK_SIZE              0x4000

/**
 * Firmware Image Load Regions.
 *
 * Bootable images can be placed in memory manually via QEMU. tinyROM and tBoot
 * will read these regions to verify if there is an image located there. If there
 * is, the region is initialised as a blockdev.
 *
 * There are three regions, and each has a maximum size.
*/
#define FIRMWARE_MAX_SIZE       0x500000

/* Region: 0x48000000 - 0x48500000 */
#define FIRMWARE_LOAD_REGION_0_BASE     (DRAM_REGION_BASE + 0x08000000)
#define FIRMWARE_LOAD_REGION_0_SIZE     FIRMWARE_MAX_SIZE

/* Region: 0x48500000 - 0x48a00000 */
#define FIRMWARE_LOAD_REGION_1_BASE     (FIRMWARE_LOAD_REGION_0_BASE + FIRMWARE_MAX_SIZE)
#define FIRMWARE_LOAD_REGION_1_SIZE     FIRMWARE_MAX_SIZE

/* Region: 0x48a00000 - 0x48f00000 */
#define FIRMWARE_LOAD_REGION_2_BASE     (FIRMWARE_LOAD_REGION_1_BASE + FIRMWARE_MAX_SIZE)
#define FIRMWARE_LOAD_REGION_2_SIZE     FIRMWARE_MAX_SIZE

/**
 * Device Tree Region
 *
 * By default, QEMU places the Device Tree at 0x4
*/
#define DEVICE_TREE_BASE                (DRAM_REGION_BASE + 0x04000000)
#define DEVICE_TREE_SIZE                0x100000


#endif /* __platform_mmap_h__ */
