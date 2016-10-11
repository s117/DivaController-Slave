/* $Id: xdsdac.c,v 1.1.2.1 2009/11/24 07:36:56 svemula Exp $ */
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
* @file xdsdac.c
*
* Functions in this file are the minimum required functions for the DS DAC
* driver. See xdsdac.h for a detailed description of the driver.
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
#include "xenv.h"
#include "xparameters.h"
#include "xdsdac.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern XDsDac_Config XDsDac_ConfigTable[];

/*****************************************************************************/
/**
*
* Initialize a specific XDsDac instance/driver.
* The initialization entails:
* 	- Initialize fields of the XDsDac instance structure.
* 	- Resets the hardware to ensure it is ready.
*
* The driver defaults to polled mode operation.
*
* @param	InstancePtr is a pointer to the XDsDac instance to be worked on.
* @param	DeviceId is the unique id of the device controlled by this
*		XDsDac instance. Passing in a device id associates the generic
*		XDsDac instance to a specific device, as chosen by the caller or
*		application developer.
*
* @return
* 		- XST_SUCCESS if initialization was successful.
* 		- XST_DEVICE_NOT_FOUND if device configuration information was
*		not found for a device with the supplied device ID.
*
* @note		None
*
******************************************************************************/
int XDsDac_Initialize(XDsDac *InstancePtr, u16 DeviceId)
{
	XDsDac_Config *DsDac_Config;	/* Pointer to Configuration data. */

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Zero the provided instance memory.
	 */
	memset(InstancePtr, 0, sizeof(XDsDac));

	/*
	 * Lookup the device configuration in the configuration table. Use this
	 * configuration info down below when initializing this component.
	 */
	DsDac_Config = XDsDac_LookupConfig(DeviceId);
	if (DsDac_Config == NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	/*
	 * Set some default values for instance data, don't indicate the device
	 * is ready to use until everything has been initialized successfully.
	 */

	InstancePtr->BaseAddress = DsDac_Config->BaseAddress;

	/*
	 * Since there were no failures, indicate the device is ready to use.
	 */
	InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

	/*
	 * Reset the device to ensure it is ready.
	 */
	XDsDac_Reset(InstancePtr);

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Starts the Delta-Sigma DAC Hardware.
*
* The Device must be initialized and configured prior to XDsDac_Start() being
* called. Enables the DAC hardware to start producing analog outputs.
*
* @param	InstancePtr is a pointer to the XDsDac instance to be worked on.
*
* @return	None
*
* @note		None
*
******************************************************************************/
void XDsDac_Start(XDsDac *InstancePtr)
{

	u32 Register;

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Enable the hardware.
	 */
	Register = XDsDac_ReadReg(InstancePtr->BaseAddress, XDAC_CR_OFFSET);
	Register |= XDAC_CR_EN_MASK;
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_CR_OFFSET, Register);
}

/*****************************************************************************/
/**
*
* Stops the Delta-Sigma DAC Hardware.
* Prevents the DAC from producing analog outputs.
*
* @param	InstancePtr is a pointer to the XDsDac instance to be worked on.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XDsDac_Stop(XDsDac *InstancePtr)
{

	u32 Register;

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Disable the hardware.
	 */
	Register = XDsDac_ReadReg(InstancePtr->BaseAddress, XDAC_CR_OFFSET);
	Register &= ~XDAC_CR_EN_MASK;
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_CR_OFFSET, Register);
}


/*****************************************************************************/
/**
*
* Transmit Delta-Sigma DAC data. The SampleCount is the total data size to be
* converted. If the amount of data to be sent will not fit in the FIFO, the
* return value indicates the amount of data sent to the DAC. This allows for
* this function to be used in a polling situation also.
*
* @param 	InstancePtr is a pointer to the XDsDac instance to be worked on.
* @param 	DataPtr is a pointer to data.
* @param 	SampleCount is the size, in samples, of the data.
*
* @return	The number of samples sent to the DAC.
*
* @note
*
* This function call is not blocking in nature, i.e. it will not wait until
* the data is sent to the hardware. Also, the interrupts are not disabled
* while the data is written to the DAC. If this is desired, the application
* must disable the interrupts prior to calling this function and then
* re-enable the interrupts when this function returns.
*
******************************************************************************/
u32 XDsDac_WriteOutput(XDsDac *InstancePtr, u16 *DataPtr, unsigned SampleCount)
{
	unsigned i;
	u32 SendLength;

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Determine if the buffer is ready.
	 */
	SendLength = XDsDac_GetVacancy(InstancePtr);

	if (SendLength == 0) {
		/*
		 * Buffer is full, return 0 samples sent to allow for polling
		 * usage.
		 */
		return 0;
	}

	/*
	 * Determine the amount of data to send.
	 */
	if (SampleCount <= SendLength) {
		SendLength = SampleCount;
	}

	/*
	 * Write each sample(s) to the DS DAC.
	 */
	for (i = 0; i < SendLength; i++) {
		XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_FIFO_OFFSET,
			  DataPtr[i]);
	}

	return SendLength;
}

/*****************************************************************************/
/**
*
* Resets the Delta-Sigma DAC Hardware.
*
* The Device must be configured again and XDsDac_Start() must be called prior
* to using the device again.
*
*
* @param	InstancePtr is a pointer to the XDsDac instance to be worked on.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XDsDac_Reset(XDsDac *InstancePtr)
{

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Force the Reset of the device.
	 */
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_IPISRR_OFFSET,
		  XDAC_IPISRR_RESET_MASK);

	/*
	 * Release the Reset.
	 */
	XDsDac_WriteReg(InstancePtr->BaseAddress, XDAC_IPISRR_OFFSET, 0);
}

/*****************************************************************************/
/**
*
* Lookup the device configuration based on the unique device ID.  The table
* XDsDac_ConfigTable contains the configuration info for each device in the
* system.
*
* @param	DeviceId is the unique device ID of the device being looked up.
*
* @return	A pointer to the configuration table entry corresponding to the
*		given device ID, or NULL if no match is found.
*
* @note		None.
*
******************************************************************************/
XDsDac_Config *XDsDac_LookupConfig(u16 DeviceId)
{
	XDsDac_Config *CfgPtr = NULL;
	int Index;

	for (Index = 0; Index < XPAR_XDSDAC_NUM_INSTANCES; Index++) {
		if (XDsDac_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XDsDac_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}
