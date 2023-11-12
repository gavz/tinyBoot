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
 *  The exception vector table on tBoot is not seperated into seperate files for
 *  el1 and el3 in tBoot, the vector table is the same for all levels.
 */

#include <arch/arm.h>
#include <plat/mmap.h>

/**
 *  This macro will setup an arm64_exception_frame structure in memory. The stack
 *  pointer is moved back 300 places, amd the x0-x1 registers are saved first.
 *
 *  The original stack pointer is saved, and the base of the frame struct is
 *  moved to x0. The vector entries will use the value in x0, rather than the sp
 *  directly when entering additional registers.
 */
.macro create_exception_frame
    msr     SPSel, #0
    sub     sp, sp, #400
    stp     x0, x1, [sp, #0]
    add     x0, sp, #400
    str     x0, [sp, #248]
    mov     x0, sp
.endm

/* exception vector table base */
    .text
    .align 12
    .globl _el3_exception_vector_base
_el3_exception_vector_base:
L__el3_sp0_synchronous_handler:         /* Current EL SP0 Synchronous */
    create_exception_frame
    mrs     x1, FAR_EL3
    str     x1, [x0, #256]
    mrs     x1, ESR_EL3
    str     x1, [x0, #264]
    bl      L__exception_entry
    bl      arm64_exception_synchronous
    b       L__exception_exit

    /* Current EL SP0 IRQ */
    .text
    .align 7
L__el3_sp0_irq_handler:
    create_exception_frame
    bl      L__exception_entry
    bl      arm64_exception_irq
    b       L__exception_exit

    /* Current EL SP0 FIQ */
    .text
    .align 7
L__el3_sp0_fiq_handler:
    create_exception_frame
    bl      L__exception_entry
    bl      arm64_exception_fiq
    b       L__exception_exit

    /* Current EL SP0 SError */
    .text
    .align 7
L__el3_sp0_serror_handler:
    create_exception_frame
    mrs     x1, FAR_EL3
    str     x1, [x0, #256]
    mrs     x1, ESR_EL3
    str     x1, [x0, #264]
    bl      L__exception_entry
    bl      arm64_exception_serror
    b       L__exception_exit

    /* Current EL SPx Synchronous */
    .text
    .align 7
L__el3_spx_synchronous_handler:
    b       .

    /* Current EL SPx IRQ */
    .text
    .align 7
L__el3_spx_irq_handler:
    b       .

    /* Current EL SPx FIQ */
    .text
    .align 7
L__el3_spx_fiq_handler:
    b       .

    /* Current EL SPx SError */
    .text
    .align 7
L__el3_spx_serror_handler:
    b       .


    .text
    .align 12
    .globl _el2_exception_vector_base
_el2_exception_vector_base:
L__el2_sp0_synchronous_handler:         /* Current EL SP0 Synchronous */
    create_exception_frame
    mrs     x1, FAR_EL2
    str     x1, [x0, #256]
    mrs     x1, ESR_EL2
    str     x1, [x0, #264]
    bl      L__exception_entry
    bl      arm64_exception_synchronous
    b       L__exception_exit

    /* Current EL SP0 IRQ */
    .text
    .align 7
L__el2_sp0_irq_handler:
    create_exception_frame
    bl      L__exception_entry
    bl      arm64_exception_irq
    b       L__exception_exit

    /* Current EL SP0 FIQ */
    .text
    .align 7
L__el2_sp0_fiq_handler:
    create_exception_frame
    bl      L__exception_entry
    bl      arm64_exception_fiq
    b       L__exception_exit

    /* Current EL SP0 SError */
    .text
    .align 7
L__el2_sp0_serror_handler:
    create_exception_frame
    mrs     x1, FAR_EL2
    str     x1, [x0, #256]
    mrs     x1, ESR_EL2
    str     x1, [x0, #264]
    bl      L__exception_entry
    bl      arm64_exception_serror
    b       L__exception_exit

    /* Current EL SPx Synchronous */
    .text
    .align 7
L__el2_spx_synchronous_handler:
    create_exception_frame
    mrs     x1, FAR_EL2
    str     x1, [x0, #256]
    mrs     x1, ESR_EL2
    str     x1, [x0, #264]
    bl      L__exception_entry
    bl      arm64_exception_synchronous
    b       L__exception_exit

    /* Current EL SPx IRQ */
    .text
    .align 7
L__el2_spx_irq_handler:
    b       .

    /* Current EL SPx FIQ */
    .text
    .align 7
L__el2_spx_fiq_handler:
    b       .

    /* Current EL SPx SError */
    .text
    .align 7
L__el2_spx_serror_handler:
    b       .


    .text
    .align 12
    .globl _el1_exception_vector_base
_el1_exception_vector_base:
L__el1_sp0_synchronous_handler:         /* Current EL SP0 Synchronous */
    create_exception_frame
    mrs     x1, FAR_EL1
    str     x1, [x0, #256]
    mrs     x1, ESR_EL1
    str     x1, [x0, #264]
    bl      L__exception_entry
    bl      arm64_exception_synchronous
    b       L__exception_exit

    /* Current EL SP0 IRQ */
    .text
    .align 7
L__el1_sp0_irq_handler:
    create_exception_frame
    bl      L__exception_entry
    bl      arm64_exception_irq
    b       L__exception_exit

    /* Current EL SP0 FIQ */
    .text
    .align 7
L__el1_sp0_fiq_handler:
    create_exception_frame
    bl      L__exception_entry
    bl      arm64_exception_fiq
    b       L__exception_exit

    /* Current EL SP0 SError */
    .text
    .align 7
L__el1_sp0_serror_handler:
    create_exception_frame
    mrs     x1, FAR_EL1
    str     x1, [x0, #256]
    mrs     x1, ESR_EL1
    str     x1, [x0, #264]
    bl      L__exception_entry
    bl      arm64_exception_serror
    b       L__exception_exit

    /* Current EL SPx Synchronous */
    .text
    .align 7
L__el1_spx_synchronous_handler:
    b       .

    /* Current EL SPx IRQ */
    .text
    .align 7
L__el1_spx_irq_handler:
    b       .

    /* Current EL SPx FIQ */
    .text
    .align 7
L__el1_spx_fiq_handler:
    b       .

    /* Current EL SPx SError */
    .text
    .align 7
L__el1_spx_serror_handler:
    b       .


/**
 *  The exception_entry saves the remaining register state to the exception
 *  frame pointed to by x0, and then returns. Upon returning, the handler for
 *  the particular exception will be called.
 */
    .text
    .align 2
L__exception_entry:
    stp     fp, lr, [sp, #-16]!
    mov     fp, sp

    /* save remaining registers */
    stp	    x2, x3, [x0, #16]
	stp	    x4, x5, [x0, #32]
	stp	    x6, x7, [x0, #48]
	stp	    x8, x9, [x0, #64]
	stp	    x10, x11, [x0, #80]
	stp	    x12, x13, [x0, #96]
	stp	    x14, x15, [x0, #112]
	stp	    x16, x17, [x0, #128]
	stp	    x18, x19, [x0, #144]
	stp	    x20, x21, [x0, #160]
	stp	    x22, x23, [x0, #176]
	stp	    x24, x25, [x0, #192]
	stp	    x26, x27, [x0, #208]
	str	    x28, [x0, #224]
    ret

    .text
    .align 2
L__exception_exit:
    /** TODO: Once interrupts are implemented, this becomes relevant and can be
        tested. */
