/*******************************************************************************
* File Name: DCF_INTR.h
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
#if !defined(CY_ISR_DCF_INTR_H)
#define CY_ISR_DCF_INTR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void DCF_INTR_Start(void);
void DCF_INTR_StartEx(cyisraddress address);
void DCF_INTR_Stop(void);

CY_ISR_PROTO(DCF_INTR_Interrupt);

void DCF_INTR_SetVector(cyisraddress address);
cyisraddress DCF_INTR_GetVector(void);

void DCF_INTR_SetPriority(uint8 priority);
uint8 DCF_INTR_GetPriority(void);

void DCF_INTR_Enable(void);
uint8 DCF_INTR_GetState(void);
void DCF_INTR_Disable(void);

void DCF_INTR_SetPending(void);
void DCF_INTR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the DCF_INTR ISR. */
#define DCF_INTR_INTC_VECTOR            ((reg32 *) DCF_INTR__INTC_VECT)

/* Address of the DCF_INTR ISR priority. */
#define DCF_INTR_INTC_PRIOR             ((reg32 *) DCF_INTR__INTC_PRIOR_REG)

/* Priority of the DCF_INTR interrupt. */
#define DCF_INTR_INTC_PRIOR_NUMBER      DCF_INTR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable DCF_INTR interrupt. */
#define DCF_INTR_INTC_SET_EN            ((reg32 *) DCF_INTR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the DCF_INTR interrupt. */
#define DCF_INTR_INTC_CLR_EN            ((reg32 *) DCF_INTR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the DCF_INTR interrupt state to pending. */
#define DCF_INTR_INTC_SET_PD            ((reg32 *) DCF_INTR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the DCF_INTR interrupt. */
#define DCF_INTR_INTC_CLR_PD            ((reg32 *) DCF_INTR__INTC_CLR_PD_REG)



#endif /* CY_ISR_DCF_INTR_H */


/* [] END OF FILE */
