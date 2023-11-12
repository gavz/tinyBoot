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
 *  ARM PrimeCell PL011 UART Driver Implementation
 *
 */
#ifndef __SERIAL_PL011_UART_H__
#define __SERIAL_PL011_UART_H__

#include <sys/symbols.h>
#include <lib/serial.h>
#include <sys/types.h>

/**
 *  @brief      PL011 MMIO Frame.
 * 
 *              A number of fields are reserved for the PL010 UART driver. This 
 *              may be implemented in the future should a target device require 
 *              it. In that case, these reserved fields would be changed.
 */
struct pl011_regs
{
    u32     dr;         /* Data Register (UARTDR)*/
    u32     ecr;        /* Error Clear Register (UARTECR/UARTRSR)*/
    
    /* 0x008-0x014 reserved (pl010) */
    u32     rs1, rs2, rs3, rs4;

    /* remaining */
    u32     fr;         /* Flag Register (UARTTFR) */
    u32     rs5;        /* Reserved (pl010) */
    u32     ilpr;       /* IrDA Low-Power Counter Register (UARTILPR) */
    u32     ibrd;       /* Integer Baud Rate Register (UARTIBRD) */
    u32     fbrd;       /* Fractioanl Baud Rate Register (UARTFBRD) */
    u32     lcr_h;      /* Line Control Register (UARTLCR_H) */
    u32     cr;         /* Control Register (UARTCR) */
    u32     ifls;       /* Interrupt FIFO Level Select (UARTIFLS) */
    u32     imsc;       /* Interrupt Mask Set/Clear (UARTIMSC) */
};

/**
 *  @brief      Private structure for the PL011 Driver containing the current
 *              register state. In the future, when other pl0x drivers are added
 *              this can also contain a type value.
 */
struct pl011_priv
{
    struct pl011_regs      *regs;
};

/**
 *  @brief      Platform data structure containing MMIO base address, clock
 *              speed and baud rate.
 */
struct pl011_plat
{
    uint64_t        base;
    uint64_t        clock;
    uint64_t        baud;
};

#define CONFIG_DEBUG 1

int pl011_serial_device_init (const char *name, uint64_t base, uint64_t clock, uint64_t baud);

/* Clock Rate */
#define SERIAL_PL011_CLOCK_RATE         0x16e3600

/* Available Baud Rates */
#define SERIAL_PL011_BAUD_921600        921600      /* UART-mode Max */
#define SERIAL_PL011_BAUD_460800        460800      /* IrDA-mode Max */
#define SERIAL_PL011_BAUD_115200        115200      /* LP IrDA-mode Max */

/* Control Register (UARTCR) bits */
#define SERIAL_PL011_CR_CTSEN             (1 << 15)
#define SERIAL_PL011_CR_RTSEN             (1 << 14)
#define SERIAL_PL011_CR_OUT2              (1 << 13)
#define SERIAL_PL011_CR_OUT1              (1 << 12)
#define SERIAL_PL011_CR_RTS               (1 << 11)
#define SERIAL_PL011_CR_DTR               (1 << 10)
#define SERIAL_PL011_CR_RXE               (1 << 9)
#define SERIAL_PL011_CR_TXE               (1 << 8)
#define SERIAL_PL011_CR_LPE               (1 << 7)
#define SERIAL_PL011_CR_IIRLP             (1 << 2)
#define SERIAL_PL011_CR_SIREN             (1 << 1)
#define SERIAL_PL011_CR_UARTEN            (1 << 0)

/* Line Control Register (LCR_H) bits */
#define SERIAL_PL011_LCRH_SPS             (1 << 7)
#define SERIAL_PL011_LCRH_WLEN_8          (3 << 5)
#define SERIAL_PL011_LCRH_WLEN_7          (2 << 5)
#define SERIAL_PL011_LCRH_WLEN_6          (1 << 5)
#define SERIAL_PL011_LCRH_WLEN_5          (0 << 5)
#define SERIAL_PL011_LCRH_FEN             (1 << 4)
#define SERIAL_PL011_LCRH_STP2            (1 << 3)
#define SERIAL_PL011_LCRH_EPS             (1 << 2)
#define SERIAL_PL011_LCRH_PEN             (1 << 1)
#define SERIAL_PL011_LCRH_BRK             (1 << 0)

/* Interrupt Mask Set/Clear Register (UARTIMSC) */
#define SERIAL_PL011_IMSC_OEIM            (1 << 10)
#define SERIAL_PL011_IMSC_BEIM            (1 << 9)
#define SERIAL_PL011_IMSC_PEIM            (1 << 8)
#define SERIAL_PL011_IMSC_FEIM            (1 << 7)
#define SERIAL_PL011_IMSC_RTIM            (1 << 6)
#define SERIAL_PL011_IMSC_TXIM            (1 << 5)
#define SERIAL_PL011_IMSC_RXIM            (1 << 4)
#define SERIAL_PL011_IMSC_DSRMIM          (1 << 3)
#define SERIAL_PL011_IMSC_DCDMIM          (1 << 2)
#define SERIAL_PL011_IMSC_CTSMIM          (1 << 1)
#define SERIAL_PL011_IMSC_RIMIM           (1 << 0)

#endif /* __serial_pl011_uart_h__ */