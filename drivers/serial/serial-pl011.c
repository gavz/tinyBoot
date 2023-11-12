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
 *  tBoot/tinyROM ARM PrimeCell PL011 UART Driver Implementation.
*/

#include <drivers/serial/serial-pl011.h>
#include <arch/arch.h>
#include <lib/serial.h>
#include <lib/libc.h>
#include <lib/heap.h>
#include <global.h>

DECLARE_GLOBAL_DATA_PTR;

/* driver priv struct */
struct pl011_serial_device
{
    struct serial_device dev;

    struct pl011_regs *regs;

    uint64_t base;
    uint64_t clock;
    uint64_t baud;
};

/*******************************************************************************
 *  Generic PL011 Driver Functions
 ******************************************************************************/

static int
pl011_putc (struct serial_device *sdev, const char c)
{
    struct pl011_serial_device *dev = (struct pl011_serial_device *) sdev;
    //struct pl011_regs *regs = dev->regs;
    struct pl011_regs *regs = (struct pl011_regs *) ((uintptr_t) dev->base);

    /* wait for the data buffer to be full */
    while (mmio_read (&regs->fr) & 0x20) {}
    mmio_write (&regs->dr, c);
    return 0;
}

static int
pl011_puts (struct serial_device *sdev, const char *s)
{
    struct pl011_serial_device *dev = (struct pl011_serial_device *) sdev;
    struct pl011_regs *regs = dev->regs;

    while (*s) {
        pl011_putc (sdev, *s);
        s++;
    }
    return 0;
}

static int
pl011_getc (struct serial_device *sdev)
{
    struct pl011_serial_device *dev = (struct pl011_serial_device *) sdev;
    struct pl011_regs *regs = (struct pl011_regs *) ((uintptr_t) dev->base);

    /* wait for the data buffer to be free */
    while (mmio_read (&regs->fr) & 0x10) {}
    return mmio_read (&regs->dr);
}

static int
pl011_setbrg (struct serial_device *sdev, int clock, int baudrate)
{
    struct pl011_serial_device *dev = (struct pl011_serial_device *) sdev;
    struct pl011_regs *regs;
    unsigned int divider;

    /* setup regs */
    regs = (struct pl011_regs *) ((uintptr_t) dev->base);
    dev->regs = regs;

    /* disable everything */
    mmio_write (&regs->cr, 0);

    /* set fractional baud rate divider */
    divider = (clock * 4) / baudrate;
    mmio_write (&regs->ibrd, divider >> 6);
    mmio_write (&regs->fbrd, divider & 0x3f);

    /* set line control register */
    mmio_write (&regs->lcr_h, SERIAL_PL011_LCRH_WLEN_8);

    /* enable interrupts */
    mmio_write (&regs->imsc, (SERIAL_PL011_IMSC_RXIM | SERIAL_PL011_IMSC_RTIM));

    /* enable uart */
    mmio_write (&regs->cr, (SERIAL_PL011_CR_UARTEN | SERIAL_PL011_CR_TXE | 
                                                        SERIAL_PL011_CR_RXE));
    return 0;
}

/*******************************************************************************
 *  Driver API
 ******************************************************************************/

#include <plat/mmap.h>

int pl011_serial_device_init (const char *name, uint64_t base, uint64_t clock, 
                              uint64_t baud)
{
    struct pl011_serial_device *sdev;

    /* setup the serial device */
    sdev = SRAM_REGION_BASE + 64; //tboot_malloc (sizeof (struct pl011_serial_device));
    sdev->base = base;
    sdev->baud = baud;
    sdev->clock = clock;

    pl011_setbrg (sdev, sdev->clock, sdev->baud);

    sdev->dev.name = name;
    sdev->dev.getc = &pl011_getc;
    sdev->dev.putc = &pl011_putc;
    sdev->dev.puts = &pl011_puts;

    // register
    gd->serial = sdev;
    gd->putc = sdev->dev.putc;
    gd->getc = sdev->dev.getc;

    return 0;
}