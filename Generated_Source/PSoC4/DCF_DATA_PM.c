/*******************************************************************************
* File Name: DCF_DATA.c  
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
#include "DCF_DATA.h"

static DCF_DATA_BACKUP_STRUCT  DCF_DATA_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: DCF_DATA_Sleep
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
*  \snippet DCF_DATA_SUT.c usage_DCF_DATA_Sleep_Wakeup
*******************************************************************************/
void DCF_DATA_Sleep(void)
{
    #if defined(DCF_DATA__PC)
        DCF_DATA_backup.pcState = DCF_DATA_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            DCF_DATA_backup.usbState = DCF_DATA_CR1_REG;
            DCF_DATA_USB_POWER_REG |= DCF_DATA_USBIO_ENTER_SLEEP;
            DCF_DATA_CR1_REG &= DCF_DATA_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DCF_DATA__SIO)
        DCF_DATA_backup.sioState = DCF_DATA_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        DCF_DATA_SIO_REG &= (uint32)(~DCF_DATA_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: DCF_DATA_Wakeup
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
*  Refer to DCF_DATA_Sleep() for an example usage.
*******************************************************************************/
void DCF_DATA_Wakeup(void)
{
    #if defined(DCF_DATA__PC)
        DCF_DATA_PC = DCF_DATA_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            DCF_DATA_USB_POWER_REG &= DCF_DATA_USBIO_EXIT_SLEEP_PH1;
            DCF_DATA_CR1_REG = DCF_DATA_backup.usbState;
            DCF_DATA_USB_POWER_REG &= DCF_DATA_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DCF_DATA__SIO)
        DCF_DATA_SIO_REG = DCF_DATA_backup.sioState;
    #endif
}


/* [] END OF FILE */
