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
 * SCTLR_ELx enable bits definitions
 */
#define SCTLR_ELx_M         (1 << 0)    /* MMU Enable */
#define SCTLR_ELx_A         (1 << 1)    /* Alignment abort enable */
#define SCTLR_ELx_C         (1 << 2)    /* Dcache enable */
#define SCTLR_ELx_SA        (1 << 3)    /* Stack Alignment check enable */
#define SCTLR_ELx_I         (1 << 12)   /* Icache enable */
#define SCTLR_ELx_WXN       (1 << 19)   /* Write Permision Imply XN */
#define SCTLR_ELx_EE        (1 << 25)   /* Exception (Big) Endian */

/**
 * SCTLR_ELx disable bits definitions
 */
#define SCTLR_ELx_EE_LE         (0 << 25)   /* Exception (Little) Endian */
#define SCTLR_ELx_WXN_DIS       (0 << 19)   /* Write Permission Imply not XN */
#define SCTLR_ELx_ICACHE_DIS    (0 << 12)   /* Icache disable */
#define SCTLR_ELx_SA_DIS        (0 << 3)    /* Stack Alignment Check disable */
#define SCTLR_ELx_DCACHE_DIS    (0 << 2)    /* Dcache disable */
#define SCTLR_ELx_ALIGN_DIS     (0 << 1)    /* Alignment check disable */
#define SCTLR_ELx_MMU_DIS       (0)         /* MMU disable */

/**
 * SCTLR_ELx reserved bits definitions
*/
#define SCTLR_ELx_RES1          0x30c50830

/**
 * SCR_EL3 bit definitions
*/
#define SCR_EL3_RW_AARCH64      (1 << 10)   /* Next lower level is AArch64 */
#define SCR_EL3_RW_AARCH32      (0 << 10)   /* Next lower level is AArch32 */
#define SCR_EL3_HCE_EN          (1 << 8)    /* Hypervisor Call enable */
#define SCR_EL3_SMD_DIS         (1 << 7)    /* Secure Monitor Call disable */
#define SCR_EL3_RES1            (3 << 4)    /* Reserved, RES1 */
#define SCR_EL3_EA_EN           (1 << 3)    /* External Aborts taken to EL3 */
#define SCR_EL3_FIQ_EN          (1 << 2)    /* Physical FIQ takne to EL3 */
#define SCR_EL3_IRQ_EN          (1 << 1)    /* Physical IRQ taken to EL3 */
#define SCR_EL3_NS_EN           (1 << 0)    /* EL0 & EL1 are Non-Secure */

/**
 * SPSR_ELx definitions
*/
#define SPSR_EL_END_LE          (0 << 9)  /* Exception Little-endian          */
#define SPSR_EL_DEBUG_MASK      (1 << 9)  /* Debug exception masked           */
#define SPSR_EL_ASYN_MASK       (1 << 8)  /* Asynchronous data abort masked   */
#define SPSR_EL_SERR_MASK       (1 << 8)  /* System Error exception masked    */
#define SPSR_EL_IRQ_MASK        (1 << 7)  /* IRQ exception masked             */
#define SPSR_EL_FIQ_MASK        (1 << 6)  /* FIQ exception masked             */
#define SPSR_EL_T_A32           (0 << 5)  /* AArch32 instruction set A32      */
#define SPSR_EL_M_AARCH64       (0 << 4)  /* Exception taken from AArch64     */
#define SPSR_EL_M_AARCH32       (1 << 4)  /* Exception taken from AArch32     */
#define SPSR_EL_M_SVC           (0x3)     /* Exception taken from SVC mode    */
#define SPSR_EL_M_HYP           (0xa)     /* Exception taken from HYP mode    */
#define SPSR_EL_M_EL1H          (5)       /* Exception taken from EL1h mode   */
#define SPSR_EL_M_EL2H          (9)       /* Exception taken from EL2h mode   */
#define SPSR_EL_M_EL3H          (13)      /* Exception taken from EL3h mode   */

/**
 * PSTATE.DAIF bits definitions
*/
#define DAIFSC_DEBUGF       (1 << 3)
#define DAIFSC_ASYNCF       (1 << 2)
#define DAIFSC_IRQF         (1 << 1)
#define DAIFSC_FIQF         (1 << 0)

#endif /* __arch_arm_h__ */