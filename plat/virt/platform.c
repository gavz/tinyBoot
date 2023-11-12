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
#include <sys/boot.h>
#include <sys.h>

#include <drivers/blockdev/mem-blk.h>

#include <lib/serial.h>
#include <lib/blockdev.h>

#include <plat/mmap.h>
#include <global.h>

DECLARE_GLOBAL_DATA_PTR;

void
platform_early_init (void)
{
    /* setup the secure uart/serial driver */
    serial_init (SERIAL_TYPE_NON_SECURE);

    /* enable interrupts (gic) */
    //arm_arch_gic3_enable ();
}

void
platform_late_init (void)
{
    struct blockdev *bdev;

    /* device tree discovery */

    /**
     * tinyBoot expects a few blockdev devices to load firmware from. For now,
     * instead of trying to use virtio devices, just create the devices as
     * mem-blk devices.
     * 
     * TODO: Only create these devices if there is data within the load regions.
     */
    mem_blockdev_create ("firmware0", FIRMWARE_LOAD_REGION_0_BASE, 512,
            FIRMWARE_LOAD_REGION_0_SIZE / 512);
    mem_blockdev_create ("firmware1", FIRMWARE_LOAD_REGION_1_BASE, 512,
            FIRMWARE_LOAD_REGION_1_SIZE / 512);
    mem_blockdev_create ("firmware2", FIRMWARE_LOAD_REGION_2_BASE, 512,
            FIRMWARE_LOAD_REGION_2_SIZE / 512);
}

void
platform_halt (void)
{
    /* stop all cpus */
}

void
platform_reset (void)
{
    /* reset */
}

int
platform_get_boot_device (enum boot_device *boot_device)
{
    /** TODO: Add detection for other boot devices */
    *boot_device = BOOT_DEVICE_PFLASH;
    return 1;
}
