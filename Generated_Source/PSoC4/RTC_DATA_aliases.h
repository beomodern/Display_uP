/*******************************************************************************
* File Name: RTC_DATA.h  
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

#if !defined(CY_PINS_RTC_DATA_ALIASES_H) /* Pins RTC_DATA_ALIASES_H */
#define CY_PINS_RTC_DATA_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RTC_DATA_0			(RTC_DATA__0__PC)
#define RTC_DATA_0_PS		(RTC_DATA__0__PS)
#define RTC_DATA_0_PC		(RTC_DATA__0__PC)
#define RTC_DATA_0_DR		(RTC_DATA__0__DR)
#define RTC_DATA_0_SHIFT	(RTC_DATA__0__SHIFT)
#define RTC_DATA_0_INTR	((uint16)((uint16)0x0003u << (RTC_DATA__0__SHIFT*2u)))

#define RTC_DATA_INTR_ALL	 ((uint16)(RTC_DATA_0_INTR))


#endif /* End Pins RTC_DATA_ALIASES_H */


/* [] END OF FILE */
