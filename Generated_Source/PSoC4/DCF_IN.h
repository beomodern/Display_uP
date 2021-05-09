/*******************************************************************************
* File Name: DCF_IN.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_DCF_IN_H)
#define CY_ISR_DCF_IN_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void DCF_IN_Start(void);
void DCF_IN_StartEx(cyisraddress address);
void DCF_IN_Stop(void);

CY_ISR_PROTO(DCF_IN_Interrupt);

void DCF_IN_SetVector(cyisraddress address);
cyisraddress DCF_IN_GetVector(void);

void DCF_IN_SetPriority(uint8 priority);
uint8 DCF_IN_GetPriority(void);

void DCF_IN_Enable(void);
uint8 DCF_IN_GetState(void);
void DCF_IN_Disable(void);

void DCF_IN_SetPending(void);
void DCF_IN_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the DCF_IN ISR. */
#define DCF_IN_INTC_VECTOR            ((reg32 *) DCF_IN__INTC_VECT)

/* Address of the DCF_IN ISR priority. */
#define DCF_IN_INTC_PRIOR             ((reg32 *) DCF_IN__INTC_PRIOR_REG)

/* Priority of the DCF_IN interrupt. */
#define DCF_IN_INTC_PRIOR_NUMBER      DCF_IN__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable DCF_IN interrupt. */
#define DCF_IN_INTC_SET_EN            ((reg32 *) DCF_IN__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the DCF_IN interrupt. */
#define DCF_IN_INTC_CLR_EN            ((reg32 *) DCF_IN__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the DCF_IN interrupt state to pending. */
#define DCF_IN_INTC_SET_PD            ((reg32 *) DCF_IN__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the DCF_IN interrupt. */
#define DCF_IN_INTC_CLR_PD            ((reg32 *) DCF_IN__INTC_CLR_PD_REG)



#endif /* CY_ISR_DCF_IN_H */


/* [] END OF FILE */
