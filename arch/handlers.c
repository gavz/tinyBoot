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

#include <arch/arch.h>
#include <sys.h>

/* taken from https://github.com/AsahiLinux/m1n1/blob/main/src/exception.c */
static char *ec_table[0x40] = {
    [0x00] = "unknown",
    [0x01] = "wf*",
    [0x03] = "c15 mcr/mrc",
    [0x04] = "c15 mcrr/mrrc",
    [0x05] = "c14 mcr/mrc",
    [0x06] = "ldc/stc",
    [0x07] = "FP off",
    [0x08] = "VMRS access",
    [0x09] = "PAC off",
    [0x0a] = "ld/st64b",
    [0x0c] = "c14 mrrc",
    [0x0d] = "branch target",
    [0x0e] = "illegal state",
    [0x11] = "svc in a32",
    [0x12] = "hvc in a32",
    [0x13] = "smc in a32",
    [0x15] = "svc in a64",
    [0x16] = "hvc in a64",
    [0x17] = "smc in a64",
    [0x18] = "other mcr/mrc/sys",
    [0x19] = "SVE off",
    [0x1a] = "eret",
    [0x1c] = "PAC failure",
    [0x20] = "instruction abort (lower)",
    [0x21] = "instruction abort (current)",
    [0x22] = "pc misaligned",
    [0x24] = "data abort (lower)",
    [0x25] = "data abort (current)",
    [0x26] = "sp misaligned",
    [0x28] = "FP exception (a32)",
    [0x2c] = "FP exception (a64)",
    [0x2f] = "SError",
    [0x30] = "BP (lower)",
    [0x31] = "BP (current)",
    [0x32] = "step (lower)",
    [0x33] = "step (current)",
    [0x34] = "watchpoint (lower)",
    [0x35] = "watchpoint (current)",
    [0x38] = "bkpt (a32)",
    [0x3a] = "vector catch (a32)",
    [0x3c] = "breakpoint (a64)",
};

#define ESR_EC_SHIFT			26
#define ESR_EC_MASK			(0x3F << ESR_EC_SHIFT)
#define ESR_EC(x)			((x & ESR_EC_MASK) >> ESR_EC_SHIFT)

void
arm64_exception_handler (struct arm64_exception_frame *frame,
                         char *exception,
                         char **kind)
{
    _panic ("arm64",
        "%s: %s at 0x%016x:\n"
        "    esr 0x%016x  far 0x%016x\n"
        "     x0 0x%016x   x1 0x%016x   x2 0x%016x\n"
        "     x3 0x%016x   x4 0x%016x   x5 0x%016x\n"
        "     x6 0x%016x   x7 0x%016x   x8 0x%016x\n"
        "     x9 0x%016x  x10 0x%016x  x11 0x%016x\n"
        "    x12 0x%016x  x13 0x%016x  x14 0x%016x\n"
        "    x15 0x%016x  x16 0x%016x  x17 0x%016x\n"
        "    x18 0x%016x  x19 0x%016x  x20 0x%016x\n"
        "    x21 0x%016x  x22 0x%016x  x23 0x%016x\n"
        "    x24 0x%016x  x25 0x%016x  x26 0x%016x\n"
        "    x27 0x%016x  x28 0x%016x\n"
        "     sp 0x%016x   lr 0x%016x\n",

        exception,
        *kind, frame->sp,
        frame->esr, frame->far,

        frame->regs[0],  frame->regs[1],  frame->regs[2],
        frame->regs[3],  frame->regs[4],  frame->regs[5], 
        frame->regs[6],  frame->regs[7],  frame->regs[8], 
        frame->regs[9],  frame->regs[10], frame->regs[11], 
        frame->regs[12], frame->regs[13], frame->regs[14], 
        frame->regs[15], frame->regs[16], frame->regs[17], 
        frame->regs[18], frame->regs[19], frame->regs[20], 
        frame->regs[21], frame->regs[22], frame->regs[23], 
        frame->regs[24], frame->regs[25], frame->regs[26], 
        frame->regs[27], frame->regs[28],
        frame->sp, frame->lr);
}

void
arm64_exception_synchronous (struct arm64_exception_frame *frame)
{
    const char **kind;
    kind = &ec_table[ESR_EC(frame->esr)];

    // synchronous exception
    arm64_exception_handler (frame, "synchronous abort", kind);
}

void
arm64_exception_irq (struct arm64_exception_frame *frame)
{
    // irq exception
    panic ("irq interrupt\n");
}

void
arm64_exception_fiq (struct arm64_exception_frame *frame)
{
    // fiq exception
    panic ("fiq interrupt\n");
}

void
arm64_exception_serror (struct arm64_exception_frame *frame)
{
    const char **kind;
    kind = &ec_table[ESR_EC(frame->esr)];

    // serror exception
    arm64_exception_handler (frame, "serror", kind);
}