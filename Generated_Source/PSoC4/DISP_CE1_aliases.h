/*******************************************************************************
* File Name: DISP_CE1.h  
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

#if !defined(CY_PINS_DISP_CE1_ALIASES_H) /* Pins DISP_CE1_ALIASES_H */
#define CY_PINS_DISP_CE1_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DISP_CE1_0			(DISP_CE1__0__PC)
#define DISP_CE1_0_PS		(DISP_CE1__0__PS)
#define DISP_CE1_0_PC		(DISP_CE1__0__PC)
#define DISP_CE1_0_DR		(DISP_CE1__0__DR)
#define DISP_CE1_0_SHIFT	(DISP_CE1__0__SHIFT)
#define DISP_CE1_0_INTR	((uint16)((uint16)0x0003u << (DISP_CE1__0__SHIFT*2u)))

#define DISP_CE1_INTR_ALL	 ((uint16)(DISP_CE1_0_INTR))


#endif /* End Pins DISP_CE1_ALIASES_H */


/* [] END OF FILE */
