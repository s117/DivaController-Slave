/* $Id: xpcie.c,v 1.1.2.1 2010/04/19 21:44:43 mhabib Exp $ */
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
*****************************************************************************/
/*****************************************************************************
**
* @file xpcie.c
*
* Implements all of functions for XPcie IP driver except initialization/
* configuration and interrupts.
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
*****************************************************************************/

/****************************** Include Files *******************************/
#include "xpcie.h"

/*************************** Constant Definitions ***************************/

/***************************** Type Definitions *****************************/

/****************** Macros (Inline Functions) Definitions *******************/

/*************************** Variable Definitions ***************************/

/*************************** Function Prototypes ****************************/

/****************************************************************************/
/**
* Initialize the XPcie instance provided by the caller based on the
* given Config structure.
*
*
* @param InstancePtr is a pointer to an XPcie instance. The memory the pointer
* references must be pre-allocated by the caller. Further calls to manipulate
* the component through the XPcie API must be made with this pointer.
*
* @param CfgPtr is the device configuration structure containing required HW
* build data.
*
* @param EffectiveAddr is the Physical address of the hardware in a Virtual
* Memory operating system environment. It is the Base Address in a stand alone
* environment.
*
* @return
*
* - XST_SUCCESS Initialization was successfull.
*
* @note
*
*
******************************************************************************/
int XPcie_CfgInitialize(XPcie * InstancePtr, XPcie_Config * CfgPtr,
                                                    u32 EffectiveAddress)
{
	/* Assert arguments */
	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(CfgPtr != NULL);

	/* Clear instance memory and make copy of configuration */
	memset(InstancePtr, 0, sizeof(XPcie));
	memcpy(&InstancePtr->Config, CfgPtr, sizeof(XPcie_Config));

	InstancePtr->IsReady = XCOMPONENT_IS_READY;

	InstancePtr->Config.BaseAddress = EffectiveAddress;

	/* If this is RC then find out the max. number of buses supported */
	if (InstancePtr->Config.IncludeRootComplex) {

		InstancePtr->MaxNumOfBuses = (u32)
				(((InstancePtr->Config.EcamHighAddress -
					InstancePtr->Config.EcamBaseAddress) + 1) >>
					MAX_NUMBER_OF_BUSES);

		}

	/* Disable all interrupts */
	XPcie_DisableInterrupts(InstancePtr, XPCIE_BEIR_DISABLE_ALL_MASK);

	return (XST_SUCCESS);
}

/****************************************************************************/
/**
* Read IP Requester ID.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param BusNumPtr is a pointer to a variable where the driver will pass
*                  back the bus number of requester ID assigned to IP.
*
* @param DevNumPtr is a pointer to a variable where the driver will pass
*                  back the device number of requester ID assigned to IP.
*
* @param FunNumPtr is a pointer to a variable where the driver will pass
*                  back the function number of requester ID assigned to IP.
*
* @return None.
*
* @note The link status has to up before calling this function in order to get
*       valid values.
*
*****************************************************************************/
void XPcie_GetRequesterID(XPcie *InstancePtr, u8 *BusNumPtr, u8 *DevNumPtr,
                             u8 *FunNumPtr)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(BusNumPtr != NULL);
	Xil_AssertVoid(DevNumPtr != NULL);
	Xil_AssertVoid(FunNumPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	Data = XPcie_ReadReg(InstancePtr->Config.BaseAddress, XPCIE_PRIDR_OFFSET);

	*BusNumPtr = (u8)((Data & XPCIE_PRIDR_BUS_MASK) >> XPCIE_PRIDR_BUS_SHIFT);

	*DevNumPtr = (u8)((Data & XPCIE_PRIDR_DEV_MASK) >> XPCIE_PRIDR_DEV_SHIFT);

	*FunNumPtr = (u8)(Data & XPCIE_PRIDR_FUN_MASK);

}

/****************************************************************************/
/**
* Write IP Requester ID.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param BusNum is IP requester ID's bus number.
*
* @param DevNum is IP requester ID's device number.
*
* @param FunNum  is IP requester ID's function number.
*
* @return None.
*
* @note This function is only valid when IP is configured as a root complex.
*
*****************************************************************************/
void XPcie_SetRequesterID(XPcie *InstancePtr, u8 BusNum, u8 DevNum,
                           u8 FunNum)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);
	Xil_AssertVoid(InstancePtr->Config.IncludeRootComplex == XPCIE_IS_RC);

	Data  = ((u32)BusNum) << XPCIE_PRIDR_BUS_SHIFT ;
	Data |= ((u32)DevNum) << XPCIE_PRIDR_DEV_SHIFT ;
	Data |= ((u32)FunNum);
	Data &= XPCIE_PRIDR_MASK;

	XPcie_WriteReg(InstancePtr->Config.BaseAddress, XPCIE_PRIDR_OFFSET, Data);

}

/****************************************************************************/
/**
* Read IP link status.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param LinkStatusPtr is a pointer to a variable where the driver will pass
*        back Link status.The driver uses the constants XPCIE_LINK_STATUS_*
*        defiened in xpcie.h file to pass back the value of this parameter.
*
* @param NumOfLanesPtr is a pointer to a variable where the driver will pass
*        back number of lanes used by IP. The driver uses the constants
*        XPCIE_LINK_WIDTH_* defiened in xpcie.h file to pass back the value
*        of this parameter.
*
* @return None.
*
* @note
*
*****************************************************************************/
void XPcie_GetLinkStatus(XPcie *InstancePtr, u8 *LinkStatusPtr,
                                                       u8 *NumOfLanesPtr)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(LinkStatusPtr != NULL);
	Xil_AssertVoid(NumOfLanesPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	Data = XPcie_ReadReg(InstancePtr->Config.BaseAddress, XPCIE_PSR_OFFSET);
	*LinkStatusPtr = (u8)((Data & XPCIE_PSR_LINK_STATUS_MASK) >>
	                                             XPCIE_PSR_LINK_STATUS_SHIFT);
	*NumOfLanesPtr = (u8)((Data & XPCIE_PSR_LINK_WIDTH_MASK) >>
	                                             XPCIE_PSR_LINK_WIDTH_SHIFT);

}

/****************************************************************************/
/**
* Read IP request control.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param MaxPayLoadPtr is a pointer to a variable where the driver will pass
*        back maximum payload. The driver uses the constants
*        XPCIE_MAX_PAYLOAD_SIZE_* defined in xpcie.h file to pass back
*        the value of this parameter.
*
* @param MaxReadPtr is a pointer to a variable where the driver will pass
*        back maximum read. The driver uses the constants
*        XPCIE_MAX_READ_REQ_* defined in xpcie.h file to pass back
*        the value of this parameter.
*
* @return None.
*
* @note
*
*****************************************************************************/
void XPcie_GetRequestControl(XPcie *InstancePtr, u8 *MaxPayLoadPtr,
                                                            u8 *MaxReadPtr)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(MaxPayLoadPtr != NULL);
	Xil_AssertVoid(MaxReadPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	Data = XPcie_ReadReg(InstancePtr->Config.BaseAddress, XPCIE_PRCR_OFFSET);
	*MaxReadPtr = (u8)(Data & XPCIE_PRCR_MAXRD_MASK);
	*MaxPayLoadPtr = (u8)((Data & XPCIE_PRCR_MAXLD_MASK) >>
	                                                  XPCIE_PRCR_MAXLD_SHIFT);

}

/****************************************************************************/
/**
* Enables IP's bridge so that TLP transaction will go through it to PCIe bus.
* The caller can specify the number of BARs needed to be enabled on the PCIe
* core (1 - 3) as well. If a value other than 1 or 2 has been specified,
* the 3 BARs will be enabled.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param NumOfBars is PCIe Core BARs the caller wants to enable (1 - 3).
*
* @return None.
*
* @note
*
*****************************************************************************/
void XPcie_EnablePCIeConnection(XPcie *InstancePtr, u8 NumOfBars)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	switch (NumOfBars)
	{
		case 1:
			XPcie_WriteReg((InstancePtr->Config.BaseAddress),
			                    XPCIE_BCR_OFFSET, XPCIE_BCR_ENABLE_1BAR_MASK);
			break;
		case 2:
			XPcie_WriteReg((InstancePtr->Config.BaseAddress),
			                    XPCIE_BCR_OFFSET, XPCIE_BCR_ENABLE_2BAR_MASK);
			break;
		case 3:
			XPcie_WriteReg((InstancePtr->Config.BaseAddress),
			                    XPCIE_BCR_OFFSET, XPCIE_BCR_ENABLE_3BAR_MASK);
		default:
			break;

	}
}

/****************************************************************************/
/**
* Disable IP's bridge so that TLP transaction will NOT go through it to PCIe
* bus until it gets enabled again.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @return None.
*
* @note
*
*****************************************************************************/
void XPcie_DisablePCIeConnection(XPcie *InstancePtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	XPcie_WriteReg((InstancePtr->Config.BaseAddress),
	                    XPCIE_BCR_OFFSET, XPCIE_BCR_DISABLE_BAR_MASK);

}

/****************************************************************************/
/**
* Read PCIe address translation vector that corresponds to one of IPIF local
* bus bars passed by the caller.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param BarNumber is IPIF bar number (0 - 5) passed by caller.
*
* @param BarAddrPtr is a pointer to a variable where the driver will pass
*        back translation vector.
*
* @return None.
*
* @note
*
*****************************************************************************/
void XPcie_GetLocalBusBar2PcieBar(XPcie *InstancePtr, u8 BarNumber,
                                                   XPcie_BarAddr *BarAddrPtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(BarAddrPtr != NULL);
	Xil_AssertVoid(BarNumber < InstancePtr->Config.LocalIPIFBARSNum);
	Xil_AssertVoid(InstancePtr->Config.IncludeBarOffsetReg != FALSE);

	BarAddrPtr->LowerAddr =
	     XPcie_ReadReg(InstancePtr->Config.BaseAddress,
	              (XPCIE_IPIF2PCI_0L_OFFSET +
	        (BarNumber * (sizeof(u32) * 2))));

	BarAddrPtr->UpperAddr =
	      XPcie_ReadReg(InstancePtr->Config.BaseAddress,
	               (XPCIE_IPIF2PCI_0U_OFFSET +
	               (BarNumber * (sizeof(u32) * 2))));

}

/****************************************************************************/
/**
* Write PCIe address translation vector that corresponds to one of IPIF local
* bus bars passed by the caller.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param BarNumber is IPIF bar number (0 - 5) passed by caller.
*
* @param BarAddrPte is a pointer to a variable where the driver will pass
*        back translation vector.
*
* @return None.
*
* @note
*
*****************************************************************************/
void XPcie_SetLocalBusBar2PcieBar(XPcie *InstancePtr, u8 BarNumber,
                                                   XPcie_BarAddr *BarAddrPtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(BarAddrPtr != NULL);
	Xil_AssertVoid(BarNumber < InstancePtr->Config.LocalIPIFBARSNum);
	Xil_AssertVoid(InstancePtr->Config.IncludeBarOffsetReg != FALSE);

	XPcie_WriteReg(InstancePtr->Config.BaseAddress,
	           XPCIE_IPIF2PCI_0L_OFFSET +
	     (BarNumber * (sizeof(u32) * 2)), (BarAddrPtr->LowerAddr));

	XPcie_WriteReg(InstancePtr->Config.BaseAddress,
	                          (XPCIE_IPIF2PCI_0U_OFFSET +
	      (BarNumber * (sizeof(u32) * 2))), (BarAddrPtr->UpperAddr));

}

/****************************************************************************/
/**
* Read 32-bit value from one of this IP own configuration space.
* Location is identified by its offset from the begginning of the
* configuration space.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param Offset from beggininng of IP own configuration space.
*
* @param DataPtr is a pointer to a variable where the driver will pass
*        back the value read from the specified location.
*
* @return None
*
* @note None
*
*****************************************************************************/
void XPcie_ReadLocalConfigSpace(XPcie *InstancePtr, u16 Offset, u32 *DataPtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(DataPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);


	*DataPtr = XPcie_ReadReg((InstancePtr->Config.BaseAddress),
	               (XPCIE_PCIE_CORE_OFFSET + ((u32) (Offset * 4))));

}

/****************************************************************************/
/**
* Write 32-bit value to one of this IP own configuration space.
* Location is identified by its offset from the begginning of the
* configuration space.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param Offset from beggininng of IP own configuration space.
*
* @param Data to be written to the specified location.
*
* @return None
*
* @note This function is valid only when IP is configured as a root complex.
*
*****************************************************************************/
void XPcie_WriteLocalConfigSpace(XPcie *InstancePtr, u16 Offset, u32 Data)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);
	Xil_AssertVoid(InstancePtr->Config.IncludeRootComplex == XPCIE_IS_RC);

	XPcie_WriteReg((InstancePtr->Config.BaseAddress),
	              (XPCIE_PCIE_CORE_OFFSET + ((u32) (Offset * 4))), Data);

}

/****************************************************************************/
/*
* Compose an address to be written to configuration address port
*
* @param Bus is the external PCIe function's Bus number.
*
* @param Device is the external PCIe function's Device number.
*
* @param Function is the external PCIe function's Function number.
*
* @param Offset from beggininng of PCIe function's configuration space.

* @return 32 bit composed value (address).
*
* @note This function is valid only when IP is configured as a root complex.
*
*****************************************************************************/
u32 XPcie_ComposeExternalConfigAddress(u8 Bus, u8 Device, u8 Function,
                                                              u16 Offset)
{
	u32 Location = 0;

	Location = ((((u32)Bus) << XPCIE_CAPR_BUS_SHIFT) & XPCIE_CAPR_BUS_MASK);
	Location |= ((((u32)Device) << XPCIE_CAPR_DEV_SHIFT) &
	                                                    XPCIE_CAPR_DEV_MASK);
	Location |= ((((u32)Function) << XPCIE_CAPR_FUN_SHIFT) &
	                                                    XPCIE_CAPR_FUN_MASK);
	Location |= ((((u32)Offset) << XPCIE_CAPR_REG_SHIFT) &
	                                                    XPCIE_CAPR_REG_MASK);
	Location &= XPCIE_CAPR_DWR_ACCS;

	return Location;
}

/****************************************************************************/
/**
* Read 32-bit value from external PCIe Function's configuration space.
* External PCIe function is identified by its Requester ID (Bus#, Device#,
* Function#). Location is identified by its offset from the begginning of the
* configuration space.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param Bus is the external PCIe function's Bus number.
*
* @param Device is the external PCIe function's Device number.
*
* @param Function is the external PCIe function's Function number.
*
* @param Offset from beggininng of PCIe function's configuration space.
*
* @param DataPtr is a pointer to a variable where the driver will pass
*          back the value read from the specified location.
*
* @return None
*
* @note This function is valid only when IP is configured as a root complex.
*
*****************************************************************************/
void XPcie_ReadRemoteConfigSpace(XPcie *InstancePtr, u8 Bus, u8 Device,
                 u8 Function, u16 Offset, u32 *DataPtr)
{
	u32 Location = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(DataPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);
	Xil_AssertVoid(InstancePtr->Config.IncludeRootComplex == XPCIE_IS_RC);

	if (InstancePtr->MaxNumOfBuses <= Bus) {
		*DataPtr = 0xFFFFFFFF;
		return;
	}

	/* compose function configuration space location */
	Location = XPcie_ComposeExternalConfigAddress (Bus, Device, Function,
	                                                         Offset);

	/* read data from that location */
	*DataPtr = XPcie_ReadReg((InstancePtr->Config.EcamBaseAddress),
	                                                Location);

}

/****************************************************************************/
/**
* Write 32-bit value to external PCIe function's configuration space.
* External PCIe function is identified by its Requester ID (Bus#, Device#,
* Function#). Location is identified by its offset from the begginning of the
* configuration space.
*
* @param InstancePtr is the PCIe component to operate on.
*
* @param Bus is the external PCIe function's Bus number.
*
* @param Device is the external PCIe function's Device number.
*
* @param Function is the external PCIe function's Function number.
*
* @param Offset from beggininng of PCIe function's configuration space.
*
* @param Data to be written to the specified location.
*
* @return None
*
* @note This function is valid only when IP is configured as a root complex.
*
*****************************************************************************/
void XPcie_WriteRemoteConfigSpace(XPcie *InstancePtr, u8 Bus, u8 Device,
                            u8 Function, u16 Offset, u32 Data)
{
	u32 Location = 0;
	u32 TestWrite = 0;
	u8 count = 3;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XCOMPONENT_IS_READY);
	Xil_AssertVoid(InstancePtr->Config.IncludeRootComplex == XPCIE_IS_RC);

	if (InstancePtr->MaxNumOfBuses <= Bus) {
		return;
	}
	/* compose function configuration space location */
	Location = XPcie_ComposeExternalConfigAddress (Bus, Device, Function,
                                                          Offset);

	/* write data to that location */
	XPcie_WriteReg((InstancePtr->Config.EcamBaseAddress) ,
	                               Location , Data);


	/* read data from that location to verify write */
	while (count) {

		TestWrite = XPcie_ReadReg((InstancePtr->Config.EcamBaseAddress),
		                                             Location);

		if (TestWrite == Data) {
			break;
		}

		count--;
	}

}

