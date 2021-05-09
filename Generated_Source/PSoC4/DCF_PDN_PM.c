/*******************************************************************************
* File Name: DCF_PDN.c  
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
#include "DCF_PDN.h"

static DCF_PDN_BACKUP_STRUCT  DCF_PDN_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: DCF_PDN_Sleep
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
*  \snippet DCF_PDN_SUT.c usage_DCF_PDN_Sleep_Wakeup
*******************************************************************************/
void DCF_PDN_Sleep(void)
{
    #if defined(DCF_PDN__PC)
        DCF_PDN_backup.pcState = DCF_PDN_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            DCF_PDN_backup.usbState = DCF_PDN_CR1_REG;
            DCF_PDN_USB_POWER_REG |= DCF_PDN_USBIO_ENTER_SLEEP;
            DCF_PDN_CR1_REG &= DCF_PDN_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DCF_PDN__SIO)
        DCF_PDN_backup.sioState = DCF_PDN_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        DCF_PDN_SIO_REG &= (uint32)(~DCF_PDN_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: DCF_PDN_Wakeup
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
*  Refer to DCF_PDN_Sleep() for an example usage.
*******************************************************************************/
void DCF_PDN_Wakeup(void)
{
    #if defined(DCF_PDN__PC)
        DCF_PDN_PC = DCF_PDN_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            DCF_PDN_USB_POWER_REG &= DCF_PDN_USBIO_EXIT_SLEEP_PH1;
            DCF_PDN_CR1_REG = DCF_PDN_backup.usbState;
            DCF_PDN_USB_POWER_REG &= DCF_PDN_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DCF_PDN__SIO)
        DCF_PDN_SIO_REG = DCF_PDN_backup.sioState;
    #endif
}


/* [] END OF FILE */
