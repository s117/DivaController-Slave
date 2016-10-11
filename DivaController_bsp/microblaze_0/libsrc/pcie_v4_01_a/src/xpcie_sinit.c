/* $Id: xpcie_sinit.c,v 1.1.2.1 2010/04/19 21:44:43 mhabib Exp $ */
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
* @file xpcie_sinit.c
*
* This file contains the implementation of PCIe driver's static initialization
* functionality.
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

#include "xparameters.h"
#include "xpcie.h"

/*************************** Constant Definitions ****************************/

/***************************** Type Definitions ******************************/

/****************** Macros (Inline Functions) Definitions ********************/

/*************************** Variable Definitions ****************************/

extern XPcie_Config XPcie_ConfigTable[];

/*************************** Function Prototypes *****************************/

/*****************************************************************************/
/**
* Lookup the device configuration based on the unique device ID.  The table
* ConfigTable contains the configuration info for each device in the system.
*
* @param DeviceId is the device identifier to lookup.
*
* @return
*
* - XPcie configuration structure pointer if DeviceID is found.
* - NULL if DeviceID is not found.
*
******************************************************************************/
XPcie_Config *XPcie_LookupConfig(u16 DeviceId)
{
	XPcie_Config *CfgPtr = NULL;

	int index;

	for (index = 0; index < XPAR_XPCIE_NUM_INSTANCES; index++) {
		if (XPcie_ConfigTable[index].DeviceId == DeviceId) {
			CfgPtr = &XPcie_ConfigTable[index];
			break;
		}
	}

	return (CfgPtr);
}





