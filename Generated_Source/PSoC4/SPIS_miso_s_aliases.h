/*******************************************************************************
* File Name: SPIS_miso_s.h  
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

#if !defined(CY_PINS_SPIS_miso_s_ALIASES_H) /* Pins SPIS_miso_s_ALIASES_H */
#define CY_PINS_SPIS_miso_s_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define SPIS_miso_s_0			(SPIS_miso_s__0__PC)
#define SPIS_miso_s_0_PS		(SPIS_miso_s__0__PS)
#define SPIS_miso_s_0_PC		(SPIS_miso_s__0__PC)
#define SPIS_miso_s_0_DR		(SPIS_miso_s__0__DR)
#define SPIS_miso_s_0_SHIFT	(SPIS_miso_s__0__SHIFT)
#define SPIS_miso_s_0_INTR	((uint16)((uint16)0x0003u << (SPIS_miso_s__0__SHIFT*2u)))

#define SPIS_miso_s_INTR_ALL	 ((uint16)(SPIS_miso_s_0_INTR))


#endif /* End Pins SPIS_miso_s_ALIASES_H */


/* [] END OF FILE */
