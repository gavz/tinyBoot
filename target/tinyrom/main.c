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

#include <global.h>
#include <version.h>

#include <lib/blockdev.h>
#include <lib/display.h>
#include <lib/debug.h>
#include <lib/image.h>
#include <lib/heap.h>

#include <platform.h>
#include <plat/mmap.h>

#include <sys/boot.h>

DECLARE_GLOBAL_DATA_PTR;

static inline int global_data_init ();
static void boot_selected_device (enum boot_device boot_device);

static void __test_heap ();

int _tboot_main (void)
{
    enum boot_device boot_device;

    /* initialise global data */
    global_data_init ();

    /* architecture init */
    arm_arch_cpu_init ();

    /* platform early init */
    platform_early_init ();
    dprintf (DEBUG, "platform early init complete\n");

    sys_init ();
    dprintf (NORMAL, "Booting tinyBoot tinyROM-tboot1\n");

    /* print tinyrom version info */
    dprintf (NORMAL, "================================\n");
    dprintf (NORMAL, ":: tinyROM-tboot1 for %s\n", PLATFORM_NAME);
    dprintf (NORMAL, ":: %s (%s)\n", TBOOT_BUILD_VERSION, __TIMESTAMP__);
    dprintf (NORMAL, ":: Build type: %s\n", TBOOT_BUILD_STYLE);
    dprintf (NORMAL, "================================\n");

    /* platform late init */
    platform_late_init ();
    dprintf (DEBUG, "platform late init complete\n");
    dprintf (NORMAL, "tinyROM Boot Complete\n");

    while (1) {
        if (!platform_get_boot_device (&boot_device))
            break;

        boot_selected_device (boot_device);
    }

    /* platform reset, no boot device */
    dprintf (CRITICAL, "No valid boot device, resetting...\n");
    platform_reset ();
}

void *load_image_from_bdev (const char *bdev_name, int type)
{
    struct timg_header *image_header;
    uintptr_t image_load_region;
    struct blockdev *bdev;

    /**
     * try to find a blockdev matching the name passed to this function. We can
     * then check the image header to determine whether it matches the type that
     * was also passed.
     */
    bdev = blockdev_lookup (bdev_name);
    if (!bdev) {
        dprintf (DEBUG, "failed to find image of type '%d' from bdev: %s\n",
            type, bdev_name);
        return 0;
    }

    /* DEBUG: Spoof image header values */
    image_header->type = TIMG_IMAGE_TYPE_TBOT;
    image_header->size = bdev->total_size;

    /**
     * the firmware images should be placed at the base of a bdev, with the timg
     * header in the first (index 0) sector. firstly, we'll read this sector and
     * determine if it matches the given type.
     */
    //blockdev_read (bdev, 0, sizeof (struct timg_header), image_header);
    if (image_header->type != type) {
        dprintf (DEBUG, "failed to find image of type '%d' in bdev: %s\n",
            type, bdev_name);
        return 0;
    }

    /* now, the entire image can be read into the firmware load region */
    image_load_region = (uintptr_t) (FIRMWARE_BASE + FIRMWARE_MAX_SIZE);
    if (image_load_region > (SRAM_REGION_BASE + SRAM_REGION_SIZE)) {
        dprintf (DEBUG, "firmware load region exceeds SRAM: 0x%p\n",
            image_load_region);
        return 0;
    }

    /** TODO: Load image from sector 1 of size total_size - block_size */
    //blockdev_read (bdev, 1, bdev->total_size - bdev->block_size, image_load_region);
    blockdev_read (bdev, 0, image_header->size, image_load_region);
    dprintf (DEBUG, "loaded image into firmware load region: 0x%p\n",
        image_load_region);

    return (void *) image_load_region;
}

void boot_selected_device (enum boot_device boot_device)
{
    struct timg_header *image_header;
    struct blockdev *bdev;
    void *image_address;


    dprintf (DEBUG, "boot_selected_device: attempting boot with device: %d\n",
        boot_device);

    /* handle boot_device */
    switch (boot_device) {
        case BOOT_DEVICE_PFLASH:

            /* lookup the tBoot image from the firmware0 device */
            image_address = load_image_from_bdev ("firmware0", TIMG_IMAGE_TYPE_TBOT);
            if (!image_address)
                panic ("failed to load image '%d' from bdev 'firmware0'\n");

            break;

        default:
            dprintf (DEBUG, "could not find bootable image on device: %d\n",
                boot_device);
            break;
    }

    dprintf (NORMAL, "Booting tinyBoot Stage2 - tBoot\n");
    dprintf (NORMAL, "Handoff\n\n");
    
    _boot_trampoline_el3 (image_address);
}


static inline int
global_data_init ()
{
    gd_t data;
    memset (&data, '\0', sizeof (data));
    gd = &data;
}

static void
__test_heap ()
{
    void *test = tboot_malloc (64);
    void *test2 = tboot_malloc (64);
    void *test3 = tboot_malloc (64);
    void *test4 = tboot_malloc (64);
    tboot_mm_dump ();

    tboot_free(test3);
    tboot_mm_dump();
    tboot_free(test4);
    tboot_mm_dump();

    char *test_data = "test_data";

    memcpy (test, test_data, strlen(test_data));
    dprintf (NORMAL, "test: %s\n", test);

    tboot_mm_dump ();
}