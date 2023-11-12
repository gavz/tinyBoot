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
 *  tBoot boot trampoline.
 */

#include <arch/arm.h>
#include <arch/proc_state.h>
#include <plat/mmap.h>


/**
 * Boot trampoline to EL3. Both tinyROM and tBoot also run at EL3, so there
 * is no need to do an exception return here, just prep and jump.
 */
    .section ".text.trampoline"
    .align 16
    .globl _boot_trampoline_el3
_boot_trampoline_el3:

    /* Preserve the parameters passed to boot_trampoline */
    mov     x28, x0

    /* Disable interrupts */
    msr     DAIFSet, #(DAIF_ENABLE_ALL)

    /* Disable the MMU */
    ldr     x0, =SCTLR_EL3_RESET_STATE
    msr     SCTLR_EL3, x0

    /* Move jump address to x0 and jump */
    mov     x30, x28
    ret


    .align 16
    .globl _boot_trampoline_el2
_boot_trampoline_el2:

    /* Preserve the parameters passed to boot_trampoline */
    mov     x28, x0

    /* Disable interrupts */
    msr     DAIFSet, #(DAIF_ENABLE_ALL)

    /* Secure Configuration Register */
    ldr     x0, =SCR_SWITCH_STATE_NS
    msr     SCR_EL3, x0

    /* Saved Program Status Register */
    ldr     x0, =SPSR_EL3_RETURN_EL2
    msr     SPSR_EL3, x0
    msr     SPSR_EL2, x0

    /* Enable GIC System Registers */
    ldr     x0, =GIC_ENABLE_ALL
    msr     ICC_SRE_EL3, x0
    msr     ICC_SRE_EL2, x0
    msr     ICC_SRE_EL1, x0

    /* Disable the MMU at EL2  */
    ldr     x0, =SCTLR_EL3_RESET_STATE
    msr     SCTLR_EL2, x0

    /* Prepare Linux boot args */
    ldr     x0, =DEVICE_TREE_BASE
    mov     x1, xzr
    mov     x2, xzr
    mov     x3, xzr

    /* Setup exception return */
    msr     ELR_EL3, x28
    eret


    .align 16
    .globl _boot_trampoline_el1
_boot_trampoline_el1:

    /* Preserve the parameters passed to boot_trampoline */
    mov     x28, x0

    /* Disable interrupts */
    msr     DAIFSet, #(DAIF_ENABLE_ALL)

    /* Disable the MMU */
    ldr     x0, =SCTLR_EL2_ENTRY_STATE
    msr     SCTLR_EL1, x0

    /* Setup EL1 stack pointer */
    mov     x0, sp
    msr     SP_EL1, x0

    /* Setup system control register */
    ldr     x0, =SCR_SWITCH_STATE_NS
    ldr     x1, =SPSR_EL3_RETURN_EL1
    msr     SCR_EL3, x0
    msr     SPSR_EL3, x1

    /* Setup exception return */
    msr     ELR_EL3, x28
    
    eret