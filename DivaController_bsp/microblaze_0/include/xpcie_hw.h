/* $Id: xpcie_hw.h,v 1.1.2.1 2010/04/19 21:44:43 mhabib Exp $ */
/****************************************************************************
*
* (c) Copyright 2009 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xpcie_hw.h
*
* This header file contains identifiers and basic driver functions for the
* XPcie device driver.
*
* @note		None.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 3.00a MH  03/16/09  Original code.
* 4.00a MH  11/23/09  Updated to use HAL Phase 1 interface.
* 4.01a MH  04/19/10  Removed "m" from Read/Write Macros.
*
* </pre>
*
******************************************************************************/
#ifndef XPCIE_HW_H       /* prevent circular inclusions */
#define XPCIE_HW_H       /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#ifndef _ASMLANGUAGE

#include "xil_types.h"
#include "xil_io.h"

#endif /* _ASMLANGUAGE */

/************************** Constant Definitions *****************************/

/** @name Registers
 *
 * Register offsets for this device. Some of the registers
 * are configurable at hardware build time such that may or may not exist
 * in the hardware.
 * @{
 */
#define XPCIE_IPIF2PCI_0U_OFFSET    0x000 /**< IPIFBAR 2 PCIBAR  translation 0
                                            *  - upper 32 bits */
#define XPCIE_IPIF2PCI_0L_OFFSET    0x004 /**< IPIFBAR to PCIBAR translation 0
                                            *  - lower 32 bits */
#define XPCIE_IPIF2PCI_1U_OFFSET    0x008 /**< IPIFBAR to PCIBAR translation 1
                                            *  - upper 32 bits */
#define XPCIE_IPIF2PCI_1L_OFFSET    0x00C /**< IPIFBAR to PCIBAR translation 1
                                            *  - lower 32 bits */
#define XPCIE_IPIF2PCI_2U_OFFSET    0x010 /**< IPIFBAR to PCIBAR translation 2
                                            *  - upper 32 bits */
#define XPCIE_IPIF2PCI_2L_OFFSET    0x014 /**< IPIFBAR to PCIBAR translation 2
                                            *  - lower 32 bits */
#define XPCIE_IPIF2PCI_3U_OFFSET    0x018 /**< IPIFBAR to PCIBAR translation 3
                                            *  - upper 32 bits */
#define XPCIE_IPIF2PCI_3L_OFFSET    0x01C /**< IPIFBAR to PCIBAR translation 3
                                            *  - lower 32 bits */
#define XPCIE_IPIF2PCI_4U_OFFSET    0x020 /**< IPIFBAR to PCIBAR translation 4
                                            *  - upper 32 bits */
#define XPCIE_IPIF2PCI_4L_OFFSET    0x024 /**< IPIFBAR to PCIBAR translation 4
                                            *  - lower 32 bits */
#define XPCIE_IPIF2PCI_5U_OFFSET    0x028 /**< IPIFBAR to PCIBAR translation 5
                                            *  - upper 32 bits */
#define XPCIE_IPIF2PCI_5L_OFFSET    0x02C /**< IPIFBAR to PCIBAR translation 5
                                            *  - lower 32 bits */
#define XPCIE_BCR_OFFSET            0x030 /**< Bridge Control Register */
#define XPCIE_PRIDR_OFFSET          0x034 /**< PCIe Request ID Register */
#define XPCIE_PRCR_OFFSET           0x038 /**< PCIe Request Control Register */
#define XPCIE_PSR_OFFSET            0x03C /**< PCIe Status Register */
#define XPCIE_BIR_OFFSET            0x040 /**< Bridge Interrupt Register */
#define XPCIE_BIER_OFFSET           0x044 /**< Bridge Interrupt
                                                             Enable Register */

#define XPCIE_PCIE_CORE_OFFSET      0x2000 /**< PCI Express hard core
                                               configuration register offset */
/*@}*/

/** @name Bridge Control Register bitmaps and masks
 *  XPCIE_BCR_OFFSET
 * @{
 */
#define XPCIE_BCR_DISABLE_BAR_MASK 	0x00000000 /**< Disable PCIe Connection */
#define XPCIE_BCR_ENABLE_1BAR_MASK  0x00000101 /**< Enable PCIe One BAR */
#define XPCIE_BCR_ENABLE_2BAR_MASK  0x00000103 /**< Enable PCIe Two BARs */
#define XPCIE_BCR_ENABLE_3BAR_MASK  0x00000107 /**< Enable PCIe Three BARs */
/*@}*/

/** @name PCIe Requester ID Register bitmaps and masks
 *  XPCIE_PRIDR_OFFSET
 * @{
 */
#define XPCIE_PRIDR_MASK       0x0000FFFF /**< Mask of all valid bits */
#define XPCIE_PRIDR_FUN_MASK   0x00000007 /**< Requester ID Function Number */
#define XPCIE_PRIDR_DEV_MASK   0x000000F8 /**< Requester ID Device Number   */
#define XPCIE_PRIDR_BUS_MASK   0x0000FF00 /**< Requester ID Bus Number */

#define XPCIE_PRIDR_DEV_SHIFT    3        /**< Requester ID Device Number
                                                           Shift Value */
#define XPCIE_PRIDR_BUS_SHIFT    8        /**< Requester ID Bus Number
                                                           Shift Value*/

/*@}*/

/** @name PCIe Request Control Register bitmaps and masks
 *  XPCIE_PRCR_OFFSET
 * @{
 */
#define XPCIE_PRCR_MASK         0x00000707   /**< Mask of all valid bits */
#define XPCIE_PRCR_MAXRD_MASK   0x00000007   /**< PCIe request max. read */
#define XPCIE_PRCR_MAXLD_MASK   0x00000700   /**< PCIe request max. payload */

#define XPCIE_PRCR_MAXLD_SHIFT    8          /**< PCIe request max.
                                                              payload Shift */
/*@}*/

/** @name PCIe Status Register bitmaps and masks
 *  XPCIE_PSR_OFFSET
 * @{
 */
#define XPCIE_PSR_LINK_STATUS_MASK  0x00000020 /**< PCIe Link Status */
#define XPCIE_PSR_LINK_WIDTH_MASK   0x000003C0 /**< PCIe Link Width */

#define XPCIE_PSR_LINK_STATUS_SHIFT    5       /**< PCIe Link Status Shift */
#define XPCIE_PSR_LINK_WIDTH_SHIFT     6       /**< PCIe Link Width Shift */
/*@}*/

/** @name Bridge Interrupt Register bitmaps and masks
 *  XPCIE_BIR_OFFSET
 * @{
 */
#define XPCIE_BIR_CLEAR_ALL_MASK 	0xFFFFFFFF /**< Clear All Pending
                                                               Interrupts */
/*@}*/


/** @name Bridge Enable Interrupt Register bitmaps and masks
 *  XPCIE_BIER_OFFSET
 * @{
 */
#define XPCIE_BEIR_ENABLE_ALL_MASK 	 0xFFFFFFFF /**< Enable All Interrupts */
#define XPCIE_BEIR_DISABLE_ALL_MASK  0xFFFFFFFF /**< Disable All Interrupts */
/*@}*/


/** @name ECAM Address Register bitmaps and masks
 *  XPCIE_CAPR_OFFSET
 * @{
 */
#define XPCIE_CAPR_MASK       0x0FFFFFFF     /**< Mask of all valid bits */
#define XPCIE_CAPR_BUS_MASK   0x0FF00000     /**< CAR Bus Number*/
#define XPCIE_CAPR_DEV_MASK   0x000F8000     /**< CAR Device Number */
#define XPCIE_CAPR_FUN_MASK   0x00007000     /**< CAR Function Number */
#define XPCIE_CAPR_REG_MASK   0x00000FFC     /**< CAR Register Number */
#define XPCIE_CAPR_BYT_MASK   0x00000003     /**< CAR BYTE ENA Offset */

#define XPCIE_CAPR_DWR_ACCS   0x0FFFFFFC     /**< CAR Double Word Access */

#define XPCIE_CAPR_BUS_SHIFT    20     /**< CAR Bus Number Shift Value */
#define XPCIE_CAPR_DEV_SHIFT    15     /**< CAR Device Number Shift Value*/
#define XPCIE_CAPR_FUN_SHIFT    12     /**< CAR Function Number Shift Value*/
#define XPCIE_CAPR_REG_SHIFT     2     /**< CAR Register Number Shift Value*/
#define XPCIE_CAPR_BYT_SHIFT     0     /**< CAR DWORD Offset Shift Value*/
/*@}*/

/****************** Macros (Inline Functions) Definitions ********************/
/*****************************************************************************/
/**
* Macro to read register.
*
* @param BaseAddress is the base address of the PCIe component.
* @param RegOffset is the register offset.
*
* @return Value of the register.
*
* @note
*
* C-style signature:
*    u32 XPcie_ReadReg(u32 BaseAddress, u32 RegOffset)
*
******************************************************************************/
#define XPcie_ReadReg(BaseAddress, RegOffset) \
  Xil_In32((BaseAddress) + (RegOffset))


/*****************************************************************************/
/**
* Macro to write register.
*
* @param BaseAddress is the base address of the PCIe component.
* @param RegOffset is the register offset.
* @param Data is the data to write.
*
* @return None
*
* @note
*
* C-style signature:
*    void XPcie_WriteReg(u32 BaseAddress, u32 RegOffset, u32 Data)
*
******************************************************************************/
#define XPcie_WriteReg(BaseAddress, RegOffset, Data) \
  Xil_Out32((BaseAddress) + (RegOffset), (Data))

/*************************** Variable Definitions ****************************/

/*************************** Function Prototypes *****************************/
#ifdef __cplusplus
}
#endif

#endif /* XPCIE_HW_H */

