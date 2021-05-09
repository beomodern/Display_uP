/*******************************************************************************
* File Name: P0_4_UNUSED.h  
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

#if !defined(CY_PINS_P0_4_UNUSED_ALIASES_H) /* Pins P0_4_UNUSED_ALIASES_H */
#define CY_PINS_P0_4_UNUSED_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define P0_4_UNUSED_0			(P0_4_UNUSED__0__PC)
#define P0_4_UNUSED_0_PS		(P0_4_UNUSED__0__PS)
#define P0_4_UNUSED_0_PC		(P0_4_UNUSED__0__PC)
#define P0_4_UNUSED_0_DR		(P0_4_UNUSED__0__DR)
#define P0_4_UNUSED_0_SHIFT	(P0_4_UNUSED__0__SHIFT)
#define P0_4_UNUSED_0_INTR	((uint16)((uint16)0x0003u << (P0_4_UNUSED__0__SHIFT*2u)))

#define P0_4_UNUSED_INTR_ALL	 ((uint16)(P0_4_UNUSED_0_INTR))


#endif /* End Pins P0_4_UNUSED_ALIASES_H */


/* [] END OF FILE */
