/* $Id: xpcie.h,v 1.1.2.1 2010/04/19 21:44:43 mhabib Exp $ */
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
/****************************************************************************/
/**
*
* @file xpcie.h
*
* This file contains the software API definition of the Xilinx PCIe IP
* (XPcie). This driver provides "C" function interface to application/upper
* layer to access the hardware.
*
* <b>Features</b>
* The driver provides its user with entry points
*   - To initialize and configure itself and the hardware
*   - To access PCIe configuration space both locally and remotely
*   - To enable/disable and to report errors (interrupts).
*
* <b>IP Hardware Configuration</b>
*
* During HW build time and on some Xilinx FPGAs , the designer can build
* PCIe IP either as a root complex or as an end point.
* All Xilinix FPGAs that support PCIe IP allows the designer to build it as
* an end point. Some selected ones (first is Virtex 6) allow the designer to
* build PCIe IP as a root complex or as an end point like the rest of FPGAs.
* This driver exports an API that supports both root complex and end point.
* All of this driver API functions could be called on behalf of IP built
* as a root complex. Only a subset of this driver's API functions could be
* called on behalf of IP built as an end point.

* <b>Driver Initialization & Configuration</b>
*
* The XPcie_Config structure is used by the driver to configure itself. This
* configuration structure is typically created by the tool-chain based on HW
* build properties.
*
* To support multiple runtime loading and initialization strategies employed
* by various operating systems, the driver instance can be initialized in the
* following way:
*
*   - XPcie_LookupConfig(DeviceId) - Use the device identifier to find the
*   static configuration structure defined in xpcie_g.c. This is setup by the
*   tools. For some operating systems the config structure will be initialized
*   by the software and this call is not needed.
*
*   - XPcie_CfgInitialize(InstancePtr, CfgPtr, EffectiveAddr) - Uses a
*   configuration structure provided by the caller. If running in a system with
*   address translation, the provided virtual memory base address replaces the
*   physical address present in the configuration structure.
*
* <b>Interrupt Management</b>
*
* The XPcie driver provides interrupt management functions. It allows
* the caller to enable/disable each individual interrupt as well as get/clear
* pending interrupts. Implementation of callback handlers is left to the user.
*
*
* @note
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ------------------------------------------------------
* 3.00a  MH  03/16/09 Original code.
* 4.00a  MH  11/23/09 Updated to use HAL Phase 1 interface.
* 4.01a  MH  04/19/10 Removed "m" from Read/Write Macros.
*
* </pre>
*
*****************************************************************************/
#ifndef XPCIE_H			/* prevent circular inclusions */
#define XPCIE_H			/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "xil_assert.h"
#include "xstatus.h"
#include "xpcie_hw.h"
#include "xenv.h"
#include "xil_types.h"
/************************** Constant Definitions ****************************/

/*
 * To figure out if PCIe IP
 * is configured as an end
 * point or as a root complex
 */
#define XPCIE_IS_RC                 0x01

/*
 * To figure out if PCIe IP
 * is configured as an end
 * point or as a root complex
 */
#define XPCIE_MIN_CORE_BARS         0x01
#define XPCIE_MAX_CORE_BARS         0x03

/*
 * To figure out the max. number
 * of buses available when IP
 * is configured as a root compex
 */
#define MAX_NUMBER_OF_BUSES         0x14

/**
 * Max Read Request Size Values
 * It is used with "XPcie_GetRequestControl"
 * and "XPcie_SetRequestControl"
 * @{
 */
#define XPCIE_MAX_READ_REQ_128      0x00
#define XPCIE_MAX_READ_REQ_256      0x01
#define XPCIE_MAX_READ_REQ_512      0x02
#define XPCIE_MAX_READ_REQ_1024     0x03
#define XPCIE_MAX_READ_REQ_2048     0x04
#define XPCIE_MAX_READ_REQ_4096     0x05
/*@}*/

/**
 * Max Read Request Size Values
 * It is used with "XPcie_GetRequestControl"
 * and "XPcie_SetRequestControl"
 */
#define XPCIE_MAX_PAYLOAD_SIZE_128    0x00
#define XPCIE_MAX_PAYLOAD_SIZE_256    0x01
#define XPCIE_MAX_PAYLOAD_SIZE_512    0x02
/*@}*/

/**
 * Link Status Values
 * It is used with "XPcie_GetLinkStatus"
 * @{
 */
#define XPCIE_LINK_STATUS_DOWN        0x00
#define XPCIE_LINK_STATUS_UP          0x01
/*@}*/

/**
 * Link Width Values
 * It is used with "XPcie_GetLinkStatus"
 * @{
 */
#define XPCIE_LINK_WIDTH_1_LANE        0x01
#define XPCIE_LINK_WIDTH_2_LANE        0x02
#define XPCIE_LINK_WIDTH_4_LANE        0x04
#define XPCIE_LINK_WIDTH_8_LANE        0x08
/*@}*/

/**************************** Type Definitions ******************************/

/**
 * This typedef contains IP hardware configuration information.
 */

typedef  struct {
	u16 DeviceId;            /**< Unique ID of PCIe IP */
	u32 BaseAddress;         /**< Register base address */
	u8  LocalIPIFBARSNum;    /**< The number of local bus (IPIF) BARs in h/w */
	u8  IncludeBarOffsetReg; /**< Are BAR Offset registers built in h/w */
	u8  IncludeRootComplex;  /**< Is IP built as root complex */
	u32 EcamBaseAddress;     /**< If this is RC, ECAM BASE ADDR */
	u32 EcamHighAddress;     /**< If this is RC, ECAM HIGH ADDR */
} XPcie_Config;

/**
 * The XPcie driver instance data. The user is required to allocate a
 * variable of this type for every PCIe device in the system that will be
 * using this API. A pointer to a variable of this type is passed to the driver
 * API functions defined here.
 */

typedef struct {
	XPcie_Config Config;    /**< Configuration data, includes base address */
	u32 IsReady;	         /**< Has IP been initialized and ready */
	u32 MaxNumOfBuses;      /**< If this is RC IP, Max Number of Buses */
} XPcie;


/**
 * The user is required to use this strucuture when reading or writing
 * translation vector between local bus BARs and PCIe BARs. It is used when
 * calling "XPcie_GetLocalBusBar2PcieBar" and "XPcie_SetLocalBusBar2PcieBar"
 * functions. The translation vectors are 64 bits wide even though they might
 * only use the lower 32 bits
 */
typedef struct {
	u32 LowerAddr;   /**< Lower 32 bits of translation value */
	u32 UpperAddr;   /**< Upper 32 bits of translation value */
} XPcie_BarAddr;

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Function Prototypes *****************************/

/*
 * API exported by the driver to the upper layer
 *
 * The following are the driver interface entry points.
 *
 */

/*
 * Config Look Up Function.
 * This API is implemented in xpcie_sinit.c
 */

XPcie_Config * XPcie_LookupConfig(u16 DeviceId);

/*
 * PCIe Setup and Configuration Functions.
 * This API is implemented in xpcie.c
 */

/* Config Initialization */
int XPcie_CfgInitialize(XPcie * InstancePtr, XPcie_Config *CfgPtr,
                                                      u32 EffectiveAddress);
/* Requester ID */
void XPcie_GetRequesterID(XPcie *InstancePtr, u8 *BusNumPtr, u8 *DevNumPtr,
                                                         u8 *FunNumPtr);

void XPcie_SetRequesterID(XPcie *InstancePtr, u8 BusNum, u8 DevNum,
                                                            u8 FunNum);

/* PCIe Link Status (Up/Dn and Number of Lanes) */
void XPcie_GetLinkStatus(XPcie *InstancePtr, u8 *LinkStatePtr,
                                                   u8 *NumOfLanesPtr);

/* Request Control (max. read and max. payload) */
void XPcie_GetRequestControl(XPcie *InstancePtr, u8 *MaxPayLoadPtr,
                                                            u8 *MaxReadPtr);

/* PCIe Bus Connection */
void XPcie_EnablePCIeConnection(XPcie *InstancePtr, u8 NumOfBars);
void XPcie_DisablePCIeConnection(XPcie *InstancePtr);

/* Translation Vectors (Local Bus Address to PCIe Bus Address) */
void XPcie_GetLocalBusBar2PcieBar(XPcie *InstancePtr, u8 BarNumber,
                                             XPcie_BarAddr *BarAddrPtr);

void XPcie_SetLocalBusBar2PcieBar(XPcie *InstancePtr, u8 BarNumber,
                                             XPcie_BarAddr *BarAddrPtr);

/* Local Configuration Space Access */
void XPcie_ReadLocalConfigSpace(XPcie *InstancePtr, u16 Offset, u32 *DataPtr);
void XPcie_WriteLocalConfigSpace(XPcie *InstancePtr, u16 Offset, u32 Data);

/* Remote Configuration Space Access */
void XPcie_ReadRemoteConfigSpace(XPcie *InstancePtr, u8 Bus, u8 Device,
                                     u8 Function, u16 Offset, u32 *DataPtr);

void XPcie_WriteRemoteConfigSpace(XPcie *InstancePtr, u8 Bus, u8 Device,
                                     u8 Function, u16 Offset, u32  Data);


/*
 * Interrupt Functions.
 * This API is implemented in xpcie_intr.c
 */

/* Enable/Disable */
void XPcie_EnableInterrupts(XPcie *InstancePtr, u32 EnableMask);
void XPcie_DisableInterrupts(XPcie *InstancePtr, u32 DisableMask);
void XPcie_GetEnabledInterrupts(XPcie *InstancePtr, u32 *EnabledMaskPtr);

/* Pending/Clearing */
void XPcie_GetPendingInterrupts(XPcie *InstancePtr, u32 *PendingMaskPtr);
void XPcie_ClearPendingInterrupts(XPcie *InstancePtr, u32 ClearMaskPtr);

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */

