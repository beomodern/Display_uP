/*******************************************************************************
* File Name: P0_4_UNUSED.c  
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
#include "P0_4_UNUSED.h"

static P0_4_UNUSED_BACKUP_STRUCT  P0_4_UNUSED_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: P0_4_UNUSED_Sleep
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
*  \snippet P0_4_UNUSED_SUT.c usage_P0_4_UNUSED_Sleep_Wakeup
*******************************************************************************/
void P0_4_UNUSED_Sleep(void)
{
    #if defined(P0_4_UNUSED__PC)
        P0_4_UNUSED_backup.pcState = P0_4_UNUSED_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            P0_4_UNUSED_backup.usbState = P0_4_UNUSED_CR1_REG;
            P0_4_UNUSED_USB_POWER_REG |= P0_4_UNUSED_USBIO_ENTER_SLEEP;
            P0_4_UNUSED_CR1_REG &= P0_4_UNUSED_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(P0_4_UNUSED__SIO)
        P0_4_UNUSED_backup.sioState = P0_4_UNUSED_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        P0_4_UNUSED_SIO_REG &= (uint32)(~P0_4_UNUSED_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: P0_4_UNUSED_Wakeup
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
*  Refer to P0_4_UNUSED_Sleep() for an example usage.
*******************************************************************************/
void P0_4_UNUSED_Wakeup(void)
{
    #if defined(P0_4_UNUSED__PC)
        P0_4_UNUSED_PC = P0_4_UNUSED_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            P0_4_UNUSED_USB_POWER_REG &= P0_4_UNUSED_USBIO_EXIT_SLEEP_PH1;
            P0_4_UNUSED_CR1_REG = P0_4_UNUSED_backup.usbState;
            P0_4_UNUSED_USB_POWER_REG &= P0_4_UNUSED_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(P0_4_UNUSED__SIO)
        P0_4_UNUSED_SIO_REG = P0_4_UNUSED_backup.sioState;
    #endif
}


/* [] END OF FILE */
