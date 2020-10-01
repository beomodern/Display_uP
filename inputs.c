/* ========================================
 *
 * Copyright HEMI, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * 
 * Functions used by BeoModern main PSoC to monitor input informaiton coming from
 * push buttons and over DataLink interface
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF HEMI.
 *
 * ========================================
*/

#include <inputs.h>

// Changes in the interrupt to signal that the transfer was complete 
uint8 flag = 0;

/*******************************************************************************
* Function Name: CY_ISR(DISPLAY_Timer_INTHandler)
********************************************************************************
* Summary:
*  The Interrupt Service Routine. Triggered by software when timer expires after 
*  DISPLAY_Timer_Start() is called. It uses timer to count 2 seconds wait.
*  This time is used to display information that are temporary (like button press
*  or BT status).
*   
* Parameters:
*  Timer interrupt handler 
*
* Return:
*  updates DISPLAY_UPDATE variable.when TC is called.
*
********************************************************************************/
CY_ISR(DISPLAY_Timer_INTHandler)
{
// Check if interrupt came from Timer Counter expired (indicating timer count elapsed)
    if (0u != (DISPLAY_Timer_ReadStatusRegister() & 0x01))
    {
// sets display update variable 
//        DISPLAY_UPDATE = 1;
// stop timer and wait for another software call to start        
        DISPLAY_Timer_Stop();
    }
}




/*******************************************************************************
* Function Name: Slave_ISR
********************************************************************************
* Summary:
*  The main function performs the following actions:
*   1. Sets the flag to 1
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Slave_ISR()
{
    /* Sets global flag for use by background thread */
    flag = 1;
}


/* [] END OF FILE */

