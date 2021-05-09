/*******************************************************************************
* File Name: output_test.c  
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
#include "output_test.h"

static output_test_BACKUP_STRUCT  output_test_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: output_test_Sleep
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
*  \snippet output_test_SUT.c usage_output_test_Sleep_Wakeup
*******************************************************************************/
void output_test_Sleep(void)
{
    #if defined(output_test__PC)
        output_test_backup.pcState = output_test_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            output_test_backup.usbState = output_test_CR1_REG;
            output_test_USB_POWER_REG |= output_test_USBIO_ENTER_SLEEP;
            output_test_CR1_REG &= output_test_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(output_test__SIO)
        output_test_backup.sioState = output_test_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        output_test_SIO_REG &= (uint32)(~output_test_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: output_test_Wakeup
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
*  Refer to output_test_Sleep() for an example usage.
*******************************************************************************/
void output_test_Wakeup(void)
{
    #if defined(output_test__PC)
        output_test_PC = output_test_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            output_test_USB_POWER_REG &= output_test_USBIO_EXIT_SLEEP_PH1;
            output_test_CR1_REG = output_test_backup.usbState;
            output_test_USB_POWER_REG &= output_test_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(output_test__SIO)
        output_test_SIO_REG = output_test_backup.sioState;
    #endif
}


/* [] END OF FILE */
