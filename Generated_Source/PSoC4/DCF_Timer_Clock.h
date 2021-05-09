/*******************************************************************************
* File Name: DCF_Timer_Clock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_DCF_Timer_Clock_H)
#define CY_CLOCK_DCF_Timer_Clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void DCF_Timer_Clock_StartEx(uint32 alignClkDiv);
#define DCF_Timer_Clock_Start() \
    DCF_Timer_Clock_StartEx(DCF_Timer_Clock__PA_DIV_ID)

#else

void DCF_Timer_Clock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void DCF_Timer_Clock_Stop(void);

void DCF_Timer_Clock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 DCF_Timer_Clock_GetDividerRegister(void);
uint8  DCF_Timer_Clock_GetFractionalDividerRegister(void);

#define DCF_Timer_Clock_Enable()                         DCF_Timer_Clock_Start()
#define DCF_Timer_Clock_Disable()                        DCF_Timer_Clock_Stop()
#define DCF_Timer_Clock_SetDividerRegister(clkDivider, reset)  \
    DCF_Timer_Clock_SetFractionalDividerRegister((clkDivider), 0u)
#define DCF_Timer_Clock_SetDivider(clkDivider)           DCF_Timer_Clock_SetDividerRegister((clkDivider), 1u)
#define DCF_Timer_Clock_SetDividerValue(clkDivider)      DCF_Timer_Clock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define DCF_Timer_Clock_DIV_ID     DCF_Timer_Clock__DIV_ID

#define DCF_Timer_Clock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define DCF_Timer_Clock_CTRL_REG   (*(reg32 *)DCF_Timer_Clock__CTRL_REGISTER)
#define DCF_Timer_Clock_DIV_REG    (*(reg32 *)DCF_Timer_Clock__DIV_REGISTER)

#define DCF_Timer_Clock_CMD_DIV_SHIFT          (0u)
#define DCF_Timer_Clock_CMD_PA_DIV_SHIFT       (8u)
#define DCF_Timer_Clock_CMD_DISABLE_SHIFT      (30u)
#define DCF_Timer_Clock_CMD_ENABLE_SHIFT       (31u)

#define DCF_Timer_Clock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << DCF_Timer_Clock_CMD_DISABLE_SHIFT))
#define DCF_Timer_Clock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << DCF_Timer_Clock_CMD_ENABLE_SHIFT))

#define DCF_Timer_Clock_DIV_FRAC_MASK  (0x000000F8u)
#define DCF_Timer_Clock_DIV_FRAC_SHIFT (3u)
#define DCF_Timer_Clock_DIV_INT_MASK   (0xFFFFFF00u)
#define DCF_Timer_Clock_DIV_INT_SHIFT  (8u)

#else 

#define DCF_Timer_Clock_DIV_REG        (*(reg32 *)DCF_Timer_Clock__REGISTER)
#define DCF_Timer_Clock_ENABLE_REG     DCF_Timer_Clock_DIV_REG
#define DCF_Timer_Clock_DIV_FRAC_MASK  DCF_Timer_Clock__FRAC_MASK
#define DCF_Timer_Clock_DIV_FRAC_SHIFT (16u)
#define DCF_Timer_Clock_DIV_INT_MASK   DCF_Timer_Clock__DIVIDER_MASK
#define DCF_Timer_Clock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_DCF_Timer_Clock_H) */

/* [] END OF FILE */
