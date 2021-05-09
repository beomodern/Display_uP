/*******************************************************************************
* File Name: output_test.h  
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

#if !defined(CY_PINS_output_test_ALIASES_H) /* Pins output_test_ALIASES_H */
#define CY_PINS_output_test_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define output_test_0			(output_test__0__PC)
#define output_test_0_PS		(output_test__0__PS)
#define output_test_0_PC		(output_test__0__PC)
#define output_test_0_DR		(output_test__0__DR)
#define output_test_0_SHIFT	(output_test__0__SHIFT)
#define output_test_0_INTR	((uint16)((uint16)0x0003u << (output_test__0__SHIFT*2u)))

#define output_test_INTR_ALL	 ((uint16)(output_test_0_INTR))


#endif /* End Pins output_test_ALIASES_H */


/* [] END OF FILE */
