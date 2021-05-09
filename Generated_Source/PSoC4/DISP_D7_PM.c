/*******************************************************************************
* File Name: DISP_D7.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "DISP_D7.h"

static DISP_D7_BACKUP_STRUCT  DISP_D7_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: DISP_D7_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet DISP_D7_SUT.c usage_DISP_D7_Sleep_Wakeup
*******************************************************************************/
void DISP_D7_Sleep(void)
{
    #if defined(DISP_D7__PC)
        DISP_D7_backup.pcState = DISP_D7_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            DISP_D7_backup.usbState = DISP_D7_CR1_REG;
            DISP_D7_USB_POWER_REG |= DISP_D7_USBIO_ENTER_SLEEP;
            DISP_D7_CR1_REG &= DISP_D7_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DISP_D7__SIO)
        DISP_D7_backup.sioState = DISP_D7_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        DISP_D7_SIO_REG &= (uint32)(~DISP_D7_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: DISP_D7_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to DISP_D7_Sleep() for an example usage.
*******************************************************************************/
void DISP_D7_Wakeup(void)
{
    #if defined(DISP_D7__PC)
        DISP_D7_PC = DISP_D7_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            DISP_D7_USB_POWER_REG &= DISP_D7_USBIO_EXIT_SLEEP_PH1;
            DISP_D7_CR1_REG = DISP_D7_backup.usbState;
            DISP_D7_USB_POWER_REG &= DISP_D7_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DISP_D7__SIO)
        DISP_D7_SIO_REG = DISP_D7_backup.sioState;
    #endif
}


/* [] END OF FILE */
