/* $Id: xpcie_intr.c,v 1.1.2.1 2010/04/19 21:44:43 mhabib Exp $ */
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
/*****************************************************************************
**
* @file xpcie_intr.c
*
* This file implements interrupt functions for the XPcie IP
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ---------------------------------------------------
* 3.00a MH  03/16/09  Original code.
* 4.00a MH  11/23/09  Updated to use HAL Phase 1 interface.
* 4.01a MH  04/19/10  Removed "m" from Read/Write Macros.
*
* </pre>
*
******************************************************************************/

/****************************** Include Files ********************************/
#include "xpcie.h"

/*************************** Constant Definitions ****************************/

/***************************** Type Definitions ******************************/

/****************** Macros (Inline Functions) Definitions ********************/

/*************************** Variable Definitions ****************************/

/*************************** Function Prototypes *****************************/

/*****************************************************************************/
/**
* Enable the IP interrupt bits passed into "EnableMask".
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param EnableMask is the bit pattern for interrupts wanted to be enabled.
*
* @return None
*
* @note If an interrupt is already enabled before calling this function,
*       it will stay enabled regardless of the value of "EnableMask" passed
*       from the caller.
*
*
******************************************************************************/
void XPcie_EnableInterrupts(XPcie *InstancePtr, u32 EnableMask)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	Data = XPcie_ReadReg(InstancePtr->Config.BaseAddress,
	                                          XPCIE_BIER_OFFSET);

	XPcie_WriteReg(InstancePtr->Config.BaseAddress,
	                     XPCIE_BIER_OFFSET, (Data | EnableMask));

}

/*****************************************************************************/
/**
* Disable the IP interrupt bits passed into "DisableMask".
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param DisableMask is the bit pattern for interrupts wanted to be disabled.
*
* @return None
*
* @note If an interrupt is already disabled before calling this function,
*       it will stay disabled regardless of the value of "DisableMask" passed
*       from the caller.
*
*
******************************************************************************/
void XPcie_DisableInterrupts(XPcie *InstancePtr, u32 DisableMask)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	Data = XPcie_ReadReg(InstancePtr->Config.BaseAddress,
	                                       XPCIE_BIER_OFFSET);

	XPcie_WriteReg(InstancePtr->Config.BaseAddress,
	              XPCIE_BIER_OFFSET, (Data & (~DisableMask)));

}

/*****************************************************************************/
/**
* Get the currently enabled interrupt bits of the IP and pass them back
* to the caller into "EnabledMask".
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param EnabledMaskPtr is a pointer to a variable where the driver will pass
*        back the enabled interrupt bits after reading them from IP.
*
* @return None.
*
* @note None.
*
*
******************************************************************************/
void XPcie_GetEnabledInterrupts(XPcie *InstancePtr, u32 *EnabledMaskPtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(EnabledMaskPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);


	*EnabledMaskPtr = XPcie_ReadReg(InstancePtr->Config.BaseAddress,
	                                               XPCIE_BIER_OFFSET);

}

/*****************************************************************************/
/**
* Get the currently pending interrupt bits of the IP and pass them back
* to the caller into "PendingMask".
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param PendingMaskPtr is a pointer to a variable where the driver will pass
*        back the pending interrupt bits after reading them from IP.
*
* @return None.
*
* @note None.
*
*
******************************************************************************/
void XPcie_GetPendingInterrupts(XPcie *InstancePtr, u32 *PendingMaskPtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(PendingMaskPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);


	*PendingMaskPtr = XPcie_ReadReg(InstancePtr->Config.BaseAddress,
	                                                    XPCIE_BIR_OFFSET);

}

/*****************************************************************************/
/**
* Clear the currently pending interrupt bits of the IP passed from
* the caller into "ClearMask".
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param ClearMask is the bit pattern for pending interrupts wanted to be
*        cleared.
*
* @return None.
*
* @note None.
*
*
******************************************************************************/
void XPcie_ClearPendingInterrupts(XPcie *InstancePtr, u32 ClearMask)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);


	XPcie_WriteReg((InstancePtr->Config.BaseAddress), XPCIE_BIR_OFFSET,
	       XPcie_ReadReg((InstancePtr->Config.BaseAddress),
	          XPCIE_BIR_OFFSET) & (ClearMask));
}
