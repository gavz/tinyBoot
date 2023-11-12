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
//  Copyright (C) 2023, Is This On? Holdings Ltd
//
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#ifndef __ARCH_ARM_H__
#define __ARCH_ARM_H__

/**
 *  NOTE:       This file contains arm64-specific functions, macros and structs
 *              to be used within C files, such as reading system registers and
 *              the cpu exception frame,
 */

#include <sys/types.h>

/* read system registers */
unsigned int arm64_read_current_el ();

/* memory-mapped io read/write */
#define mmio_write(register, val)      *(volatile uint32_t *) register = val
#define mmio_read(register)            *(volatile uint32_t *) register

/* arm64 exception stack frame. */
struct arm64_exception_frame {
	uint64_t    regs[29];
    uint64_t    fp;
    uint64_t    lr;
    uint64_t    sp;
    uint64_t    far;
    uint32_t    esr;
    uint32_t    res0;
};

#endif /* __arch_arm_h__ */
