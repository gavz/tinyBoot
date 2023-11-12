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

#ifndef __PLAT_PROC_STATE_H__
#define __PLAT_PROC_STATE_H__

#include <arch/arm.h>

/**
 * SCTLR_EL3 states:
*/
#define SCTLR_EL3_RESET_STATE   (SCTLR_ELx_RES1 | SCTLR_ELx_EE_LE | \
                                 SCTLR_ELx_ICACHE_DIS | SCTLR_ELx_DCACHE_DIS | \
                                 SCTLR_ELx_MMU_DIS)
#define SCTLR_EL2_ENTRY_STATE   (SCTLR_ELx_RES1 | SCTLR_ELx_EE_LE | \
                                 SCTLR_ELx_WXN_DIS | SCTLR_ELx_ICACHE_DIS | \
                                 SCTLR_ELx_SA_DIS | SCTLR_ELx_DCACHE_DIS | \
                                 SCTLR_ELx_ALIGN_DIS | SCTLR_ELx_MMU_DIS)

/**
 * SCR_EL3 states:
*/
#define SCR_SWITCH_STATE_S      (SCR_EL3_RES1 | \
                                 SCR_EL3_RW_AARCH64 | \
                                 SCR_EL3_HCE_EN | \
                                 SCR_EL3_EA_EN)
#define SCR_SWITCH_STATE_NS     (SCR_SWITCH_STATE_S | \
                                 SCR_EL3_NS_EN)

/**
 * SPSR_ELx states:
*/
#define SPSR_EL3_RETURN_EL2     (SPSR_EL_END_LE | \
                                 SPSR_EL_DEBUG_MASK | \
                                 SPSR_EL_ASYN_MASK | \
                                 SPSR_EL_SERR_MASK | \
                                 SPSR_EL_IRQ_MASK | \
                                 SPSR_EL_FIQ_MASK | \
                                 SPSR_EL_M_AARCH64 | \
                                 SPSR_EL_M_EL2H)
#define SPSR_EL3_RETURN_EL1     (SPSR_EL_END_LE | \
                                 SPSR_EL_DEBUG_MASK | \
                                 SPSR_EL_ASYN_MASK | \
                                 SPSR_EL_SERR_MASK | \
                                 SPSR_EL_IRQ_MASK | \
                                 SPSR_EL_FIQ_MASK | \
                                 SPSR_EL_M_AARCH64 | \
                                 SPSR_EL_M_EL1H)

/**
 * PSTATE DAIF Fields:
 *  - Enable/Disable Debug Interrupts
 *  - Enable/Disable Async Interrupts
 *  - Enable/Disable IRQ Interrupts
 *  - Enable/Disable FIQ Interrupts
*/
#define DAIF_ENABLE_ALL         (DAIFSC_DEBUGF | \
                                 DAIFSC_ASYNCF | \
                                 DAIFSC_IRQF | \
                                 DAIFSC_FIQF)

/* GIC enable for Linux */
#define GIC_ENABLE_ALL          ((1 << 3) | (1 << 0))

#endif /* __plat_proc_state_h__ */