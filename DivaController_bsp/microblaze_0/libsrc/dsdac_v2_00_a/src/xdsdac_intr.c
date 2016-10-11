/* $Id: xdsdac_intr.c,v 1.1.2.1 2009/11/24 07:36:57 svemula Exp $ */
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
* @file xdsdac_intr.c
*
* Functions in this file are for the interrupt driven processing functionality.
* See xdsdac.h for a detailed description of the driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- --------------------------------------------------------
* 1.00a ecm  03/30/04 First release
* 1.11a mta  03/23/07 Updated to new coding style
* 2.00a ktn  11/16/09 Updated to use the HAL APIs/macros.
*		      The macros have been renamed to remove _m from the name.
*		      Updated to use XDsDac_WriteReg/XDsDac_ReadReg in place of
*		      Xil_Out32/Xil_In32
* </pre>
******************************************************************************/

/***************************** Include Files *********************************/

#include "xstatus.h"
#include "xio.h"
#include "xdsdac.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
/*****************************************************************************/
/**
*
* Enable the DsDac Interrupts.
*
* This function must be called before other functions to transmit data in
* interrupt driven mode. The user should have connected the interrupt handler
* within the application to an interrupt source such as an interrupt controller
* or the processor interrupt prior to this function being called.
*
* @param	InstancePtr is a pointer to the XDsDac instance to be worked on.
*
* @return	None.
*
* @note		None
*
******************************************************************************/
void XDsDac_EnableInterrupts(XDsDac *InstancePtr)
{
	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);


	/*
	 * Enable interrupts.
	 */
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_IPIER_OFFSET,
		  (XDAC_IPIXR_PIRQ_MASK | XDAC_IPIXR_EMPTY_MASK));
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_GIER_OFFSET,
		  XDAC_GIER_GIE_MASK);

}

/*****************************************************************************/
/**
*
* Disables the interrupts from the device(the higher layer software is
* responsible for disabling interrupts at the interrupt controller).
*
* To start using the device again in interrupt driven mode,
* XDsDac_EnableInterrupts() must be called.
*
* @param	InstancePtr is a pointer to the XDsDac instance to be worked on.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XDsDac_DisableInterrupts(XDsDac *InstancePtr)
{
	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Disable the GIE.
	 */
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_GIER_OFFSET, 0);

	/*
	 * Disable interrupts.
	 */
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_IPIER_OFFSET, 0);

}

/*****************************************************************************/
/**
*
* Get the interrupt status from the registers to determine the source of the
* interrupt.
*
* @param	InstancePtr contains a pointer to the DS DAC device instance
*		for the interrupt.
*
* @return	The currently asserted interrupts. Bits are defined in
*		xdsdac_l.h.
*
* @note		 None.
*
******************************************************************************/
u32 XDsDac_GetInterruptStatus(XDsDac *InstancePtr)
{

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Return the asserted interupt(s).
	 */
	return (XDsDac_ReadReg(InstancePtr->BaseAddress, XDAC_IPISR_OFFSET));

}

/*****************************************************************************/
/**
*
* Clears the interrupt status in the register as directed by the bits set in
* the provided Mask
*
* @param	InstancePtr contains a pointer to the DS DAC device instance
*		for the interrupt.
* @param	Mask contains the interrupt sources to be cleared.
*		Bits are defined in xdsdac_l.h.
*
* @return	None.
*
* @note		None
*
******************************************************************************/
void XDsDac_ClearInterruptStatus(XDsDac *InstancePtr, u32 Mask)
{
	u32 Register;

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Get the currently set interrupt bits.
	 */
	Register = XDsDac_GetInterruptStatus(InstancePtr);

	/*
	 * Only clear the bits in the ISR that are set in the Mask.
	 */
	Register &= Mask;

	/*
	 * Acknowledge the interupt(s).
	 */
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_IPISR_OFFSET, Register);

}

/****************************************************************************/
/**
*
* Set the level of the FIFO which triggers the interrupt.
*
* @param	InstancePtr is the instance of the device to operate on
* @param	Depth is the intended interrupt trigger depth
*		(0 through XDAC_MAX_DATA_LENGTH-1 samples)
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void XDsDac_SetInterruptDepth(XDsDac *InstancePtr, u32 Depth)
{

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	Xil_AssertVoid(Depth < XDAC_MAX_DATA_LENGTH);

	/*
	 * Set the Threshold Register.
	 */
	XDsDac_WriteReg((InstancePtr->BaseAddress), XDAC_PIRQ_OFFSET,
							Depth & XDAC_PIRQ_MASK);
}
