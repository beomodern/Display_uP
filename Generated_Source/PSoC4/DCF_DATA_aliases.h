/*******************************************************************************
* File Name: DCF_DATA.h  
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

#if !defined(CY_PINS_DCF_DATA_ALIASES_H) /* Pins DCF_DATA_ALIASES_H */
#define CY_PINS_DCF_DATA_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DCF_DATA_0			(DCF_DATA__0__PC)
#define DCF_DATA_0_PS		(DCF_DATA__0__PS)
#define DCF_DATA_0_PC		(DCF_DATA__0__PC)
#define DCF_DATA_0_DR		(DCF_DATA__0__DR)
#define DCF_DATA_0_SHIFT	(DCF_DATA__0__SHIFT)
#define DCF_DATA_0_INTR	((uint16)((uint16)0x0003u << (DCF_DATA__0__SHIFT*2u)))

#define DCF_DATA_INTR_ALL	 ((uint16)(DCF_DATA_0_INTR))


#endif /* End Pins DCF_DATA_ALIASES_H */


/* [] END OF FILE */
