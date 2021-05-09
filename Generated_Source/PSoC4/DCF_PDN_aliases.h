/*******************************************************************************
* File Name: DCF_PDN.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_DCF_PDN_ALIASES_H) /* Pins DCF_PDN_ALIASES_H */
#define CY_PINS_DCF_PDN_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DCF_PDN_0			(DCF_PDN__0__PC)
#define DCF_PDN_0_PS		(DCF_PDN__0__PS)
#define DCF_PDN_0_PC		(DCF_PDN__0__PC)
#define DCF_PDN_0_DR		(DCF_PDN__0__DR)
#define DCF_PDN_0_SHIFT	(DCF_PDN__0__SHIFT)
#define DCF_PDN_0_INTR	((uint16)((uint16)0x0003u << (DCF_PDN__0__SHIFT*2u)))

#define DCF_PDN_INTR_ALL	 ((uint16)(DCF_PDN_0_INTR))


#endif /* End Pins DCF_PDN_ALIASES_H */


/* [] END OF FILE */
