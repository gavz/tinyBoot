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
 *  Generic AArch64 startup code. This is not platform-dependent so should work
 *  for all devices in plat/
 */

#include <arch/arm.h>
#include <arch/proc_state.h>
#include <plat/mmap.h>

/*******************************************************************************
 *  Reset Vector
 ******************************************************************************/

/**
 *  tBoot Reset Vector
 */
    .section ".text.boot"
    .globl _start
_start:

    /* Relocate if needed */
    adr     x0, _start
    ldr     x1, =FIRMWARE_BASE
    cmp     x1, x0
    b.eq    L__continue_boot

    mov     lr, x1
    ldr     x2, =(FIRMWARE_BASE + FIRMWARE_SIZE)
    ldr     x3, =FIRMWARE_BASE
    sub     x2, x2, x3
L__relocate_loop:
    ldp     x3, x4, [x0], #16
    stp     x3, x4, [x1], #16
    subs    x2, x2, #16
    b.ne    L__relocate_loop
    ret

    /* disable all interrupts */
    msr     DAIFSet, #(DAIF_ENABLE_ALL)

    /* fetch the current exception level (should be EL3 or EL1) */
    mrs     x0, CurrentEL
    lsr     x0, x0, #2
    cmp     x0, #3                  /* CurrentEL == EL3 */
    b.eq    L__check_cpuid
    b       L__boot_cond_fail       /* boot conditions failed */

    /* check the cpuid is 0 */
L__check_cpuid:
    mrs     x1, MPIDR_EL1
    and     x1, x1, #3
    cbz     x1, L__continue_boot
    b       L__boot_cond_fail

/**
 *  if the boot conditions were not met, spin here forever.
 */
L__boot_cond_fail:
    wfe
    b       L__boot_cond_fail

/**
 *  cpuid is zero and exception level is either EL3 or EL1, so it's fine to
 *  continue booting.
 */
L__continue_boot:

    /* disable the MMU */
    ldr     x0, =SCTLR_EL3_RESET_STATE  /* default state for EL3 after reset */
    mrs     x1, CurrentEL
    lsr     x1, x1, #2
    cmp     x1, #3
    msr     SCTLR_EL3, x0

    /**
     *  setup the exception vector base for whichever ELx we're running at. The
     *  VBAR_ELx register is set to the vector base.
     */
    cmp     x1, #3
    adr     x0, _el3_exception_vector_base
    msr     VBAR_EL3, x0
    adr     x0, _el1_exception_vector_base
    msr     VBAR_EL1, x0

    /**
     *  setup the stack using SP0, and zero it out. The stack base address is
     *  defined in each platforms mmap.h header and is placed between the heap
     *  and tBoot in memory.
     */
    msr     SPSel, #0
    ldr     x1, =STACK_BASE
    ldr     x2, =STACK_SIZE
    add     x2, x2, x1
    mov     x3, #0
L__loop_stack_zero:
    stp     x3, x3, [x1], #16
    cmp     x1, x2
    b.ne    L__loop_stack_zero
    mov     sp, x1

    /* Re-enable interrupts */
    msr     DAIFSet, #2

    /* Jump to tBoot main */
    bl      _tboot_main

L__spin:
    b       L__spin
