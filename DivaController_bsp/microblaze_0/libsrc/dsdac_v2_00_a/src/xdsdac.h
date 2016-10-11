/* $Id: xdsdac.h,v 1.1.2.1 2009/11/24 07:36:57 svemula Exp $ */
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
* @file xdsdac.h
*
* The Xilinx Delta-Sigma Digital-to-Analog-Converter (DAC)  driver.
* This driver supports the Xilinx Delta-Sigma DAC (DS DAC) hardware.
*
*
* <b>Driver Description</b>
*
* The device driver enables higher layer software (e.g., an application) to
* communicate to the DS DAC .
*
* The driver supports interrupt driven mode and the default mode of operation
* is polled mode.
*
* <b>Device Configuration</b>
*
* The device can be configured in various ways during the FPGA implementation
* process.  Configuration parameters are stored in the xdsdac_g.c file. A table
* is defined where each entry contains configuration information for a DsDac
* device.  This information includes such things as the base address
* of the memory-mapped device.
*
* <b>Interrupt Processing</b>
*
* After XDsDac_Initialize() and XDsDac_Start() are called,
* XDsDac_EnableInterrupts() is called to enable interrupts after initializing
* the Interrupt handling such as XIntc_SetHandler() through the XIntc driver and
* enabling the interrupt. There is no callback handler provided by this driver.
* An example is provided in the examples directory on how to construct a user
* defined handler.
*
* When processing is complete, call XDsDac_DisableInterrupts() to disable the
* interrupts.
*
* The Threshold and Empty interrupts are enabled within the
* XDsDac_EnableInterrupts() function and disabled in the
* XDsDac_DisableInterrupts() function. The XDsDac_WriteOutput() function does
* not acknowledge the DS DAC generated interrupts. The application must call
* XDsDac_GetInterruptStatus() to determine which interrupts are pending and call
* XDsDac_ClearInterruptStatus() to acknowledge the appropriate interrupt
* sources.
*
* It is the application's responsibility to acknowledge any associated Interrupt
* Controller interrupts if it is used in the system.
*
* <b>Buffer Management</b>
*
* There is no buffer management provided in the driver. It is up to the
* application to provide the buffer space and data necessary to keep the DAC
* from underrunning on the data to be converted. If there isn't any new data
* when the DAC needs it, the last sample is repeated until the device is reset
* or new data is provided.
*
* <b>Asserts</b>
*
* Asserts are used within all Xilinx drivers to enforce constraints on argument
* values. Asserts can be turned off on a system-wide basis by defining, at
* compile time, the NDEBUG identifier.  By default, asserts are turned on and
* it is recommended that application developers leave asserts on during
* development.
*
* @note
*
* This driver is intended to be RTOS and processor independent.  It works
* with physical addresses only.  Any needs for dynamic memory management,
* threads or thread mutual exclusion, virtual memory, or cache control must
* be satisfied by the layer above this driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  03/30/04 First release
* 1.01a ecm  10/20/04 Added the changes for the new occupancy hw change
* 1.11a mta  03/23/07 Updated to new coding style
* 1.12a sdm  08/22/08 Removed support for static interrupt handlers from the MDD
*		      file
* 2.00a ktn  11/16/09 Updated the driver to use the HAL APIs/macros.
*		      The macros have been renamed to remove _m from the name.
*		      Updated to use XDsDac_WriteReg/XDsDac_ReadReg in place of
*		      Xil_Out32/Xil_In32
* </pre>
*
******************************************************************************/

#ifndef XDSDAC_H		/* prevent circular inclusions */
#define XDSDAC_H		/* by using protection macros */

/***************************** Include Files *********************************/

#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xdsdac_l.h"

/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/

/**
 * This typedef contains configuration information for a device.
 */
typedef struct {
	u16 DeviceId;		/**< Unique ID  of device */
	u32 BaseAddress;	/**< Device base address */
} XDsDac_Config;

/**
 * The XDsDac driver instance data. The user is required to allocate a
 * variable of this type for every DsDac device in the system. A pointer
 * to a variable of this type is then passed to the driver API functions.
 */

typedef struct {
	u32 BaseAddress;	/**< Base address for device */
	u32 IsReady;		/**< Device is initialized and ready */
} XDsDac;

/***************** Macros (Inline Functions) Definitions *********************/
/****************************************************************************/
/**
*
* Check to see if the FIFO is empty. This macro is typically used when
* the polling method is employed.
*
* @param	InstancePtr is the instance of the device to operate on
*
* @return	TRUE if it is Empty, or FALSE if it is not.
*
* @note		C-style signature:
*		int XDsDac_IsEmpty(XDsDac *InstancePtr)
*
*****************************************************************************/
#define XDsDac_IsEmpty(InstancePtr)				\
	((XDsDac_ReadReg(((InstancePtr)->BaseAddress), XDAC_IPISR_OFFSET) & \
		XDAC_IPIXR_EMPTY_MASK) == XDAC_IPIXR_EMPTY_MASK)

/****************************************************************************/
/**
*
* Is the FIFO full. Can be used in polling also to keep the FIFO full.
*
* @param	InstancePtr is the instance of the device to operate on
*
* @return	FALSE  if at least 1 sample can be written, or TRUE if it not.
*
* @note		C-style signature:
*		int XDsDac_FifoIsFull(XDsDac *InstancePtr)
*
*****************************************************************************/
#define XDsDac_FifoIsFull(InstancePtr)				\
	(((XDsDac_ReadReg(((InstancePtr)->BaseAddress), XDAC_OCCY_OFFSET) & \
		   XDAC_OCCY_MASK)) == (XDAC_MAX_DATA_LENGTH))

/****************************************************************************/
/**
*
* Get the space available in the FIFO in sample count.
*
* @param	InstancePtr is the instance of the device to operate on
*
* @return	The current vacancy of the DS DAC FIFO.
*
* @note		C-style signature:
*		u32 XDsDac_GetVacancy(XDsDac *InstancePtr)
*
*****************************************************************************/
#define XDsDac_GetVacancy(InstancePtr)					\
	(XDAC_MAX_DATA_LENGTH -					\
	(XDsDac_ReadReg(((InstancePtr)->BaseAddress), XDAC_OCCY_OFFSET) & \
		XDAC_OCCY_MASK))

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/

/*
 * Initialization functions in xdsdac.c
 */
int XDsDac_Initialize(XDsDac *InstancePtr, u16 DeviceId);
void XDsDac_Start(XDsDac *InstancePtr);
void XDsDac_Stop(XDsDac *InstancePtr);

void XDsDac_Reset(XDsDac *InstancePtr);

XDsDac_Config *XDsDac_LookupConfig(u16 DeviceId);

u32 XDsDac_WriteOutput(XDsDac *InstancePtr, u16 *DataPtr,
		       unsigned SampleCount);

/*
 * Interrupt driven functions in xdsdac_intr.c
 */
void XDsDac_EnableInterrupts(XDsDac *InstancePtr);
void XDsDac_DisableInterrupts(XDsDac *InstancePtr);
void XDsDac_GlobalInterruptEnable(XDsDac *InstancePtr);

u32 XDsDac_GetInterruptStatus(XDsDac *InstancePtr);
void XDsDac_ClearInterruptStatus(XDsDac *InstancePtr, u32 Mask);

void XDsDac_SetInterruptDepth(XDsDac *InstancePtr, u32 Depth);


/*
 * Diagnostic or selftest functions in xdsdac_selftest.c
 */
int XDsDac_SelfTest(XDsDac *InstancePtr);

#endif /* end of protection macro */
