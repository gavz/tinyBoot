======================================
tinyOS - Simple ARM64 Operating System
======================================

tinyOS is a hobby operating system consisting of a two-stage bootloader, called
tinyBoot, and a UNIX-like Kernel called tinyKern.This repository contains the 
two stage bootloader. The first stage, tinyROM and the second stage, tBoot.

This project encompasses a few of the major components one might associate with 
an operating system - bootloader, kernel, basic user applications, and a C 
compiler. This project may also form the basis of a future Master's degree thesis, 
likely to focus on the compiler aspect. 

Currently, the development is focusing on the bootloader and the kernel. This
project is not necessarily intended for practical real-world use, however that
doesn't mean it wouldn't be useful. tinyOS is targeted at ARM64/AArch64 systems 
and therefore is very lightweight from the start. In particular tinyOS is tested
on the QEMU Cortex-A76 platform. 

You can find the tinyKern repository on my Github page.

0. Building
-----------

To build tinyBoot, you'll need the aarch64-none-elf toolchain from ARM's website,
which you can find here: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads.

Create a build folder, and run CMake with the following options:

$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DTARGET_PLATFORM=virt -DTOOLCHAIN=<path>
$ make

You'll want to set TINYROM_FIRMWARE to the tinyROM.img binary, and TBOOT_FIRMWARE
to the tBoot.img firmware, do not use the ELF files.

You can then boot tinyOS in QEMU by running the boot.sh script. If you'd like to
try loading a linux kernel, pass this as KERNEL_IMAGE and ensure the correct
option is set in bootm.c to prepare the kernel.

1. tinyBoot overview
---------------------

The bootloader, called tinyBoot, is two-stage. The goal for tinyBoot is to provide
the functionality one might usually find within a bootloader, such as iBoot, grub
or u-boot - even Trusted-Firmware-A. The intended features for tinyBoot include
memory maangement, device drivers, console and support for various kernels such
as Linux and XNU (as well as tinyKern).

Memory managemnt is a simple region-based design. There currently is no use of
the MMU, although this may change. The device driver model was originally intended
to be like u-boot, however it is now just static device drivers which are invoked
by platform setup code.

The cli console provides a number of commands such as `mem` for manipulating memory
and `bootm` for booting the image found in the FIRMWARE1 load region.

The memory mapping for tinyBoot is found under plat/virt/include/plat/mmap.h, and
defines the regions for ROM, SRAM, DRAM and firmware load regions. 

2. Boot flow
------------

The first stage, tinyROM, is passed to QEMU via the `-bios` flag, and therefore is
loaded within the ROM region at 0x0. It will begin executing and check where it
is running. If tinyROM is not running at `FIRMWARE_BASE` (base of secure memory, 
or SRAM), it will move itself to the correct location and continue executing. 

It will conduct as much setup as is required to be able to output over UART, handle
any exceptions, and load the tBoot image from a disk. At the moment the only implemented
disk types are memory blockdevices, however the blockdevice API can be extended
to VIRTIO, although this requires GICv3.

Once tinyROM has loaded tBoot, tBoot will then follow the exact same assembly
setup code, again checking that it is in the correct place. The difference between
tinyROM and tBoot is that tBoot will host a CLI console.

There is no real need for tinyROM, just a neat thing to have.

tBoot provides boot choices over the console. As each Kernel requires a different
way to prepare boot arguments, these will eventually be seperate commands. However,
as it stands the kernel in `FIRMWARE1` can be loaded with `bootm`, just ensure
the correct setup is compiled in `bootm.c`.
