/* $Id: xdsdac_l.h,v 1.1.2.1 2009/11/24 07:36:57 svemula Exp $ */
/******************************************************************************
*
* (c) Copyright 2004-2009 Xilinx, Inc. All rights reserved.
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
* @file xdsdac_l.h
*
* This header file contains identifiers and basic driver functions and
* macros that can be used to access the device.
*
* Refer to xdsdac.h for more details.
*
* @note
*
* None
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  03/30/04 First release
* 1.01a ecm  10/20/04 Added the changes for the new occupancy hw change
* 1.11a mta  03/23/07 Updated to new coding style
* 2.00a ktn  11/16/09 Updated to use the HAL APIs/macros.
*		      The macros have been renamed to remove _m from the name.
*		      Updated to use XDsDac_WriteReg/XDsDac_ReadReg in place of
*		      Xil_Out32/Xil_In32
* </pre>
*
******************************************************************************/

#ifndef XDSDAC_L_H		/* prevent circular inclusions */
#define XDSDAC_L_H		/* by using protection macros */

/***************************** Include Files *********************************/

#include "xil_types.h"
#include "xil_assert.h"
#include "xil_io.h"

/************************** Constant Definitions *****************************/
/**
 * Register offsets for the Delta-Sigma DAC MAC.
 */
#define XDAC_GIER_OFFSET	0x01C	/**< Global Interrupt Enable */
#define XDAC_IPISR_OFFSET	0x020	/**< Interrupt status */
#define XDAC_IPIER_OFFSET	0x028	/**< Interrupt enable */
#define XDAC_IPISRR_OFFSET	0x040	/**< Sw reset register */

#define XDAC_CR_OFFSET		0x100	/**< DAC control register */
#define XDAC_FIFO_OFFSET	0x104	/**< DAC Data FIFO */
#define XDAC_OCCY_OFFSET	0x108	/**< DAC Occupancy register */
#define XDAC_PIRQ_OFFSET	0x10C	/**< DAC Programmable Depth register */

/**
 * Global Interrupt Enable Register (GIER) Masks
 */
#define XDAC_GIER_GIE_MASK  0x80000000 /**< Global Enable */

/**
 * Interrupt Enable Register (IPER) and Interrupt Status Register (IPSR) Masks
 */
#define XDAC_IPIXR_PIRQ_MASK	0x00000001 /**< Depth interrupt is set */
#define XDAC_IPIXR_EMPTY_MASK	0x00000002 /**< FIFO empty interrupt is set */

/**
 * Software Reset Register (IPISRR) Mask
 */
#define XDAC_IPISRR_RESET_MASK	0x00000001	/**< Reset bit is set */

/**
 * Control Register (CR) Masks
 */
#define XDAC_CR_FIFO_RESET_MASK	0x00000002  /**< FIFO reset bit */
#define XDAC_CR_EN_MASK		0x00000001  /**< Device enable bit */

/**
 * Data FIFO Register (FIFO)
 */
#define XDAC_FIFO_DATA_MASK	0x000000FF  /**< Valid data FIFO bits */

/**
 * Data Occupancy Register (OCCY)
 */
#define XDAC_OCCY_MASK		0x0000001F  /**< Valid FIFO occupancy bits */

/**
 * Programmable depth interrupt Register (DEPTH)
 */
#define XDAC_PIRQ_MASK		0x0000000F  /**< Valid compare value bits */

#define XDAC_MAX_DATA_LENGTH	16


/***************** Macros (Inline Functions) Definitions *********************/

#define XDsDac_In32 Xil_In32
#define XDsDac_Out32 Xil_Out32

/****************************************************************************/
/**
*
* Write value to DS DAC register.
*
* @param	BaseAddress is the base address of the DS DAC device.
* @param	RegOffset is the register offset from the base to write to.
* @param	Data is the data to be written to the device.
*
* @return	None.
*
* @note		C-style signature:
*		void XDsDac_WriteReg(u32 BaseAddress, u32 RegOffset,
*						 u32 Data)
*
*****************************************************************************/
#define XDsDac_WriteReg(BaseAddress, RegOffset, Data)		\
		(XDsDac_Out32((BaseAddress) + (RegOffset), (u32)(Data)))

/****************************************************************************/
/**
*
* Read value from DS DAC register.
*
* @param	BaseAddress is the base address of the DS DAC device.
* @param	RegOffset is the register offset from the base to read from.
*
* @return	The data read from the device.
*
* @note		C-style signature :
*		u32  XDsDac_ReadReg(u32 BaseAddress, u32 RegOffset)
*
*****************************************************************************/
#define XDsDac_ReadReg(BaseAddress, RegOffset)		\
		(XDsDac_In32((BaseAddress) + (RegOffset)))

/************************** Function Prototypes ******************************/

#endif /* end of protection macro */
