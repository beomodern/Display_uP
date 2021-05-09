/*******************************************************************************
* File Name: SPIS_sclk_s.c  
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
#include "SPIS_sclk_s.h"

static SPIS_sclk_s_BACKUP_STRUCT  SPIS_sclk_s_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: SPIS_sclk_s_Sleep
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
*  \snippet SPIS_sclk_s_SUT.c usage_SPIS_sclk_s_Sleep_Wakeup
*******************************************************************************/
void SPIS_sclk_s_Sleep(void)
{
    #if defined(SPIS_sclk_s__PC)
        SPIS_sclk_s_backup.pcState = SPIS_sclk_s_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            SPIS_sclk_s_backup.usbState = SPIS_sclk_s_CR1_REG;
            SPIS_sclk_s_USB_POWER_REG |= SPIS_sclk_s_USBIO_ENTER_SLEEP;
            SPIS_sclk_s_CR1_REG &= SPIS_sclk_s_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SPIS_sclk_s__SIO)
        SPIS_sclk_s_backup.sioState = SPIS_sclk_s_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        SPIS_sclk_s_SIO_REG &= (uint32)(~SPIS_sclk_s_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: SPIS_sclk_s_Wakeup
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
*  Refer to SPIS_sclk_s_Sleep() for an example usage.
*******************************************************************************/
void SPIS_sclk_s_Wakeup(void)
{
    #if defined(SPIS_sclk_s__PC)
        SPIS_sclk_s_PC = SPIS_sclk_s_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            SPIS_sclk_s_USB_POWER_REG &= SPIS_sclk_s_USBIO_EXIT_SLEEP_PH1;
            SPIS_sclk_s_CR1_REG = SPIS_sclk_s_backup.usbState;
            SPIS_sclk_s_USB_POWER_REG &= SPIS_sclk_s_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SPIS_sclk_s__SIO)
        SPIS_sclk_s_SIO_REG = SPIS_sclk_s_backup.sioState;
    #endif
}


/* [] END OF FILE */
