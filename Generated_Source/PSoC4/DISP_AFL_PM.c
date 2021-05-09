/*******************************************************************************
* File Name: DISP_AFL.c  
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
#include "DISP_AFL.h"

static DISP_AFL_BACKUP_STRUCT  DISP_AFL_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: DISP_AFL_Sleep
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
*  \snippet DISP_AFL_SUT.c usage_DISP_AFL_Sleep_Wakeup
*******************************************************************************/
void DISP_AFL_Sleep(void)
{
    #if defined(DISP_AFL__PC)
        DISP_AFL_backup.pcState = DISP_AFL_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            DISP_AFL_backup.usbState = DISP_AFL_CR1_REG;
            DISP_AFL_USB_POWER_REG |= DISP_AFL_USBIO_ENTER_SLEEP;
            DISP_AFL_CR1_REG &= DISP_AFL_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DISP_AFL__SIO)
        DISP_AFL_backup.sioState = DISP_AFL_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        DISP_AFL_SIO_REG &= (uint32)(~DISP_AFL_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: DISP_AFL_Wakeup
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
*  Refer to DISP_AFL_Sleep() for an example usage.
*******************************************************************************/
void DISP_AFL_Wakeup(void)
{
    #if defined(DISP_AFL__PC)
        DISP_AFL_PC = DISP_AFL_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            DISP_AFL_USB_POWER_REG &= DISP_AFL_USBIO_EXIT_SLEEP_PH1;
            DISP_AFL_CR1_REG = DISP_AFL_backup.usbState;
            DISP_AFL_USB_POWER_REG &= DISP_AFL_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DISP_AFL__SIO)
        DISP_AFL_SIO_REG = DISP_AFL_backup.sioState;
    #endif
}


/* [] END OF FILE */
