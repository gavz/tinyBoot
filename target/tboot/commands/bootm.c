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

#include <commands/bootm.h>
#include <plat/mmap.h>
#include <lib/libc.h>

#include <lib/display.h>
#include <lib/debug.h>

#define BOOT_FIRMWARE_TARGET_LINUX 1

int
tboot_command_bootm (console_command_t *cmd, int argc, char **argv)
{
    uintptr_t image_base;
//
//#if BOOT_FIRMWARE_TARGET_LINUX
//    dprintf (NORMAL, "Booting Linux\n");
//
//    dprintf (DEBUG, "bootm: relocating device tree from 0x%llx to 0x%llx\n",
//        DRAM_REGION_BASE, DEVICE_TREE_BASE);
//    memmove (DEVICE_TREE_BASE, DRAM_REGION_BASE, DEVICE_TREE_SIZE);
//
//    dprintf (DEBUG, "bootm: relocating kernel from 0x%llx to 0x%llx\n",
//        FIRMWARE_LOAD_REGION_1_BASE, DRAM_REGION_BASE);
//    memmove(DRAM_REGION_BASE, FIRMWARE_LOAD_REGION_1_BASE, FIRMWARE_LOAD_REGION_1_SIZE);
//
//    dprintf (DEBUG, "kernel: 0x%llx-0x%llx, fdt: 0x%llx-0x%llx\n",
//        DRAM_REGION_BASE, DRAM_REGION_BASE + FIRMWARE_LOAD_REGION_1_SIZE,
//        DEVICE_TREE_BASE, DEVICE_TREE_BASE + DEVICE_TREE_SIZE);
//
//    image_base = DRAM_REGION_BASE;
//
//#elif BOOT_FIRMWARE_TARGET_UBOOT
//    dprintf (NORMAL, "Booting Das U-Boot\n");
//
//    image_base = 0x44000000;
//
//    dprintf (DEBUG, "bootm: relocating uboot from 0x%llx to 0x%llx\n",
//        FIRMWARE_LOAD_REGION_1_BASE, image_base);
//    memmove (image_base, FIRMWARE_LOAD_REGION_1_BASE, FIRMWARE_LOAD_REGION_1_SIZE);
//    
//#endif

    image_base = FIRMWARE_LOAD_REGION_1_BASE;

    dprintf (DEBUG, "boot trampoline to: 0x%llx\n", image_base);
    _boot_trampoline_el2 (image_base);

    return 0;
}